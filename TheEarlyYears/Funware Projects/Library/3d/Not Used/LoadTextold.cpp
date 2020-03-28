//
// File name: LoadText.CPP
//
// Description: A BMP image class implementation
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size

#include "LoadText.h"

int LoadTexture::Load ( char *FileName )
  {
  FileName;
  long ImageSize, ScanWidth, ScanPadding, Y, YPos;
  FILE *InFile;
  if ( ( InFile = fopen ( FileName, "rb" ) ) == 0 )
     return OpenErr;

  // Load the file header data:
  if ( fread ( &FileHeader, sizeof FileHeader, 1, InFile ) 
       < 1 )
     return ReadErr;

  // Load the information header data:
  if ( fread ( &InfoHeader, sizeof InfoHeader, 1, InFile ) 
       < 1 )
     return ReadErr;

  ColorCount = ( long ) pow ( 2.0F, InfoHeader.biBitCount );
  BitCount   = InfoHeader.biBitCount;

  if ( BitCount <= 16 )
     {
     if ( ( Palette = new RGBQUAD [ ColorCount ] ) == 0 )
        return MemErr;
     if ( fread ( Palette, sizeof ( RGBQUAD ), ColorCount, 
                  InFile ) < ColorCount )
        return ReadErr;
     }

  Width       = InfoHeader.biWidth;
  Height      = InfoHeader.biHeight;
  ImageSize   = Width * Height;
  ScanWidth   = ( ( Width * ( BitCount / 8 ) ) + 3 ) & ( ~3 );
  ScanPadding = ScanWidth - ( Width * ( BitCount / 8 ) );

  if ( ( Image = new BYTE [ ImageSize * 
       ( BitCount / 8 ) ] ) == NULL )
     return MemErr;

  // Load the bitmap:
  for ( Y = 0; Y < abs ( Height ); ++Y )
      {
      // Load a scan-line:
      switch ( InfoHeader.biCompression )
             {
             // Uncompressed image:
             case ( BI_RGB ):
                  {
                  switch ( BitCount )
                         {
                         // 256-color image:
                         case ( 8 ):
                              {
                              if ( Height < 0 )
                                 {
                                 YPos = Y * Width;
                                 }
                              else YPos = ( ( Height - Y ) * 
                                              Width ) - Width;

                              if ( fread ( &Image [ YPos ], 
                                   Width, 1, InFile ) < 1 )
                                 return ReadErr;
                              break;
                              }
                         default: return ResErr;
                         }
                  break;
                  }
             default: return CompErr;
             }
      // Skip the padding possibly located at the end of each
      // scan-line:
      fseek ( InFile, ScanPadding, SEEK_CUR );
      }
  fclose ( InFile );
  Height = abs ( Height );
  return Success;
  }

int LoadTexture::SaveBT ( FILE *OutFile )
  {
  // Write the image width:
  if ( fwrite ( &Width, sizeof Width, 1, OutFile ) < 1 )
     return 0;

  // Write the image height:
  if ( fwrite ( &Height, sizeof Height, 1, OutFile ) < 1 )
     return 0;

  // Write the image's bit count per pixel:
  if ( fwrite ( &BitCount, sizeof BitCount, 1, OutFile ) < 1 )
     return 0;

  // Write the image's color count:
  if ( fwrite ( &ColorCount, sizeof ColorCount, 1, 
       OutFile ) < 1 )
     return 0;

  // Write the image:
  if ( fwrite ( Image, Width * Height * ( BitCount / 8 ), 1, 
       OutFile ) < 1 )
     return 0;
  return 1;
  }

int LoadTexture::LoadBT ( FILE *InFile )
  {
  // Read the image width:
  if ( fread ( &Width, sizeof Width, 1, InFile ) < 1 )
     return 0;

  // Read the image height:
  if ( fread ( &Height, sizeof Height, 1, InFile ) < 1 )
     return 0;

  // Read the image's bit count per pixel:
  if ( fread ( &BitCount, sizeof BitCount, 1, InFile ) < 1 )
     return 0;

  // Read the image's color count:
  if ( fread ( &ColorCount, sizeof ColorCount, 1, InFile ) < 1 )
     return 0;

  // Allocate memory for image:
  if ( ( Image = new BYTE [ Width * Height * ( BitCount / 8 ) ] )
       == NULL )
     return 0;

  // Read the image:
  if ( fread ( Image, Width * Height * ( BitCount / 8 ), 1, 
       InFile ) < 1 )
     return 0;
  return 1;
  }

int LoadTexture::SavePal ( FILE *OutFile )
  {
  // Write the color count:
  if ( fwrite ( &ColorCount, sizeof ColorCount, 1, OutFile ) 
       < 1 )
     return 0;

  // Write the bit count:
  if ( fwrite ( &BitCount, sizeof BitCount, 1, OutFile ) < 1 )
     return 0;

  // Write the palette:
  if ( fwrite ( Palette, sizeof ( RGBQUAD ), ColorCount, 
       OutFile ) < 1 )
     return 0;
  return 1;
  }

int LoadTexture::LoadPal ( FILE *InFile )
  {
  // Load the color count:
  if ( fread ( &ColorCount, sizeof ColorCount, 1, InFile ) 
       < 1 )
     return 0;

  // Load the bit count:
  if ( fread ( &BitCount, sizeof BitCount, 1, InFile ) < 1 )
     return 0;

  // Allocate memory for palette:
  if ( ( Palette = new RGBQUAD [ ColorCount ] ) == 0 )
     return 0;

  // Load the palette:
  if ( fread ( Palette, sizeof ( RGBQUAD ), ColorCount, 
       InFile ) < ColorCount )
     return 0;
  return 1;
  }
