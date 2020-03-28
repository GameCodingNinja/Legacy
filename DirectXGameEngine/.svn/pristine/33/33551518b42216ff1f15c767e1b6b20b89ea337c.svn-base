
/************************************************************************
*    FILE NAME:       object.h
*
*    DESCRIPTION:     Object interface class
************************************************************************/

#ifndef __object_h__
#define __object_h__

// Game lib dependencies
#include "point.h"
#include "matrix.h"
#include "misc\\color.h"

class CObject
{
public:

    CObject():dummyColor(1,1,1,1)
    {};
    virtual ~CObject(){};

    // Set the passed in matrix to world view 
    virtual void SetMatrixToWorldView( CMatrix & matrix ){};

    // Set the object position
    virtual void SetPos( CPoint & position ) = 0;

    // Increment the object position
    virtual void IncPos( CPoint & position ) = 0;

    // Set the pre-translation matrix
    virtual void SetPreTransRot( CPoint & rotation ) = 0;

    // Increment the pre-translation matrix
    virtual void IncPreTransRot( CPoint & rotation ) = 0;

    // Set the post-translation matrix
    virtual void SetPostTransRot( CPoint & rotation ) = 0;

    // Increment the post-translation matrix
    virtual void IncPostTransRot( CPoint & rotation ) = 0;

    // Set the post-translation matrix
    virtual void SetPostRotTrans( CPoint & position ) = 0;

    // Increment the post-translation matrix
    virtual void IncPostRotTrans( CPoint & position ) = 0;

    // Transform the sprite's center point
    virtual void Transform( CMatrix & matrix ) = 0;
    virtual void Transform() = 0;

    // Get the position
    virtual CPoint & GetPos() = 0;

    // Get the object pre-translation rotation
    virtual CPoint & GetPreTransRot() = 0;

    // Get the object post-translation rotation
    virtual CPoint & GetPostTransRot() = 0;

    // Get the translated center point
    virtual CPoint & GetCenter() = 0;

    virtual void SetVisible(bool value){};
    virtual bool IsVisible(){return false;};

    virtual void SetAnimating(bool value){};
    virtual bool IsAnimating(){return false;};

    virtual void SetCurrentFrame(int frame){};
    virtual int GetCurrentFrame(){return 0;};

    virtual void SetFrameCount(int count){};
    virtual int GetFrameCount(){return 0;};

    virtual void SetFrameCounter(int count){};
    virtual int GetFrameCounter(){return 0;};

    virtual void SetTimer(float time){};
    virtual float GetTimer(){return 0;};

    virtual void SetFrameTimer(float time){};
    virtual float GetFrameTimer(){return 0;};

    virtual void SetStepCounter(unsigned int counter){};
    virtual unsigned int GetStepCounter(){return 0;};

    virtual void SetColor(float r, float g, float b, float a){};
    virtual void SetColor( D3DXCOLOR & _color ){};
    virtual void SetColor( CColor & _color ){};

    virtual void IncColor(float r, float g, float b, float a){};
    virtual void IncColor( D3DXCOLOR & _color ){};
    virtual void IncColor( CColor & _color ){};

    //virtual D3DXCOLOR & GetColor(){return dummyColor;};
    virtual CColor & GetColor(){return dummyColor;};

    

    //D3DXCOLOR dummyColor;
    CColor dummyColor;

protected:

};

#endif  // __object3d_h__
