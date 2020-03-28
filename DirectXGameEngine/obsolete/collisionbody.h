
/************************************************************************
*    FILE NAME:       collisionbody.h
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

#ifndef __collision_body_h__
#define __collision_body_h__

// Boost lib dependencies

// Game lib dependencies
#include <common/point.h>
#include <2d/objectcollisiondata2d.h>

class CCollisionBody
{
public:

    // Constructor
    CCollisionBody():restitution(0), 
                     mass(0), 
                     inverseMass(0),
                     inertia(-1), 
                     inverseInertia(0), 
                     angVelocity(0)
    {}


    /***************************************************************************
    *    desc:  Copy constructor
    *
    *	 param: const CObjectCollisionData2D & data - to copy
    ****************************************************************************/
    CCollisionBody( const CObjectCollisionData2D & data )
        :inverseMass(0),
         inverseInertia(0), 
         angVelocity(0)
    {
        mass = data.GetDensity();
        inertia = data.GetDensity();
        restitution = data.GetRestitution();

    }	// constructor

    CCollisionBody( const CCollisionBody & body )
    {
        velocity = body.GetVelocity();
        angVelocity = body.GetAngVelocity();
        restitution = body.GetRestitution();
        mass = body.GetMass();
        inverseMass = body.GetInverseMass();
        inertia = body.GetInertia();
        inverseInertia = body.GetInverseInertia();
        positionCorrection = body.GetPositionCorrection();

    }	// constructor


    /***************************************************************************
    *    desc:  Initialize the collision body
    *
    *	 param: float radius - radius of the sprite
    ****************************************************************************/
    void Init( float radius )
    {
        if( mass > 0 )
            inverseMass = 1.f / mass;
        
        if( inertia < 0 )
            SetInertia( mass * radius * radius * 0.5f );

    }	// Init */


    /***************************************************************************
    *    desc:  Set the mass and calculate the inverse mass
    *
    *	 param: float value - value to set to
    ****************************************************************************/
    void SetMass( float value )
    { 
        mass = value; 
        
        if( value > 0 )
            inverseMass = 1.f / value; 
        else
            inverseMass = 0;

    }	// SetMass */


    /***************************************************************************
    *    desc:  Set the inertia and calculate the inverse inertia
    *
    *	 param: float value - value to set to
    ****************************************************************************/
    void SetInertia( float value )
    { 
        inertia = value; 

        if( value > 0 )
            inverseInertia = 1.f / value;
        else
            inverseInertia = 0;

    }	// SetInertia */


    // Set and get the velocity
    void SetVelocity( CPoint & vec ){ velocity = vec; }
    const CPoint & GetVelocity() const { return velocity; }

    // Set and get the angular velocity
    void SetAngVelocity( float value ){ angVelocity = value; }
    const float GetAngVelocity() const { return angVelocity; } 

    // Set and get the restitution
    void SetRestitution( float value ){ restitution = value; }
    const float GetRestitution() const { return restitution; } 

    // Get the mass, inertia, inverse mass, and inverse inertia
    const float GetMass() const { return mass; } 
    const float GetInertia() const { return inertia; }
    const float GetInverseMass() const { return inverseMass; }
    const float GetInverseInertia() const { return inverseInertia; }

    // Set and get the position correction
    void SetPositionCorrection( const CPoint & point ) { positionCorrection = point; }
    const CPoint & GetPositionCorrection() const { return positionCorrection; }

public:

    // Velocity and angular velocity of the body
    CPoint velocity;
    float angVelocity;

    // Effectively the elasticity of the body
    float restitution;

    // The mass and 1/mass. Mass represents how much an object 
    // resists movement
    float mass;
    float inverseMass;

    // The inertia and 1/inertia. Inertia represents how man an 
    // object resists rotation
    float inertia;
    float inverseInertia;

    // Amount to offset by after a collision
    CPoint positionCorrection;
   
};

#endif  // __collision_body_h__
