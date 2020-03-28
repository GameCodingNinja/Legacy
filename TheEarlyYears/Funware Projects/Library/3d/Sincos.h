//
// File name: SinCos.h
//
// Description:
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef SINCOSHPP
#define SINCOSHPP

#define COS(a) ( CosTable [a & (DEGREECOUNT - 1)] )
#define SIN(a) ( SinTable [a & (DEGREECOUNT - 1)] )

const int DEGREECOUNT = 1024;
extern float SinTable [ DEGREECOUNT ];
extern float CosTable [ DEGREECOUNT ];
#endif