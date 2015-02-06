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
* description: Various utilities.                *
*                                                *
*************************************************/

#ifndef _EasyBMP_VariousBMPutilities_h_
#define _EasyBMP_VariousBMPutilities_h_

BMFH GetBMFH( const char* szFileNameIn );
BMIH GetBMIH( const char* szFileNameIn );
void DisplayBitmapInfo( const char* szFileNameIn );
int GetBitmapColorDepth( const char* szFileNameIn );
void PixelToPixelCopy( BMP& From, int FromX, int FromY,  
                       BMP& To, int ToX, int ToY);
void PixelToPixelCopyTransparent( BMP& From, int FromX, int FromY,  
                                  BMP& To, int ToX, int ToY,
                                  RGBApixel& Transparent );
void RangedPixelToPixelCopy( BMP& From, int FromL , int FromR, int FromB, int FromT, 
                             BMP& To, int ToX, int ToY );
void RangedPixelToPixelCopyTransparent( 
     BMP& From, int FromL , int FromR, int FromB, int FromT, 
     BMP& To, int ToX, int ToY ,
     RGBApixel& Transparent );
bool CreateGrayscaleColorTable( BMP& InputImage );

BMFH GetBMFH( const char* szFileNameIn )
{
 using namespace std;
 BMFH bmfh;

 FILE* fp;
 fp = fopen( szFileNameIn,"rb");
 
 if( !fp  )
 {
  cout << "EasyBMP Error: Cannot initialize from file " << szFileNameIn << ".\n"
       << "               File cannot be opened or does not exist.\n";
  bmfh.bfType = 0;
  return bmfh;
 } 
 
 SafeFread( (char*) &(bmfh.bfType) , sizeof(WORD) , 1 , fp );
 SafeFread( (char*) &(bmfh.bfSize) , sizeof(DWORD) , 1 , fp ); 
 SafeFread( (char*) &(bmfh.bfReserved1) , sizeof(WORD) , 1 , fp ); 
 SafeFread( (char*) &(bmfh.bfReserved2) , sizeof(WORD) , 1 , fp ); 
 SafeFread( (char*) &(bmfh.bfOffBits) , sizeof(DWORD) , 1 , fp ); 
 
 fclose( fp );
 
 if( IsBigEndian() )
 {
  cout << "\tSwitching endianess for big-endian architecture ... \n";
  bmfh.SwitchEndianess();
 }

 return bmfh;
}

BMIH GetBMIH( const char* szFileNameIn )
{
 using namespace std;
 BMFH bmfh;
 BMIH bmih;

 FILE* fp;
 fp = fopen( szFileNameIn,"rb");

 if( !fp  )
 {
  cout << "EasyBMP Error: Cannot initialize from file " << szFileNameIn << ".\n"
       << "               File cannot be opened or does not exist.\n";
  return bmih;
 } 
 
 // read the bmfh, i.e., first 14 bytes (just to get it out of the way);
 
 BYTE TempBYTE;
 int i;
 for( i = 14 ; i > 0 ; i-- )
 {
  SafeFread( (char*) &TempBYTE , sizeof(BYTE) , 1, fp );
 }

 // read the bmih 

 SafeFread( (char*) &(bmih.biSize) , sizeof(DWORD) , 1 , fp );
 SafeFread( (char*) &(bmih.biWidth) , sizeof(DWORD) , 1 , fp ); 
 SafeFread( (char*) &(bmih.biHeight) , sizeof(DWORD) , 1 , fp ); 
 SafeFread( (char*) &(bmih.biPlanes) , sizeof(WORD) , 1 , fp ); 
 
 SafeFread( (char*) &(bmih.biBitCount) , sizeof(WORD) , 1 , fp );
 SafeFread( (char*) &(bmih.biCompression) , sizeof(DWORD) , 1 , fp ); 
 SafeFread( (char*) &(bmih.biSizeImage) , sizeof(DWORD) , 1 , fp ); 
 SafeFread( (char*) &(bmih.biXPelsPerMeter) , sizeof(DWORD) , 1 , fp ); 
 
 SafeFread( (char*) &(bmih.biYPelsPerMeter) , sizeof(DWORD) , 1 , fp ); 
 SafeFread( (char*) &(bmih.biClrUsed) , sizeof(DWORD) , 1 , fp ); 
 SafeFread( (char*) &(bmih.biClrImportant) , sizeof(DWORD) , 1 , fp ); 
 
 fclose( fp );
 
 if( IsBigEndian() )
 {
  cout << "\tSwitching endianess for big-endian architecture ... \n";
  bmih.SwitchEndianess();
 }

 return bmih;
}

void DisplayBitmapInfo( const char* szFileNameIn )
{
 using namespace std;
 FILE* fp;
 fp = fopen( szFileNameIn,"rb");
 
 if( !fp  )
 {
  cout << "EasyBMP Error: Cannot initialize from file " << szFileNameIn << ".\n"
       << "               File cannot be opened or does not exist.\n";
  return;
 } 
 fclose( fp );

 // don't duplicate work! Just use the functions from above!
 
 BMFH bmfh = GetBMFH(szFileNameIn);
 BMIH bmih = GetBMIH(szFileNameIn);

 cout << "File information for file " << szFileNameIn << ":\n\n";

 cout << "BITMAPFILEHEADER: \n"
      << "bfType: " << bmfh.bfType << "\n"
      << "bfSize: " << bmfh.bfSize << "\n"
      << "bfReserved1: " << bmfh.bfReserved1 << "\n"
      << "bfReserved2: " << bmfh.bfReserved2 << "\n"    
      << "bfOffBits: " << bmfh.bfOffBits << "\n\n";

 cout << "BITMAPINFOHEADER: \n"
      << "biSize: " << bmih.biSize << "\n"
      << "biWidth: " << bmih.biWidth << "\n"
      << "biHeight: " << bmih.biHeight << "\n"
      << "biPlanes: " << bmih.biPlanes << "\n"
      << "biBitCount: " << bmih.biBitCount << "\n"
      << "biCompression: " << bmih.biCompression << "\n"
      << "biSizeImage: " << bmih.biSizeImage << "\n"
      << "biXPelsPerMeter: " << bmih.biXPelsPerMeter << "\n"
      << "biYPelsPerMeter: " << bmih.biYPelsPerMeter << "\n"
      << "biClrUsed: " << bmih.biClrUsed << "\n"
      << "biClrImportant: " << bmih.biClrImportant << "\n\n";  
 return;
}

int GetBitmapColorDepth( const char* szFileNameIn )
{
 BMFH bmfh = GetBMFH( szFileNameIn );
 BMIH bmih = GetBMIH( szFileNameIn );
 
 return (int) bmih.biBitCount;
}

void PixelToPixelCopy( BMP& From, int FromX, int FromY,  
                       BMP& To, int ToX, int ToY)
{
 *To(ToX,ToY) = *From(FromX,FromY);
 return;
}

void PixelToPixelCopyTransparent( BMP& From, int FromX, int FromY,  
                                  BMP& To, int ToX, int ToY,
                                  RGBApixel& Transparent )
{
 if( From(FromX,FromY)->Red != Transparent.Red ||
     From(FromX,FromY)->Green != Transparent.Green ||
     From(FromX,FromY)->Blue != Transparent.Blue )	 
 {
  *To(ToX,ToY) = *From(FromX,FromY);
 }
 return;
}

void RangedPixelToPixelCopy( BMP& From, int FromL , int FromR, int FromB, int FromT, 
                             BMP& To, int ToX, int ToY )
{
 // make sure the conventions are followed
 if( FromB < FromT )
 { int nTemp = FromT; FromT = FromB; FromB = nTemp; }

 // make sure that the copied regions exist in both bitmaps
 if( FromR >= From.TellWidth() )
 { FromR = From.TellWidth()-1; }
 if( FromT >= From.TellHeight() )
 { FromT = From.TellHeight()-1; }
 
 if( ToX+(FromR-FromL+1) >= To.TellWidth() )
 { FromR = To.TellWidth()-2+FromL-ToX; }
 if( ToY+(FromB-FromT+1) >= To.TellHeight() )
 { FromB = To.TellHeight()-2+FromT-ToY; } 

 int i,j;
 for( j=FromT ; j <= FromB ; j++ )
 { 
  for( i=FromL ; i <= FromR ; i++ )
  {
   PixelToPixelCopy( From, i,j,  
                     To, ToX+(i-FromL), ToY+(j-FromT) );
  }
 }

 return;
}

void RangedPixelToPixelCopyTransparent( 
     BMP& From, int FromL , int FromR, int FromB, int FromT, 
     BMP& To, int ToX, int ToY ,
     RGBApixel& Transparent )
{
 // make sure the conventions are followed
 if( FromB < FromT )
 { int nTemp = FromT; FromT = FromB; FromB = nTemp; }

 // make sure that the copied regions exist in both bitmaps
 if( FromR >= From.TellWidth() )
 { FromR = From.TellWidth()-1; }
 if( FromT >= From.TellHeight() )
 { FromT = From.TellHeight()-1; }
 
 if( ToX+(FromR-FromL+1) >= To.TellWidth() )
 { FromR = To.TellWidth()-2+FromL-ToX; }
 if( ToY+(FromB-FromT+1) >= To.TellHeight() )
 { FromB = To.TellHeight()-2+FromT-ToY; } 

 int i,j;
 for( j=FromT ; j <= FromB ; j++ )
 { 
  for( i=FromL ; i <= FromR ; i++ )
  {
   PixelToPixelCopyTransparent( From, i,j,  
                     To, ToX+(i-FromL), ToY+(j-FromT) , 
                     Transparent);
  }
 }

 return;
}

bool CreateGrayscaleColorTable( BMP& InputImage )
{
 using namespace std;
 int BitDepth = InputImage.TellBitDepth();
 if( BitDepth != 1 && BitDepth != 4 && BitDepth != 8 )
 {
  cout << "EasyBMP Warning: Attempted to create color table at a bit \n"
       << "                 depth that does not require a color table. \n"
 	   << "                 Ignoring request.\n";  
  return false;
 }
 int i;
 int NumberOfColors = InputImage.TellNumberOfColors();
 
 BYTE StepSize;
 if( BitDepth != 1 )
 { StepSize = 255/(NumberOfColors-1); }
 else
 { StepSize = 255; }
  
 for( i=0 ; i < NumberOfColors ; i++ )
 {
  BYTE TempBYTE = i*StepSize;
  RGBApixel TempColor;
  TempColor.Red = TempBYTE;
  TempColor.Green = TempBYTE;
  TempColor.Blue = TempBYTE;
  TempColor.Alpha = 0;
  InputImage.SetColor( i , TempColor );  
 }
 return true;
}

#endif
