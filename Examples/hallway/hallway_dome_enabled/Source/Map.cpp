// ** SYSTEM HEADER FILES **
// -------------------------
#include <math.h>
#include <stdio.h>																	
#include <gl\glaux.h>
#include <windows.h>


// ** CUSTOM STRUCTURES **
// -----------------------
struct TRIANGLE
{
	float x[3], y[3], z[3];
	float u[3], v[3];
	float r[3], g[3], b[3];
	float trans;
	int texture;
};

struct VECTOR3
{
	float x, y, z;
};

struct PARTICLES
{
	float	life;		// Particle life
	float	fade;		// Fade speed
	float	x;			// X pos
	float	y;			// Y pos
	float	z;			// Z pos
	float	xi;			// X direction
	float	yi;			// Y direction
	float	zi;			// Z direction
};


// ** CONSTANTS **
// ---------------
const int TEXCOUNT = 10;						// Texture count
const float piOver180 = 0.0174532925f;		// Pi divided by 180
const int MAX_STEAM_PARTICLES  = 35;			// Max steam particles
const int MAX_BUBBLE_PARTICLES = 10;			// Max bubble particles


// ** GLOBAL VARIABLES **
// ----------------------

// Camera
float yaw			= 16.0f;	 // Camera yaw
float walkbiasangle = 0;		 // Walk bias angle

// Misc
float space				= 0.0f;	 // Space movement
float fade				= 1.0f;	 // Fade value
float doorx				= 0.0f;  // Left door horizontal value
bool  Door				= false; // Opening door flag
int   polyCount			= 0;	 // Polycount of map

TRIANGLE  *map;							// Map data
PARTICLES steam[MAX_STEAM_PARTICLES];	// Steam data
PARTICLES bubble[MAX_BUBBLE_PARTICLES];	// Bubble data
GLuint    texture[TEXCOUNT];			// Texture array

VECTOR3   camPos    = {  -5.25f, -7.5f, -29.6f};	// Camera position


// ** READ STRING FUNCTION **
// --------------------------
void readStr(FILE *f, char *string)
{
	do
	{
		fgets(string, 255, f);
	} while ((string[0] == '/') || (string[0] == '\n'));
	return;
}


// ** LOAD MAP **
// ----------------
void LoadMap()
{
	FILE *fileIn;
	char oneLine[255];
	fileIn = fopen("maps/map.txt", "rt");				// File to load

	readStr(fileIn, oneLine);
	sscanf (oneLine, "POLYCOUNT: %d\n", &polyCount);	// Read in polyCount

	map = new TRIANGLE[polyCount];						// Resize array

	for (int i = 0; i < polyCount; i++)					// Read in values from file
	{
		readStr(fileIn, oneLine);
		sscanf (oneLine, "%i%f", &map[i].texture, &map[i].trans);

		for (int vert = 0; vert < 3; vert++)
		{
			readStr(fileIn, oneLine);
			sscanf(oneLine, "%f%f%f%f%f%f%f%f%f%f", &map[i].x[vert], 
													&map[i].y[vert], 
													&map[i].z[vert], 
													&map[i].u[vert], 
													&map[i].v[vert],
												    &map[i].r[vert],
													&map[i].g[vert],
													&map[i].b[vert]);
		}
	}
	fclose(fileIn);
	
	return;
}



// ** LOAD BITMAP **
// -----------------
AUX_RGBImageRec *LoadBMP(char *Filename)
{
	FILE *File = NULL;						// File handle

	if (!Filename)							// Check for a file name
	{
		return NULL;						// If load failed return NULL
	}

	File = fopen(Filename, "r");			// Check file exists

	if(File)
	{
		fclose(File);						// Close handle
		return auxDIBImageLoad(Filename);	// Load bitmap and return pointer
	}

	return NULL;							// If load failed return NULL
}


// ** LOAD BITMAPS AND CONVERT TO TEXTURES **
// ------------------------------------------
void LoadGLTextures()
{
	AUX_RGBImageRec *TextureImage[TEXCOUNT];			// Create space for textures
	memset(TextureImage, 0, sizeof(void *)*TEXCOUNT);	// Set pointer to NULL

	// Load textures
	if ((TextureImage[0]  = LoadBMP("textures/00.bmp"  ))&&
		(TextureImage[1]  = LoadBMP("textures/01.bmp"  ))&&
		(TextureImage[2]  = LoadBMP("textures/02.bmp"  ))&&
		(TextureImage[3]  = LoadBMP("textures/03.bmp"  ))&&
		(TextureImage[4]  = LoadBMP("textures/04.bmp"  ))&&
		(TextureImage[5]  = LoadBMP("textures/05.bmp"  ))&&
		(TextureImage[6]  = LoadBMP("textures/06.bmp"  ))&&
		(TextureImage[7]  = LoadBMP("textures/07.bmp"  ))&&
		(TextureImage[8]  = LoadBMP("textures/p01.bmp" ))&&
		(TextureImage[9]  = LoadBMP("textures/p02.bmp" )))
	{

		glGenTextures(TEXCOUNT, &texture[0]);	// Create texture
		
		int i;
		// Loop through textures
		for( i = 0; i < TEXCOUNT; i++)					
		{
			glBindTexture(GL_TEXTURE_2D, texture[i]);	
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[i]->sizeX, 
											    TextureImage[i]->sizeY, 
												GL_RGB, GL_UNSIGNED_BYTE, 
												TextureImage[i]->data);
			// Mipmapped
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		for(i = 0; i < TEXCOUNT; i++)
		{
			// If texture exists
			if(TextureImage[i])	
			{
				// If image exists
				if(TextureImage[i]->data)
				{
					free(TextureImage[i]->data);	// Free texture image from memory
				}

				free(TextureImage[i]);				// Free image structure
			}
		}
	}
}


// ** RENDER SCENE FUNCTION **
// ---------------------------
void RenderScene()
{
	// Loop through polys
	for (int i = 0; i < polyCount; i++)
	{
		// NOTE: z-buffer writes are disabled when drawing transparent polys,
		//       this sorts out any drawing issues when rendering the particles
		if (map[i].trans == 1)
		{
			glDisable(GL_BLEND);		// Disable blending
			glDepthMask(GL_TRUE);		// Re-enable z-buffer writes
		}
		else
		{
			glEnable(GL_BLEND);			// Enable blending
			glDepthMask(GL_FALSE);		// Disable z-buffer writes
		}
		
		// Bind texture to object
		glBindTexture(GL_TEXTURE_2D, texture[map[i].texture]);

		// Load triangles from memory
		glBegin(GL_TRIANGLES);			
			glColor4f   (map[i].r[0], map[i].g[0], map[i].b[0], map[i].trans);
			glTexCoord2f(map[i].u[0], map[i].v[0]);
			glVertex3f  (map[i].x[0], map[i].y[0], map[i].z[0]);

			glColor4f   (map[i].r[1], map[i].g[1], map[i].b[1], map[i].trans);
			glTexCoord2f(map[i].u[1], map[i].v[1]);
			glVertex3f  (map[i].x[1], map[i].y[1], map[i].z[1]);
		
			glColor4f   (map[i].r[2], map[i].g[2], map[i].b[2], map[i].trans);
			glTexCoord2f(map[i].u[2], map[i].v[2]);
			glVertex3f  (map[i].x[2], map[i].y[2], map[i].z[2]);	
		glEnd();
	}
}


// ** RENDER SPACE **
// ----------------------
void RenderSpace(float speed)
{
	
	// Update camera position since we're at start of loop
	glRotatef(yaw,    1, 0, 0);
	glTranslatef(camPos.x, camPos.y, camPos.z);

	glDisable(GL_BLEND);						// Disable transparency
	glBindTexture(GL_TEXTURE_2D, texture[6]);	// Assign space texture

	glBegin(GL_QUADS);
		
		// Left Window
		glColor3f(1,1,1);
		glTexCoord2f(0 + space, 1); glVertex3f(-7, 9, -35);
		glTexCoord2f(2 + space, 1); glVertex3f(-7, 9, -70);
		glTexCoord2f(2 + space, 0); glVertex3f(-7, 6, -70);
		glTexCoord2f(0 + space, 0); glVertex3f(-7, 6, -35);

		// Right Window
		glTexCoord2f(0 + space, 1); glVertex3f(17, 9, -35);
		glTexCoord2f(2 + space, 1); glVertex3f(17, 9, -70);
		glTexCoord2f(2 + space, 0); glVertex3f(17, 6, -70);
		glTexCoord2f(0 + space, 0); glVertex3f(17, 6, -35);

	glEnd();

	space += 2 * speed;	// Move space (texture coordinates)
}


// ** RENDER FADE IN **
// --------------------
void RenderFade(float speed)
{
	if (fade > 0)
	{
		glDisable(GL_DEPTH_TEST);							// Disable depth testing so fade quad infront

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Allow for a transparent black
		glDisable(GL_TEXTURE_2D);							// Disable textures for full colour
				
		// Render fade quad
		glBegin(GL_QUADS);
			glColor4f(0, 0, 0, fade);
			glVertex3f(-100,  100, -1);
			glVertex3f( 100,  100, -1);
			glVertex3f( 100, -100, -1);
			glVertex3f(-100, -100, -1);
		glEnd();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);					// Normal blending

		// NOTE: before decreasing fade value we check that the frame interval is smaller than 10,
		//       this is because when the program first loads the speed will be too fast and the
		//       fade in will be finished before its even seen!
		if (speed < 10)	
		{
			fade -= 0.20f * speed;
		}

		glEnable(GL_TEXTURE_2D);	// Re-enable textures
		glDisable(GL_BLEND);		// Disable transparency
		glEnable(GL_DEPTH_TEST);	// Re-enable depth testing
	}
}


// ** RENDER REFLECTIVE DOORS **
// -----------------------------
void RenderDoors()
{
	glBindTexture(GL_TEXTURE_2D, texture[7]);	// Assign texture
	
	glEnable(GL_TEXTURE_GEN_S);					// Enable automatic texture coordination (sphere)
	glEnable(GL_TEXTURE_GEN_T);	
	
	glBegin(GL_QUADS);
		
		glColor3f(0.45f,0.45f,0.45f);
	
		// Left Door
		glTexCoord2f(0.0f, 0.5f); glVertex3f(-5.5f,  8.5f, -12.0f);
		glTexCoord2f(0.5f, 0.5f); glVertex3f(-5.5f,  8.5f, -18.0f);
		glTexCoord2f(0.5f, 0.0f); glVertex3f(-5.5f, -0.5f, -18.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.5f, -0.5f, -12.0f);

		// Right Door
		glTexCoord2f(0, 1); glVertex3f(15.5f,  8.5f, -12.0f);
		glTexCoord2f(1, 1); glVertex3f(15.5f,  8.5f, -18.0f);
		glTexCoord2f(1, 0); glVertex3f(15.5f, -0.5f, -18.0f);
		glTexCoord2f(0, 0); glVertex3f(15.5f, -0.5f, -12.0f);

		// End Door
		glTexCoord2f(0, 1); glVertex3f(10.5f,  10.5f, -225.5f);
		glTexCoord2f(1, 1); glVertex3f( 0.0f,  10.5f, -225.5f);
		glTexCoord2f(1, 0); glVertex3f( 0.0f, - 0.5f, -225.5f);
		glTexCoord2f(0, 0); glVertex3f(10.5f, - 0.5f, -225.5f);

		// Front Door
		glTexCoord2f(0, 1); glVertex3f(doorx + 10.5f,  10.5f, 24.5f);
		glTexCoord2f(1, 1); glVertex3f(doorx        ,  10.5f, 24.5f);
		glTexCoord2f(1, 0); glVertex3f(doorx        , - 0.5f, 24.5f);
		glTexCoord2f(0, 0); glVertex3f(doorx + 10.5f, - 0.5f, 24.5f);

		glColor3f(0.35f,0.35f,0.35f);

		glTexCoord2f(0, 1); glVertex3f(doorx + 10.5f,  10.0f, 24.5f);
		glTexCoord2f(1, 1); glVertex3f(doorx + 10.5f,  10.0f, 24.2f);
		glTexCoord2f(1, 0); glVertex3f(doorx + 10.5f, -0.5f, 24.2f);
		glTexCoord2f(0, 0); glVertex3f(doorx + 10.5f, -0.5f, 24.5f);

	glEnd();								

	glDisable(GL_TEXTURE_GEN_S);				// Disable tex coord gen
	glDisable(GL_TEXTURE_GEN_T);	

}


// ** OPEN DOOR **
// ---------------
void OpenDoor(float speed)
{
	if (Door == true)
	{
		if (doorx > - 10.5f) 
		{
			doorx -= 2.5f * speed;
		}
		else
		{
			Door = false;
		}
	}
}


// ** MOVE CAMERA **
// -----------------
void MoveCamera(float speed)
{
	if (Door == false)	// If not waiting for door then move...
	{
		camPos.z += 15.0f * speed;

		// Make camera bob to simulate walking
		if (walkbiasangle >= 359.0f)
		{
			walkbiasangle = 0.0f;
		}
		else
		{
			walkbiasangle += 250.0f * speed;
		}
		
		camPos.y = ((float)sin(walkbiasangle * piOver180)/2.2f) - 7.5f;

		// If camera at end of corridor bring it back to the beginning to start the loop again
		if (camPos.z > 220.4f) 
		{
			camPos.z = -29.6f;		// Original camera pos
			doorx    = 0;			// Close door
			Door     = true;		// Door open flag = true
		}
	}
}


// ** RENDER PIPE STEAM **
// -----------------------
void RenderSteam(float speed)
{
	glEnable(GL_BLEND);			// Enable transparency

	// Loop through steam particles
	for (int loop = 0; loop < MAX_STEAM_PARTICLES; loop++)
	{
		// Position of particle
		float x = (steam[loop].x -   3.5f);
		float y = (steam[loop].y +   2.0f);
		float z = (steam[loop].z - 130.0f);
		
		float pSize = 0.5f;	// Particle size

		glBindTexture(GL_TEXTURE_2D, texture[8]);	// Assign texture
					
		glColor4f(10, 10, 10, steam[loop].life);    // R,G,B,A

		// Render particles
		glBegin(GL_TRIANGLE_STRIP);						
			glTexCoord2d(1, 1); glVertex3f(x + pSize, y + pSize, z); 
			glTexCoord2d(0, 1); glVertex3f(x - pSize, y + pSize, z); 
			glTexCoord2d(1, 0); glVertex3f(x + pSize, y - pSize, z); 
			glTexCoord2d(0, 0); glVertex3f(x - pSize, y - pSize, z); 
		glEnd();										


		steam[loop].x += (steam[loop].xi / 30.0f) * speed;	// Move on the x axis by x speed
		steam[loop].y += (steam[loop].yi / 30.0f) * speed;	// Move on the y axis by y speed
		steam[loop].z += (steam[loop].zi / 30.0f) * speed;	// Move on the z axis by z speed

		steam[loop].life -= steam[loop].fade * speed;		// Reduce particles life by fade

		// If particle has depleted reset it
		if (steam[loop].life <= 0.0f)	
		{
			steam[loop].life = 0.75f;							// Full life
			steam[loop].fade = (rand()%100) / 100.0f + 0.75f;	// Random fade value
			steam[loop].x    = 0.0f;							// Reset X axis
			steam[loop].y    = 0.0f;							// Reset Y axis
			steam[loop].z    = 0.0f;							// Reset Z axis
			steam[loop].xi   = (rand()%60)  +  70.0f;			// X axis speed & direction
			steam[loop].yi   = (rand()%60)  +  80.0f;			// Y axis speed & direction
			steam[loop].zi   = (rand()%60)  -  30.0f;			// Z axis speed & direction
		}
	}		
}


// ** RENDER BUBBLES **
// --------------------
void RenderBubbles(float xPos, int num, float speed)
{
	// Loop through bubble particles
	for (int loop = 0; loop < MAX_BUBBLE_PARTICLES; loop++)
	{
		// Position of particle
		float x = (bubble[loop].x +   xPos);
		float y = (bubble[loop].y +   1.0f);
		float z = (bubble[loop].z - 174.0f);
	
		float pSize = 0.3f;	// Particle size

		glBindTexture(GL_TEXTURE_2D, texture[9]);	// Assign texture
					
		glColor4f(10, 10, 10, bubble[loop].life);	// R,G,B,A

		// Render particles
		glBegin(GL_TRIANGLE_STRIP);						
			glTexCoord2d(1, 1); glVertex3f(x + pSize, y + pSize, z); 
			glTexCoord2d(0, 1); glVertex3f(x - pSize, y + pSize, z); 
			glTexCoord2d(1, 0); glVertex3f(x + pSize, y - pSize, z); 
			glTexCoord2d(0, 0); glVertex3f(x - pSize, y - pSize, z); 
		glEnd();										


		// NOTE: The same function is called for each of the two sets of bubbles, 
		//       i.e. they are identical, so if we are on the 2nd call - update the 
		//       bubbles positions.
		if (num == 2)
		{
			bubble[loop].x += (bubble[loop].xi / 30.0f) * speed;	// Move on the x axis by x speed
			bubble[loop].y += (bubble[loop].yi / 30.0f) * speed;	// Move on the y axis by y speed
			bubble[loop].z += (bubble[loop].zi / 30.0f) * speed;	// Move on the z axis by z speed

			bubble[loop].life -= bubble[loop].fade * speed;			// Reduce particles life by fade

			// If particle has depleted reset it
			if (bubble[loop].life <= 0.0f)	
			{
				bubble[loop].life = 1.20f;							// Full life
				bubble[loop].fade = (rand()%100) / 100.0f + 0.75f;	// Random fade value
				bubble[loop].x    = (rand()%2) - 1.0f;				// Reset X axis
				bubble[loop].y    = 0.0f;							// Reset Y axis
				bubble[loop].z    = (rand()%2) - 1.0f;				// Reset Z axis
				bubble[loop].xi   = (rand()%50)  -  25.0f;			// X axis speed & direction
				bubble[loop].yi   = (rand()%60)  +  80.0f;			// Y axis speed & direction
				bubble[loop].zi   = (rand()%60)  -  30.0f;			// Z axis speed & direction
			}
		}
	}		
}
