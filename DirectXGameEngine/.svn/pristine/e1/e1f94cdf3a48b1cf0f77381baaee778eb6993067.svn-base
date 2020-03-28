#ifndef __MS_PLUGIN_IMPL_H__
#define __MS_PLUGIN_IMPL_H__

#include "msPlugIn.h"
#include "msLib.h"
#include <vector>
#include <string>
#include <Math.H>

#include "3d\\matrix.h"

#pragma warning(disable : 4996)
#pragma warning(disable : 4244)

std::string ConvertToString( float value )
{
    char pStr[20];
    sprintf( pStr, "%f", value );
    std::string result = std::string(pStr);

    if( result == "-0.000000" )
    {
        result = "0.000000";
    }

    return result;
}

class CJoint
{
public:

    class CKeyFrame
    {
    public:
        std::string time, x, y, z, rx, ry, rz;
		float btime, bx, by, bz, brx, bry, brz;

        void Init( msPositionKey *pKeyPos, msRotationKey *pKeyRot )
        {
            time = ConvertToString( pKeyPos->fTime );
			btime = pKeyPos->fTime;

            x = ConvertToString( pKeyPos->Position[0] );
            y = ConvertToString( pKeyPos->Position[1] );
            z = ConvertToString( pKeyPos->Position[2] );

			bx = pKeyPos->Position[0];
            by = pKeyPos->Position[1];
            bz = pKeyPos->Position[2];

            rx = ConvertToString( pKeyRot->Rotation[0] );
            ry = ConvertToString( pKeyRot->Rotation[1] );
            rz = ConvertToString( pKeyRot->Rotation[2] );

			brx = pKeyRot->Rotation[0];
            bry = pKeyRot->Rotation[1];
            brz = pKeyRot->Rotation[2];
        }
    };

	int jointVertCount;
	std::string x, y, z, rx, ry, rz, name, parentName;
    std::vector<CKeyFrame> keyFrame;
	float bx, by, bz, brx, bry, brz;

    bool Init( msBone *pBone, bool leftHandSystem )
    {
        char pStr[MS_MAX_NAME];
        msVec3 pos, rot;

        msBone_GetPosition( pBone, pos );
        msBone_GetRotation( pBone, rot );

        msBone_GetName( pBone, pStr, MS_MAX_NAME );
        name = std::string(pStr);

        msBone_GetParentName( pBone, pStr, MS_MAX_NAME );
        parentName = std::string(pStr);

		// The z sign needs to be flipped for left handed system
        if( leftHandSystem )
		{
			pos[2] = -pos[2];
			rot[0] = -rot[0];
			rot[1] = rot[2];
			rot[2] = rot[1];
		}

        x = ConvertToString( pos[0] );
        y = ConvertToString( pos[1] );
        z = ConvertToString( pos[2] );

		bx = pos[0];
        by = pos[1];
        bz = pos[2];

        rx = ConvertToString( rot[0] );
        ry = ConvertToString( rot[1] );
        rz = ConvertToString( rot[2] );

		brx = rot[0];
        bry = rot[1];
        brz = rot[2];

        int animCount = msBone_GetPositionKeyCount( pBone );

        // Make sure positions and rotations are the same
        if( animCount != msBone_GetRotationKeyCount(pBone) )
        {
            MessageBox (NULL, "Number of joint positions and rotations don't match", "XML Export", MB_OK | MB_ICONWARNING);
            return false;
        }

        for( int i = 0; i < animCount; i++ )
        {
            msPositionKey *pKeyPos = msBone_GetPositionKeyAt( pBone, i );
            msRotationKey *pKeyRot = msBone_GetRotationKeyAt( pBone, i );

			// The z sign needs to be flipped for left handed system
			if( leftHandSystem )
			{
				CMatrix matrix;

				matrix.Rotate( CRadian(pKeyRot->Rotation[0], pKeyRot->Rotation[1], pKeyRot->Rotation[2]) );
				matrix.Rotate( CPoint(180, 180, 180) );

				CRadian tmp = matrix.GetMatrixRotation();

				pKeyRot->Rotation[0] = tmp.z;
				pKeyRot->Rotation[1] = tmp.y;
				pKeyRot->Rotation[2] = -tmp.x;
			}

            keyFrame.push_back( CKeyFrame() );
            keyFrame.back().Init(pKeyPos, pKeyRot);
        }

        return true;
    }

    bool IsJointValue( msPositionKey *pKeyPos, msRotationKey *pKeyRot )
    {
        bool result = true;
        std::string x, y, z, rx, ry, rz;
        std::string cv("0.000000");

        x = ConvertToString( pKeyPos->Position[0] );
        y = ConvertToString( pKeyPos->Position[1] );
        z = ConvertToString( pKeyPos->Position[2] );

        rx = ConvertToString( pKeyRot->Rotation[0] );
        ry = ConvertToString( pKeyRot->Rotation[1] );
        rz = ConvertToString( pKeyRot->Rotation[2] );

        // check if it's all zeros
        if( cv == x && cv == y && cv == z &&
            cv == rx && cv == ry && cv == rz )
        {
            result = false;
        }
    
        return result;
    }
};

class CVert
{
public:
    std::string x, y, z;
    int index;
    int jointIndex;
	msVec3 vertAry;

    // Converts to a string for better comparison
    CVert operator = ( msVec3 &vert )
  	{
		memcpy( vertAry,  vert, sizeof(msVec3));
        x = ConvertToString( vert[0] );
        y = ConvertToString( vert[1] );
        z = ConvertToString( vert[2] );

		return *this;

	}	// operator =

    bool operator == ( CVert &vert )
    {
        if( vert.x == x )
        {
            if( vert.y == y )
            {
                if( vert.z == z )
                {
                    return true;
                }
            }
        }
        return false;

    }	// operator ==

    void CVert::SetJointIndex( int index )
    {
        jointIndex = index;
    }
};

class CUV
{
public:
    std::string u, v;
    int index;
	msVec2 uvAry;

    // Converts to a string for better compare
    CUV operator = ( msVec2 &uv )
  	{
		memcpy( uvAry,  uv, sizeof(msVec2));
        u = ConvertToString( uv[0] );
        v = ConvertToString( uv[1] );

		return *this;

	}	// operator =

    bool operator == ( CUV &uv )
    {
        if( uv.u == u )
        {
            if( uv.v == v )
            {
                return true;
            }
        }
        return false;

    }	// operator ==
};

class CMaterial
{
public:
    std::string alpha;
    std::string defuse;
};

class CFace
{
public:
    CVert vert[3];
    CUV uv[3];
    CVert vertNorm[3];
    int matIndex;
};


struct msModel;
class cPlugIn : public cMsPlugIn
{
    char szTitle[64];

public:
	cPlugIn ();
    virtual ~cPlugIn ();

public:
    int             GetType ();
    const char *    GetTitle ();
    int             Execute (msModel* pModel);

    //CVert & CalcSurfaceNormal( std::vector<CVert> & vertVect, int v1, int v2, int v3 );
    int GetPathTrimIndex( char * path );
	void find_and_replace( std::string &source, const std::string find, std::string replace );
};



#endif // __MS_PLUGIN_IMPL_H__