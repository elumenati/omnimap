The original source is Copyright (C) 1998-2006 Robert Osfield 
This application is open source and may be redistributed and/or modified   
freely and without restriction, both in commericial and non commericial applications,
as long as this copyright notice is maintained.
 
This application is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

The original OpenSceneGraph code has been modified by The Elumenati, and IDSI to provide
an example of how Elumenati's OmniMap API can be integrated into an OpenSceneGraph 
application.  All of the above applies to these changes.
 
 
This is an example of how the OmniMap API can be 
integrated into an OpenSceneGraph application.  It is
an adaptation of the osgviewer application that is provided
as part of the OpenSceneGraph release.  

Prerequisites
=============
This code is known to work with the 2.2.0 release of OpenSceneGraph.
The documentation and code assume the application developer has installed
the binary release of OpenSceneGraph 2.2.0.


TO BUILD
========
A VC++ 2008 project file is included for building this example.
You must have installed the 2.x release of the OmniMap API for
this example to work.  If the reader has installed OpenSceneGraph and this package in the
default locations, all of the lib search paths and include search paths
will be correct out of the box. However, if you installed these packages
in a different location you will need to correct the search paths to
point to the OmniMap API lib directory, and the OpenSceneGraph 2.2.0
lib directory.  You will need to correct the search paths for the
include directory to reference the OmniMap API include directory and the
OpenSceneGraph 2.2.0 include directory.  See the VC++ 2008 documentation
for information on setting library and include search paths.  

To build, just select "Build Solution" from the build menu.  

TO RUN
======
To run the application, it is assumed you will be executing from the
OmniMapAPI bin directory.
(Default is "C:\Program Files\Elumenati\OmniMap\bin")
You will need the OpenSceneGraph 2.2.0 bin in your path so that the application
can find the OSG dll's.
(Default is "C:\Program Files\OpenSceneGraph\bin")
The example uses the OmniMapOSGDefaults directory that is installed in
"C:\Program Files\Elumenati\OmniMap\bin" as the LUA defaults directory for
OmniMap.

osgViewer with OmniMap integration has the following command line arguments:
 --width n where n is the width of the output window.(default 1280)
 --height n where n is the height of the output window.(default 1024)
 --windowX n where n is the x location of the output window.(default 50)
 --windowY n where n is the y location of the output window.(default 50)
 --numChannels n where n is the number of channels expected by OmniMap.
	The number of channels must match the number of channels specified by the OmniMap lua configuration files.
	The default is 3, which is the number of channels used by the Elumenati vertical and horizontal domes.
 --channelResX The horizontal resolution of the channels.
 --channelResY The vertical resolution of the channels.
 The channel resolution and the number of channels is specified by the OmniMap configuration files,
 but since we must create the channel cameras before reading the OmniMap configuration files, we need to
 get these values from the command line.
 
 The name of the model to load is also expected on the command line.  So for example, to
 load the cessna.osg file and place a 1400x1050 window at the location 1280,0, the command line
 would be:
 osgViewer --width 1280 --height 1024 --windowX 1500 --windowY 0 "C:\Program Files\OpenSceneGraph\data\cessna.osg"
 
 

Known issues
==============
We use the back buffer
for the implementation of the offscreen rendering channels.  
We are forced to do this because if we run the viewer
multithreaded, Framebuffer Ojbects do not work.  This has nothing to do with
OmniMap.  It appears to be a problem with OSG.  This can be proven by running
the osgdistortion example(which uses frame buffer objects) with the viewer 
in multithreaded mode.  OSG gets an FBO error when this is done.  

Since we are using the back buffer for channel rendering, channel resolutions
must be <= the resolution of the final screen output.  Otherwise the off screen
render channels will get clipped to the size of the final screen output, wich
causes missing pixels in the final output..

Please contact Jim Terhorst (jym@elumenati.com) if you have any questions or feedback.