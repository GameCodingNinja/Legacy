/************************************************************************
*    FILE NAME:       xalphaface.h
*
*    DESCRIPTION:     3D DirectX alpha face class
************************************************************************/  

#ifndef __x_alphaface_h__
#define __x_alphaface_h__

// Physical component dependency
#include <common/xface.h>

class CXAlphaFace : public CXFace
{
public:
    
    CXAlphaFace();
    ~CXAlphaFace();

    // Calculate the center point of the polygon
    void CalcPolyCenterPoint();

private:

    CPoint center;
    CPoint trans_center;
};

#endif  // __x_alphaface_h__