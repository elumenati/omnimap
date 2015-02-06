/*************************************************
*                                                *
*  EasyBMP Cross-Platform Windows Bitmap Library * 
*                                                *
*  Author: Paul Macklin                          *
*   email: macklin01@users.sourceforge.net       *
* support: http://easybmp.sourceforge.net        *
*                                                *
*          file: EasyBMP_VariousBMPutilities.h   *
*    date added: 01-31-2005                      *
* date modified: 10-22-2005                      *
*       version: 0.70                            *
*                                                *
*   License: BSD (revised/modified)              *
* Copyright: 2005 by the EasyBMP Project         * 
*                                                *
* description: Main include file                 *
*                                                *
*************************************************/

#include <iostream>
#include <cmath>

#ifdef __BCPLUSPLUS__ 
// The Borland compiler must use this because something
// is wrong with their cstdio file. 
#include <stdio.h>
#else 
#include <cstdio>
#endif

#ifdef _MSC_VER 
// If MSVC++ specific code is ever required, this is 
// how it's done. 
#endif

#ifdef __GNUC__
// If g++ specific code is ever required, this is 
// how it's done. 
#endif

#ifdef __INTEL_COMPILER
// If Intel specific code is ever required, this is 
// how it's done. 
#endif

#include "EasyBMP_DataStructures.h"
#include "EasyBMP_BMP.h"
#include "EasyBMP_VariousBMPutilities.h"

#ifndef _EasyBMP_Version_
#define _EasyBMP_Version_ "0.70"
#endif
