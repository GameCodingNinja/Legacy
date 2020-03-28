//
// File name: Vector3D.h
//
// Description: The header file a vector class
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef VECTOR3DHPP
#define VECTOR3DHPP

struct Vector
{
    float Tx, Ty, Tz, Td; // The updated (transformed) X, Y, Z
                        // direction vector
    float X, Y, Z, D;     // X, Y, Z direction vector
    Vector(){ Tx = Ty = Tz = Td = X = Y = Z = D = 0.0F; }
};


#endif
