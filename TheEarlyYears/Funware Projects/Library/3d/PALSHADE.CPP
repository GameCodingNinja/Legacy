//
// File name: PalShade.CPP
//
// Description: The support file for a palette shading class
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#include <Math.H>

#include "PalShade.h"


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
ShadeDat::ShadeDat()
{

}


/************************************************************************
*    FUNCTION NAME:              Destructor                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
ShadeDat::~ShadeDat()
{
}


/************************************************************************
*    FUNCTION NAME:         GenTable()                                                             
*
*    DESCRIPTION:           Generate shaded table
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void ShadeDat::GenTable( double R2, double G2, double B2, RGBQUAD *Pal )
{
    double R, G, B, R1, G1, B1, DeltaR, DeltaG;
    double DeltaB, StepR, StepG, StepB;
    WORD Value = 0;
    PSWordToByte RGB16 = PSWordToByte( &Value );

    for( int count = 0; count < COLOR_COUNT_8_BIT; ++count )
    {
        R1 = Pal[ count ].rgbRed;
        G1 = Pal[ count ].rgbGreen;
        B1 = Pal[ count ].rgbBlue;
        DeltaR = R2 - R1;
        DeltaG = G2 - G1;
        DeltaB = B2 - B1;
        R = R1;
        G = G1;
        B = B1;
        StepR = DeltaR / ( double ) SHADE_COUNT;
        StepG = DeltaG / ( double ) SHADE_COUNT;
        StepB = DeltaB / ( double ) SHADE_COUNT;

        // Generate 8 and 16 bit color shaded tables
        for( int i = 0; i < SHADE_COUNT; ++i )
        {
            // Search through the palette for a matching color index
            Shade8to8 [ ( i * COLOR_COUNT_8_BIT ) + count ] = ( BYTE )
                             GetColor ( ( int ) R,
                                        ( int ) G,
                                        ( int ) B, Pal );

            // Take the RGB color and transform it to a 16 bit color value
            RGB16->R = R / 8;
            RGB16->G = G / 8;
            RGB16->B = B / 8;
             
            // Save the color to the shaded table
            Shade8to16 [ ( i * COLOR_COUNT_8_BIT ) + count ] = Value;

            R += StepR;
            G += StepG;
            B += StepB;
        }
    }

	// Generate 16 bit color shaded tables
	Generate16BitShadedTable( Shade16to16 );

}   // GenTable


/************************************************************************
*    FUNCTION NAME:         LoadTable8to8bit()                                                             
*
*    DESCRIPTION:           Load an 8 bit shaded table from file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL ShadeDat::LoadTable8to8bit( char *FileName )
{
    FILE *InFile;

    if( ( InFile = fopen ( FileName, "rb" ) ) == 0 )
        return FALSE;

    fread( Shade8to8, sizeof( Shade8to8 ), 1, InFile );
    fclose( InFile );

    return TRUE;

}   // LoadTable8to8bit


/************************************************************************
*    FUNCTION NAME:         LoadTable8to16bit()                                                             
*
*    DESCRIPTION:           Load a 16 bit shaded table from file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL ShadeDat::LoadTable8to16bit( char *FileName )
{
    FILE *InFile;

    if( ( InFile = fopen ( FileName, "rb" ) ) == 0 )
        return FALSE;

    fread( Shade8to16, sizeof( Shade8to16 ), 1, InFile );
    fclose( InFile );

    return TRUE;

}   // LoadTable8to16bit


/************************************************************************
*    FUNCTION NAME:         LoadTable16to16bit()                                                             
*
*    DESCRIPTION:           Load a 16 bit RGB shaded table from file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL ShadeDat::LoadTable16to16bit( char *FileName )
{
    FILE *InFile;

    if( ( InFile = fopen ( FileName, "rb" ) ) == 0 )
        return FALSE;

    fread( Shade16to16, sizeof( Shade16to16 ), 1, InFile );
    fclose( InFile );

    return TRUE;

}   // LoadTable16to16bit


/************************************************************************
*    FUNCTION NAME:         SaveTable8to8bit()                                                             
*
*    DESCRIPTION:           Save an 8 bit shaded table to file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL ShadeDat::SaveTable8to8bit( char *FileName )
{
    FILE *OutFile;

    if( ( OutFile = fopen ( FileName, "wb" ) ) == 0 )
        return FALSE;

    fwrite( Shade8to8, sizeof( Shade8to8 ), 1, OutFile );
    fclose( OutFile );

    return TRUE;

}   // SaveTable8to8bit


/************************************************************************
*    FUNCTION NAME:         SaveTable8to16bit()                                                             
*
*    DESCRIPTION:           Save a 16 bit shaded table to file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL ShadeDat::SaveTable8to16bit( char *FileName )
{
    FILE *OutFile;

    if( ( OutFile = fopen ( FileName, "wb" ) ) == 0 )
        return FALSE;

    fwrite( Shade8to16, sizeof( Shade8to16 ), 1, OutFile );
    fclose( OutFile );

    return TRUE;

}   // SaveTable8to16bit


/************************************************************************
*    FUNCTION NAME:         SaveTable16to16bit()                                                             
*
*    DESCRIPTION:           Save a 16 bit RGB shaded table to file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL ShadeDat::SaveTable16to16bit( char *FileName )
{
    FILE *OutFile;

    if( ( OutFile = fopen ( FileName, "wb" ) ) == 0 )
        return FALSE;

    fwrite( Shade16to16, sizeof( Shade16to16 ), 1, OutFile );
    fclose( OutFile );

    return TRUE;

}   // SaveTable16to16bit


/************************************************************************
*    FUNCTION NAME:         GetColor()                                                             
*
*    DESCRIPTION:           Save shaded table
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int ShadeDat::GetColor( int Red, int Green, int Blue, RGBQUAD *Pal )
{
    double Dist[ COLOR_COUNT_8_BIT ], DRed, DGreen, DBlue, DistC;
    int Count, CloseC;

    for ( Count = 0; Count < COLOR_COUNT_8_BIT; ++Count )
    {
        DRed   = ( Red   - Pal [ Count ].rgbRed );
        DGreen = ( Green - Pal [ Count ].rgbGreen );
        DBlue  = ( Blue  - Pal [ Count ].rgbBlue );
        Dist [ Count ] = sqrt ( DRed   * DRed +
                  DGreen * DGreen +
                  DBlue  * DBlue );
    }

    CloseC = 0;
    DistC  = Dist[ 0 ];

    for( Count = 0; Count < COLOR_COUNT_8_BIT; ++Count )
    {
        if( Dist[ Count ] < DistC )
        {
            DistC  = Dist[ Count ];
            CloseC = Count;
        }
    }

    return CloseC;

}   // GetColor


/************************************************************************
*    FUNCTION NAME:         LoadFromPointer8To8()                                                             
*
*    DESCRIPTION:           Load 8 bit shaded table from pointer
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void ShadeDat::LoadFromPointer8To8( BYTE *pData )
{
    // Copy the data into the class array
    memcpy( Shade8to8, pData, sizeof( Shade8to8 ) );

}   // LoadFromPointer8To8


/************************************************************************
*    FUNCTION NAME:         LoadFromPointer8To8()                                                             
*
*    DESCRIPTION:           Load 16 bit shaded table from pointer
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void ShadeDat::LoadFromPointer8To16( WORD *pData )
{
    // Copy the data into the class array
    memcpy( Shade8to16, pData, sizeof( Shade8to16 ) );

}   // LoadFromPointer8To8


/************************************************************************
*    FUNCTION NAME:         Generate16BitShadedTable()                                                             
*
*    DESCRIPTION:           Generate a 16 bit shaded table
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void Generate16BitShadedTable( unsigned short int *shadeTbl )
{
	int textel_R, textel_G, textel_B;

	for( int color = 0; color < COLOR_COUNT_16_BIT; ++color )
    {
        for( int i = 0; i < SHADE_COUNT; ++i )
        {
			// Calculate the shaded 16 bit RGB color
			textel_R = (color & 0x7C00) >> 10;
            textel_G = (color & 0x3E0) >> 5;
            textel_B = color & 0x1F;
            
            // Inc the 16 bit RGB color
            textel_R -= i;
            textel_G -= i;
            textel_B -= i;
            
            if( textel_R < 0 )
            	textel_R = 0;
            	
            if( textel_G < 0 )
            	textel_G = 0;

            if( textel_B < 0 )
            	textel_B = 0; 
             
            // Save the color to the shaded table
            shadeTbl[ ( i * COLOR_COUNT_16_BIT ) + color ] = (textel_R << 10) | (textel_G << 5) | textel_B;
        }
    }
}	// Generate16BitShadedTable