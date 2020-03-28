// Particle.h: interface for the CExplosionManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CPARTICLE_H__
#define __CPARTICLE_H__

#include "Matrix3D.h"        // The matrix class for using it's sin and cos table

// Acceleration type
enum
{
    ACCEL_REF_PTR_VALUE=0,
    ACCEL_RAND_TBL_VALUE,
};

// Velocity type
enum
{
    VEL_ALL_RANDOM=0,
    VEL_RAND_PER_PLAIN,
    VEL_MAX_ONLY,
};

// Signs
enum
{
    SGN_NO_CHANGE=0,
    SGN_ALL_RAND,
    SGN_ALL_POS,
    SGN_ALL_NEG,
	SGN_MATCH_VEL,
};

// Burst type
enum
{
    BURST_RANDOM=0,
    BURST_CIRCULAR,
};

// Circular rotation
enum
{
    CIR_ROT_NONE=0,
    CIR_ROT_X,
    CIR_ROT_Y,
    CIR_ROT_Z,  
};
      

// Describes a 3 dimensional point
struct SParticlePoint3D
{
   float X;
   float Y;
   float Z;

    SParticlePoint3D()
    {
        X = Y = Z = 0;
    }    
};

// Structure that contains the particle data
struct SParticle
{
    float velocity_X;
    float velocity_Y;
    float velocity_Z;
    float acceleration_X;
    float acceleration_Y;
    float acceleration_Z;
    int frameIndex;
};


class CParticle  
{
protected:

    float startPt_X;
    float startPt_Y;
    float startPt_Z;
    int count;
    SParticle *ptParicle;

public:
        
    CParticle();
    ~CParticle();

    // Call this function to free up all allocated memory
    void Free();
    // Allocate the particle array
    bool AllocateArray( int NumOfParticles );
    // Does a burst 
    void InitBurst( int activeCount,
                    float ptX, float ptY, float ptZ,           
                    int maxVelocity, int minVelocity,
                    float *accelTbl_X, float *accelTbl_Y, float *accelTbl_Z,
                    int accelType = ACCEL_REF_PTR_VALUE,
                    int velType = VEL_ALL_RANDOM, int velSign = SGN_ALL_RAND, 
                    int burstType = BURST_CIRCULAR, int cirRot = CIR_ROT_NONE, int rotAmount = 0 );
                       
    // Set the matric to the position of the particle                                          
    void SetMatrixArray( int activeCount, PMatrix3D matrix, float BillboardX, float BillboardY, float BillboardZ, float Time );
    // Get the number of particles
    int Count(){ return count; };
    // Get the frame index
    int GetFrame( int index ){ return ptParicle[index].frameIndex; };
    // Set the frame index
    void SetFrame( int index, int value ){ ptParicle[index].frameIndex = value; };
}; 

#endif  // __CPARTICLE_H__