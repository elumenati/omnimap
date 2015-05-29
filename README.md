![Panorama](http://www.clementshimizu.com/wp-content/uploads/2008/12/4mpanodome1.jpg)
# Elumenati OmniMap™ Perspective Projection Mapping toolkit for Fisheye based projection systems
By Clement Shimizu, D'nardo Colucci, and Jim Terhorst

Typical video projection systems display rectangular images on flat screens. Optical and perspective correction techniques must be employed to produce undistorted output on non-planar display surfaces. A two-pass algorithm, called projective perspective mapping, is a solution well suited for use with commodity graphics hardware. This algorithm is implemented in the OmniMap API providing an extensible, reusable C++ interface for porting 3D engines to wide field-of-view, non-planar displays. This API can be easily integrated into a wide variety of 3D applications.
<p align="center">
<img src="http://www.clementshimizu.com/wp-content/uploads/2008/12/Elumenati-Magic-Planet-Dome-Spin.gif" >
</p>
# Attention Unity3D Developers
![Unity3D Projection Mapping](http://www.clementshimizu.com/wp-content/uploads/2014/08/Pacman-3D-Projection-Mapped-1024x576.jpg)
Elumenati built a special projection mapping kit for Unity3D based applications that is available on the asset store.
*     If you are building a new application, this is the easiest path to dome enabled visualizations.
*     Find out more here: http://www.elumenati.com/products/software/omnity/
*     Asset store link: http://u3d.as/content/the-elumenati/omnity-projection-mapping-for-immersive-environments/4u3
*     Intro and tutorial video
   *     https://www.youtube.com/watch?v=7l959KWxpuI
   *     https://www.youtube.com/watch?v=jzCoRoz7Pps
*     Advanced multiprojector edge blending upgrades available.

# What version of Elumenati OmniMap should I use? 
Most OpenGL and DirectX developers should the prebuilt binary Omnity.DLL in thier project along with the needed runtime environment.  This works for most application developers.
* Download the installer here:
  * http://www.elumenati.com/products/software/omnimap-api/
  * Make sure to read the Omnimap API integration guide

If the above doesn't work, for example your application uses custom rendering loops and manages it's own camera/render textures, then the process of dome enabling will require the developer to take additional steps.

Another option is to use the pre built Omnimap.dll/Omnimap.h from the installer and derive a custom OmniMap class from one of the exposed classes in the application's source code.

This repository contains the source code needed to build the Omnity.dll.  You will only need to do this if you need to get deep access to the way Omnity works.

# Attention Academic Researchers
For those interested in learning more about the Perspective Projection Mapping algorthim can learn more from our academic publication:

[http://www.clementshimizu.com/wp-content/uploads/2014/08/Omnimap-Projection-Mapping-Using-Fisheye-Lens.pdf](http://www.clementshimizu.com/wp-content/uploads/2014/08/Omnimap-Projection-Mapping-Using-Fisheye-Lens.pdf)

BIBTEX CITATION
```
@InProceedings{shimizu0009,
title = {OmniMap: Projective Perspective Mapping API for Non-planar Immersive Display Surfaces},
author = {Clement Shimizu and Jim Terhorst and David McConville},
booktitle = {ISVC '08},
year = {2008},
pages = {975--986},
}
```
# Where to find more information
For more information on immersive projection systems please visit the Elumenati homepage: 
http://www.elumenati.com/
 
<a href="http://www.elumenati.com/" rel="Elumenati Logo">![Logo](http://www.elumenati.com/wp-content/themes/elumenati/images/logo.png)</a>
