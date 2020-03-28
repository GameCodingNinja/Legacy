
/************************************************************************
*    FILE NAME:       axischeck.h
*
*    DESCRIPTION:     Axis check class
************************************************************************/

#ifndef __axis_check_h__
#define __axis_check_h__

class CAxisCheck
{
public:

    CAxisCheck()
    {
        axisX = false;
        axisY = false;
        axisZ = false;
        axisRX = false;
        axisRY = false;
        axisRZ = false;
    }

    bool axisX;
    bool axisY;
    bool axisZ;

    bool axisRX;
    bool axisRY;
    bool axisRZ;
};

#endif  // __axis_check_h__


