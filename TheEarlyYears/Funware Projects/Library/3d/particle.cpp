// ExplosionManger.cpp: implementation of the CExplosionManger class.
//
//////////////////////////////////////////////////////////////////////

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include "Particle.h"        // Class header
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.


/************************************************************************
*    FUNCTION NAME:              Constructor                                                            
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
                
CParticle::CParticle()
{
   // Initialize class members
   ptParicle = NULL;
   count = 0;
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
 
CParticle::~CParticle()
{
   // Free all memory
   Free();
}


/************************************************************************
*    FUNCTION NAME:         Free                                                             
*
*    DESCRIPTION:           Free allocated particles
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
 
void CParticle::Free()
{
   if( ptParicle )
   {
      delete [] ptParicle;
      ptParicle = NULL;
      count = 0;
   }
}   // Free


/************************************************************************
*    FUNCTION NAME:         AllocateArray                                                             
*
*    DESCRIPTION:           Allocate the particle array
*
*    FUNCTION PARAMETERS:
*    Input:    var1type int NumOfParticles - Particle count
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool CParticle::AllocateArray( int NumOfParticles )
{
   // Free all allocations
   Free();

   // We have to allocate memory for the number of particles also
   ptParicle = new SParticle[ NumOfParticles ];

   // Check all allocations
   if( ptParicle )
   {
      // Save the allocated number of particles
      count = NumOfParticles;
      return TRUE;
   }
   else
      return FALSE;

}   // AllocateParticleArray


/************************************************************************
*    FUNCTION NAME:         InitBurst                                                             
*
*    DESCRIPTION:           Init the allocated particles
*
*    FUNCTION PARAMETERS:
*    Input:     int NumOfParticles : Number of particles the explosion 
*                                 will have.            
*               int init_X : The initial X position of the explosion
*               int init_Y : The initial Y position of the explosion               
*               int init_Z : The initial Z position of the explosion
*               int MaxVelocity : The maximum velocity the particles will go.
*               int MinVelocity : The mimimun velocity the particles will go.
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CParticle::InitBurst( int activeCount,
                           float ptX, float ptY, float ptZ,           
                           int maxVelocity, int minVelocity,
                           float *accelTbl_X, float *accelTbl_Y, float *accelTbl_Z,
                           int accelType,
                           int velType, int velSign, 
                           int burstType, int cirRot, int rotAmount )
{
    float X, Y, Z, fPosNeg[2] = {1.0f,-1.0f};
    int i, rotation, velocity[3], posNeg[2] = {1,-1};
    int randCount;

    // Set up the initial positions
    startPt_X = ptX;
    startPt_Y = ptY;
    startPt_Z = ptZ;

    // Double check we didn't excede our amount
    if( activeCount > count )
        activeCount = count;

    // Now put in random data for the explosion.  Initial velocities and  
    // initial positions are calculated here. 
    for( i = 0; i < activeCount; ++i )
    {
        /////////////////////////////////
        //   Setup the velocity
        /////////////////////////////////

        // Randomize all velocitys
        if( velType == VEL_ALL_RANDOM )
        {
            // Calculate a random velocity for each plain
            // Randomize the max velocity
            velocity[0] = (rand() % maxVelocity) + minVelocity;
            velocity[1] = (rand() % maxVelocity) + minVelocity;
            velocity[2] = (rand() % maxVelocity) + minVelocity;
        }
        // All points of the velocity is random
        else if( velType == VEL_RAND_PER_PLAIN )
        {
            velocity[0] = (rand() % maxVelocity) + minVelocity;
            velocity[1] = velocity[0];
            velocity[2] = velocity[0];

        }
        // All velocitys use only the max
        else if( velType == VEL_MAX_ONLY )
        {
            // The same velocity for each plain
            velocity[0] = maxVelocity;
            velocity[1] = maxVelocity;
            velocity[2] = maxVelocity;
        }
        
        // Do a sign conversion
        if( velSign == SGN_NO_CHANGE )
        {}
        if( velSign == SGN_ALL_RAND )
        {
            // Randomly make the velocity positive or negitive
            velocity[0] *= posNeg[ rand() % 2 ];
            velocity[1] *= posNeg[ rand() % 2 ];
            velocity[2] *= posNeg[ rand() % 2 ];
        }
        else if( velSign == SGN_ALL_POS )
        {
            // Make all the velocities positive
            velocity[0] *= posNeg[ 0 ];
            velocity[1] *= posNeg[ 0 ];
            velocity[2] *= posNeg[ 0 ];
        }
        else if( velSign == SGN_ALL_NEG )
        {
            // Make all the velocities negative
            velocity[0] *= posNeg[ 1 ];
            velocity[1] *= posNeg[ 1 ];
            velocity[2] *= posNeg[ 1 ];
        }
        
        // What type of burst are we using?
        if( burstType == BURST_RANDOM )
        {
            ptParicle[i].velocity_X = velocity[0];
            ptParicle[i].velocity_Y = velocity[1];
            ptParicle[i].velocity_Z = velocity[2];
        }
        else if( burstType == BURST_CIRCULAR )
        { 
            // Need to get a rotation for 3 planes.
            rotation = rand() % DEGREECOUNT;

            ptParicle[i].velocity_X = velocity[0] * GetCos( rotation );
            ptParicle[i].velocity_Y = velocity[1] * GetSin( rotation );
            ptParicle[i].velocity_Z = velocity[2] * GetSin( rotation );
        }
        
        // Apply a circular rotation
        if( cirRot == CIR_ROT_NONE )
        {}
        if( cirRot == CIR_ROT_X )
        {
            X = ptParicle[i].velocity_X;
            Y = ptParicle[i].velocity_Y;
            Z = ptParicle[i].velocity_Z;

            // Rotate by a given amount or random
            if( rotAmount )
                rotation = rotAmount % DEGREECOUNT;
            else
                rotation = rand() % DEGREECOUNT;

            // Rotate on the X axis for a round burst
            ptParicle[i].velocity_X = X + (X / 2.2);
            ptParicle[i].velocity_Y = (Y * GetCos( rotation )) + (Z * GetSin( rotation ));
            ptParicle[i].velocity_Z = (Z * GetCos( rotation )) - (Y * GetSin( rotation ));
        }
        else if( cirRot == CIR_ROT_Y )
        {
            X = ptParicle[i].velocity_X;
            Y = ptParicle[i].velocity_Y;
            Z = ptParicle[i].velocity_Z;

            // Rotate by a given amount or random
            if( rotAmount )
                rotation = rotAmount % DEGREECOUNT;
            else
                rotation = rand() % DEGREECOUNT;

            // Rotate on the X axis for a round burst
            ptParicle[i].velocity_X = (X * GetCos( rotation )) - (Z * GetSin( rotation ));
            ptParicle[i].velocity_Y = Y + (Y / 2.2);
            ptParicle[i].velocity_Z = (X * GetSin( rotation )) + (Z * GetCos( rotation ));
        }
        else if( cirRot == CIR_ROT_Z )
        {
            X = ptParicle[i].velocity_X;
            Y = ptParicle[i].velocity_Y;
            Z = ptParicle[i].velocity_Z;

            // Rotate by a given amount or random
            if( rotAmount )
                rotation = rotAmount % DEGREECOUNT;
            else
                rotation = rand() % DEGREECOUNT;

            // Rotate on the X axis for a round burst
            ptParicle[i].velocity_X = (X * GetCos( rotation )) + (Y * GetSin( rotation ));
            ptParicle[i].velocity_Y = (Y * GetCos( rotation )) - (X * GetSin( rotation ));
            ptParicle[i].velocity_Z = Z + (Z / 2.2);
        }
        
        /////////////////////////////////
        //   Setup the acceleration
        /////////////////////////////////
        
        // The pointer is not a table so just reference the pointer
        if( accelType == ACCEL_REF_PTR_VALUE )
        {
            ptParicle[i].acceleration_X = *accelTbl_X;
            ptParicle[i].acceleration_Y = *accelTbl_Y;
            ptParicle[i].acceleration_Z = *accelTbl_Z;
        }
        // Randomly pick from the table
        else if( accelType == ACCEL_RAND_TBL_VALUE )
        {
            // The first item in the table is the number of members in the table
            // The second item in the table is what we want to do with the sign
            // We need to convert this to an int first before we can randomly pick
            // an acceleration
            randCount = accelTbl_X[0];
            ptParicle[i].acceleration_X = accelTbl_X[ (rand() % randCount)+2 ];
            randCount = accelTbl_Y[0];
            ptParicle[i].acceleration_Y = accelTbl_Y[ (rand() % randCount)+2 ];
            randCount = accelTbl_Z[0];
            ptParicle[i].acceleration_Z = accelTbl_Z[ (rand() % randCount)+2 ];

            // Since we have a table, we need to know what we want to do about the sign for the X
            if( accelTbl_X[ 1 ] == SGN_NO_CHANGE )
            {}
            else if( accelTbl_X[ 1 ] == SGN_ALL_RAND )
                ptParicle[i].acceleration_X *= fPosNeg[ rand() % 2 ];

            else if( accelTbl_X[ 1 ] == SGN_ALL_POS )
                ptParicle[i].acceleration_X *= fPosNeg[ 0 ];

            else if( accelTbl_X[ 1 ] == SGN_ALL_NEG )
                ptParicle[i].acceleration_X *= fPosNeg[ 1 ];

            else if( accelTbl_X[ 1 ] == SGN_MATCH_VEL )
            {
                if( ptParicle[i].velocity_X > 0 )
                    ptParicle[i].acceleration_X *= fPosNeg[ 0 ];
                else
                    ptParicle[i].acceleration_X *= fPosNeg[ 1 ];
            }

            // Since we have a table, we need to know what we want to do about the sign for the Y
            if( accelTbl_Y[ 1 ] == SGN_NO_CHANGE )
            {}
            else if( accelTbl_Y[ 1 ] == SGN_ALL_RAND )
                ptParicle[i].acceleration_Y *= fPosNeg[ rand() % 2 ];

            else if( accelTbl_Y[ 1 ] == SGN_ALL_POS )
                ptParicle[i].acceleration_Y *= fPosNeg[ 0 ];

            else if( accelTbl_Y[ 1 ] == SGN_ALL_NEG )
                ptParicle[i].acceleration_Y *= fPosNeg[ 1 ];

            else if( accelTbl_Y[ 1 ] == SGN_MATCH_VEL )
            {
                if( ptParicle[i].velocity_Y > 0 )
                    ptParicle[i].acceleration_Y *= fPosNeg[ 0 ];
                else
                    ptParicle[i].acceleration_Y *= fPosNeg[ 1 ];
            }

            // Since we have a table, we need to know what we want to do about the sign for the Z
            if( accelTbl_Z[ 1 ] == SGN_NO_CHANGE )
            {}
            else if( accelTbl_Z[ 1 ] == SGN_ALL_RAND )
                ptParicle[i].acceleration_Z *= fPosNeg[ rand() % 2 ];

            else if( accelTbl_Z[ 1 ] == SGN_ALL_POS )
                ptParicle[i].acceleration_Z *= fPosNeg[ 0 ];

            else if( accelTbl_Z[ 1 ] == SGN_ALL_NEG )
                ptParicle[i].acceleration_Z *= fPosNeg[ 1 ];

            else if( accelTbl_Z[ 1 ] == SGN_MATCH_VEL )
            {
                if( ptParicle[i].velocity_Z > 0 )
                    ptParicle[i].acceleration_Z *= fPosNeg[ 0 ];
                else
                    ptParicle[i].acceleration_Z *= fPosNeg[ 1 ];
            }
        }         
    }
}   // InitBurst


/************************************************************************
*    FUNCTION NAME:         SetMatrixArray                                                             
*
*    DESCRIPTION:           Function that calculates the position of the 
*                           particles using the function 
*                           x = xo + vo*x + 1/2 * (a*t*t ). Each particle
*                           has to have it's own matrix.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var2Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CParticle::SetMatrixArray( int activeCount, PMatrix3D matrix, float BillboardX, float BillboardY, float BillboardZ, float Time )
{
    // Pre multi this in advance to save time
    float preMultiTime( Time * Time );
    float x, y, z;
    int i;

    // Double check we didn't excede our amount
    if( activeCount > count )
        activeCount = count;

    // Only calculate the active group
    for( i = 0; i < activeCount; ++i )
    {
        // Calculate for the X direction
        x = startPt_X + (ptParicle[i].velocity_X * Time) + 
                     ((ptParicle[i].acceleration_X * preMultiTime) / 2);

        // Calculate for the Y direction
        y = startPt_Y + (ptParicle[i].velocity_Y * Time) + 
                     ((ptParicle[i].acceleration_Y * preMultiTime) / 2);

        // Calculate for the Z Direction
        z = startPt_Z + (ptParicle[i].velocity_Z * Time) + 
                     ((ptParicle[i].acceleration_Z * preMultiTime) / 2);

        matrix[i].InitilizeMatrix();
        matrix[i].Rotate( -BillboardX, -BillboardY, -BillboardZ );

        // Set the matric
        matrix[i].Translate( x, y, z );
        matrix[i].Rotate( BillboardX, BillboardY, BillboardZ );
    }

}   // SetMatrixArray




