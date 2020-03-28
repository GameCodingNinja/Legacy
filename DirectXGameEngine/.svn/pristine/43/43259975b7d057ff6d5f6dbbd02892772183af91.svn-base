
/************************************************************************
*    FILE NAME:       objectotherdata2d.h
*
*    DESCRIPTION:     Class containing the object's misc other data
************************************************************************/

#ifndef __object_other_data_2d_h__
#define __object_other_data_2d_h__

class CObjectOtherData2D
{
public:

    CObjectOtherData2D():playerCollision(false),objectCollision(false),gravity(false){}

    // Set and get the player collision flag
    void SetPlayerCollision(bool value){playerCollision = value;}
    bool IsPlayerCollision() const {return playerCollision;}

    // Set and get the object collision flag
    void SetObjectCollision(bool value){objectCollision = value;}
    bool IsObjectCollision() const {return objectCollision;}

    // Set and get the gravity flag
    void SetGravity(bool value){gravity = value;}
    bool IsGravity() const {return gravity;}

private:

    // Booleans used for later sorting
    bool playerCollision, objectCollision, gravity;		

};

#endif  // __object_other_data_2d_h__