//
//
// File name: LightP.h
//
// Description: The structure for a point-light source
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef LIGHTP_H
#define LIGHTP_H

// A point light source structure:
class TLight
{
protected:
    float X, Y, Z;
	double Mag;

public:
    TLight(){ X = Y = Z = 0; Mag = 1.0F; }

    // Set the direction of the lighting
    void SetLight( float aX, float aY, float aZ )
    { 
    	X = aX; Y = aY; Z = aZ;

    	// Calculate the magnitude
    	Mag = ( sqrt( X * X + Y * Y + Z * Z ) );

		// Don't let Mag = 0
		if( Mag == 0.0F )
			Mag = 1.0F;
    }
    inline float GetX(){ return X; }
    inline float GetY(){ return Y; }
    inline float GetZ(){ return Z; }
	inline double GetMag(){ return Mag; }
};

typedef TLight *PTLight;

#endif