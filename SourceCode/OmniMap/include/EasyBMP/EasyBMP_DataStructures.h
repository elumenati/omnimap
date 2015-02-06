/*************************************************
*                                                *
*  EasyBMP Cross-Platform Windows Bitmap Library * 
*                                                *
*  Author: Paul Macklin                          *
*   email: macklin01@users.sourceforge.net       *
* support: http://easybmp.sourceforge.net        *
*                                                *
*          file: EasyBMP_VariousBMPutilities.h   *
*    date added: 05-02-2005                      *
* date modified: 10-22-2005                      *
*       version: 0.70                            *
*                                                *
*   License: BSD (revised/modified)              *
* Copyright: 2005 by the EasyBMP Project         * 
*                                                *
* description: Defines basic data structures for *
*              the BMP class                     *
*                                                *
*************************************************/

#ifndef _EasyBMP_Custom_Math_Functions_
#define _EasyBMP_Custom_Math_Functions_
inline double Square( double number )
{ return number*number; }

inline int IntSquare( int number )
{ return number*number; }

inline int IntPow( int base, int exponent )
{
 int i=0;
 int output = 1;
 for( i=0 ; i < exponent ; i++ )
 { output *= base; }
 return output;
}
#endif

#ifndef _WINGDI_H
#ifndef _SELF_DEFINED_WINGDI
#define _SELF_DEFINED_WINGDI
 typedef unsigned char BYTE;
 typedef unsigned short WORD;
 typedef unsigned long DWORD;
#endif
#endif 

#ifndef _EasyBMP_DataStructures_h_
#define _EasyBMP_DataStructures_h_

inline bool IsBigEndian()
{
 short word = 0x0001;
 if((*(char *)& word) != 0x01 )
 { return true; }
 return false;
}

inline WORD FlipWORD( WORD in )
{ return ( (in >> 8) | (in << 8) ); }

inline DWORD FlipDWORD( DWORD in )
{
 return ( ((in&0xFF000000)>>24) | ((in&0x000000FF)<<24) | 
          ((in&0x00FF0000)>>8 ) | ((in&0x0000FF00)<<8 )   );
}

// it's easier to use a struct than a class
// because we can read/write all four of the bytes 
// at once (as we can count on them being continuous 
// in memory

typedef struct RGBApixel {
	BYTE	Blue;
	BYTE	Green;
	BYTE	Red;
	BYTE	Alpha;
} RGBApixel; 

class BMFH{
public:
 WORD	bfType;
 DWORD	bfSize;
 WORD	bfReserved1;
 WORD	bfReserved2;
 DWORD	bfOffBits; 

 BMFH();
 void display( void );
 void SwitchEndianess( void );
};

BMFH::BMFH()
{
 bfType = 19778;
 bfReserved1 = 0;
 bfReserved2 = 0;
}

void BMFH::SwitchEndianess( void )
{
 bfType = FlipWORD( bfType );
 bfSize = FlipDWORD( bfSize );
 bfReserved1 = FlipWORD( bfReserved1 );
 bfReserved2 = FlipWORD( bfReserved2 );
 bfOffBits = FlipDWORD( bfOffBits );
 return;
}

class BMIH{
public:
 DWORD biSize;
 DWORD biWidth;
 DWORD biHeight;
 WORD biPlanes;
 WORD biBitCount;
 DWORD biCompression;
 DWORD biSizeImage;
 DWORD biXPelsPerMeter;
 DWORD biYPelsPerMeter;
 DWORD biClrUsed;
 DWORD biClrImportant;

 BMIH();
 void display( void );
 void SwitchEndianess( void );
};

BMIH::BMIH()
{
 biPlanes = 1;
 biCompression = 0;
 biXPelsPerMeter = 0; // change this?
 biYPelsPerMeter = 0; // change this?
 biClrUsed = 0;
 biClrImportant = 0;
}

void BMIH::SwitchEndianess( void )
{
 biSize = FlipDWORD( biSize );
 biWidth = FlipDWORD( biWidth );
 biHeight = FlipDWORD( biHeight );
 biPlanes = FlipWORD( biPlanes );
 biBitCount = FlipWORD( biBitCount );
 biCompression = FlipDWORD( biCompression );
 biSizeImage = FlipDWORD( biSizeImage );
 biXPelsPerMeter = FlipDWORD( biXPelsPerMeter );
 biYPelsPerMeter = FlipDWORD( biYPelsPerMeter );
 biClrUsed = FlipDWORD( biClrUsed );
 biClrImportant = FlipDWORD( biClrImportant );
 return;
}

void BMIH::display( void )
{
 using namespace std;
 cout << "biSize: " << (int) biSize << "\n"
      << "biWidth: " << (int) biWidth << "\n"
      << "biHeight: " << (int) biHeight << "\n"
      << "biPlanes: " << (int) biPlanes << "\n"
      << "biBitCount: " << (int) biBitCount << "\n"
      << "biCompression: " << (int) biCompression << "\n"
      << "biSizeImage: " << (int) biSizeImage << "\n"
      << "biXPelsPerMeter: " << (int) biXPelsPerMeter << "\n"
      << "biYPelsPerMeter: " << (int) biYPelsPerMeter << "\n"
      << "biClrUsed: " << (int) biClrUsed << "\n"
      << "biClrImportant: " << (int) biClrImportant << "\n\n";
}

void BMFH::display( void )
{
 using namespace std;
 cout << "bfType: " << (int) bfType << "\n"
      << "bfSize: " << (int) bfSize << "\n"
      << "bfReserved1: " << (int) bfReserved1 << "\n"
      << "bfReserved2: " << (int) bfReserved2 << "\n"
      << "bfOffBits: " << (int) bfOffBits << "\n\n";
}

#endif
