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
* description: Defines BMP class                 *
*                                                *
*************************************************/

#ifndef _EasyBMP_BMP_h_
#define _EasyBMP_BMP_h_

bool SafeFread( char* buffer, int size, int number, FILE* fp );

class BMP
{private:
 int BitDepth;
 int Width;
 int Height;
 RGBApixel** Pixels;
 RGBApixel* Colors;

 public: 

 int TellBitDepth( void );
 int TellWidth( void );
 int TellHeight( void );
 int TellNumberOfColors( void );
  
 BMP();
 ~BMP();
 RGBApixel* operator()(int i,int j);
 
 bool CreateStandardColorTable( void );
 
 bool SetSize( int NewWidth, int NewHeight );
 bool SetBitDepth( int NewDepth );
 bool WriteToFile( const char* FileName );
 bool ReadFromFile( const char* FileName );
 
 RGBApixel GetColor( int ColorNumber );
 bool SetColor( int ColorNumber, RGBApixel NewColor ); 
};

bool BMP::SetColor( int ColorNumber , RGBApixel NewColor )
{
 using namespace std;
 if( BitDepth != 1 && BitDepth != 4 && BitDepth != 8 )
 {
  cout << "EasyBMP Warning: Attempted to change color table for a BMP object\n"
       << "                 that lacks a color table. Ignoring request.\n";
  return false;
 }
 if( ColorNumber >= TellNumberOfColors() )
 {
  cout << "EasyBMP Warning: Requested color number " << ColorNumber << " is outside the allowed \n"
       << "                 range [0," << TellNumberOfColors()-1 << "]. Ignoring request to set this color.\n";
  return false;
 }
 Colors[ColorNumber] = NewColor;
 return true;
}

RGBApixel BMP::GetColor( int ColorNumber )
{ 
 RGBApixel Output;
 using namespace std;
 if( BitDepth != 1 && BitDepth != 4 && BitDepth != 8 )
 {
  cout << "EasyBMP Warning: Attempted to access color table for a BMP object\n"
       << "                 that lacks a color table. Ignoring request.\n";
  Output.Alpha=0;
  Output.Red=0;
  Output.Green=0;
  Output.Blue=0;
  return Output;
 }
 if( ColorNumber >= TellNumberOfColors() )
 {
  cout << "EasyBMP Warning: Requested color number " << ColorNumber << " is outside the allowed \n"
       << "                 range [0," << TellNumberOfColors()-1 << "]. Ignoring request to get this color.\n";
    Output.Alpha=0;
  Output.Red=0;
  Output.Green=0;
  Output.Blue=0;
  return Output;
 }
 Output = Colors[ColorNumber];
 return Output;
}

BMP::BMP()
{
 Width = 1;
 Height = 1;
 BitDepth = 24;
 Pixels = new RGBApixel* [Width];
 Pixels[0] = new RGBApixel [Height];
 Colors = NULL;
}

BMP::~BMP()
{
 int i;
 for(i=0;i<Width;i++)
 {
  delete [] Pixels[i];
 }
 delete [] Pixels;
 if( Colors )
 { delete Colors; }
} 

RGBApixel* BMP::operator()(int i, int j)
{
 using namespace std;
 bool Warn = false;
 if( i >= Width ){ i = Width-1; Warn = true; }
 if( i < 0 ){ i = 0; Warn = true; }
 if( j >= Height ){ j = Height-1; Warn = true; }
 if( j < 0 ){ j = 0; Warn = true; }
 if( Warn )
 {
  cout << "EasyBMP Warning: Attempted to access non-existent pixel;\n"
       << "                 Truncating request to fit in the range [0,"
       << Width-1 << "] x [0," << Height-1 << "].\n";
 }	
 return &(Pixels[i][j]);
}

int BMP::TellBitDepth( void )
{ return BitDepth; }

int BMP::TellHeight( void )
{ return Height; }

int BMP::TellWidth( void )
{ return Width; }

int BMP::TellNumberOfColors( void )
{
 int output = IntPow( 2, BitDepth );
 if( BitDepth == 32 )
 { output = IntPow( 2, 24 ); }
 return output;
}

bool BMP::SetBitDepth( int NewDepth )
{
 using namespace std;
 if( NewDepth != 1 && NewDepth != 4 && 
     NewDepth != 8 && NewDepth != 16 && 
	 NewDepth != 24 && NewDepth != 32 )
 {
  cout << "EasyBMP Warning: User attempted to set unsupported bit depth " << NewDepth << ".\n"
       << "                 Bit depth remains unchanged at " << BitDepth << ".\n";
  return false;
 }
 
 if( NewDepth == 16 )
 {
  // we'll temporarily disable 16-bit work 
  cout << "EasyBMP Warning: User attempted to set unsupported bit depth " << NewDepth << ".\n"
       << "                 Bit depth remains unchanged at " << BitDepth << ".\n";
  return false;
 }

 BitDepth = NewDepth;
 if( Colors )
 { delete Colors; }
 int NumberOfColors = IntPow( 2, BitDepth );
 if( BitDepth == 1 || BitDepth == 4 || BitDepth == 8 )
 { Colors = new RGBApixel [NumberOfColors]; }
 else
 { Colors = NULL; } 
 if( BitDepth == 1 || BitDepth == 4 || BitDepth == 8 )
 {
  CreateStandardColorTable();
 }
 
 return true;
}

bool BMP::SetSize(int NewWidth , int NewHeight )
{
 using namespace std;
 if( NewWidth <= 0 || NewHeight <= 0 )
 {
  cout << "EasyBMP Warning: User attempted to set a non-positive width or height.\n"
       << "                 Size remains unchanged at " << Width << " x " << Height << ".\n";
  return false;
 }

 int i,j; 

 for(i=0;i<Width;i++)
 {
  delete [] Pixels[i];
 }
 delete [] Pixels;

 Width = NewWidth;
 Height = NewHeight;
 Pixels = new RGBApixel* [ Width ]; 
 
 for(i=0; i<Width; i++)
 {
  Pixels[i] = new RGBApixel [ Height ];
 }
 
 for( i=0 ; i < Width ; i++)
 {
  for( j=0 ; j < Height ; j++ )
  {
   Pixels[i][j].Red = 255; 
   Pixels[i][j].Green = 255; 
   Pixels[i][j].Blue = 255; 
   Pixels[i][j].Alpha = 0;    
  }
 }

 return true; 
}

bool BMP::WriteToFile( const char* FileName )
{
 using namespace std;
 FILE* fp = fopen( FileName, "wb" );
 if( fp == NULL )
 {
  cout << "EasyBMP Error: Cannot open file " << FileName << " for output.\n";
  fclose( fp );
  return false;
 }
 
 if( BitDepth == 16 )
 {
  // temporary: if 16-bit, save as 24-bit
  cout << "EasyBMP Warning: 16-bit output currently not supported. Using 24-bit depth.\n";
  SetBitDepth( 24 ); 
 }
 
 // some preliminaries
 
 double dBytesPerPixel = ( (double) BitDepth ) / 8.0;
 double dBytesPerRow = dBytesPerPixel * (Width+0.0);
 dBytesPerRow = ceil(dBytesPerRow);
  
 int BytePaddingPerRow = 4 - ( (int) (dBytesPerRow) )% 4;
 if( BytePaddingPerRow == 4 )
 { BytePaddingPerRow = 0; } 
 
 double dActualBytesPerRow = dBytesPerRow + BytePaddingPerRow;
 
 double dTotalPixelBytes = Height * dActualBytesPerRow;
 
 double dPaletteSize = 0;
 if( BitDepth == 1 || BitDepth == 4 || BitDepth == 8 )
 { dPaletteSize = IntPow(2,BitDepth)*4.0; }
 
 double dTotalFileSize = 14 + 40 + dPaletteSize + dTotalPixelBytes;
 
 // write the file header 
 
 BMFH bmfh;
 bmfh.bfSize = (DWORD) dTotalFileSize; // bfSize
 bmfh.bfReserved1 = 0; // bfReserved1
 bmfh.bfReserved2 = 0; // bfReserved2
 bmfh.bfOffBits = (DWORD) (14+40+dPaletteSize); // bfOffBits 
 
 if( IsBigEndian() )
 {
  cout << "\tSwitching endianess for big-endian architecture ... \n";
  bmfh.SwitchEndianess();
 }
 
 fwrite( (char*) &(bmfh.bfType) , sizeof(WORD) , 1 , fp );
 fwrite( (char*) &(bmfh.bfSize) , sizeof(DWORD) , 1 , fp );
 fwrite( (char*) &(bmfh.bfReserved1) , sizeof(WORD) , 1 , fp );
 fwrite( (char*) &(bmfh.bfReserved2) , sizeof(WORD) , 1 , fp );
 fwrite( (char*) &(bmfh.bfOffBits) , sizeof(DWORD) , 1 , fp );
 
 // write the info header 
 
 BMIH bmih;
 bmih.biSize = 40;
 bmih.biWidth = Width;
 bmih.biHeight = Height;
 bmih.biPlanes = 1;
 bmih.biBitCount = (WORD)BitDepth;
 bmih.biCompression = 0;
 bmih.biSizeImage = (DWORD) dTotalPixelBytes;
 bmih.biXPelsPerMeter = 0;
 bmih.biYPelsPerMeter = 0;
 bmih.biClrUsed = 0;
 bmih.biClrImportant = 0;
 
 if( IsBigEndian() )
 {
  cout << "\tSwitching endianess for big-endian architecture ... \n";
  bmih.SwitchEndianess();
 }
 
 fwrite( (char*) &(bmih.biSize) , sizeof(DWORD) , 1 , fp );
 fwrite( (char*) &(bmih.biWidth) , sizeof(DWORD) , 1 , fp );
 fwrite( (char*) &(bmih.biHeight) , sizeof(DWORD) , 1 , fp );
 fwrite( (char*) &(bmih.biPlanes) , sizeof(WORD) , 1 , fp );
 fwrite( (char*) &(bmih.biBitCount) , sizeof(WORD) , 1 , fp );
 fwrite( (char*) &(bmih.biCompression) , sizeof(DWORD) , 1 , fp );
 fwrite( (char*) &(bmih.biSizeImage) , sizeof(DWORD) , 1 , fp );
 fwrite( (char*) &(bmih.biXPelsPerMeter) , sizeof(DWORD) , 1 , fp );
 fwrite( (char*) &(bmih.biYPelsPerMeter) , sizeof(DWORD) , 1 , fp ); 
 fwrite( (char*) &(bmih.biClrUsed) , sizeof(DWORD) , 1 , fp);
 fwrite( (char*) &(bmih.biClrImportant) , sizeof(DWORD) , 1 , fp);
 
 // write the palette 
 if( BitDepth == 1 || BitDepth == 4 || BitDepth == 8 )
 {
  int NumberOfColors = IntPow(2,BitDepth);
  
  // if there is no palette, create one 
  if( !Colors )
  {
   if( !Colors )
   { Colors = new RGBApixel [NumberOfColors]; }
   CreateStandardColorTable(); // ( &Colors , BitDepth );
  }
   
  int n;
  for( n=0 ; n < NumberOfColors ; n++ )
  {
   fwrite( (char*) &(Colors[n]) , 4 , 1 , fp );
  }
 }
 
 // write the pixels 
 int i,j;
 if( BitDepth == 24 || BitDepth == 32 )
 {
  int MagicNumber = BitDepth / 8;
  for( j = Height-1 ; j >= 0 ; j-- )
  {
   for( i = 0 ; i < Width ; i++ )
   {
    fwrite( (char*) &(Pixels[i][j])  , MagicNumber , 1 , fp);     
   }
   // add any necessary padding to align to _ byte boundaries
   char* szTemp;
   szTemp = new char [BytePaddingPerRow];
   fwrite( szTemp , BytePaddingPerRow , 1 , fp );   
  }
 }
 
 if( BitDepth == 8 )
 {
  int DataBytes = Width;
  int PaddingBytes = ( 4 - DataBytes % 4 ) % 4;
 
  for( j = Height-1 ; j >= 0 ; j-- )
  {
   // write all row pixel data
   i=0;
   int WriteNumber = 0;
   while( WriteNumber < DataBytes )
   {
    int Index1 = 0;
	
    int k=0;
    int BestDistance = 999999;	
    if( i < Width )
    {
     // choose a best index.
     while( k < 256 && BestDistance > 0 )
     {
      int TempDistance = IntSquare( (int) Pixels[i][j].Red - (int) Colors[k].Red )
                       + IntSquare( (int) Pixels[i][j].Green - (int) Colors[k].Green ) 
                       + IntSquare( (int) Pixels[i][j].Blue - (int) Colors[k].Blue );
      if( TempDistance < BestDistance )
      {
       BestDistance = TempDistance;
       Index1 = k;
      }	  
      k++;
     }
     i++;	
    }
    BYTE RawBYTE = (BYTE) Index1;
    fwrite( (char*) &RawBYTE , 1, 1, fp);
    WriteNumber++;
   }
   // read any necessary row padding
   WriteNumber = 0;
   while( WriteNumber < PaddingBytes )
   {
    BYTE TempBYTE;
    fwrite( (char*) &TempBYTE , 1, 1, fp);
    WriteNumber++;
   }
  }
 }

 if( BitDepth == 4 )
 {
  int DataBytes = (int) ceil( Width / 2.0 );
  int PaddingBytes = ( 4 - DataBytes % 4 ) % 4;
 
  for( j = Height-1 ; j >= 0 ; j-- )
  {
   // write all row pixel data
   i=0;
   int WriteNumber = 0;
   while( WriteNumber < DataBytes )
   {
    int Index1 = 0;
    int Index2 = 0;
    
    int k;
    int BestDistance;	
    if( i < Width )
    {
     k=0;
     BestDistance = 999999;	
     // choose a best index.
     while( k < 16 )
     {
      int TempDistance = IntSquare( (int) Pixels[i][j].Red - (int) Colors[k].Red )
                       + IntSquare( (int) Pixels[i][j].Green - (int) Colors[k].Green )
                       + IntSquare( (int) Pixels[i][j].Blue - (int) Colors[k].Blue );
      if( TempDistance < BestDistance )
      {
       BestDistance = TempDistance;
       Index1 = k;
      }	  
      k++;
     }
     i++;	
    }
    
    if( i < Width )
    {
     k=0;
     BestDistance = 999999;	
     // choose a best index.
     while( k < 16 )
     {
      int TempDistance = IntSquare( (int) Pixels[i][j].Red - (int) Colors[k].Red )
                       + IntSquare( (int) Pixels[i][j].Green - (int) Colors[k].Green )
                       + IntSquare( (int) Pixels[i][j].Blue - (int) Colors[k].Blue );
      if( TempDistance < BestDistance )
      {
       BestDistance = TempDistance;
       Index2 = k;
      }	  
      k++;
     }
     i++;	
    }
    
    BYTE RawBYTE = (BYTE) (Index2 + Index1*16);
    fwrite( (char*) &RawBYTE , 1, 1, fp);
    WriteNumber++;
   }
   // read any necessary row padding
   WriteNumber = 0;
   while( WriteNumber < PaddingBytes )
   {
    BYTE TempBYTE;
    fwrite( (char*) &TempBYTE , 1, 1, fp);
    WriteNumber++;
   }
  }
 }

 if( BitDepth == 1 )
 {
  int DataBytes = (int) ceil( Width / 8.0 );
  int PaddingBytes = ( 4 - DataBytes % 4 ) % 4;
 
  for( j = Height-1 ; j >= 0 ; j-- )
  {
   // write all row pixel data
   i=0;
   int WriteNumber = 0;
   while( WriteNumber < DataBytes )
   {
    int Index1 = 0;
    int Index2 = 0;
    int Index3 = 0;
    int Index4 = 0;
    int Index5 = 0;
    int Index6 = 0;
    int Index7 = 0;
    int Index8 = 0;
	
    int k;
    int BestDistance;	
    if( i < Width )
    {
     k=0;
     BestDistance = 999999;	
     // choose a best index.
     while( k < 2 )
     {
      int TempDistance = IntSquare( (int) Pixels[i][j].Red - (int) Colors[k].Red )
                       + IntSquare( (int) Pixels[i][j].Green - (int) Colors[k].Green )
                       + IntSquare( (int) Pixels[i][j].Blue - (int) Colors[k].Blue );
      if( TempDistance < BestDistance )
      {
       BestDistance = TempDistance;
       Index1 = k;
      }	  
      k++;
     }
     i++;	
    }
              
    if( i < Width )
    {
     k=0;
     BestDistance = 999999;	
     // choose a best index.
     while( k < 2 )
     {
      int TempDistance = IntSquare( (int) Pixels[i][j].Red - (int) Colors[k].Red )
                       + IntSquare( (int) Pixels[i][j].Green - (int) Colors[k].Green )
                       + IntSquare( (int) Pixels[i][j].Blue - (int) Colors[k].Blue );
      if( TempDistance < BestDistance )
      {
       BestDistance = TempDistance;
       Index2 = k;
      }	  
      k++;
     }
     i++;	
    }
       	
    if( i < Width )
    {
     k=0;
     BestDistance = 999999;	
     // choose a best index.
     while( k < 2 )
     {
      int TempDistance = IntSquare( (int) Pixels[i][j].Red - (int) Colors[k].Red )
                       + IntSquare( (int) Pixels[i][j].Green - (int) Colors[k].Green )
                       + IntSquare( (int) Pixels[i][j].Blue - (int) Colors[k].Blue );
      if( TempDistance < BestDistance )
      {
       BestDistance = TempDistance;
       Index3 = k;
      }	  
      k++;
     }
     i++;	
    }
          	
    if( i < Width )
    {
     k=0;
     BestDistance = 999999;	
     // choose a best index.
     while( k < 2 )
     {
      int TempDistance = IntSquare( (int) Pixels[i][j].Red - (int) Colors[k].Red )
                       + IntSquare( (int) Pixels[i][j].Green - (int) Colors[k].Green )
                       + IntSquare( (int) Pixels[i][j].Blue - (int) Colors[k].Blue );
      if( TempDistance < BestDistance )
      {
       BestDistance = TempDistance;
       Index4 = k;
      }	  
      k++;
     }
     i++;	
    }
           
    if( i < Width )
    {
     k=0;
     BestDistance = 999999;	
     // choose a best index.
     while( k < 2 )
     {
      int TempDistance = IntSquare( (int) Pixels[i][j].Red - (int) Colors[k].Red )
                       + IntSquare( (int) Pixels[i][j].Green - (int) Colors[k].Green )
                       + IntSquare( (int) Pixels[i][j].Blue - (int) Colors[k].Blue );
      if( TempDistance < BestDistance )
      {
       BestDistance = TempDistance;
       Index5 = k;
      }	  
      k++;
     }
     i++;	
    }
         	
    if( i < Width )
    {
     k=0;
     BestDistance = 999999;	
     // choose a best index.
     while( k < 2 )
     {
      int TempDistance = IntSquare( (int) Pixels[i][j].Red - (int) Colors[k].Red )
                       + IntSquare( (int) Pixels[i][j].Green - (int) Colors[k].Green )
                       + IntSquare( (int) Pixels[i][j].Blue - (int) Colors[k].Blue );
      if( TempDistance < BestDistance )
      {
       BestDistance = TempDistance;
       Index6 = k;
      }	  
      k++;
     }
     i++;	
    }
        	
    if( i < Width )
    {
     k=0;
     BestDistance = 999999;	
     // choose a best index.
     while( k < 2 )
     {
      int TempDistance = IntSquare( (int) Pixels[i][j].Red - (int) Colors[k].Red )
                       + IntSquare( (int) Pixels[i][j].Green - (int) Colors[k].Green )
                       + IntSquare( (int) Pixels[i][j].Blue - (int) Colors[k].Blue );
      if( TempDistance < BestDistance )
      {
       BestDistance = TempDistance;
       Index7 = k;
      }	  
      k++;
     }
     i++;	
    }
        
    if( i < Width )
    {
     k=0;
     BestDistance = 999999;	
     // choose a best index.
     while( k < 2 )
     {
      int TempDistance = IntSquare( (int) Pixels[i][j].Red - (int) Colors[k].Red )
                       + IntSquare( (int) Pixels[i][j].Green - (int) Colors[k].Green )
                       + IntSquare( (int) Pixels[i][j].Blue - (int) Colors[k].Blue );
      if( TempDistance < BestDistance )
      {
       BestDistance = TempDistance;
       Index8 = k;
      }	  
      k++;
     }
     i++;	
    }
        
    BYTE RawBYTE = (BYTE) ( Index8    + Index7*2  + Index6*4  + Index5*8
                           +Index4*16 + Index3*32 + Index2*64 + Index1*128 );
    fwrite( (char*) &RawBYTE , 1, 1, fp);
    WriteNumber++;
   }
   // read any necessary row padding
   WriteNumber = 0;
   while( WriteNumber < PaddingBytes )
   {
    BYTE TempBYTE;
    fwrite( (char*) &TempBYTE , 1, 1, fp);
    WriteNumber++;
   }
  }
 }

 fclose(fp);
 return true;
}

bool BMP::ReadFromFile( const char* FileName )
{ 
 using namespace std;
 FILE* fp = fopen( FileName, "rb" );
 if( fp == NULL )
 {
  cout << "EasyBMP Error: Cannot open file " << FileName << " for input.\n";
  SetBitDepth(1);
  SetSize(1,1);
  return false;
 }
 
 // read the file header 
 
 BMFH bmfh;
 bool NotCorrupted = true;
 
 NotCorrupted &= SafeFread( (char*) &(bmfh.bfType) , sizeof(WORD), 1, fp); // bfType
 
 bool IsBitmap = false;
 
 if( IsBigEndian() && bmfh.bfType == 16973 )
 { IsBitmap = true; }
 if( !IsBigEndian() && bmfh.bfType == 19778 )
 { IsBitmap = true; }
 
 if( !IsBitmap ) 
 {
  cout << "EasyBMP Error: " << FileName << " is not a Windows BMP file!\n"; 
  fclose( fp ); 
  return false;
 }

 NotCorrupted &= SafeFread( (char*) &(bmfh.bfSize) , sizeof(DWORD) , 1, fp); // bfSizse 
 NotCorrupted &= SafeFread( (char*) &(bmfh.bfReserved1) , sizeof(WORD) , 1, fp); // bfReserved1
 NotCorrupted &= SafeFread( (char*) &(bmfh.bfReserved2) , sizeof(WORD) , 1, fp); // bfReserved2
 NotCorrupted &= SafeFread( (char*) &(bmfh.bfOffBits) , sizeof(DWORD) , 1 , fp); // bfOffbits
 
 if( IsBigEndian() ) 
 {
  cout << "\tSwitching endianess for big-endian architecture ... \n"; 
  bmfh.SwitchEndianess();
 }
  
 // read the info header

 BMIH bmih; 
 
 NotCorrupted &= SafeFread( (char*) &(bmih.biSize) , sizeof(DWORD) , 1 , fp); // bmih.biSize
 NotCorrupted &= SafeFread( (char*) &(bmih.biWidth) , sizeof(DWORD) , 1 , fp); // bmih.biWidth 
 NotCorrupted &= SafeFread( (char*) &(bmih.biHeight) , sizeof(DWORD) , 1 , fp); // bmih.biHeight
 NotCorrupted &= SafeFread( (char*) &(bmih.biPlanes) , sizeof(WORD) , 1, fp);  // bmih.biPlanes 
 NotCorrupted &= SafeFread( (char*) &(bmih.biBitCount) , sizeof(WORD) , 1, fp); //   bmih.biBitCount 

 NotCorrupted &= SafeFread( (char*) &(bmih.biCompression) , sizeof(DWORD) , 1 , fp); // bmih.biCompression
 NotCorrupted &= SafeFread( (char*) &(bmih.biSizeImage) , sizeof(DWORD) , 1 , fp); // bmih.biSizeImage
 NotCorrupted &= SafeFread( (char*) &(bmih.biXPelsPerMeter) , sizeof(DWORD) , 1 , fp); // bmih.biXPelsPerMeter
 NotCorrupted &= SafeFread( (char*) &(bmih.biYPelsPerMeter) , sizeof(DWORD) , 1 , fp); // bmih.biYPelsPerMeter
 NotCorrupted &= SafeFread( (char*) &(bmih.biClrUsed) , sizeof(DWORD) , 1 , fp); // bmih.biClrUsed
 NotCorrupted &= SafeFread( (char*) &(bmih.biClrImportant) , sizeof(DWORD) , 1 , fp); // bmih.biClrImportant
 
 if( IsBigEndian() ) 
 {
  cout << "\tSwitching endianess for big-endian architecture ... \n"; 
  bmih.SwitchEndianess();
 }

 // a safety catch: if any of the header information didn't read properly, abort
 // future idea: check to see if at least most of self-consistent
  
 if( !NotCorrupted )
 {
  cout << "EasyBMP Error: " << FileName << " is obviously corrupted. \n";
  SetSize(1,1);
  SetBitDepth(1);
  fclose(fp);
  return false;
 } 

 // set the bit depth
 
 int TempBitDepth = (int) bmih.biBitCount;
 if(    TempBitDepth != 1  && TempBitDepth != 4 
     && TempBitDepth != 8  // && TempBitDepth != 16 // temporarily disable 16 bpp 
     && TempBitDepth != 24 && TempBitDepth != 32 )
 {
  cout << "EasyBMP Error: " << FileName << " has unrecognized bit depth. \n";
  SetSize(1,1);
  SetBitDepth(1);
  fclose(fp);
  return false;
 }
 SetBitDepth( (int) bmih.biBitCount ); 
 
 // set the size

 if( (int) bmih.biWidth <= 0 || (int) bmih.biHeight <= 0 ) 
 {
  cout << "EasyBMP Error: " << FileName << " has a non-positive width or height. \n";
  SetSize(1,1);
  SetBitDepth(1);
  fclose(fp);
  return false;
 } 
 SetSize( (int) bmih.biWidth , (int) bmih.biHeight );
  
 // some preliminaries
 
 double dBytesPerPixel = ( (double) BitDepth ) / 8.0;
 double dBytesPerRow = dBytesPerPixel * (Width+0.0);
 dBytesPerRow = ceil(dBytesPerRow);
  
 int BytePaddingPerRow = 4 - ( (int) (dBytesPerRow) )% 4;
 if( BytePaddingPerRow == 4 )
 { BytePaddingPerRow = 0; }  
 
 // if < 16 bits, read the palette

 if( BitDepth == 1 || BitDepth == 4 || BitDepth == 8 )
 {
  int NumberOfColors = IntPow(2,BitDepth);
  Colors = new RGBApixel [NumberOfColors];
  int n;
  for( n=0; n < NumberOfColors ; n++ )
  {
   SafeFread( (char*) &(Colors[n]) , 4 , 1 , fp);     
  }
 }
  
 //read the pixels

 int i,j;
 if( BitDepth == 24 || BitDepth == 32 )
 {
  int SizePerRead = BitDepth / 8;
  for( j = Height-1 ; j >= 0 ; j-- )
  {
   for( i = 0 ; i < Width ; i++ )
   {
    SafeFread( (char*) &(Pixels[i][j])  , SizePerRead , 1 , fp);     
   }
   // read any necessary padding to align to 4 byte boundaries
   char* szTemp;
   szTemp = new char [BytePaddingPerRow];
   SafeFread( szTemp , BytePaddingPerRow , 1 , fp );   
  }
 }

 if( BitDepth == 16 )
 {
  // don't support 16-bit files for now
  cout << __LINE__ << "EasyBMP Error: " << FileName << " has unrecognized bit depth. \n";
  SetSize(1,1);
  SetBitDepth(1);
  fclose(fp);
  return false;
 }
 
 if( BitDepth == 8 )
 {
  int DataBytes = Width;
  int PaddingBytes = ( 4 - DataBytes % 4 ) % 4;
 
  for( j = Height-1 ; j >= 0 ; j-- )
  {
   // read all row pixel data
   i=0;
   int ReadNumber = 0;
   while( ReadNumber < DataBytes )
   {
    BYTE TempBYTE;
    SafeFread( (char*) &TempBYTE , 1, 1, fp);
    ReadNumber++;
    int RawIndex = (int)TempBYTE;
    int Index1 = RawIndex;
    (Pixels[i][j]).Red = Colors[Index1].Red;
    (Pixels[i][j]).Green = Colors[Index1].Green;
    (Pixels[i][j]).Blue = Colors[Index1].Blue;
    (Pixels[i][j]).Alpha = Colors[Index1].Alpha;
    i++;
   }
   // read any necessary row padding
   ReadNumber = 0;
   while( ReadNumber < PaddingBytes )
   {
    BYTE TempBYTE;
    SafeFread( (char*) &TempBYTE , 1, 1, fp);
    ReadNumber++;
   }
  }
 }
 
 if( BitDepth == 4 )
 {
  int DataBytes = (int) ceil( Width*0.5 );
  int PaddingBytes = ( 4 - DataBytes % 4 ) % 4;
 
  for( j = Height-1 ; j >= 0 ; j-- )
  {
   // read all row pixel data
   i=0;
   int ReadNumber = 0;
   while( ReadNumber < DataBytes )
   {
    BYTE TempBYTE;
    SafeFread( (char*) &TempBYTE , 1, 1, fp);
    ReadNumber++;
    int RawIndex = (int)TempBYTE;
    int Index2 = RawIndex % 16;
    int Index1 = (RawIndex - Index2)/16;
    (Pixels[i][j]).Red = Colors[Index1].Red;
    (Pixels[i][j]).Green = Colors[Index1].Green;
    (Pixels[i][j]).Blue = Colors[Index1].Blue;
    (Pixels[i][j]).Alpha = Colors[Index1].Alpha;
    i++;
    if( i < Width )
    {
     (Pixels[i][j]).Red = Colors[Index2].Red;
     (Pixels[i][j]).Green = Colors[Index2].Green;
     (Pixels[i][j]).Blue = Colors[Index2].Blue;
     (Pixels[i][j]).Alpha = Colors[Index2].Alpha;
     i++;
    }	
   }
   
   // read any necessary row padding
   ReadNumber = 0;
   while( ReadNumber < PaddingBytes )
   {
    BYTE TempBYTE;
    SafeFread( (char*) &TempBYTE , 1, 1, fp);
    ReadNumber++;
   }
  }  
 }
 
 if( BitDepth == 1 )
 {
  int DataBytes = (int) ceil( Width/8.0 );
  int PaddingBytes = ( 4 - DataBytes % 4 ) % 4;
 
  for( j = Height-1 ; j >= 0 ; j-- )
  {
   // read all row pixel data
   i=0;
   int ReadNumber = 0;
   while( ReadNumber < DataBytes )
   {
    BYTE TempBYTE;
    SafeFread( (char*) &TempBYTE , 1, 1, fp);
    ReadNumber++;
    int RawIndex = (int)TempBYTE;
    int Index8 = RawIndex % 2;
    RawIndex = (RawIndex - Index8)/2;
    int Index7 = RawIndex % 2;
    RawIndex = (RawIndex - Index7)/2;
    int Index6 = RawIndex % 2;
    RawIndex = (RawIndex - Index6)/2;
    int Index5 = RawIndex % 2;
    RawIndex = (RawIndex - Index5)/2;
    int Index4 = RawIndex % 2;
    RawIndex = (RawIndex - Index4)/2;
    int Index3 = RawIndex % 2;
    RawIndex = (RawIndex - Index3)/2;
    int Index2 = RawIndex % 2;
    RawIndex = (RawIndex - Index2)/2;
    int Index1 = RawIndex;
    
    int Index = Index1;
    (Pixels[i][j]).Red = Colors[Index].Red;
    (Pixels[i][j]).Green = Colors[Index].Green;
    (Pixels[i][j]).Blue = Colors[Index].Blue;
    (Pixels[i][j]).Alpha = Colors[Index].Alpha;
    i++;
    if( i < Width )
    {Index = Index2;
     (Pixels[i][j]).Red = Colors[Index].Red;
     (Pixels[i][j]).Green = Colors[Index].Green;
     (Pixels[i][j]).Blue = Colors[Index].Blue;
     (Pixels[i][j]).Alpha = Colors[Index].Alpha;
     i++;
    }	
    if( i < Width )
    {Index = Index3;
     (Pixels[i][j]).Red = Colors[Index].Red;
     (Pixels[i][j]).Green = Colors[Index].Green;
     (Pixels[i][j]).Blue = Colors[Index].Blue;
     (Pixels[i][j]).Alpha = Colors[Index].Alpha;
     i++;
    }	
    if( i < Width )
    {Index = Index4;
     (Pixels[i][j]).Red = Colors[Index].Red;
     (Pixels[i][j]).Green = Colors[Index].Green;
     (Pixels[i][j]).Blue = Colors[Index].Blue;
     (Pixels[i][j]).Alpha = Colors[Index].Alpha;
     i++;
    }	
    if( i < Width )
    {Index = Index5;
     (Pixels[i][j]).Red = Colors[Index].Red;
     (Pixels[i][j]).Green = Colors[Index].Green;
     (Pixels[i][j]).Blue = Colors[Index].Blue;
     (Pixels[i][j]).Alpha = Colors[Index].Alpha;
     i++;
    }	
    if( i < Width )
    {Index = Index6;
     (Pixels[i][j]).Red = Colors[Index].Red;
     (Pixels[i][j]).Green = Colors[Index].Green;
     (Pixels[i][j]).Blue = Colors[Index].Blue;
     (Pixels[i][j]).Alpha = Colors[Index].Alpha;
     i++;
    }	
    if( i < Width )
    {Index = Index7;
     (Pixels[i][j]).Red = Colors[Index].Red;
     (Pixels[i][j]).Green = Colors[Index].Green;
     (Pixels[i][j]).Blue = Colors[Index].Blue;
     (Pixels[i][j]).Alpha = Colors[Index].Alpha;
     i++;
    }	
    if( i < Width )
    {Index = Index8;
     (Pixels[i][j]).Red = Colors[Index].Red;
     (Pixels[i][j]).Green = Colors[Index].Green;
     (Pixels[i][j]).Blue = Colors[Index].Blue;
     (Pixels[i][j]).Alpha = Colors[Index].Alpha;
     i++;
    }		
   }
   // read any necessary row padding
   ReadNumber = 0;
   while( ReadNumber < PaddingBytes )
   {
    BYTE TempBYTE;
    SafeFread( (char*) &TempBYTE , 1, 1, fp);
    ReadNumber++;
   }
  }   
 }
 fclose(fp);
 return true;
}

bool BMP::CreateStandardColorTable( void )
{
 using namespace std;
 if( BitDepth != 1 && BitDepth != 4 && BitDepth != 8 )
 {
  cout << "EasyBMP Warning: Attempted to create color table at a bit \n"
       << "                 depth that does not require a color table. \n"
 	   << "                 Ignoring request.\n";  
  return false;
 }

 if( BitDepth == 1 )
 {
  int i;
  for( i=0 ; i < 2 ; i++ )
  {
   Colors[i].Red = (BYTE)(i*255);
   Colors[i].Green = (BYTE)(i*255);
   Colors[i].Blue = (BYTE)(i*255);
   Colors[i].Alpha = 0;
  } 
  return true;
 } 

 if( BitDepth == 4 )
 {
  int i = 0;
  int j,k,ell;
  
  // simplify the code for the first 8 colors
  for( ell=0 ; ell < 2 ; ell++ )
  {
   for( k=0 ; k < 2 ; k++ )
   {
    for( j=0 ; j < 2 ; j++ )
    {
     Colors[i].Red = (BYTE)(j*128); 
	 Colors[i].Green = (BYTE)(k*128);
 	 Colors[i].Blue = (BYTE)(ell*128);
   	 i++;
    }
   }
  }
 
  // simplify the code for the last 8 colors
  for( ell=0 ; ell < 2 ; ell++ )
  {
   for( k=0 ; k < 2 ; k++ )
   {
    for( j=0 ; j < 2 ; j++ )
    {
     Colors[i].Red = (BYTE)(j*255);
     Colors[i].Green = (BYTE)(k*255); 
     Colors[i].Blue = (BYTE)(ell*255);
	 i++;
    }
   }
  }
 
  // overwrite the duplicate color
  i=8; 
  Colors[i].Red = 192;
  Colors[i].Green = 192;
  Colors[i].Blue = 192;
   
  for( i=0 ; i < 16 ; i++ )
  {
   Colors[i].Alpha = 0;
  }
  return true;
 }

 if( BitDepth == 8 )
 {
  int i=0;
  int j,k,ell;
    
  // do an easy loop, which works for all but colors 
  // 0 to 9 and 246 to 255
  for( ell=0 ; ell < 4 ; ell++ ) 
  {
   for( k=0 ; k < 8 ; k++ )
   {
    for( j=0; j < 8 ; j++ )
    {
     Colors[i].Red = (BYTE)(j*32); 
     Colors[i].Green = (BYTE)(k*32);
     Colors[i].Blue = (BYTE)(ell*64);
     Colors[i].Alpha = 0;
     i++;
    }
   }
  } 
   
  // now redo the first 8 colors  
  i=0;
  for( ell=0 ; ell < 2 ; ell++ ) 
  {
   for( k=0 ; k < 2 ; k++ )
   {
    for( j=0; j < 2 ; j++ )
    {
     Colors[i].Red = (BYTE)(j*128);
     Colors[i].Green = (BYTE)(k*128);
     Colors[i].Blue = (BYTE)(ell*128);
     i++;
    }
   }
  } 
 
  // overwrite colors 7, 8, 9
  i=7;
  Colors[i].Red = 192;
  Colors[i].Green = 192;
  Colors[i].Blue = 192;
  i++; // 8
  Colors[i].Red = 192;
  Colors[i].Green = 220;
  Colors[i].Blue = 192;
  i++; // 9
  Colors[i].Red = 166;
  Colors[i].Green = 202;
  Colors[i].Blue = 240;
   
  // overwrite colors 246 to 255 
  i=246;
  Colors[i].Red = 255;
  Colors[i].Green = 251;
  Colors[i].Blue = 240;
  i++; // 247
  Colors[i].Red = 160;
  Colors[i].Green = 160;
  Colors[i].Blue = 164;
  i++; // 248
  Colors[i].Red = 128;
  Colors[i].Green = 128;
  Colors[i].Blue = 128;
  i++; // 249
  Colors[i].Red = 255;
  Colors[i].Green = 0;
  Colors[i].Blue = 0;
  i++; // 250
  Colors[i].Red = 0;
  Colors[i].Green = 255;
  Colors[i].Blue = 0;
  i++; // 251
  Colors[i].Red = 255;
  Colors[i].Green = 255;
  Colors[i].Blue = 0;
  i++; // 252
  Colors[i].Red = 0;
  Colors[i].Green = 0;
  Colors[i].Blue = 255;
  i++; // 253
  Colors[i].Red = 255;
  Colors[i].Green = 0;
  Colors[i].Blue = 255;
  i++; // 254
  Colors[i].Red = 0;
  Colors[i].Green = 255;
  Colors[i].Blue = 255;
  i++; // 255
  Colors[i].Red = 255;
  Colors[i].Green = 255;
  Colors[i].Blue = 255;
  
  return true;
 }
 return true;
}

bool SafeFread( char* buffer, int size, int number, FILE* fp )
{
 using namespace std;
 if( !feof(fp) )
 {
  fread( buffer , size , number , fp );
  return true;
 }
 return false;
}

#endif
