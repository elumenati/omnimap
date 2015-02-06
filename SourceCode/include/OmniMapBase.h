#ifndef __OMNIMAP_BASE_H
#define __OMNIMAP_BASE_H
// OmniMap Geometry Correction Libs
// (c)2006 Elumenati

/*! \mainpage OmniMap API Real-Time Geometry Correction Library
 *
 * <center>For additional information, please see:<br><b><a href="../Omnimap_API_Integration.pdf" target="_blank">Installation Guide PDF</a></b> and the <b><a href="../Omnimap_API_Configuration.pdf" target="_blank">Configuration Guide PDF</a></b>
 *
 * For further information about immersive projection products and services from The Elumenati, please visit <br><b><a href="http://www.elumenati.com" target="_blank">http://www.elumenati.com</a></b>. </center>
 *
 * <img src="omnimap.jpg" valign="right">
 *
 *
 * \section intro_sec Introduction
 *
 * The OmniMap Geometry Correction Library provides developers with an easy-to-integrate solution to make real-time OpenGL applications compatible with OmniFocus projection systems.

Typical video projection systems display rectangular images on flat screens. However for planetariums, domes, panoramas, and other non-planar display surfaces optical and geometric correction techniques must be employed to prevent video projected on the curved display from appearing distorted. OmniMap is an API for use with OmniFocus projection systems or other custom immersive projection environments that maps real-time content onto arbitrary projection surface shapes.

Geometry correction routines are encapsulated in an easy-to-integrate C++ library. The pre-rendering API calls render the virtual environment into off screen temporary buffers, while the OmniMap API's post-rendering calls account for lens optical distortion and screen geometry correction. All configuration and algorithm options are handled by configurationfiles and scripts. Configuration changes can be made without the need to recompile or restart the program using the drop down console window. OpenGL 2.0 shaders are leveraged to take full advantage of new functionality provided by emerging graphics hardware. The cross-platform system is hardware accelerated using OpenGL 2.0 shaders to take full advantage of new functionality provided by emerging graphics hardware.
 *
 * \section features_sec Features
 * 
 * <table><tr><td class="indexkey">Hardware Accelerated</td>
<td class="indexvalue">- Uses OpenGL 2.0<br>- Vertex and Pixel Shaders</td></tr>
	<tr>
	<td class="indexkey">OpenGL-based and Cross-Platform</td>
	<td class="indexvalue">- OpenGL-based<br>- Currently compiled for Windows, future versions for OSX and Linux</td>
	</tr>
	<tr>
	<td class="indexkey">Arbitrary Projection Surfaces</td>
	<td class="indexvalue">- Dome of any size or shape<br>- Flat Walls<br>- Any 3D mesh</td>
	</tr>
	<tr>
	<td class="indexkey">Multiple Dome Modes</td>
	<td class="indexvalue">- 180x180<br>- 180x135<br>- Can define any others</td>
	</tr>
	<tr>
	<td class="indexkey">Adjustable Dome Orientation</td>
	<td class="indexvalue">- Vertical Dome Truncation<br>- Horizontal Dome Truncation<br>- Arbitrary Tilt</td>
	</tr>
	<tr>
	<td class="indexkey">Adjustable projector orientation and position</td>
	<td class="indexvalue">- Arbitrary off-axis projector positioning<br>- Projector does not have to be at the dome origin</td>
	</tr>
	<tr>
	<td class="indexkey">Adjustable Audience Position</td>
	<td class="indexvalue">- "Sweet spot" configuration for variable audience locations</td>
	</tr>
	<tr>
	<td class="indexkey">Flexible Render Channelsm</td>
	<td class="indexvalue">- 1,2,3,4,5, or 6 channel geometry correction render channels<br>- Off-axis render channels</td>
	</tr>
	<tr>
	<td class="indexkey">Unit Conversions</td>
	<td class="indexvalue">- Screen, audience, and projector geometry can be specified in terms of feet, inches, meters, or dome radii</td>
	</tr>
	<tr>
	<td class="indexkey">Heads-Up Display</td>
	<td class="indexvalue">- Render Channel Preview<br>- Console Window for interactive tweaking of configuration parameters</td>
	</tr>
	<tr>
	<td class="indexkey">Lua Scripting</td>
	<td class="indexvalue">- Open access to algorithm details if desired through Lua scripts (<a href="http://www.lua.org" target="_blank">http://www.lua.org</a>)</td>
	</tr>
	</table>

 * 
 * \section inquiries_sec Inquiries
 * 
 * OmniMap is one of the many software tools we make available to our clients. For more information, please contact us via the client inquiry form at <a href="http://www.elumenati.com/connect" target="_blank">http://www.elumenati.com/connect</a><p></p>
 * 
 * &nbsp;
 * &nbsp;
 *
 * 
 *
 */


#pragma once
#include <GL/glew.h>   // make sure to link to static glew and define glewbuild
#include <windows.h>
#include <GL/gl.h>


#ifdef OMNIMAP_EXPORTS
#define OMNIMAP_API __declspec(dllexport)
#else
#define OMNIMAP_API __declspec(dllimport)
#endif

#include "IncludePropertySet.h"


#define OMNIMAP_DEFAULT_RENDERCHANNEL_RESOLUTION 512
#define OMNIMAP_DEFAULT_RENDERCHANNEL_USE_FBO true


#define OMNIMAP_DEFAULT_ConsolidateCameraIntoProjectionMatrix false

#define OMNIMAP_DEFAULT_STARTUPSCRIPT "OmniMapConfig/omnimap_startup.lua"



#define OMNIMAP_DEFAULT_QUADLIST_TESSELATION 10

#define OMNIMAP_DEFAULT_clearcolor_r 0.0f
#define OMNIMAP_DEFAULT_clearcolor_g 0.0f
#define OMNIMAP_DEFAULT_clearcolor_b 0.0f
#define OMNIMAP_DEFAULT_clearcolor_a 1.0f

#define OMNIMAP_DEFAULT_AudiencePosition_x 0.0f
#define OMNIMAP_DEFAULT_AudiencePosition_y 0.0f
#define OMNIMAP_DEFAULT_AudiencePosition_z 0.0f

#define OMNIMAP_DEFAULT_ProjectorPosition_x 0.0f
#define OMNIMAP_DEFAULT_ProjectorPosition_y 0.0f
#define OMNIMAP_DEFAULT_ProjectorPosition_z 0.0f
#define OMNIMAP_DEFAULT_LookAtPosition_x 0.0f
#define OMNIMAP_DEFAULT_LookAtPosition_y 0.0f
#define OMNIMAP_DEFAULT_LookAtPosition_z 1.0f
#define OMNIMAP_DEFAULT_HeadsUpPosition_x 0.0f
#define OMNIMAP_DEFAULT_HeadsUpPosition_y -1.0f
#define OMNIMAP_DEFAULT_HeadsUpPosition_z 0.0f

//#define OMNIMAP_DEFAULT_fovY 90.0 /// removed...
//#define OMNIMAP_DEFAULT_AspectRatio 1.0/// removed...
#define OMNIMAP_DEFAULT_ClipNear 0.100000f
#define OMNIMAP_DEFAULT_ClipFar 1000.000000f
#define OMNIMAP_DEFAULT_ClipLeft -0.147004f
#define OMNIMAP_DEFAULT_ClipRight 0.147004f
#define OMNIMAP_DEFAULT_ClipTop    0.147004f
#define OMNIMAP_DEFAULT_ClipBottom -0.147004f

#define OMNIMAP_DEFAULT_CHANNEL_RES  512


#define OMNIMAP_DEFAULT_displayHUD true




class OmniMap_Scripting;
class OmniMapChannelBase;
class RenderChannel;

class OMNIMAP_API OmniMapString
{
    OmniMapString(const char *string);
private:
    
};

/**
 * \brief Message logger.
 *
 * Class for logging messages and errors to a file.
 */
class OMNIMAP_API   OmniMap_Log
{
public:
	/**
	 * Provides access to properties not defined in the public interface.
	 * The properties accessible for the OmniMap_Log class are:
	 */
	IncludePropertySet access;
	
//#ifdef OMNIMAP_EXPORTS
private:
public:
	
	/**
	 * Construct an OmniMap log object.
	 */
	OmniMap_Log::OmniMap_Log();
	/**
	 * Destroy an OmniMap log object.
	 */
	OmniMap_Log::~OmniMap_Log();

	/**
	 * Clear the log
	 */
	void OmniMap_Log::clear();

//#endif


	/////////// export ///////
public:
	/**
	 * Send a string to the message log.
	 *
	 * @param fmt The formatting string in 'printf' style.
	 */
	void OmniMap_Log::ReportMessage(const char *fmt, ...);
	/**
	 * Send a string to the error log.
	 *
	 * @param fmt The formatting string in 'printf' style.
	 */
	void OmniMap_Log::ReportError(const char *fmt, ...);
};

/**
 * \brief Data container for simple types.
 *
 * This is a generic class for containing integer, floating point, string and boolean
 * data.
 */
class OMNIMAP_API GenericDataContainer
{
private:
	int				m_int;
	float			m_float;
	void *		m_string;
	bool			m_bool;

public:
	/**
	 * An enumeration for specifying the type of data contained.
	 */
	enum Type
	{
		INT,
		FLOAT,
		STRING,
		BOOL,
		EMPTY
	};
	/**
	 * The type of data contained.
	 */
	Type	m_type;
public:
	/**
	 * \brief Contstructs a GenericDataContainer object of type t.
	 *
	 * @param t The type of data to be contained.
	 */
	GenericDataContainer::GenericDataContainer(Type t);
	/**
	 * \brief Destroys the GenericDataContainer
	 */
	GenericDataContainer::~GenericDataContainer();


	/**
	 * Set the value of the data.
	 *
	 * @param value The value to set this GenericDataContainer to.
	 */
	void GenericDataContainer::Set(int value);
	/**
	 * \brief Set the value of the data.
	 *
	 * @param value The value to set this GenericDataContainer to.
	 */
	void GenericDataContainer::Set(bool  value);
	/**
	 * \brief Set the value of the data.
	 *
	 * @param value The value to set this GenericDataContainer to.
	 */
	void GenericDataContainer::Set(float  value);
	/**
	 * \brief Set the value of the data.
	 *
	 * @param value The value to set this GenericDataContainer to.
	 */
	void GenericDataContainer::Set(const char * value);

	/**
	 * \brief Get the integer value.
	 */
	int  GenericDataContainer::GetINT();
	/**
	 * \brief Get the boolean value.
	 */
	bool  GenericDataContainer::GetBOOL();
	/**
	 * \brief Get the float value.
	 */
	float GenericDataContainer::GetFLOAT();
	/**
	 * \brief Get the string value.
	 */
	const char *  GenericDataContainer::GetSTRING();
};

/**
 * \brief Container for holding multiple data values.
 *
 * A data container that can hold multiple values.  It uses the std::map class to
 * hold the data values.
 */
class OMNIMAP_API ExtraDataMapContainer
{	
private:
	void * ExtraDataMap;// of type map<std::string,GenericDataContainer>
public:
	/**
	 * Construct an ExtraDataMapContainer object.
	 */
	ExtraDataMapContainer::ExtraDataMapContainer();
	/**
	 * Destroy an ExtraDataMapContainer object.
	 */
	ExtraDataMapContainer::~ExtraDataMapContainer();
	/**
	 * \brief Creates a GenericDataContainer for a float value, and adds a reference
	 * to that GenericDataContainer to the data map.
	 *
	 * @param key The key for accessing the value.
	 * @return A pointer to the GenericDataContainer created.
	 */
	GenericDataContainer * ExtraDataMapContainer::AddFLOAT(const char * key);
	/**
	 * \brief Creates a GenericDataContainer for a boolean value, and adds a reference
	 * to that GenericDataContainer to the data map.
	 *
	 * @param key The key for accessing the value.
	 * @return A pointer to the GenericDataContainer created.
	 */
	GenericDataContainer * ExtraDataMapContainer::AddBOOL(const char * key);
	/**
	 * \brief Creates a GenericDataContainer for a integer value, and adds a reference
	 * to that GenericDataContainer to the data map.
	 *
	 * @param key The key for accessing the value.
	 * @return A pointer to the GenericDataContainer created.
	 */
	GenericDataContainer * ExtraDataMapContainer::AddINT(const char * key);
	/**
	 * \brief Creates a GenericDataContainer for a std::string value, and adds a reference
	 * to that GenericDataContainer to the data map.
	 *
	 * @param key The key for accessing the value.
	 * @return A pointer to the GenericDataContainer created.
	 */
	GenericDataContainer * ExtraDataMapContainer::AddSTRING(const char * key);
	/**
	 * \brief Returns GenericDataContainer a value in the data map.
	 *
	 * @param key The key for the value sought.
	 * @return A pointer to the GenericDataContainer sought.
	 */
	GenericDataContainer * ExtraDataMapContainer::IndexDataMap(const char * key);
};

/**
 * \brief OmniMap's 3D vector utility class.
 */
class OMNIMAP_API OmniVec3 {

public:
	/** 
	 * \brief The x coordinate of the vector.
	 */
	float x;
	/** 
	 * \brief The y coordinate of the vector.
	 */
	float y;
	/** 
	 * \brief The z coordinate of the vector.
	 */
	float z;

	/**
	 * Construct an OmniVec3 with undefined values.
	 */
	OmniVec3();
	/**
	 * Construct an OmniVec3 with the specified values
	 * @param x The x value
	 * @param y The y value
	 * @param z The z value
	 */
	OmniVec3(float x, float y, float z);
	/**
	 * Construct an OmniVec3 with the specified values
	 * @param incoord The vector coordinates in a 3 element array.
	 */
	OmniVec3(float incoord[3]);
	/**
	 * Construct a vector whose value is defined by a ray between two points
	 * @param pt1 The first point on the ray.
	 * @param pt2 The second point on the ray.
	 */
	OmniVec3(OmniVec3 pt1, OmniVec3 pt2);	//creates a vector between pt1 and pt2  (Pt1 - Pt2)

	/**
	 * \brief Calculate the cross product of this and the given vector.
	 *
	 * @param invec The vector with which to cross this vector.
	 * @return The cross product of this vector and the given vector.
	 */
	inline OmniVec3 cross(OmniVec3 invec)
	{ return OmniVec3(y*invec.z - z*invec.y, z*invec.x - x*invec.z, x*invec.y - y*invec.x); }
	/**
	 * \brief Calculate the dot product of this vector and the given vector.
	 *
	 * @param invec The vector with which to calculate the dot product.
	 * @return The dot product of this and the given vector.
	 */
	inline float dot(OmniVec3 invec)
	{ return x*invec.x + y*invec.y + z*invec.z; }

	/**
	 * \brief Return the normalized value for this vector.
	 *
	 * @returns The normalized value for this vector.
	 */
	OmniVec3 returnNormalized();
	/**
	 * Normalize this vector.
	 */
	void normalize();
	/**
	 * \brief Get the length of this vector.
	 *
	 * @return The length of the vector.
	 */
	float length();
	/**
	 * \brief Add this vector to the vector provided.
	 *
	 * @param invec The vector to add this vector to.
	 * @return The sum  of this vector and the vector provided.
	 */
	OmniVec3& operator+=(OmniVec3 invec);
	/**
	 * \brief Subtract the vector provided from this vector.
	 *
	 * @param invec The vector to subtract from this vector.
	 * @return The sum  of this vector and the vector provided.
	 */
	OmniVec3& operator-=(OmniVec3 invec);
	/**
	 * \brief Assignment operator
	 */
	OmniVec3& operator=(OmniVec3 invec);
	/**
	 * \brief Make a 4-element array from this vector.
	 *
	 * Fill the 4-element array with x, y, z and 0 into the first, second, third and fourth
	 * element of the array.
	 * @param v The array to fill with this vector's values.
	 */
	void make4tuple(float v[]);
	/**
	 * \brief Set the value of this vector to the given vector's values.
	 *
	 * @param vec The values to set to.
	 */
	void set(OmniVec3 vec);

	void set(float x, float y, float z);
	/**
	 * \brief Set the value of this vector to the given values.
	 *
	 * @param x The x value to set to.
	 * @param y The y value to set to.
	 * @param z The z value to set to.
	 */
	void add(OmniVec3 vec);
	/**
	 * \brief Calculate a linear interpolation between this and the given vector.
	 *
	 * @param the value to interpolate between from and to vectors. Should be between 0 and 1.
	 * @param The end of the interpolation(The given vector).
	 * @return The interpolated position.
	 */
	OmniVec3 lerp(float amount, OmniVec3 endvec);
	


	/**
	 * \brief Calculates a vector using a weighted average.  
	 *
	 * @param amount is 0-1.  0 returns the this vector. 1 returns the endvec.  Inbetween, the weighted average.
	 * @returns the weighted average of this + endvec
	 */
	OmniVec3 average(float amount, OmniVec3 endvec);

	/**
	 * \brief Calculate a vector that is this vector rotated by the given angle about the given vector.
	 *
	 * @param vector The vector to rotate about.
	 * @param angle The angle to rotate in radians.
	 * @return a vector which is the result of rotating this vector about the provided vector by the given angle (in radians)
	 */
	OmniVec3 rotateByVector(OmniVec3 vector, float angle);

	/**
	 * \brief Calculate this vector's angle in radians
	 *
	 * @return This vector's angle in radians
	 */
	float angleBetween(OmniVec3 invec);
};

/**
 * \brief A 4 vertice polygon.
 */
class OMNIMAP_API OmniQuad
{
	public:
	/**
	 * Construct an OmniQuad object with undefined vertices.
	 */
	OmniQuad::OmniQuad();
	/**
	 * Destroy an OmniQuad object.
	 */
	OmniQuad::~OmniQuad();
	/**
	 * Construct an OmniQuad object with the specified vertex coordinates.
	 *
	 * @param blx The bottom left coordinate's x value
	 * @param bly The bottom left coordinate's y value
	 * @param blz The bottom left coordinate's z value
	 * @param tlx The top left coordinate's x value
	 * @param tly The top left coordinate's y value
	 * @param tlz The top left coordinate's z value
	 * @param trx The top right coordinate's x value
	 * @param tr_y The top right coordinate's y value
	 * @param trz The top right coordinate's z value
	 * @param brx The bottom right coordinate's x value
	 * @param bry The bottom right coordinate's y value
	 * @param brz The bottom right coordinate's z value
	 */
	OmniQuad::OmniQuad(
		float blx,float bly,float blz,
		float tlx,float tly,float tlz,
		float trx,float tr_y,float trz,
		float brx,float bry,float brz
		);
	/**
	 * \brief Set the quad's coordinates
	 *
	 * @param blx The bottom left coordinate's x value
	 * @param bly The bottom left coordinate's y value
	 * @param blz The bottom left coordinate's z value
	 * @param tlx The top left coordinate's x value
	 * @param tly The top left coordinate's y value
	 * @param tlz The top left coordinate's z value
	 * @param trx The top right coordinate's x value
	 * @param tr_y The top right coordinate's y value
	 * @param trz The top right coordinate's z value
	 * @param brx The bottom right coordinate's x value
	 * @param bry The bottom right coordinate's y value
	 * @param brz The bottom right coordinate's z value
	 */
	void set(
		float blx,float bly,float blz,
		float tlx,float tly,float tlz,
		float trx,float tr_y,float trz,
		float brx,float bry,float brz
		);
	void QuadToMatrix();
	OmniVec3 FourPointInterpolation(OmniVec3 p00,OmniVec3 p01,OmniVec3 p11,OmniVec3 p10, 
		double x, double y);
	void QuadToEquations(float UPlaneEquation[4] ,float VPlaneEquation[4],float PlaneEquation[4]);
	/**
	 * \brief The top left corner of the quad
	 */
	OmniVec3 tl;
	/**
	 * \brief The bottom left corner of the quad
	 */
	OmniVec3 bl;
	/**
	 * \brief The top right corner of the quad
	 */
	OmniVec3 tr;
	/**
	 * \brief The bottom right corner of the quad
	 */
	OmniVec3 br;

protected:
	void QuadToPlaneEquation(float PlaneEquation[4]);

};


/**
 * \brief Defines a 3D viewing postion and orientation.
 * 
 * Defines an eye point, a reference point indicating the
 * center of the scene, and an UP vector.
 */
class OMNIMAP_API Omni_ViewerPosition
{
public:
	/**
	 * headsUp is the up direction of the viewer. Default is 0,0,0
	 */
	OmniVec3  headsUp;
	/**
	 * a reference point indicating the
	 * center of the scene the viewer is focused on. Default is 0,0,0
	 */
	OmniVec3  lookAtpos;
	/**
	 * pos is the position of the viewer. Default is 0,0,0
	 */
	OmniVec3  pos;
public:
	/**
	 * Creates a viewing matrix derived from an eye point, a reference point indicating the
	 * center of the scene, and an UP vector
	 */
	void doGluLookAt();
	/**
	 * Construct an Omni_ViewerPosition object
	 */
	Omni_ViewerPosition::Omni_ViewerPosition();
	/**
	 * Destroy an Omni_ViewerPosition object
	 */
	~Omni_ViewerPosition();

};

/**
 * \brief A camera. 
 *
 * Defined by a viewing frustum into the world coordinate system, 
 * and a viewing matrix.
 */
class OMNIMAP_API   OmniMap_CameraInfo
{
public:


	///// dll export

	/** 
	 * \brief The left vertical clipping plane.
	 */
	float ClipLeft;
	/** 
	 * \brief The right vertical clipping plane.
	 */
	float ClipRight;
	/** 
	 * \brief The top horizontal clipping plane.
	 */
	float ClipTop;
	/** 
	 * \brief The bottom horizontal clipping plane.
	 */
	float ClipBottom ;


	/**
	 * \brief The distance from the viewer to the near clipping plane (always positive).
	 */
	float ClipNear;
	/**
	 * \brief The distance from the viewer to the far clipping plane (always positive).
	 */
	float ClipFar;
	/**
	 * \brief The matrix defining the rotation of the camera.
	 */
	float Matrix_WorldView[16];
	/**
	 * Constructor.
	 */
	OmniMap_CameraInfo::OmniMap_CameraInfo();
	/**
	 * Destructor
	 */
	OmniMap_CameraInfo::~OmniMap_CameraInfo();
	/**
	 * \brief Apply this camera's projection matrix. 
	 *
	 * Multiplies the current matrix by the matrix generated by the viewing frustum
	 * defined by this object.
	 */
	void OmniMap_CameraInfo::ApplyProjectionMatrix();
	/**
	 * \brief Apply this camera's view matrix.
	 *
	 * Multiplies the current matrix by the matrix in Matrix_WorldView.
	 */
	void OmniMap_CameraInfo::ApplyViewMatrix();
	
};


/**
 * \brief An abstract class that defines a generic shader interface.
 *
 * The base class for managing shaders.  This class is used by the final rendering pass for
 * loading, setting parameters for, and unloading the shaders used for compositing and projecting
 * the final image to the display.  Graphics system specific shader classes can be derived from this
 * class.  The OmniMapBase class expects shaders to be derived from this base class.
 */
class OMNIMAP_API OmniMapShaderBase
{
public:
	//OmniMap_Shader::OmniMap_Shader();
	//OmniMap_Shader::~OmniMap_Shader();
	/**
	 * \brief Set the file name of the vertex and fragment shaders.
	 */
	void setShadersFilenames(
		const char * _VertexShaderFilename ,	const char * _FragmentShaderFilename);

	/**
	 * \brief Initialize the shader
	 *
	 * Initialize the shader.  It is expected that the filenames for the vertex and fragment shaders
	 * have been set when this method is called.
	 */
	virtual void init() = 0;


	/**
	 * \brief Enable the Shader.  
	 *
	 * Derived classes are expected to enable the shader for subsequent 3D rendering.
	 */
	virtual void EnableShader() = 0;
	/**
	 * \brief Disable the Shader.  
	 *
	 * Derived classes are expected to disable the shader.
	 */
	virtual void DisableShader() = 0;
	/*
	virtual void SetTextureIds(int numChannels, int *ids) = 0;
	virtual void SetPlaneEquations(int numChannels, float *plane_Eq_XYZ,
		float *plane_Eq_U, float *plane_Eq_V) = 0;
	virtual void SetUserPosition(OmniVec3 position) = 0;
	*/

protected:
	char * VertexShaderFilename;
	char * FragmentShaderFilename;
};


/**
 * \brief Main controller class for the dome Screen
 *
 * This is the abstract base class for defining a main controller class for the dome Screen.  
 * It handles the image-based projection technique
 * of projecting the scene generated by the channels onto the curved surface. 
 */
class OMNIMAP_API   OmniMap_Screen 
{
public:
	/**
	 * \brief Enables access to screen properties.
	 *
	 * Manages and allows the setting of screen properties.  The abstract class OmniMap_Screen
	 * has no properties.  This member is to be used by classes derived from OmniMap_Screen.
	 */
	IncludePropertySet access;
	/**
	 * Container for extra scriptable data elements.
	 */
	ExtraDataMapContainer ScreenMetaData;
protected:
	
	virtual void SetUpPropertyAccess() = 0;

public:
	/**
	 * Project the scene generated by the channels onto the display.
	 */
	virtual void Display() = 0;
	OmniMap_Screen::OmniMap_Screen();
	virtual OmniMap_Screen::~OmniMap_Screen();

	/////////// export /////////

};

/**
 * \brief The base class for channels.
 *
 * The OmniMapBase class pure virtual method OmniMapBase::Create_Channel
 * must return a pointer to an object of this type.  Classes derived from the
 * OmniMapBase class must also derive a class from OmniMapChannelBase. The class derived
 * from OmniMapBase must implement OmniMapBase::Create_Channel which constructs an
 * object of that derived type and returns a pointer to it.  Classes derived from OmniMapChannelBase
 * must implement beginRenderToChannel, endRenderToChannel, bindTexture, unbindTexture, and
 * DrawHeadsUpDisplay.
 */
class OMNIMAP_API OmniMapChannelBase {
public:
	
	/**
	 * Construct an OmniMap_Channel Object.
	 */
	OmniMapChannelBase::OmniMapChannelBase();
	/**
	 * Destroy this channel
	 */
	~OmniMapChannelBase();
	/**
	 * \brief Recomputes plane equations for projection of the channel.
	 *
	 * Recomputes plane equations for projection of the channel onto the
	 * screen.  Needs to be called whenever UPlaneEquation, VPlaneEquation, PlaneEquation
	 * are changed.
	 */
	void RecomputePlaneEquations();

	// PUBLIC MEMBER  VARIABLES --------------------------------------------------------
	/**
	 * \brief Container for extra scriptable data elements.
	 */
	ExtraDataMapContainer ChannelMetaData;
	/**
	 * \brief The property set for managing LUA-scriptable parameters for the OmniMap_Channel
	 * class.
	 *
	 * See the OmniMap_Channel class description for details on what parameters can be set.
	 */
	IncludePropertySet access;

	/**
	 * \brief The camera settings for this channel.
	 *
	 * Defines the frustum and view rotation for this channel.  These values are used
	 * to set the OpenGL projection matrix prior to this channel being rendered.
	 */
	OmniMap_CameraInfo cam_info;

	

	//	int GL_TEXTURE_ARB_NUMBER;
	float UPlaneEquation[4]; // needs recompute every time the ViewWindow_Quad changes
	float VPlaneEquation[4]; // needs recompute every time the ViewWindow_Quad changes
	float PlaneEquation[4];  // needs recompute every time the ViewWindow_Quad changes

	OmniQuad ViewWindow_Quad;
	/**
	 * \brief The quad on to which the channels contents will be displayed in
	 * the heads up display.
	 */
	OmniQuad HeadsUpDisplay_Quad;
public:
	// PURE VIRTUAL METHODS -----------------------------------------------------------------
	/**
	 * \brief Initialize the channel.  
	 *
	 * Derived classes can do initializations here. This can
	 * be called from LUA scripts after calls to Create_Channel.
	 */
	virtual void Initialize() {};
	/**
	 * \brief Setup matrices and graphics state for rendering this channel.
	 *
	 * This method will be called before rendering to the channel begins. 
	 * Inheriting classes must implement this method.
	 * Inheriting classes can push the current graphics state, and set up special
	 * graphics states here.
	 */
	virtual void beginRenderToChannel() = 0;
	/**
	 * \brief Return the matrix and graphics state to its condition prior to the call to
	 * OmniMapChannelBase::beginRenderToChannel.
	 * 
	 * This method will be called after rendering to the channel finishes.  
	 * Inheriting classes should pop the graphics state here.
	 */
	virtual void endRenderToChannel() = 0;

	/**
	 * \brief Make the texture unit for this channel the active texture unit.
	 *
	 * Makes the texture unit for this channel the active texture unit
	 * and then binds the texture id that contains the content for this channel to that
	 * active texture unit.
	 */
	virtual void BindTexture(int index) = 0;
	/**
	 * \brief Unbind the texture for this channel.
	 */
	virtual void UnbindTexture(int index) = 0;
	/**
	 * \brief Draw the contents of this channel to the heads up display.
	 */
	virtual void DrawHeadsUpDisplay() = 0;

	


	
protected:
	// PROTECTED MEMBER VARIABLES -----------------------------------------------------------
	

	// PROTECTED METHODS
	/**
	 * Sets up access to the properties of a channel.  
	 */
	void SetUpPropertyAccess();
	

};

/**
 * \brief An abstract class from which can be derived specific projection mapping classes.
 *
 * An abstract class for deriving specific projection mapping implementations from. Developers who want
 * to derive specific implementations will need to create derivations of the OmniMapShaderBase,
 * OmniMapChannelBase, and OmniMap_Screen classes, as well as implementing the pure virtual methods :
 * OmniMapBase::Create_Channel, OmniMapBase::Create_ScreenShape, OmniMapBase::PostRender, 
 * and OmniMapBase::Clear.  If the developer requires additional scripting parameters, 
 * OmniMapBase::SetUpPropertyAccess must also be overriden.  If it is overridden, the developer
 * is encouraged to call the base class implementation in order to have the scripting paramaters for
 * the base class set.
 *
 * The OmniMapBase class has the following LUA-scriptable paramaters:
 * <ul>
 * 		<li><b>clearcolor_r,clearcolor_g,clearcolor_b,clearcolor_a</b> : 
 *			The clear color to be used to clear the frame before the final compositing of the
 *			 channel images into the final spherical projection.
 *		</li>
 *		<li><b>AudiencePosition_x, AudiencePosition_y, AudiencePosition_z</b> : The audience position
 *		</li>
 *		<li><b>ProjectorPosition.lookAtpos.x, ProjectorPosition.lookAtpos.y, ProjectorPosition.lookAtpos.z</b> :
 *			The direction in which the projecter is pointed.
 *		</li>
 *		<li><b>ProjectorPosition.headsUp.x, ProjectorPosition.headsUp.y, ProjectorPosition.headsUp.z</b> :
 *			The up vector for the projector.
 *		</li>
 *		<li><b>ProjectorPosition.pos.x, ProjectorPosition.pos.y, ProjectorPosition.pos.z</b> :
 *			The position of the projector.
 *		</li>
 *		<li><b>displayHUD</b> : True to show the results of the render channels in a heads up
 *			display, False to not show them.
 *		</li>
 *		<li><b>resWidth</b> : The width of the projector screen
 *		</li>
 * </ul>
 */
class OMNIMAP_API OmniMapBase {
public:
	/**
	 * Construct an OmniMap object.
	 * 
	 * @param _resW The width of the final rendered image.
	 * @param _resH The height of the final rendered image.
	 * @param strStartUpScriptFile The name of the file containing the startup script for
	 *			the OmniMap library.  On Win32 systems, the default is OmniMapConfig/omnimap_startup.lua.
	 *			The startup file loads some Lua Support files.  The location of these files is given by the luaResDir
	 *			parameter.  It also inlcudes two installation-specific files: omnimap_user_edit.lua, and omnimap_dome_wiz_ai.lua.
	 *			If you decide to move the startup file, you must make sure it includes these files from the correct
	 *			location.
	 * @param luaResDir This is the directory that contains Lua support files.  The default is NULL, in which case
	 *		the directory is determined by getting the Windows registry value "LuaResources" in the key 
	 *		"SOFTWARE\Elumenati\Elumenati OmniMap API\OmniMapAPI".  In the OmniMap API release this directory is
	 *		OmniMapAPI/LuaResources.  If the application developer whishes to move these files, this argument needs to
	 *		be passed to the OmniMap constructor.  The path must use forward slashes as the directory separator.
	 */
	OmniMapBase(int _resW, int _resH,
		const char* strStartUpScriptFile= OMNIMAP_DEFAULT_STARTUPSCRIPT,
		const char *luaResDir = NULL);
	/**
	 * Destroys an OmniMapBase object.  
	 */
	~OmniMapBase();
	
	/**
	 * Clear the list of rendering channels.
	 */
	void Clear_Channels();
	/**
	 * Clear the list of screens to render to.
	 */
	void ClearScreenShapes();
	/**
	  * @param strChannelName The name of the channel to return.
	  *
	  * @returns a pointer to the channel named name,
	  *	null if that channel doesn't exist.
	  */
	OmniMapChannelBase *GetChannel(const char* strChannelName);
	/**
	 * Recompute the plane equations for each of the channels.  This must be called after 
	 * all channels have been created.
	 */
	void RecomputePlaneEquations();
	/**
	 * \brief Create a channel.
	 *
	 * This method will call CreateChannel to do the actual creation
	 * of the channel.  CreateChannel is a pure virtual that must be defined by classes
	 * derived from OmniMapBase.
	 * @param name The name of the channel.
	 * @param res The channel's width and height in pixels.
	 */
	void BaseCreateChannel(const char * name);
	/**
	 * \brief Create an OmniMap_Screen to render to.
	 *
	 * This method calls the inheriting classes'
	 * Create_ScreenShape method.  This enables derived implementations of the OmniMapBase
	 * class to create customm screens derived from OmniMap_Screen.
	 * @param name The name of the screen to be created.
	 */
	virtual void BaseCreateScreenShape(const char * name);
	/**
	 * \brief Bind all channel textures to a multitexturing texture unit.
	 *
	 * This method makes a call the to all channels' OmniMapChannel::BindTexture methods.
	 * Each channel is responsible for binding its texture to a multitexturing texture unit.
	 */
	void BindAllChannelTextures();
	/**
	 * \brief Unbind all channel textures from a multitexturing texture unit.
	 *
	 * This method makes a call the to all channels' OmniMapChannel::UnBindTexture methods.
	 * Each channel unbinds its texture from a multitexturing texture unit.
	 */
	void UnBindAllChannelTextures();
	/**
	 * \brief Call main applications rendering method for each channel.
	 *
	 * Calls a function for each channel, added to this OmniMapBase projection, and passes
	 * a pointer to that channel.  This method is called by the application to begin the
	 * rendering of each channel that will be composited and projecte onto the display.
	 *
	 * @param fun The function to be called.
	 */
	void ForEachChannel(void fun(OmniMapChannelBase *chan));
	/**
	 * \brief Draw the heads up displays.
	 *
	 * Draws the heads up display that shows the contents of each individual channel.
	 * This method calls the OmniMapChannelBase::DrawHeadsUpDisplay for each channel.
	 */
	void DrawHeadsUpDisplays();
	/**
	 * \brief Renders the channel outputs to the final screen.
	 *
	 * This method calls the virtual method OmniMap_Screen::Display for each screen
	 * added by BaseCreateScreenShape.  OmniMap_Screen::Display composites and projects
	 * the textures provided by each channel to the final display.
	 */
	void DisplayScreenShapes();
	
	void GetShaderVariables(int &N_Channels, int *TextureIds, 
									  float *Plane_Eq_XYZ, 
									  float *Plane_Eq_U, 
									  float *Plane_Eq_V);
	
	/**
	 * A utility function for allocating a new char buffer, copying the contents of a 
	 * string to a that buffer.
	 *
	 * @return A pointer to the buffer allocated.
	 */
	static const char *CreateMemAndCopy(const char * contents);

	/**
	 * /brief The property set for managing LUA-scriptable parameters for the OmniMap class.
	 *
	 * See the OmniMap class description for details on what parameters can be set.
	 */
	IncludePropertySet access;


	/**
	 * Scripting engine for executing an OmniMap script
	 */
	OmniMap_Scripting *ScriptingEngine;

	
	
	/**
	 * Create a channel for rendering a portion of the final composited, 
	 * spherically projected image into.
	 * 
	 * @param name The name of the channel.
	 * @param res The resolution of the channel.  Default is 512.
	 */
	virtual OmniMapChannelBase *Create_Channel(const char * name) = 0;

	/**
	 * Create a screen shape.
	 *
	 * @param name The name of the screen. 
	 * @return The OmniMap_Screen created.
	 *
	 * This is a pure virtual method.  It is required to
	 * be defined in derived classes.  The return value, OmniMap_Screen is an abstract class.
	 * This enables the derived class to create custom OmniMap_Screen to match the screen being
	 * rendered to.
	 */
	virtual OmniMap_Screen *Create_ScreenShape(const char * name) = 0;

	/**
	 * Composite and draw the images rendered to the channels into the final image.  Draw the
	 * Heads up display if displayHUD is true. When finished, PostRender
	 * resets the OpenGL context to the way it was found before channel rendering began.
	 */
	virtual void PostRender() = 0;

	/**
	 * Clear the screen.  This method is a pure virtual and must be defined by inheriting
	 * classes.
	 */
	virtual void Clear() = 0;

	/**
	 * \brief Set up access to properties accessible through the LUA configuration script.  
	 *
	 * The properties accessible for the OmniMapBase class are defined in the Detailed Description
	 * section of OmniMapBase documentation.
	 */
	virtual void SetUpPropertyAccess();

	/**
	 * \brief Get the name of the given channel
	 *
	 * @param The channel for which a name is requested.
	 * @return A pointer to the name of the channel.
	 */
	const char *OmniMapBase::GetChannelName(OmniMapChannelBase *channel);

	/**
	 * The file name of the LUA configuration script.
	 */
	char *StartUpScript;
	/**
	 * The width of the OmniMap display.
	 */
	int resWidth;
	/**
	 * The height of the OmniMap display.
	 */
	int resHeight;
	/**
	 * The clear color to be used to clear the frame before the final compositing of the
	 * channel images into the final spherical projection.
	 */
	float clearcolor_r,clearcolor_g,clearcolor_b,clearcolor_a;
	/**
	 * True to show the results of the render channels in a heads up
	 * display, False to not show them.
	 */
	bool displayHUD;
	/**
	 * The audience position.
	 */
	OmniVec3 AudiencePosition;
	/**
	 * The projector position.
	 */
	Omni_ViewerPosition ProjectorPosition;
	/**
	 * The vertex and fragment shaders used by OmniMap_Screen to implement the final compositing 
	 * step of the OmniMap Library.
	 */
	OmniMapShaderBase *shaders;


protected :
	const char *luaSupportDir;

};

/**
 * \brief OmniMap library scripting.
 *
 * Implements the OmniMap scripting functionality.  OmniMap scripting is done
 * via the LUA scripting language.
 */
class OMNIMAP_API    OmniMap_Scripting
{
//#ifdef OMNIMAP_EXPORTS
	// uses global lua pointer to make .h file smaller
private:
	OmniMapBase *parent;	
	void OmniMap_Scripting::LoadFunctionTable();
public:
	/**
	 * Construct an OmniMap_Scripting object.
	 *
	 * @param omnimap The OmniMap projection instance to be scripted.
	 */
	OmniMap_Scripting::OmniMap_Scripting(OmniMapBase *omnimap);
	/**
	 * Destroys an OmniMap_Scripting object.
	 */
	OmniMap_Scripting::~OmniMap_Scripting();
//#endif
	////// export ///////
public:
	/**
	 * Execute a LUA script from a file.
	 *
	 * @param strfile The name of the file containing the script to execute.
	 */
	bool OmniMap_Scripting::RunScript(const char *strfile);
	/**
	 * Execute a LUA script.
	 *
	 * @param strChunk The script to execute.
	 */
	bool OmniMap_Scripting::RunString(const char *strChunk);
	/**
	 * Execute a LUA script.
	 * 
	 * @param fmt A printf style formatting string
	 */
	bool OmniMap_Scripting::RunString(char *fmt, ...);

	/**
	 * Get a value from lua.  Will force everything to be a string
	 */
	const char * OmniMap_Scripting::GetVariableString(const char * VarName);
	/**
	 * Get a value from lua.  Will try to force some type to be a number
	 */
	double OmniMap_Scripting::GetVariableNumber(const char * VarName);
	/**
	 * Get a value from lua.  Will try to force some type to be a bool
	 */
	bool OmniMap_Scripting::GetVariableBool(const char * VarName);

};

#endif
