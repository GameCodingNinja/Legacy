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
#define MINX    10
#define MAXX    309
#define MINY    10
#define MAXY    189
#define WIDTH   320
#define HEIGHT  200
#define XCENTER 160
#define YCENTER 100
#define MINZ    100.0F
#define MAXZ    10000.0F
#define XSCALE  120
#define YSCALE -120

extern long *ZBuffer;
extern long ZTrans;

void InitMath ();
#endif
