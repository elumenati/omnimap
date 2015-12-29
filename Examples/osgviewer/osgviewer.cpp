/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This application is open source and may be redistributed and/or modified   
 * freely and without restriction, both in commericial and non commericial applications,
 * as long as this copyright notice is maintained.
 * 
 * This application is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  The original OpenSceneGraph code has been modified by The Elumenati, and IDSI to provide
 *  an example of how Elumenati's OmniMap API can be integrated into aon OpenSceneGraph 
 *  application can be integrated.  All of the above applies to these changes.
*/

#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osg/CoordinateSystemNode>
#include <osg/TextureCubeMap>
#include <osg/Notify>
#include <osg/GLExtensions>
#include <osg/Node>
#include <osg/Geometry>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osg/Stencil>
#include <osg/ColorMask>
#include <osg/Depth>
#include <osg/Billboard>
#include <osg/Material>
#include <osg/Projection>
#include <osg/io_utils>

#include <osg/Switch>
#include <osgText/Text>

#include <osgViewer/Scene>
#include <osgViewer/Renderer>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>

#include <iostream>

// =-=-=-=-=-=-=-
#ifdef USE_OMNIMAP
//
// Include the OmniMapAppTextureOGL, and OmniMapChannelATOGL classes. We are using theses classes because
// it allows the application to create the off screen render buffer and 
// offset cameras, and execute the rendering passes for each of those channels.
// The application then just supplies the texture id of the off screen render buffer
// to the OmniMapChannelATOGL object, and calls the OmniMapAppTextureOGL::postRender method
// The OmniMapAppTextureOGL::postRender method composites the channels and spherically corrects
// and draws the composited image to the output screen.
#include "OmniMapAppTextureOGL.h"


// =-=-=-=-=-=-=-
// Callback information for the channel prerendering callback.
struct CBInfo
{
	OmniMapChannelATOGL *om_chan;
	osg::Camera *viewerCam;
};

CBInfo chInfo[6];

// =-=-=-=-=-=-=-

// This predraw callback is conntected to each channel camera.
// CBInfo contains a pointer to the viewer camera and to the OmniMap
// channel
struct om_predraw_callback : public osg::Camera::DrawCallback
{
	// The constructor simply makes a note of the CBInfo.
	om_predraw_callback( CBInfo* _ch ) : ch_(_ch)
	{
		
	}


	virtual void operator ()( const osg::Camera& _c /*camera*/ ) const
	{

		GLenum e = glGetError();
		// Make sure the channel has been created.
		if (ch_->om_chan == NULL)
			return;
		
		// Get the OmniMap channel frustum values, so we can set the camera with values that match
		// what OmniMap expects.  We need to set the projection matrix for both
		// the main viewer camera and the channel cameras.
		float m[16];
		ch_->om_chan->GetProjectionMatrix(m);
		osg::Matrixf osgm(&(m[0]));
		ch_->viewerCam->setProjectionMatrix(osgm);
		((osg::Camera&) _c).setProjectionMatrix(osgm);


		// Set the texture id for the channel to the texture id of the osg fbo attached to
		// the camera being rendered
		osg::Camera::BufferAttachmentMap b = _c.getBufferAttachmentMap();
		const osg::GraphicsContext* view_gc = _c.getGraphicsContext();
		unsigned int ctx_id = view_gc->getState()->getContextID();
		osg::Texture::TextureObject* tobj = b[osg::Camera::COLOR_BUFFER]._texture->getTextureObject( ctx_id );
		// The texture object may not have been attached to this context yet, 
		// make sure it is before dereferencing it.
		if (tobj != NULL)
			ch_->om_chan->SetTexureID(tobj->_id);


	}

	CBInfo* ch_;

}; // om_predraw_callback

// =-=-=-=-=-=-=-

struct om_postdraw_callback : public osg::Camera::DrawCallback
{
	om_postdraw_callback( CBInfo* _ch ) : ch_(_ch)
	{

	}

	virtual void operator ()( const osg::Camera& _c /*camera*/ ) const
	{
	}

	CBInfo* ch_;

}; // om_postdraw_callback

// =-=-=-=-=-=-=-
// This callback is called after rendering the final slave, which has no scene to
// render.  Instead we just call OmniMap::PostRender here to render the final
// composited dome image.
struct om_finaldraw_callback : public osg::Camera::DrawCallback
{
	// Constructor just keeps a reference ot OmniMap.
	om_finaldraw_callback( OmniMap* _om) : om_(_om)
	{
	}

	// Render the composited channels to the dome
	virtual void operator ()( const osg::Camera& _c /*camera*/ ) const
	{
		om_->PostRender();

	}

	OmniMap* om_;

}; // om_finaldraw_callback


// This is a viewer class, that knows about OmniMap.  
// It may or may not be necessary for
// your application. 
class OmniMapOSGViewer : public osgViewer::Viewer
{
public:
	OmniMapAppTextureOGL* omnimap;
	OmniMapOSGViewer(osg::ArgumentParser& arguments) :
		osgViewer::Viewer(arguments)
	{
		omnimap = NULL;
	}

	bool init_omnimap();
	bool initSlaveCameras(int numCameras, osg::ArgumentParser& arguments);
	
	osg::ref_ptr<osg::Camera> finalRenderCamera;
	osg::ref_ptr<osg::GraphicsContext> omgc;
	int numChannels;
};

// Since we need to have an OpenGL context to construct 
// and initialize OmniMap, we do it once the viewer has been
// realized.  This is the operator that does that.
struct OMVRealizeOp : public osg::Operation 
{
	// The constructor just keeps a reference to the viewer.
	OMVRealizeOp(OmniMapOSGViewer *viewer) : 
        osg::Operation("RealizeOmniMapViewer",false),
        _viewer(viewer)
    {}

	// The operator calls the method that constructs and initializes OmniMap
    virtual void operator () (osg::Object* object)
    {
        _viewer->init_omnimap();
    }
	OmniMapOSGViewer *_viewer;

	
};

// =-=-=-=-=-=-=-
using namespace osgViewer;


/*
 * init_omnimap is called when the OmniMapOSGViewer is realized.  We need to wait until
 * the viewer is realizedl, because initialization of OmniMap requires a valid OpenGL
 * context.
 */
bool OmniMapOSGViewer::init_omnimap( )
{
	this->setRealizeOperation(NULL);

	// Get the traits for the graphics context so that we can
	// use it to provide resolution information when constructing OmniMap.
	const osg::GraphicsContext::Traits * wt = omgc.get()->getTraits();
	// Make sure that our graphics context is the current one.
	makeCurrent(omgc.get());
	// Construct the OmniMap object.  Provide the width and height of
	// the final render window.  Caution: We are going to be using the back buffer
	// for rendering channels.  We are forced to do this because if we run
	// multithreaded, Framebuffer Ojbects do not work.  This has nothing to do with
	// OmniMap.  It appears to be a problem with OSG.  This can be proven by running
	// the osgdistortion example(which uses frame buffer objects) in multthreaded mode.
	// Since we are using the back buffer for channel rendering, channel resolutions
	// must be <= the resolution of the frame buffer.(wt->width, wt->height)
	omnimap = new OmniMapAppTextureOGL( wt->width, wt->height );
	if( !omnimap )
	{
		osg::notify( osg::NOTICE ) << 
			"Failed to create OmniMap Context" << std::endl;
		return false;
	}

	/*
	 * Initialize OmniMap.  This will execute the lua scripts that control the
	 * configuration of OmniMap.  See the configuration document on the start menu under
	 * Elumenati - OmniMap for more information on configuring OmniMap.
	 */
	omnimap->init();

	/*
	 * Get the number of channels OmniMap expects to be rendered.
	 */
	int num_om_ch = omnimap->GetNumChannels();
	if( !num_om_ch || num_om_ch != numChannels)
	{
		osg::notify( osg::NOTICE ) << 
			"No OmniMap Channels Found.  Check your OmniMapConfig" << std::endl;
		return false;
	}
		
	// =-=-=-
	/*
	 * Loop through the channels.
	 */
	for( int cc = 0; cc < num_om_ch; cc++ )
	{
		OmniMapChannelBase*  om_ch_base = omnimap->GetChannel( cc );
		OmniMapChannelATOGL* om_ch      = dynamic_cast<OmniMapChannelATOGL*>( om_ch_base );

		const char* ch_name = omnimap->GetChannelName( om_ch );
		// =-=-=-
		// Set the name of the camera being used for this channel
		// to the name of the OmniMap channel.
		// to osg::Camera::FRAME_BUFFER, and attaching the texture object created above.
		_slaves[cc]._camera->setName( ch_name );

		// Set callback data values for the predraw/postdraw callbacks.
		chInfo[cc].om_chan = om_ch;

		// Set the viewing offset matrix for this slave camera.  Point it in
		// the direction of the omnimap channel.
		// For now, we set the projection matrix to identity, then set it
		// in the prerender call before each frame.
		float m[16];
		om_ch->GetViewMatrix( m );
		osg::Matrix view(m);
		osg::Matrix proj;
		_slaves[cc]._viewOffset.set(view);

		// =-=-=-

	} // for cc

	
	// =-=-=-

	// final render camera
	{
		// Finally, set the post draw callback for the final pass render camera.
		this->finalRenderCamera->setPostDrawCallback( new om_finaldraw_callback( omnimap ) );	

	}

	return true;

} // init_omnimap

static char * chanNames[] = {
	"Chan0", "Chan1", "Chan2", "Chan3"
};
bool OmniMapOSGViewer::initSlaveCameras(int num_om_ch, osg::ArgumentParser& arguments)
{
	GLenum e = glGetError();
	numChannels = num_om_ch;
	// This width and height are the default values for the pixel resolution of the final output.
	// These values should match the resolution of the projector you are sending the final
	// output to.
	unsigned int width = 1280;
	unsigned int height = 1024;
	// These are the window position values.  To move the window position to a second monitor(projector), just 
	// set windowX to the resolution of the first monitor.
	unsigned int windowX = 50;
	unsigned int windowY = 50;
	while (arguments.read("--width",width)) {}
    while (arguments.read("--height",height)) {}
	while (arguments.read("--windowX",windowX)) {}
    while (arguments.read("--windowY",windowY)) {}

	// These values represent the width and height of the offscreen buffers used for\
	// rendering a single channel.  These should always be a power of 2, and should always
	// be <= to both dimensions of the resolution of the final output window.
	unsigned int channelWidth = 1024;
	unsigned int channelHeight = 1024;

	// Set up traits to be used by the graphics context.  This will be the context for all
	// rendering(channel and final output).
	osg::ref_ptr< osg::GraphicsContext::Traits > traits = new osg::GraphicsContext::Traits;
	traits->x = windowX;
	traits->y = windowY;
	traits->width  = width;
	traits->height = height;
	traits->windowDecoration = false;
	traits->doubleBuffer     = true;
	traits->sharedContext    = 0;
    
	// Create the graphics context
    omgc = osg::GraphicsContext::createGraphicsContext(traits.get());
    if (!omgc)
    {
        osg::notify(osg::NOTICE)<<"GraphicsWindow has not been created successfully."<<std::endl;
        return false;
    }
	
	
	// When OSG frame buffer objects work in multi-threaded mode, these two lines could be used
	// to specify a frame buffer object as the underlying implementation for offscreen channel
	// rendering.
	// osg::Camera::RenderTargetImplementation rti = osg::Camera::FRAME_BUFFER_OBJECT;
	// GLenum buffer = GL_FRONT;

	// For now we specify the OpenGL back buffer.
	osg::Camera::RenderTargetImplementation rti = osg::Camera::FRAME_BUFFER;
	GLenum buffer = GL_BACK;

	// =-=-=-
	for( int cc = 0; cc < num_om_ch; cc++ )
	{
	
		
		e = glGetError();

		// =-=-=-
		// Create a texture to attach to offscreen channel rendering result
		osg::Texture2D* texture = new osg::Texture2D;
		texture->setTextureSize(1024, 1024);
		texture->setInternalFormat( GL_RGB );
		texture->setFilter( osg::Texture::MIN_FILTER, osg::Texture::LINEAR  );
		texture->setFilter( osg::Texture::MAG_FILTER, osg::Texture::LINEAR  );
		texture->setWrap( osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE );
		texture->setWrap( osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE );

		// =-=-=-
		// Create a camera for this channel, setting the render target implementation
		// to osg::Camera::FRAME_BUFFER, and attaching the texture object created above.
		osg::ref_ptr<osg::Camera> sl_cam = new osg::Camera;
		sl_cam->setName( chanNames[cc] );
		sl_cam->setGraphicsContext( omgc.get() );
		sl_cam->setViewport( new osg::Viewport(0,0,1024, 1024) );	
		sl_cam->setDrawBuffer( buffer );
		sl_cam->setReadBuffer( buffer );
		sl_cam->setAllowEventFocus( false );
		sl_cam->setRenderTargetImplementation( rti );
		
		// Attach the render texture to this channel's camera.
		sl_cam->attach( osg::Camera::COLOR_BUFFER, texture );

		// Set callback data values for the predraw/postdraw callbacks.
		// We will not create an OmniMap channel until the realize operation
		// for this viewer.  For now, set the reference to the OmniMap channel
		// to NULL.
		chInfo[cc].om_chan = NULL;
		chInfo[cc].viewerCam = getCamera();
		// set the camera to render before the main camera.
        sl_cam->setRenderOrder(osg::Camera::PRE_RENDER);
		sl_cam->setPreDrawCallback ( new om_predraw_callback( &chInfo[cc] ) );
		sl_cam->setPostDrawCallback( new om_postdraw_callback( &chInfo[cc] ) );
		sl_cam->addChild(getSceneData());

		// =-=-=-
		// Add the slave camera to the viewer with the offset view 
		// Set the viewing offset matrix for this slave camera.  Point it in
		// the direction of the omnimap channel.
		// For now, we set the projection matrix to identity, then set it
		// in the prerender call before each frame.

		// For now, we just set the offsets to identity.  Upon realization, we will
		// set the view matrix to an offset that matches the OmniMap viewing offset for
		// this channel.
		osg::Matrix view;
		osg::Matrix proj;
		addSlave( sl_cam.get(), proj, view, true );

		// =-=-=-

	} // for cc

	
	// =-=-=-

	// final render camera
	{
		// Create the final render pass camera.  This camera will have nothing to render.
		// It will simply provide a way for to access the OmniMap::postRender method at after
		// the channel's have been rendered.
		finalRenderCamera = new osg::Camera;
		finalRenderCamera->setName( "om_final_camera" );
		finalRenderCamera->setGraphicsContext( omgc.get() );
		finalRenderCamera->setClearMask ( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
		finalRenderCamera->setClearColor( osg::Vec4(0.1,0.1,1.0,1.0 ) );
		finalRenderCamera->setViewport(  0, 0, width, height );	

		// =-=-=- 
		GLenum buffer = GL_BACK;
		finalRenderCamera->setDrawBuffer( buffer );
		finalRenderCamera->setReadBuffer( buffer );
		finalRenderCamera->setReferenceFrame( osg::Camera::ABSOLUTE_RF );
		finalRenderCamera->setAllowEventFocus( false );
		
		// =-=-=-
		finalRenderCamera->setClearColor( osg::Vec4( 0.1, 0.1, 1.0, 1.0 ) );
		finalRenderCamera->setProjectionMatrixAsOrtho2D( 0, width, 0, height );
		finalRenderCamera->setViewMatrix( osg::Matrix::identity() );	

		// =-=-=-
		addSlave( finalRenderCamera.get(), osg::Matrixd(), osg::Matrixd(), false );

	}


	return true;
}
#endif


// =-=-=-=-=-=-=-

int main(int argc, char** argv)
{
    // use an ArgumentParser object to manage the program arguments.
    osg::ArgumentParser arguments(&argc,argv);

    arguments.getApplicationUsage()->setApplicationName(arguments.getApplicationName());
    arguments.getApplicationUsage()->setDescription(arguments.getApplicationName()+" is the standard OpenSceneGraph example which loads and visualises 3d models.");
    arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName()+" [options] filename ...");
    arguments.getApplicationUsage()->addCommandLineOption("--image <filename>","Load an image and render it on a quad");
    arguments.getApplicationUsage()->addCommandLineOption("--dem <filename>","Load an image/DEM and render it on a HeightField");
    arguments.getApplicationUsage()->addCommandLineOption("-h or --help","Display command line parameters");
    arguments.getApplicationUsage()->addCommandLineOption("--help-env","Display environmental variables available");
    arguments.getApplicationUsage()->addCommandLineOption("--help-keys","Display keyboard & mouse bindings available");
    arguments.getApplicationUsage()->addCommandLineOption("--help-all","Display all command line, env vars and keyboard & mouse bindings.");
    arguments.getApplicationUsage()->addCommandLineOption("--SingleThreaded","Select SingleThreaded threading model for viewer.");
    arguments.getApplicationUsage()->addCommandLineOption("--CullDrawThreadPerContext","Select CullDrawThreadPerContext threading model for viewer.");
    arguments.getApplicationUsage()->addCommandLineOption("--DrawThreadPerContext","Select DrawThreadPerContext threading model for viewer.");
    arguments.getApplicationUsage()->addCommandLineOption("--CullThreadPerCameraDrawThreadPerContext","Select CullThreadPerCameraDrawThreadPerContext threading model for viewer.");
#ifdef USE_OMNIMAP
	// Add command line options for the window location, the resolution of the channels, and the number of channels.
	// The number of channels must match the number of channels specified by the OmniMap lua configuration files.
	// The default is 3, which is the number of channels used by the Elumenati vertical and horizontal domes.
	// The channel resolution and the number of channels is specified by the OmniMap configuration files,
	// but since we must create the channel cameras before reading the OmniMap configuration files, we need to
	// get these values from the command line.
	arguments.getApplicationUsage()->addCommandLineOption("--width", "The width of the final output window");
	arguments.getApplicationUsage()->addCommandLineOption("--height", "The height of the final output window");
	arguments.getApplicationUsage()->addCommandLineOption("--windowX", "The X location of the final output window");
	arguments.getApplicationUsage()->addCommandLineOption("--windowY", "The Y location of the final output window");
	arguments.getApplicationUsage()->addCommandLineOption("--channelResX", "The X resolution of the offscreen chanels(Default 1024)");
	arguments.getApplicationUsage()->addCommandLineOption("--channelResY", "The Y resolution of the offscreen chanels(Default 1024)");
	arguments.getApplicationUsage()->addCommandLineOption("--numChannels", "The number of OmniMap channels.");
#endif

    // if user request help write it out to cout.
    bool helpAll = arguments.read("--help-all");
    unsigned int helpType = ((helpAll || arguments.read("-h") || arguments.read("--help"))? osg::ApplicationUsage::COMMAND_LINE_OPTION : 0 ) |
                            ((helpAll ||  arguments.read("--help-env"))? osg::ApplicationUsage::ENVIRONMENTAL_VARIABLE : 0 ) |
                            ((helpAll ||  arguments.read("--help-keys"))? osg::ApplicationUsage::KEYBOARD_MOUSE_BINDING : 0 );
    if (helpType)
    {
        arguments.getApplicationUsage()->write(std::cout, helpType);
        return 1;
    }

    

#ifdef USE_OMNIMAP
	OmniMapOSGViewer viewer(arguments);
	viewer.setLightingMode( osg::View::SKY_LIGHT );	
#else
	osgViewer::Viewer viewer(arguments);
#endif

    // report any errors if they have occurred when parsing the program arguments.
    if (arguments.errors())
    {
        arguments.writeErrorMessages(std::cout);
        return 1;
    }
    
    if (arguments.argc()<=1)
    {
        arguments.getApplicationUsage()->write(std::cout,osg::ApplicationUsage::COMMAND_LINE_OPTION);
        return 1;
    }

    // set up the camera manipulators.
    {
        osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

        keyswitchManipulator->addMatrixManipulator( '1', "Trackball", new osgGA::TrackballManipulator() );
        keyswitchManipulator->addMatrixManipulator( '2', "Flight", new osgGA::FlightManipulator() );
        keyswitchManipulator->addMatrixManipulator( '3', "Drive", new osgGA::DriveManipulator() );
        keyswitchManipulator->addMatrixManipulator( '4', "Terrain", new osgGA::TerrainManipulator() );

        std::string pathfile;
        char keyForAnimationPath = '5';
        while (arguments.read("-p",pathfile))
        {
            osgGA::AnimationPathManipulator* apm = new osgGA::AnimationPathManipulator(pathfile);
            if (apm || !apm->valid()) 
            {
                unsigned int num = keyswitchManipulator->getNumMatrixManipulators();
                keyswitchManipulator->addMatrixManipulator( keyForAnimationPath, "Path", apm );
                keyswitchManipulator->selectMatrixManipulator(num);
                ++keyForAnimationPath;
            }
        }

        viewer.setCameraManipulator( keyswitchManipulator.get() );
    }

    // add the state manipulator
    viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
    
    // add the thread model handler
    viewer.addEventHandler(new osgViewer::ThreadingHandler);

    // add the window size toggle handler
    viewer.addEventHandler(new osgViewer::WindowSizeHandler);
        
    // add the stats handler
    viewer.addEventHandler(new osgViewer::StatsHandler);

    // add the help handler
    viewer.addEventHandler(new osgViewer::HelpHandler(arguments.getApplicationUsage()));

    // add the record camera path handler
    viewer.addEventHandler(new osgViewer::RecordCameraPathHandler);

    // load the data
    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFiles(arguments);
    if (!loadedModel) 
    {
        std::cout << arguments.getApplicationName() <<": No data loaded" << std::endl;
        return 1;
    }

    // any option left unread are converted into errors to write out later.
    arguments.reportRemainingOptionsAsUnrecognized();

    // report any errors if they have occurred when parsing the program arguments.
    if (arguments.errors())
    {
        arguments.writeErrorMessages(std::cout);
        return 1;
    }

    // optimize the scene graph, remove redundant nodes and state etc.
    //osgUtil::Optimizer optimizer;
    //optimizer.optimize(loadedModel.get());

#ifdef USE_OMNIMAP
	unsigned int numChannels = 3;
	while (arguments.read("--numChannels",numChannels)) {}
	viewer.initSlaveCameras(numChannels, arguments);
#endif
    viewer.setSceneData( loadedModel.get() );
	
#ifdef USE_OMNIMAP	
	// Add the Realize Operation to construct OmniMap.  At Realization, we have the OpenGL
	// context necessary for OmniMap construction.
	OMVRealizeOp *realizeOp = new OMVRealizeOp(&viewer);
	viewer.setRealizeOperation(realizeOp);
    viewer.realize();
	viewer.frame();
#endif

    viewer.run();

}
