/************************************************************************
*    FILE NAME:       group.h
*
*    DESCRIPTION:     Group class
************************************************************************/

#ifndef __group_h__
#define __group_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Game lib dependencies
#include <common/matrix.h>
#include <common/point.h>
#include <3d/objectdatalist3d.h>
#include <3d/collisioncheck.h>
#include <3d/sprite3d.h>
#include <3d/objectdata3d.h>

class CGroup : boost::noncopyable
{
public:

	CGroup():pObjectData(NULL)
	{}
	virtual ~CGroup(){};

	// Load the meshs from their group and type
	virtual void Init( std::string & group, std::string & type ) = 0;

	virtual void SetPos( CPoint & position ) = 0;
	virtual void SetPos( float x, float y, float z ) = 0;

	virtual void SetRot( CPoint & rotation ) = 0;
	virtual void SetRot( float x, float y, float z ) = 0;

	virtual void IncPos( CPoint & position ) = 0;
	virtual void IncPos( float x, float y, float z ) = 0;

	virtual void IncRot( CPoint & rotation ) = 0;
	virtual void IncRot( float x, float y, float z ) = 0;

	virtual void Transform( CMatrix & matrix ) = 0;
	
	virtual void IncAnimationTime(){};
	virtual void SetAnimation( const std::string & name ){};

	virtual void SetScale(float scale) = 0;
	virtual void SetScale(CPoint & scale) = 0;
	virtual void SetDefaultScale(float scale) = 0;
	virtual void SetDefaultScale(CPoint & scale) = 0;

	// Check for collision and react to it
	virtual CCollisionCheck & ReactToCollision( CPoint & center, float radius,
												float floorPadding, float wallPadding ) = 0;

	// Check for collision and react to it
	virtual CCollisionCheck & ReactToCollision( CMatrix & matrix, CPoint & center, float radius,
												float floorPadding, float wallPadding ) = 0;

	virtual CSprite3D & GetVisualSprite() = 0;
	virtual CSprite3D & GetCollisionSprite() = 0;

	// Set the Object's data
	CObjectData3D & GetObjectData(){return *pObjectData;};

protected:

	// Object's data. 
	// NOTE: This pointer belongs to the CObjectDataList2D singleton
	CObjectData3D * pObjectData;
};

#endif  // __group_h__
