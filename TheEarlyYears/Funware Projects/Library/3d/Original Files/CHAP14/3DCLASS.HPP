//
// File name: 3DClass.HPP
//
// Description: Assorted functions and variables
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef _3DCLASSHPP
#define _3DCLASSHPP

#include <Stdio.h>
#include <String.h>

#include "SinCos.HPP"
#include "Point2D.HPP"
#include "Point3D.HPP"
#include "Vector3D.HPP"
#include "Matrix3D.HPP"
#include "ATypes.HPP"
#include "Panel3D.HPP"
#include "PolyObj.HPP"

#define PI      3.141592654
#define MINX    0
#define MAXX    320
#define MINY    0
#define MAXY    200
#define WIDTH   320
#define HEIGHT  200
#define XCENTER 160
#define YCENTER 100
#define MINZ    100.0F
#define MAXZ    3000.0F
#define XSCALE  120
#define YSCALE -120
#define COLOR_RANGE 8
#define COLOR_START 8
#define SHADE_DIV ( ( double ) ( MAXZ - MINZ ) / ( double ) SHADE_COUNT )

extern long *ZBuffer;
extern long ZTrans;

void InitMath ();
#endif
