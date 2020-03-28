//
//
// File name: LightP.HPP
//
// Description: The structure for a point-light source
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef LIGHTPHPP
#define LIGHTPHPP

// A point light source structure:
struct PointLight {
float X, Y, Z;
PointLight ( float i, float j, float k ) { X = i; Y = j; Z = k; }
};

extern PointLight Light;

#endif