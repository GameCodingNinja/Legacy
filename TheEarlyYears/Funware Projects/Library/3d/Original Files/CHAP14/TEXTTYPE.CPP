//
// File name: TextType.CPP
//
// Description:
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#include <StdIO.H>

#include "TextType.HPP"

int ImageData::LoadINI ( char *FileName )
  {
  int ErrCode, FileCount = 0, N;
  char String [ 256 ];
  FILE *InFile;
  if ( ( InFile = fopen ( FileName, "rt" ) ) == 0 )
     return 0;

  for ( ;; )
      {
      ErrCode = fscanf ( InFile, "%s", String );
      if ( ( ErrCode != 0 ) && ( ErrCode != EOF ) )
         ++FileCount;
      else break;
      }

  rewind ( InFile );

  TCount = FileCount;
  if ( ( TMap = new BMPImage [ TCount ] ) == 0 )
     {
     fclose ( InFile );
     return 0;
     }

  for ( N = 0; N < TCount; N++ )
      {
      fscanf ( InFile, "%s", String );
      if ( TMap [ N ].Load ( String ) != BMPImage::Success )
         {
         fclose ( InFile );
         return 0;
         }
      }
  return 1;
  }

int ImageData::LoadBT ( FILE *InFile )
  {
  int N;

  // Load the number of textures:
  fread ( &TCount, sizeof TCount, 1, InFile );

  // Allocate memory for bitmap images:
  if ( ( TMap = new BMPImage [ TCount ] ) == 0 )
     return 0;

  // Load palette:
  TMap [ 0 ].LoadPal ( InFile );

  // Assign palette to all images:
  for ( N = 1; N < TCount; N++ )
      TMap [ N ].Palette = TMap [ 0 ].Palette;

  // Load the textures:
  for ( N = 0; N < TCount; N++ )
      {
      if ( TMap [ N ].LoadBT ( InFile ) == 0 )
         return 0;
      }
  return 1;
  }

int ImageData::SaveBT ( FILE *OutFile )
  {
  int N;
  // Save the number of texture maps:
  fwrite ( &TCount, sizeof TCount, 1, OutFile );

  // Save the palette:
  TMap [ 0 ].SavePal ( OutFile );

  // Save the texture data:
  for ( N = 0; N < TCount; N++ )
      {
      if ( TMap [ N ].SaveBT ( OutFile ) == 0 )
         return 0;
      }
  return 1;
  }
