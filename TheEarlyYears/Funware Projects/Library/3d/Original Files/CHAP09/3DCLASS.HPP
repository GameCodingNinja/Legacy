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

#define PI 3.141592654

extern long *ZBuffer;
extern long ZTrans;
extern const double MINZ, MAXZ;

void InitMath ();

#endif
