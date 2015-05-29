
![Omnimap](http://www.clementshimizu.com/wp-content/uploads/2008/12/Elumenati-Magic-Planet-Dome-Spin.gif)
# Elumenati OmniMap™ Intro
Typical video projection systems display rectangular images on flat screens. Optical and perspective correction techniques must be employed to produce undistorted output on non-planar display surfaces. A two-pass algorithm, called projective perspective mapping, is a solution well suited for use with commodity graphics hardware. This algorithm is implemented in the OmniMap API providing an extensible, reusable C++ interface for porting 3D engines to wide field-of-view, non-planar displays. This API can be easily integrated into a wide variety of 3D applications.

# Note to Unity3D developers 
Elumenati built a special projection mapping kit that is availble on the Unity3D asset store
*     http://www.elumenati.com/products/software/omnity/
*     http://u3d.as/content/the-elumenati/omnity-projection-mapping-for-immersive-environments/4u3
*     Intro and tutorial video here
   *     https://www.youtube.com/watch?v=7l959KWxpuI
   *     https://www.youtube.com/watch?v=jzCoRoz7Pps
*     Advanced multiprojector edge blending upgrades availble.

# What version of Elumenati OmniMap/Omnity should I use? 
Most developers should the prebuilt binary Omnity.DLL in thier project along with the needed runtime environment.  This repository contains the source code needed to build Omnity.dll.

To help you get on the correct track, we list a few options here:
* OpenGL or D3D 9, 10 based windows applications with source code access and typical display loop 
   * Use a pre built Omnimap.dll
   * Download an installer here:
      * http://www.elumenati.com/products/software/omnimap-api/
      * Read the Omnimap API integration guide
* Applications (with source code access) and atypical display loop
   * If the above doesn't work, for example your application uses custom rendering loops and manages it's own camera/render textures, then the process of dome enabling will require the developer to take aditional steps.
      * Derived OmniMap Class
         * In this case the developer uses the pre built Omnimap.dll/Omnimap.h from the installer and derive a custom OmniMap class from one of the exposed classes in thier applciations source code.
      * Rebuild OmniMap from source code
         * In this case the developer would download the source code from the Git repo, and customize it to suite the needs of the specific application.
* Everything else
   * Please contact Elumenati with your custom need
