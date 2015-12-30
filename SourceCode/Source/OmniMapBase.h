#ifndef __OMNIMAP_BASE_H__
#define __OMNIMAP_BASE_H__
// OmniMap Geometry Correction Libs
// (c)2006, 2007, 2010 Elumenati


/*! \mainpage OmniMap API Real-Time Geometry Correction Library 2.0.3
 *
 * <center>For additional information, please see:<br><b><a href="../Omnimap_API_Integration.pdf" target="_blank">OpenGL Integration Guide PDF</a></b>  or the <b><a href="../OmnimapD3D_API_Integration.pdf" target="_blank">DirectX Integration Guide PDF</a></b>and the <b><a href="../Omnimap_API_Configuration.pdf" target="_blank">Configuration Guide PDF</a></b>
 *
 * For further information about immersive projection products and services from The Elumenati, please visit <br><b><a href="http://www.elumenati.com" target="_blank">http://www.elumenati.com</a></b>. </center>
 *
 * <img src="omnimap.jpg" valign="right">
 *
 *
 * \section intro_sec Introduction
 *
 * This is a official Release of OmniMap 2.0.3.  
 * The OmniMap, OmniMapD3D, and OmniMapD3D10 Geometry Correction Libraries provide developers with an easy-to-integrate solution to make 
 * real-time OpenGL, DirectX9, and DirectX10 applications compatible with OmniFocus projection systems.
 * Typical video projection systems display rectangular images on flat screens. However for
 * planetariums, domes, panoramas, and other non-planar display surfaces optical and geometric 
 * correction techniques must be employed to prevent video projected on the curved display from appearing distorted. 
 * OmniMap, OmniMapD3D, and OmniMapD3D10 are API's for use with OmniFocus projection systems or other custom immersive 
 * projection environments that maps real-time content onto arbitrary projection surface shapes.
 * Geometry correction routines are encapsulated in an easy-to-integrate C++ library. 
 * The pre-rendering API calls render the virtual environment into off screen temporary buffers, 
 * while the OmniMap, OmniMapD3D, and OmniMapD3D10 APIs' post-rendering calls account for lens optical distortion 
 * and screen geometry correction. All configuration and algorithm options are handled by configurationfiles 
 * and scripts. All configuration and algorithm options are handled by configuration files and scripts, 
 * so configuration changes can be made without recompiling the program. OpenGL 2.0, Shader Model 3.0, and Shader Model 4.0 shaders are 
 * leveraged to take full advantage of new functionality provided by emerging graphics hardware. 
 * The cross-platform system is hardware accelerated using all of these shader models to take full advantage 
 * of new functionality provided by emerging graphics hardware.<br>
 * This version of the library includes a watermark.  The watermark is a transparent Elumenati logo that 
 * is displayed on the lower middle portion of the final screen output.  To have the logo removed, please contact
 * Elumenati, LLC at 612-605-0826 or support@elumenati.com.
 *
 * \section features_sec Features
 * 
 * <table><tr><td class="indexkey">Hardware Accelerated</td>
    <td class="indexvalue">-OpenGL 2.0 Vertex and Pixel Shaders<br>-Direct3D Pixel Shader 3.0/4.0</td></tr>
	<tr>
	<td class="indexkey">OpenGL or Direct3D 9 or 10 based and Cross-Platform</td>
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
 * OmniMap, OmniMapD3D, and OmniMapD3D10 are just a couple of the many software tools we make available to our clients. For more information, please contact us via the client inquiry form at <a href="http://www.elumenati.com/connect" target="_blank">http://www.elumenati.com/connect</a><p></p>
 * 
 * &nbsp;
 * &nbsp;
 *
 * 
 *
 */


#pragma once

#ifndef OMNIMAP_UNMANAGEDGLUE_API
#define OMNIMAP_UNMANAGEDGLUE_API extern "C" __declspec(dllexport) 
#endif


#ifdef WIN32
#ifdef OMNIMAP_EXPORTS
#define OMNIMAP_API __declspec(dllexport)
#else
#define OMNIMAP_API __declspec(dllimport)
#endif
#else
#define OMNIMAP_API
#endif

#include "IncludePropertySet.h"
#ifdef __APPLE__
#include "OmniMapAPI/ScreenGeometryBase.h"
#else
#include "ScreenGeometryBase.h"
#endif



#define OMNIMAP_DEFAULT_RENDERCHANNEL_RESOLUTION 512
#define OMNIMAP_DEFAULT_RENDERCHANNEL_USE_FBO true


#define OMNIMAP_DEFAULT_ConsolidateCameraIntoProjectionMatrix false

#ifdef __APPLE__
#define OMNIMAP_DEFAULT_STARTUPSCRIPT 0
#else
#define OMNIMAP_DEFAULT_STARTUPSCRIPT "OmniMapConfig/omnimap_startup.lua"
#endif



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



#define OMNIMAP_maxChannelFaces 6



enum {
  OMNIMAP_COBRA_InterfaceFound = 1,
  OMNIMAP_COBRA_RunningOnBackground = 2,
  OMNIMAP_COBRA_WasWarping = 4,
};



class OmniMap_Scripting;
class OmniMapChannelBase;
class OmnimapScreenShapeFactory;
class CLua;


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
	
	/**
	 * Construct an OmniMap log object.
	 */
	OmniMap_Log();
	/**
	 * Destroy an OmniMap log object.
	 */
	~OmniMap_Log();

	/**
	 * Clear the log
	 */
	void clear();



	/////////// export ///////
public:
	/**
	 * Send a string to the message log.
	 *
	 * @param fmt The formatting string in 'printf' style.
	 */
	void ReportMessage(const char *fmt, ...);
	/**
	 * Send a string to the error log.
	 *
	 * @param fmt The formatting string in 'printf' style.
	 */
	void ReportError(const char *fmt, ...);
};

/**
 * \brief Data container for simple types.
 *
 * This is a generic class for containing integer, floating point, string and boolean
 * data.
 */
class OMNIMAP_API GenericDataContainer
{
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
private:
	int				m_int;
	float			m_float;
	void *		m_string;
	bool			m_bool;
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
	GenericDataContainer(Type t);
	/**
	 * \brief Destroys the GenericDataContainer
	 */
	~GenericDataContainer();


	/**
	 * Set the value of the data.
	 *
	 * @param value The value to set this GenericDataContainer to.
	 */
	void Set(int value);
	/**
	 * \brief Set the value of the data.
	 *
	 * @param value The value to set this GenericDataContainer to.
	 */
	void Set(bool  value);
	/**
	 * \brief Set the value of the data.
	 *
	 * @param value The value to set this GenericDataContainer to.
	 */
	void Set(float  value);
	/**
	 * \brief Set the value of the data.
	 *
	 * @param value The value to set this GenericDataContainer to.
	 */
	void Set(const char * value);

	/**
	 * \brief Get the integer value.
	 */
	int  GetINT();
	/**
	 * \brief Get the boolean value.
	 */
	bool  GetBOOL();
	/**
	 * \brief Get the float value.
	 */
	float GetFLOAT();
	/**
	 * \brief Get the string value.
	 */
	const char *  GetSTRING();
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
	ExtraDataMapContainer();
	/**
	 * Destroy an ExtraDataMapContainer object.
	 */
	~ExtraDataMapContainer();
	/**
	 * \brief Creates a GenericDataContainer for a float value, and adds a reference
	 * to that GenericDataContainer to the data map.
	 *
	 * @param key The key for accessing the value.
	 * @return A pointer to the GenericDataContainer created.
	 */
	GenericDataContainer * AddFLOAT(const char * key);
	/**
	 * \brief Creates a GenericDataContainer for a boolean value, and adds a reference
	 * to that GenericDataContainer to the data map.
	 *
	 * @param key The key for accessing the value.
	 * @return A pointer to the GenericDataContainer created.
	 */
	GenericDataContainer * AddBOOL(const char * key);
	/**
	 * \brief Creates a GenericDataContainer for a integer value, and adds a reference
	 * to that GenericDataContainer to the data map.
	 *
	 * @param key The key for accessing the value.
	 * @return A pointer to the GenericDataContainer created.
	 */
	GenericDataContainer * AddINT(const char * key);
	/**
	 * \brief Creates a GenericDataContainer for a std::string value, and adds a reference
	 * to that GenericDataContainer to the data map.
	 *
	 * @param key The key for accessing the value.
	 * @return A pointer to the GenericDataContainer created.
	 */
	GenericDataContainer * AddSTRING(const char * key);
	/**
	 * \brief Returns GenericDataContainer a value in the data map.
	 *
	 * @param key The key for the value sought.
	 * @return A pointer to the GenericDataContainer sought.
	 */
	GenericDataContainer * IndexDataMap(const char * key);
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

	
	/**
	 * \brief Set the value of this vector to the given values.
	 *
	 * @param x The x value to set to.
	 * @param y The y value to set to.
	 * @param z The z value to set to.
	 */
	void set(float x, float y, float z);
	/**
	 * Add the given vector to this vector.
	 *
	 * @param vec The vector to be added.
	 */
	void add(OmniVec3 vec);
	/**
	 * \brief Calculate a linear interpolation between this and the given vector.
	 *
	 * @param amount the value to interpolate between from and to vectors. Should be between 0 and 1.
	 * @param endvec The end of the interpolation(The given vector).
	 * @return The interpolated position.
	 */
	OmniVec3 lerp(float amount, OmniVec3 endvec);
	


	/**
	 * \brief Calculates a vector using a weighted average.  
	 *
	 * @param amount is 0-1.  0 returns the this vector. 1 returns the endvec.  Inbetween, the weighted average.
	 * @param endvec The vector to average this vector with.
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

	/**
	 * \brief Create a vector perpendicular to this vector
	 */
	static OmniVec3 MakePerpendicluarVector(OmniVec3 vecin);
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
	OmniQuad();
	/**
	 * Destroy an OmniQuad object.
	 */
	~OmniQuad();
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
	OmniQuad(
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
	
	/**
	 * Construct an Omni_ViewerPosition object
	 */
	Omni_ViewerPosition();
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

	/**
	 * \brief Constructor
	 */
	OmniMap_CameraInfo();
	/**
	 * \brief Destructor
	 */
	~OmniMap_CameraInfo();


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
	/**
	 * \brief Enables access to shader properties.
	 *
	 * Manages and allows the setting of screen properties.  This abstract class OmniMapShaderBase
	 * has no properties.  This member is to be used by classes derived from OmniMapShaderBase.
	 */
	IncludePropertySet access;

public:
	OmniMapShaderBase();
	/**
	 * \brief Set the file name of the vertex and fragment shaders.
	 */
	virtual void setShadersFilenames(
		const char * _VertexShaderFilename ,	const char * _FragmentShaderFilename);
	virtual void setShadersPrelude(const char *vertPrelude, const char *fragPrelude);
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
#ifdef ELUMENATI_INTERNAL
	virtual ~OmniMapShaderBase() {}
#endif
	

protected:
	char * VertexShaderFilename;
	char * FragmentShaderFilename;
	const char *vertexPrelude;
	const char *fragmentPrelude;

	int CobraWarpWithTrueDimension;
};


/**
 * \brief Main controller class for the dome Screen
 *
 * This is the abstract class for defining a main controller class for an OmniMap Screen.  
 * It handles the image-based projection technique
 * of projecting the scene generated by the channels onto the curved surface.
 * Deriving classes can define any geometry necessary to match the geometry of the
 * screen where the final image is rendered.  Those deriving classes only need implement
 * methods to define the geometry of that screen in a vertex buffer. That buffer is drawn by
 * a renderer which can be either OpenGL or Direct3D.
 */
class OMNIMAP_API   OmniMap_Screen : public ScreenGeometryBase
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
	 * \brief Container for extra scriptable data elements.
	 */
	ExtraDataMapContainer ScreenMetaData;
protected:
	
	virtual void SetUpPropertyAccess() = 0;
	
public:
	
	/**
	 * \brief Construct an OmniMap_Screen
	 */
	OmniMap_Screen();
	/**
	 * \brief Destroy an OmniMap_Screen
	 */
	virtual ~OmniMap_Screen();

	/////////// export /////////

};




typedef OmniMap_Screen * (* fpScreenCreatorFunction) (void);  // function pointer to a screenshape creator...
/**
 * \brief A factory for screenshape types.
 *
 * This is useful for host applications that want to implement
 * new screenshapes without making a derived version of omnimap.
 */
class OMNIMAP_API OmnimapScreenShapeFactory
{
private:  // there is private stuff...
	void *LocalString_fpScreenCreatorFunction_MAP;
public:
	/**
	 * \brief Add a new screen shape.
	 *
	 * Add a new class that represents a new screen shape.
	 *
	 * @param type A string that can be used to refer to this screen shape.
	 * @param functionPointer  A pointer to a function that returns a pointer to an object of a type
	 * derived from OmniMap_Screen.
	 */
	void addScreenShapeCreator(const char * type,fpScreenCreatorFunction functionPointer);
	/**
	 * \brief Create a screen shape.
	 * 
	 * Creates an object derived from OmniMap_Screen.
	 * @param type The type of screen to create.  This must be a string that has been registered using
	 * OmnimapScreenShapeFactory::addScreenShapeCreator
	 * @returns A pointer to the OmniMap_Screen derived object created.
	 */
	OmniMap_Screen * Create_ScreenShape(const char * type);
	/**
	 * \brief Construct an OmnimapScreenShapeFactory
	 */
	OmnimapScreenShapeFactory();
	/**
	 * \brief Destroy an OmnimapScreenShapeFactory
	 */
	~OmnimapScreenShapeFactory();

	// to do
	//// list availilbe screenshapes
	//// list parameters to the screenshapes
	//// add a pointer to a screenshape destructor function.....
#ifdef ELUMENATI_INTERNAL
	int getNumShapes();
	const char *getScreenTypeName(int i);
#endif
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
 * DrawHeadsUpDisplay.  Scriptable parameters can be set and accessed through the OmniMapChannelBase::access
 * member variable.  These variable can also be set via the LUA configuration files using the LUA function:
 * SetChannelParameter. The OmniMapChannelBase class has the following LUA-scriptable paramaters:
 * <ul>
 *  <li>
 *      <b>XResolution</b> : The width in pixels of this channel.
 * </li>
 *  <li>
 *		<b>YResolution</b> : The height in pixels of this channel.
 * </li>
 *  <li>
 *		<b>RenderTarget</b> : The kind of off screen render target to be used for this channel. Values can be:
 *		<ul>
 *			<li>
 *				<b>OmniMapChannelBase::FRAME_BUFFER_OBJECT (RT_FRAME_BUFFER_OBJECT in LUA)</b> : 
					Frame Buffer object in OpenGL or a ID3DXRenderToSurface in DirectX.
 *			</li>
 *			<li>
 *				<b>OmniMapChannelBase::PBUFFER (RT_PBUFFER in Lua)</b> : pbuffer in OpenGL, IDirect3DSurface9 in DirectX.
 *			</li>
 *			<li>
 *				<b>OmniMapChannelBase::BACK_BUFFER (RT_BACK_BUFFER in Lua)</b>: Back buffer on OpenGL, 
						not yet supported in DirectX.
 *			</li>
 *		</ul>
 * </li>
 *	<li>
 *		<b>HeadsUpDisplay_Quad.bl.x, HeadsUpDisplay_Quad.bl.y, HeadsUpDisplay_Quad.bl.z</b> : The 
 *		bottom left position of the heads up display for this channel.
 *	</li>
 *  <li>
 *		<b>HeadsUpDisplay_Quad.br.x, HeadsUpDisplay_Quad.br.y, HeadsUpDisplay_Quad.br.z</b> : The 
 *		bottom right position of the heads up display for this channel.
 *	</li>
 *  <li>
 *		<b>HeadsUpDisplay_Quad.tl.x, HeadsUpDisplay_Quad.tl.y, HeadsUpDisplay_Quad.tl.z</b> : The 
 *		top left position of the heads up display for this channel.
 *	</li>
 *  <li>
 *		<b>HeadsUpDisplay_Quad.tr.x, HeadsUpDisplay_Quad.tr.y, HeadsUpDisplay_Quad.tr.z</b> : The 
 *		top right position of the heads up display for this channel.
 *	</li>
 *  <li>
 *      <b>UseOmniMapMatrix</b> : If true, OmniMap will set viewing and/or projection matrix
 *          for channel Rendering.  If false, the application takes that responsibility.
 *  </li>
 *	<li>
 *		<b>cam_info.ClipTop</b> : The top of this channel's vertical clipping plane.
 *	</li>
 *	<li>
 *		<b>cam_info.ClipBottom</b> : The bottom of this channel's vertical clipping plane.
 *	</li>
 *	<li>
 *		<<b>cam_info.ClipLeft</b> : The left side of this channel's horizontal clipping plane.
 *	</li>
 *  <li>
 *		<b>cam_info.ClipNear</b> : The distance from the viewer to the near clipping 
 *			plane (always positive).
 *	</li>
 *	<li>
 *		<b>cam_info.ClipFar</b> : The distance from the viewer to the far clipping 
 *			plane (always positive).
 *	</li>
 *	<li>
 *		<b>Matrix_WorldView.0.0, Matrix_WorldView.0.1, Matrix_WorldView.0.2, Matrix_WorldView.0.3,
 *			Matrix_WorldView.1.0, Matrix_WorldView.1.1, Matrix_WorldView.1.2, Matrix_WorldView.1.3,
 *			Matrix_WorldView.2.0, Matrix_WorldView.2.1, Matrix_WorldView.2.2, Matrix_WorldView.2.3,
 *			Matrix_WorldView.3.0, Matrix_WorldView.3.1, Matrix_WorldView.3.2, Matrix_WorldView.3.3</b> :
 *			The matrix defining the rotation of the camera.
 *	</li>
 * </ul>
 */
class OMNIMAP_API OmniMapChannelBase {
public:
	typedef enum {
		RC_RT_NONE = 0,
		FRAME_BUFFER_OBJECT = 1,
		PBUFFER = 2,
		BACK_BUFFER = 3
	} TargetType;
	
	/**
	 * Construct an OmniMap_Channel Object.
	 */
	OmniMapChannelBase();
	/**
	 * Destroy this channel
	 */
	virtual ~OmniMapChannelBase();


public:
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
	/**
	 * \brief Apply this channel's projection matrix. 
	 *
	 * Multiplies the current matrix by the matrix generated by the viewing frustum
	 * defined by the cam_info class member.
	 */
	virtual void ApplyProjectionMatrix() = 0;
	/**
	 * \brief Apply this channel's view matrix.
	 *
	 * Multiplies the current matrix by the matrix in cam_info.Matrix_WorldView.
	 */
	virtual void ApplyViewMatrix()  = 0;

	/**
	 * \brief Get the channel's view matrix
	 *
	 * This method returns the view matrix for the channel in the given array.
	 * The base class returns the view matrix in column major format.
	 * @param m The array to contain the returned matrix.
	 */
	virtual void GetViewMatrix(float m[16]);

	/**
	 * \brief Get the Euler angles for the camera offset rotation for this channel.
	 *
	 * @param x The returned angle of rotation about the x axis.
	 * @param y The returned angle of rotation about the y axis.
	 * @param z The returned angle of rotation about the z axis.
	 */
	virtual void GetEulerAngles(float *x, float *y, float *z);

	/**
	 * \brief Set the camera offset rotation for this channel in Euler angles.
	 *
	 * @param x The angle of rotation about the x axis.
	 * @param y The angle of rotation about the y axis.
	 * @param z The angle of rotation about the z axis.
	 */
	virtual void SetEulerAngles(float x, float y, float z);

	/**
	 * \brief Set the camera frustum for this channel.
	 *
	 * This method can be used to set an asymmmetrical camera frustum.
	 * @param fovUp The field of view in the up direction.
	 * @param fovDown The field of view in the down direction.
	 * @param fovLeft The field of view in the left direction.
	 * @param fovRight The field of view in the right direction.
	 * @param nearv The near clipping plane.
	 * @param farv The far clipping plane.
	 */
	virtual void SetCameraInfo(float fovUp, float fovDown, float fovLeft, float fovRight,
								float nearv, float farv);
	/**
	 * \brief Set the camera frustum for this channel.
	 *
	 * This method can only be used to set symmetrical frustums.
	 * @param fovy The field of view in the up and down directions.
	 * @param apectRatio The aspect ratio of the field of view.
	 * @param nearv The near clipping plane.
	 * @param farv The far clipping plane.
	 */
	virtual void SetCameraInfo(float fovy ,
								float apectRatio, float nearv, float farv);

	/**
	 * \brief Set the camera offset rotation for this channel in up direction, view direction.
	 *
	 * @param headsUp The up vector for the camera offset rotation.
	 * @param viewVec The viewing direction for the camera offset rotation.
	 */
	virtual void SetChannelOrientation(OmniVec3 headsUp, OmniVec3 viewVec);

	/**
	 * \brief Get the projective texturing matrix used to project this channel onto the output display shape.
	 *
	 * This is an abstract method. This method must be implemented by deriving classes, and
	 * must return the projective texturing matrix used to project this channel onto the final output display
	 * in a 16 element float array.
	 * @param m The returned matrix.
	 */
	virtual void GetProjTextMatrix(float m[16]) = 0;

	/**
	 * \brief Get the projection matrix.
	 *
	 * This method returns the projection matrix in a 16 element float array.
	 *
	 * This method must be implemented by deriving classes, and
	 * must return the projection matrix for this channel in a 16 element float array.
	 * @param m The returned matrix.
	 */
	virtual void GetProjectionMatrix(float m[16]) = 0;

#ifdef OM_PICKING
	bool screenXYToChannelXY(float screenGeomLoc[4], int &channelX, int &channelY);
#endif


protected:
	// PROTECTED MEMBER VARIABLES -----------------------------------------------------------
	

	// PROTECTED METHODS
	/**
	 * Sets up access to the properties of a channel.  
	 */
	virtual void SetUpPropertyAccess();
	
	/**
	 * \brief The camera settings for this channel.
	 *
	 * Defines the frustum and view rotation for this channel.  These values are used
	 * to set the OpenGL projection matrix prior to this channel being rendered.
	 */
	OmniMap_CameraInfo cam_info;

	bool useOmniMapMatrix;

	int xResolution;
	int yResolution;
	TargetType renderTarget;
	/**
	 * \brief The quad on to which the channels contents will be displayed in
	 * the heads up display.
	 */
	OmniQuad HeadsUpDisplay_Quad;
};

typedef  OmniMap_Screen  * pOmniMap_Screen;
typedef  OmniMapChannelBase * pOmniMap_Channel;


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
 *		<li><b>RendererType</b> The underlying graphics subsystem used for rendering the screen geometry.  Can
 *				be either "OGL" or "D3D".
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
		const char *luaResDir = 0);
	/**
	 * Destroys an OmniMapBase object.  
	 */
	virtual ~OmniMapBase();
	
	/**
	 * Clear the list of rendering channels.
	 */
	virtual void Clear_Channels();
	/**
	 * Clear the list of screens to render to.
	 */
	virtual void ClearScreenShapes();
	/**
	  * @param strChannelName The name of the channel to return.
	  *
	  * @returns a pointer to the channel named name,
	  *	null if that channel doesn't exist.
	  */
	virtual OmniMapChannelBase *GetChannel(const char* strChannelName);
	/**
	  * @param index The index of the channel to return.
	  *
	  * @returns a pointer to the channel indexed by index,
	  *	null if that channel doesn't exist.
	  */
	virtual OmniMapChannelBase *GetChannel(int index);

	/**
	 * \brief Create a channel.
	 *
	 * This method will call CreateChannel to do the actual creation
	 * of the channel.  CreateChannel is a pure virtual that must be defined by classes
	 * derived from OmniMapBase.
	 * @param name The name of the channel.
	 */
	virtual void BaseCreateChannel(const char * name);
	
	/**
	 * \brief Create an OmniMap_Screen to render to.
	 *
	 * This method calls the inheriting classes'
	 * Create_ScreenShape method.  This enables derived implementations of the OmniMapBase
	 * class to create customm screens derived from OmniMap_Screen.
	 *
	 * @param name The name of the screen to be created.
	 * @param shape The type of screen shape to create.  This is the name of a shape that has been registered
	 *		with OmniMapBase::ScreenshapeFactory.
	 */
	virtual void BaseCreateScreenShape(const char * name, const char *shape);

	virtual int GetNumScreenShapes();

	/**
	 * \brief Bind all channel textures to a multitexturing texture unit.
	 *
	 * This method makes a call the to all channels' OmniMap_Channel::BindTexture methods.
	 * Each channel is responsible for binding its texture to a multitexturing texture unit.
	 */
	virtual void BindAllChannelTextures();
	/**
	 * \brief Unbind all channel textures from a multitexturing texture unit.
	 *
	 * This method makes a call the to all channels' OmniMap_Channel::UnBindTexture methods.
	 * Each channel unbinds its texture from a multitexturing texture unit.
	 */
	virtual void UnBindAllChannelTextures();
	/**
	 * \brief Get the number of channels
	 *
	 * @return The number of channels
	 */
	virtual int GetNumChannels();
	/**
	 * \brief Call main application's rendering method for each channel.
	 *
	 * Calls a function for each channel, added to this OmniMapBase projection, and passes
	 * a pointer to that channel.  This method is called by the application to begin the
	 * rendering of each channel that will be composited and projecte onto the display.
	 *
	 * @param fun The function to be called.  The function to be called takes a pointer to an 
	 * OmniMapChannelBase object that
	 * represents the channel to be rendered.
	 */
	virtual void ForEachChannel(void fun(OmniMapChannelBase *chan));
	/**
	 * \brief Call main application's rendering method for each channel.
	 *
	 * Calls a function for each channel, added to this OmniMapBase projection, and passes
	 * a pointer to that channel.  This method is called by the application to begin the
	 * rendering of each channel that will be composited and projecte onto the display.
	 *
	 * @param fun The function to be called.  The function to be called takes a pointer to an 
	 * OmniMapChannelBase object that
	 * represents the channel to be rendered, and a void * pointer.
	 * @param userData A pointer to be passed back to the function that is called for each channel.
	 */
	virtual void ForEachChannel(void fun(OmniMapChannelBase *chan, void *userData), void *userData);
	/**
	 * \brief Draw the heads up displays.
	 *
	 * Draws the heads up display that shows the contents of each individual channel.
	 * This method calls the OmniMapChannelBase::DrawHeadsUpDisplay for each channel.
	 */
	virtual void DrawHeadsUpDisplays();
	/**
	 * \brief Renders the channel outputs to the final screen.
	 *
	 * This method calls the virtual method OmniMap_Screen::Display for each screen
	 * added by BaseCreateScreenShape.  OmniMap_Screen::Display composites and projects
	 * the textures provided by each channel to the final display.
	 */
	virtual void DisplayScreenShapes();

	virtual void GetShaderVariables(int &N_Channels, int *TextureIds);
	
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
	 */
	virtual OmniMapChannelBase *Create_Channel(const char * name) = 0;


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
	 * @param channel The channel for which a name is requested.
	 * @return A pointer to the name of the channel.
	 */
	virtual const char *GetChannelName(OmniMapChannelBase *channel);


protected:
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
	 * The X value of the lower left corner of the OpenGL viewport rectangle, in pixels.
	 */
	float glViewportsettings0;
	/**
	 * The Y value of the lower left corner of the OpenGL viewport rectangle, in pixels.
	 */
	float glViewportsettings1;
	/**
	 * The width of the OpenGL viewport rectangle, in pixels.
	 */
	float glViewportsettings2;
	/**
	 * The height of the OpenGL viewport rectangle, in pixels.
	 */
	float glViewportsettings3; 
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


	/**
	 * The screenshape factory
	 *  This is for creating screenshapes... use this to allow the host application to insert new types of screenshapes into omnimap...
	 *  We can also, make the new types of screenshapes that are not exposed by the dll interface, but rather through the lua and property set accessors...
	 */
	OmnimapScreenShapeFactory ScreenshapeFactory;

	static OmniMapScreenRendererFactory ScreenRendererFactory;

	

public:
	

	/**
	 * \brief Set the stencil mask file name.
	 *
	 * This is a mask file whose pixels are multiplied by the pixels rendered to the final display output.
	 *
	 * @param FileName The name of the file containing the png image to be used as the mask.
	 */
	virtual void StencilMask_filename_set(const char * FileName); // relative to lua support directory...
	
	/**
	 * \brief Create a screen shape.
	 *
	 * This is a pure virtual method.  It is required to
	 * be defined in derived classes.  The return value, OmniMap_Screen is an abstract class.
	 * This enables the derived class to create custom OmniMap_Screen to match the screen being
	 * rendered to.
	 *
	 * @param name The name of the screen.
	 * @param type The type of screen shape to create.  This is the name of a shape that has been registered
	 *		with OmniMapBase::ScreenshapeFactory.
	 * @return The OmniMap_Screen created.
	 *
	 */
	virtual void create_ScreenShape(const char * name, const char * type);


	/**
	 * Get a reference to a screen shape.
	 *
	 * @param name The name of the screen.
	 * @return A pointer to the screen shape with the given name.
	 */
	virtual OmniMap_Screen * GetScreenShape(const char * name);// return screenshape by index, if not found return null with an errror logged..

	/**
	 * Get a reference to a screen shape.
	 *
	 * @param index The index of the screen.
	 * @return A pointer to the screen shape with the given index.
	 */
	virtual OmniMap_Screen * GetScreenShape(int index); // return screenshape by index, if not found return null with no errror logged...

	/**
	 * \brief Get the CLua interface object used by OmniMap for configuration.
	 *
	 * The CLua object is responsible for providing an interface for lua scripts to set attributes of
	 * OmniMap objects, and for OmniMap objects to call lua functions.
	 * @returns A pointer to the CLua interface object used by OmniMap for configuration.
	 */
	virtual CLua *GetCLuaRef();

	/**
	 * \brief Set the orientation of the given channel.
	 *
	 * This method will make all of the necessary calls to set the orientation of the camera
	 * of the given channel to the given orientation, including setting
	 * the appropriate shader values to be used by OmniMapBase::PostRender.
	 * @param channel The channel whose camera orientation will be set.
	 * @param headsUp The up value of the camera orientation.
	 * @param viewVec The viewing angle of the camera orientation.
	 */
	virtual void SetChannelOrientation(OmniMapChannelBase *channel, OmniVec3 headsUp, OmniVec3 viewVec);


	/**
	 * \ brief Set camera information for the given channel.
	 *
	 * Sets the field of view, aspect ratio, near clip plane, far clip plane, and whether to consolidate
	 * the viewing matrix into the projection matrix.
	 * @param channel The channel whose camera values are to be set.
	 * @param fovy The vertical field of view.
	 * @param aspectRatio The aspect ratio of the camera frustum.
	 * @param near The near clipping plane.
	 * @param far The far clipping plane.
	 */
	virtual void SetCameraInfo(OmniMapChannelBase *channel, float fovy ,
								float aspectRatio, float near, float far);

	virtual void SetCameraInfo(OmniMapChannelBase *channel, float fovUp, float fovDown, float fovLeft, float fovRight,
								float nearv, float farv);


	/**
	 * \brief Set the camera orientation for a channel using Euler angles
	 * 
	 */
	virtual void SetChannelEulerAngles(OmniMapChannelBase *channel, float x, float y, float z);

#ifdef OM_PICKING
	bool screenXYToChannelXY(OmniMapChannelBase * channel, 
									  int x, int y, int &channelX, int &channelY);
	bool screenXYToChannelXY(int x, int y, OmniMapChannelBase * &channel, int &channelX, int &channelY);
#endif
#ifdef ELUMENATI_INTERNAL
	const char *getScreenName(OmniMap_Screen *screen);
#endif


	
protected:
	/**
	 * \brief Clear the channel map.
	 * 
	 * Clears the channel map.  This does not delete channels.  It just empties the map that maps channels to 
	 * their names.  Once this is done, channels must be recreated before channel rendering is possible.
	 */
	virtual void ClearChannelMap();
	char rendererType[128];

	char *luaSupportDir;
	/////////////////////////////////////////////////////////////////////////////
	///////////////////////////  StencilMask  setup functions ///////////////////
	char *StencilMask_filename;
	void *graphicsContext;
	


	
	/////////////////////////////////////////////////////////////////////////////
    //////////////////////////   Cobra
	int CobraWarpWithTrueDimension;
	int MaxFPCChannels;

};

/**
 * \brief OmniMap library scripting.
 *
 * Implements the OmniMap scripting functionality.  OmniMap scripting is done
 * via the LUA scripting language.
 */
class OMNIMAP_API    OmniMap_Scripting
{

private:
	OmniMapBase *parent;	
	void LoadFunctionTable();
public:
	/**
	 * Construct an OmniMap_Scripting object.
	 *
	 * @param omnimap The OmniMap projection instance to be scripted.
	 */
	OmniMap_Scripting(OmniMapBase *omnimap);
	/**
	 * Destroys an OmniMap_Scripting object.
	 */
	~OmniMap_Scripting();

	////// export ///////
public:
	/**
	 * Execute a LUA script from a file.
	 *
	 * @param strfile The name of the file containing the script to execute.
	 */
	bool RunScript(const char *strfile);
	/**
	 * Execute a LUA script.
	 *
	 * @param strChunk The script to execute.
	 */
	bool RunString(const char *strChunk);
	/**
	 * Execute a LUA script.
	 * 
	 * @param fmt A printf style formatting string
	 */
	bool RunString(char *fmt, ...);

	/**
	 * Get a value from lua.  Will force everything to be a string
	 */
	const char * GetVariableString(const char * VarName);
	/**
	 * Get a value from lua.  Will try to force some type to be a number
	 */
	double GetVariableNumber(const char * VarName);
	/** 
	 * Get a value from lua.  Will try to force some type to be a bool
	 */
	bool GetVariableBool(const char * VarName);

};


class OMNIMAP_API  BaseImage
{
	friend class OmniMap_Shader;
public:
	BaseImage(const char * filename);
	~BaseImage();
	int h; 
	int w;
	unsigned char *bytes;

	bool Baseloadfile(const char * filename);
	void loadNullObject();

protected:
	bool hasAlpha;

};


OMNIMAP_API  OmniMap_Log*  LogSystem(); // singleton of the log system...


#endif
