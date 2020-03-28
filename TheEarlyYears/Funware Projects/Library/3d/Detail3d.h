//
// File name: Detail3D.h
//
// Description: A 3D point's detail settings
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef DETAIL3DHPP
#define DETAIL3DHPP
#pragma pack(1)
class Detail3D
{
public:
    Detail3D(){ U = V = I = 0; };
    int U, V, I; // U/Z, V/Z, and intensity variables
    inline Detail3D operator - ( Detail3D &D );
    inline Detail3D operator + ( Detail3D &D );
    inline Detail3D operator * ( double N );
    inline Detail3D operator / ( double N );
};

inline Detail3D Detail3D::operator - ( Detail3D &D )
{
    Detail3D Temp;
    Temp.U = U - D.U;
    Temp.V = V - D.V;
    Temp.I = I - D.I;
    return Temp;
}

inline Detail3D Detail3D::operator + ( Detail3D &D )
{
    Detail3D Temp;
    Temp.U = U + D.U;
    Temp.V = V + D.V;
    Temp.I = I + D.I;
    return Temp;
}

inline Detail3D Detail3D::operator * ( double N )
{
    Detail3D Temp;
    Temp.U = ( double ) U * ( double ) N;
    Temp.V = ( double ) V * ( double ) N;
    Temp.I = ( double ) I * ( double ) N;
    return Temp;
}

inline Detail3D Detail3D::operator / ( double N )
{
    Detail3D Temp;
    Temp.U = ( double ) U / ( double ) N;
    Temp.V = ( double ) V / ( double ) N;
    Temp.I = ( double ) I / ( double ) N;
    return Temp;
}
#pragma pack()
#endif
