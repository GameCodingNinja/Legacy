
//#include "stdafx.h"
#include "genfunc.h"
#include "spritebinaryfileheader.h"

#include "msPlugInImpl.h"

using namespace std;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}



cMsPlugIn*
CreatePlugIn ()
{
    return new cPlugIn ();
}



cPlugIn::cPlugIn ()
{
    strcpy (szTitle, "XML Exporter...");
}



cPlugIn::~cPlugIn ()
{
}



int cPlugIn::GetType ()
{
    return cMsPlugIn::eTypeExport;
}



const char* cPlugIn::GetTitle ()
{
    return szTitle;
}


int cPlugIn::Execute (msModel *pModel)
{
    if (!pModel)
        return -1;

    //
    // check, if we have something to export
    //
    if (msModel_GetMeshCount (pModel) == 0)
    {
        MessageBox (NULL, "The model is empty!  Nothing exported!", "XML Export", MB_OK | MB_ICONWARNING);
        return 0;
    }

    bool leftHandSystem = false;
    bool meshOnlyExport = true;

    if( msModel_GetBoneCount(pModel) > 0 )
    {
        if( MessageBox(NULL, "Export joint animation only?", "XML Export", MB_YESNO | MB_ICONQUESTION) == IDYES )
        {
            meshOnlyExport = false;
        }
    }

    if( MessageBox(NULL, "Export left handed system?", "XML Export", MB_YESNO | MB_ICONQUESTION) == IDYES )
    {
       leftHandSystem = true;
    }

    //
    // choose filename
    //
    OPENFILENAME ofn;
    memset (&ofn, 0, sizeof (OPENFILENAME));
    
    char szFile[MS_MAX_PATH];
    char szFileTitle[MS_MAX_PATH];
    char szDefExt[32] = "txt";
    char szFilter[128] = "XML Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0\0";
    szFile[0] = '\0';
    szFileTitle[0] = '\0';

    ofn.lStructSize = sizeof (OPENFILENAME);
    ofn.lpstrDefExt = szDefExt;
    ofn.lpstrFilter = szFilter;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MS_MAX_PATH;
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = MS_MAX_PATH;
    ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = "Export XML";

	vector<CVert> vertVector;
    vector<CUV> uvVector;
    vector<CVert> vertNormVector;
    vector< vector<CFace> > faceLstVector;
    vector<CMaterial> materialVector;
	vector<CJoint *> jointVector;
	unsigned int totalFaceCount = 0;


    if (!::GetSaveFileName (&ofn))
        return 0;

    //
    // export
    //
    FILE *file = fopen (szFile, "wt");
    if (!file)
        return -1;

    fprintf (file, "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n");

    if( meshOnlyExport )
    {
        // Indcate which system was exported
        if( leftHandSystem )
        {
            fprintf (file, "<!-- Export: Left handed System -->\n");
        }
        else
        {
            fprintf (file, "<!-- Export: Right handed System -->\n");
        }
    
        fprintf (file, "<mesh>\n\n");
    
        //////////////////////////////////
        //   organize and file duplicates
        //////////////////////////////////
    
        for( int i = 0; i < msModel_GetMeshCount(pModel); i++ )
        {
            msMesh * pMesh = msModel_GetMeshAt( pModel, i );
    
            for( int j = 0; j < msMesh_GetVertexCount(pMesh); j++ )
            {
                msVertex *pVertex = msMesh_GetVertexAt(pMesh, j);
                msVec3 msVert;
                msVertex_GetVertex(pVertex, msVert);
    
                // The z sign needs to be flipped for left handed system
                if( leftHandSystem )
                {
                    msVert[2] = -msVert[2];
                }
    
                CVert vert;
                vert = msVert;
                vert.SetJointIndex( msVertex_GetBoneIndex(pVertex) );
    
                bool found = false;
    
                for( unsigned int w = 0; w < vertVector.size() && !found; w++ )
                {
                    if( vertVector[w] == vert )
                    {
                        found = true;
                    }
                }
    
                if( !found )
                {
                    vertVector.push_back( vert );
                }
    
                msVec2 msUv;
                msVertex_GetTexCoords( pVertex, msUv );
                found = false;
                CUV uv;
                uv = msUv;
    
                for( unsigned int w = 0; w < uvVector.size() && !found; w++ )
                {
                    if( uvVector[w] == uv )
                    {
                        found = true;
                    }
                }
    
                if( !found )
                {
                    uvVector.push_back( uv );
                }
            }
    
            for( int j = 0; j < msMesh_GetVertexNormalCount(pMesh); j++ )
            {
                msVec3 msVectNorm;
                msMesh_GetVertexNormalAt( pMesh, j, msVectNorm );

				// The z sign needs to be flipped for left handed system
                if( leftHandSystem )
                {
                    msVectNorm[2] = -msVectNorm[2];
                }
    
                CVert vectNorm;
                vectNorm = msVectNorm;
    
                bool found = false;
    
                for( unsigned int w = 0; w < vertNormVector.size() && !found; w++ )
                {
                    if( vertNormVector[w] == vectNorm )
                    {
                        found = true;
                    }
                }
    
                if( !found )
                {
                    vertNormVector.push_back( vectNorm );
                }
            }
        }
    
        for( int i = 0; i < msModel_GetMaterialCount(pModel); i++ )
        {
            msMaterial *pMaterial = msModel_GetMaterialAt(pModel, i);
            char szTexture[MS_MAX_PATH];
            CMaterial material;
            msMaterial_GetDiffuseTexture(pMaterial, szTexture, MS_MAX_PATH);
    
            // Remove the ../ from the file path
            material.defuse = &szTexture[GetPathTrimIndex( szTexture )];
    
            // Find the "\" and replace it with "/"
            find_and_replace( material.defuse, "\\", "/" );
    
            msMaterial_GetAlphaTexture(pMaterial, szTexture, MS_MAX_PATH);
    
            // Remove the ../ from the file path
            material.alpha = &szTexture[GetPathTrimIndex( szTexture )];
    
            // Find the "\" and replace it with "/"
            find_and_replace( material.alpha, "\\", "/" );
    
            materialVector.push_back( material );
        }
    
        //////////////////////////////////
        //   match faces to lists
        //////////////////////////////////
    
        for( int i = 0; i < msModel_GetMeshCount(pModel); i++ )
        {
            msMesh * pMesh = msModel_GetMeshAt( pModel, i );
            int matIndex = msMesh_GetMaterialIndex(pMesh);
			vector< CFace > faceVector;
    
            for( int j = 0; j < msMesh_GetTriangleCount(pMesh); j++ )
            {
                msTriangle *pTriangle = msMesh_GetTriangleAt( pMesh, j );
                
                word vIndices[3];
                msTriangle_GetVertexIndices( pTriangle, vIndices );
    
                word nIndices[3];
                msTriangle_GetNormalIndices( pTriangle, nIndices );
    
                CFace face;
    
                face.matIndex = matIndex;
    
                for( int w = 0; w < 3; w++ )
                {
                    msVertex *pVertex = msMesh_GetVertexAt(pMesh, vIndices[w]);
                    msVec3 msVert;
                    msVertex_GetVertex(pVertex, msVert);

					// The z sign needs to be flipped for left handed system
					if( leftHandSystem )
					{
						msVert[2] = -msVert[2];
					}
                    
                    CVert vert;
                    vert = msVert;
    
                    bool found = false;
                    int indexFound;
    
                    for( unsigned int z = 0; z < vertVector.size() && !found; z++ )
                    {
                        if( vertVector[z] == vert )
                        {
                            found = true;
                            indexFound = z;
                        }
                    }
        
                    if( found )
                    {
                        face.vert[w].index = indexFound;
                    }
                    else
                    {
                        MessageBox (NULL, "face vert index not found!", "XML Export", MB_OK | MB_ICONWARNING);
                        return 0;
                    }
    
                    msVec2 msUv;
                    msVertex_GetTexCoords( pVertex, msUv );
                    found = false;
                    CUV uv;
                    uv = msUv;
        
                    for( unsigned int z = 0; z < uvVector.size() && !found; z++ )
                    {
                        if( uvVector[z] == uv )
                        {
                            found = true;
                            indexFound = z;
                        }
                    }
        
                    if( found )
                    {
                        face.uv[w].index = indexFound;
                    }
                    else
                    {
                        MessageBox (NULL, "face uv index not found!", "XML Export", MB_OK | MB_ICONWARNING);
                        return 0;
                    }
    
                    msVec3 msVertNorm;
                    msMesh_GetVertexNormalAt(pMesh, nIndices[w], msVertNorm);

					// The z sign needs to be flipped for left handed system
					if( leftHandSystem )
					{
						msVertNorm[2] = -msVertNorm[2];
					}
                    
                    CVert vertNorm;
                    vertNorm = msVertNorm;
    
                    found = false;
    
                    for( unsigned int z = 0; z < vertNormVector.size() && !found; z++ )
                    {
                        if( vertNormVector[z] == vertNorm )
                        {
                            found = true;
                            indexFound = z;
                        }
                    }
        
                    if( found )
                    {
                        face.vertNorm[w].index = indexFound;
                    }
                    else
                    {
                        MessageBox (NULL, "face vert norm index not found!", "XML Export", MB_OK | MB_ICONWARNING);
                        return 0;
                    }
                }
    
                faceVector.push_back( face );

				totalFaceCount++;
            }

			faceLstVector.push_back( faceVector );
        }
    
        //////////////////////////////////
        //   vert list
        //////////////////////////////////
    
        fprintf( file, "\t<vert_list count=\"%d\">\n", vertVector.size() );
    
        for( vector<CVert>::iterator it = vertVector.begin(); 
             it != vertVector.end(); it++ )
        {
            if( msModel_GetBoneCount(pModel) > 0 )
            {
                fprintf( file, "\t\t<vert x=\"%s\" y=\"%s\" z=\"%s\" j=\"%d\"/>\n", 
                         (*it).x.c_str(), (*it).y.c_str(), (*it).z.c_str(), (*it).jointIndex );
            }
            else
            {
                fprintf( file, "\t\t<vert x=\"%s\" y=\"%s\" z=\"%s\" />\n", 
                         (*it).x.c_str(), (*it).y.c_str(), (*it).z.c_str() );
            }
        }
         
        fprintf( file, "\t</vert_list>\n\n" );
    
    
        //////////////////////////////////
        //   UV list
        //////////////////////////////////
    
		if( materialVector.size() > 0 )
		{
			fprintf( file, "\t<uv_list count=\"%d\">\n", uvVector.size() );
	    
			for( vector<CUV>::iterator it = uvVector.begin(); 
				 it != uvVector.end(); it++ )
			{
				fprintf( file, "\t\t<uv u=\"%s\" v=\"%s\"/>\n", 
						 (*it).u.c_str(), (*it).v.c_str() );
			}
	         
			fprintf( file, "\t</uv_list>\n\n" );
    
 
			//////////////////////////////////
			//   vertex normal list
			//////////////////////////////////
	    
			fprintf( file, "\t<vert_norm_list count=\"%d\">\n", vertNormVector.size() );
	    
			for( vector<CVert>::iterator it = vertNormVector.begin(); 
				 it != vertNormVector.end(); it++ )
			{
				fprintf( file, "\t\t<vn x=\"%s\" y=\"%s\" z=\"%s\"/>\n", 
						 (*it).x.c_str(), (*it).y.c_str(), (*it).z.c_str() );
			}
	         
			fprintf( file, "\t</vert_norm_list>\n\n" );

		}
    
    
        fprintf( file, "\t<face_group count=\"%d\">\n", totalFaceCount );
    
        //////////////////////////////////
        //   faces
        //////////////////////////////////
    
        if( materialVector.size() > 0 )
        {
            // Sort by material
            for( unsigned int i = 0; i < faceLstVector.size(); i++ )
            {
				msMesh * pMesh = msModel_GetMeshAt( pModel, i );
        
                if( faceLstVector[i].size() > 0 )
                {
                    fprintf( file, "\t\t<face_list count=\"%d\" mIndex=\"%d\" name=\"%s\">\n", faceLstVector[i].size(), faceLstVector[i][0].matIndex, pMesh->szName );
	            
					for( vector<CFace>::iterator it = faceLstVector[i].begin(); 
						 it != faceLstVector[i].end(); it++ )
					{
						// Arrange verticies in lelt or right handed system
						if( leftHandSystem )
						{
							fprintf( file, "\t\t\t<face v1=\"%d\" v2=\"%d\" v3=\"%d\" uv1=\"%d\" uv2=\"%d\" uv3=\"%d\" vn1=\"%d\" vn2=\"%d\" vn3=\"%d\"/>\n",
									 (*it).vert[0].index, (*it).vert[2].index, (*it).vert[1].index,
									 (*it).uv[0].index, (*it).uv[2].index, (*it).uv[1].index, 
									 (*it).vertNorm[0].index, (*it).vertNorm[2].index, (*it).vertNorm[1].index );
						}
						else
						{
							fprintf( file, "\t\t\t<face v1=\"%d\" v2=\"%d\" v3=\"%d\" uv1=\"%d\" uv2=\"%d\" uv3=\"%d\" vn1=\"%d\" vn2=\"%d\" vn3=\"%d\"/>\n",
									 (*it).vert[0].index, (*it).vert[1].index, (*it).vert[2].index,
									 (*it).uv[0].index, (*it).uv[1].index, (*it).uv[2].index, 
									 (*it).vertNorm[0].index, (*it).vertNorm[1].index, (*it).vertNorm[2].index );
						}
					}
            
                    fprintf( file, "\t\t</face_list>\n" );
                }
            }
        }
        else
        {
			// Sort by material
            for( unsigned int i = 0; i < faceLstVector.size(); i++ )
            {
				msMesh * pMesh = msModel_GetMeshAt( pModel, i );
        
                if( faceLstVector[i].size() > 0 )
                {
					fprintf( file, "\t\t<face_list count=\"%d\" name=\"%s\">\n", faceLstVector[i].size(), pMesh->szName );
		            
					for( vector<CFace>::iterator it = faceLstVector[i].begin(); 
						 it != faceLstVector[i].end(); it++ )
					{
						// Arrange verticies in lelt or right handed system
						if( leftHandSystem )
						{
							fprintf( file, "\t\t\t<face v1=\"%d\" v2=\"%d\" v3=\"%d\"/>\n",
									 (*it).vert[0].index, (*it).vert[2].index, (*it).vert[1].index );
						}
						else
						{
							fprintf( file, "\t\t\t<face v1=\"%d\" v2=\"%d\" v3=\"%d\"/>\n",
									 (*it).vert[0].index, (*it).vert[1].index, (*it).vert[2].index );
						}
					}
		            
					fprintf( file, "\t\t</face_list>\n" );
				}
			}
        }

		fprintf( file, "\t</face_group>\n\n" );


        //////////////////////////////////
        //   materials
        //////////////////////////////////
        
		if( materialVector.size() > 0 )
		{
			fprintf( file, "\t<material_list count=\"%d\">\n", msModel_GetMaterialCount(pModel) );
	    
			for( vector<CMaterial>::iterator it = materialVector.begin(); 
				 it != materialVector.end(); it++ )
			{
				fprintf( file, "\t\t<material defuse=\"%s\" alpha=\"%s\"/>\n", 
						 (*it).defuse.c_str(), (*it).alpha.c_str() );
			}
	         
			fprintf( file, "\t</material_list>\n\n" );
		}

		//////////////////////////////////
        //   joints
        //////////////////////////////////

		if( msModel_GetBoneCount(pModel) > 0 )
		{
			fprintf( file, "\t<joint_list joint_count=\"%d\" frame_count=\"%d\">\n", msModel_GetBoneCount(pModel), msModel_GetTotalFrames(pModel) );

			for( int i = 0; i < msModel_GetBoneCount(pModel); i++)
			{
				// made a seperate class for varifying all the data
				CJoint * pJoint = new CJoint();
	    
				// check for positions and rotations to be the same time frame
				if( !pJoint->Init( msModel_GetBoneAt(pModel, i), leftHandSystem ) )
				{
					PostMsg( "XML Export", "Joint (%s) has different position and rotation time values.", pJoint->name.c_str() );
				}

				pJoint->jointVertCount = 0;
				//int jointCount = 0;

				// count up all the verts using this joint index
				for( vector<CVert>::iterator it = vertVector.begin(); 
				     it != vertVector.end(); it++ )
				{
					if( it->jointIndex == i )
					{
						pJoint->jointVertCount++;
					}
				}

				// Make sure the joint has verts assigned to it
				if( pJoint->jointVertCount == 0 )
				{
					PostMsg( "XML Export", "Joint (%s) has no verts assigned to it", pJoint->name.c_str() );
				}
	    
				fprintf( file, "\t\t<joint vert_count=\"%d\" name=\"%s\" parent=\"%s\" x=\"%s\" y=\"%s\" z=\"%s\"/>\n",
						 pJoint->jointVertCount, pJoint->name.c_str(), pJoint->parentName.c_str(),
						 pJoint->x.c_str(), pJoint->y.c_str(), pJoint->z.c_str(), 
						 pJoint->rx.c_str(), pJoint->ry.c_str(), pJoint->rz.c_str());

				if( pJoint->rx != "0.000000" || pJoint->ry != "0.000000" || pJoint->rx != "0.000000" )
				{
					PostMsg( "XML Export", "Joint (%s) has rotation values. Rotation values not supported.", pJoint->name.c_str() );
				}

				jointVector.push_back(pJoint);
			}

			fprintf( file, "\t</joint_list>\n\n" );
		}

        fprintf (file, "</mesh>\n\n");
    }
    else
    {
		// Indcate which system was exported
        if( leftHandSystem )
        {
            fprintf (file, "<!-- Export: Left handed System -->\n");
        }
        else
        {
            fprintf (file, "<!-- Export: Right handed System -->\n");
        }

        fprintf (file, "<joint_animation>\n\n");

        //////////////////////////////////
        //   joints
        //////////////////////////////////
        fprintf( file, "\t<joint_list joint_count=\"%d\" frame_count=\"%d\">\n", msModel_GetBoneCount(pModel), msModel_GetTotalFrames(pModel) );
    
        for( int i = 0; i < msModel_GetBoneCount(pModel); i++)
        {
            // made a seperate class for varifying all the data
            CJoint * pJoint = new CJoint();
    
			// check for positions and rotations to be the same
            if( !pJoint->Init( msModel_GetBoneAt(pModel, i), leftHandSystem ) )
            {
                return 0;
            }
    
            fprintf( file, "\t\t<joint count=\"%d\" name=\"%s\" parent=\"%s\">\n",
                     pJoint->keyFrame.size(), pJoint->name.c_str(), pJoint->parentName.c_str() );
    
            for( unsigned int j = 0; j < pJoint->keyFrame.size(); j++ )
            {
                fprintf( file, "\t\t\t<keyframe time=\"%s\" x=\"%s\" y=\"%s\" z=\"%s\" rx=\"%s\" ry=\"%s\" rz=\"%s\"/>\n",
                         pJoint->keyFrame[j].time.c_str(),
                         pJoint->keyFrame[j].x.c_str(), pJoint->keyFrame[j].y.c_str(), pJoint->keyFrame[j].z.c_str(),
                         pJoint->keyFrame[j].rx.c_str(), pJoint->keyFrame[j].ry.c_str(), pJoint->keyFrame[j].rz.c_str() );
            }
    
            fprintf( file, "\t\t</joint>\n" );

			jointVector.push_back( pJoint );
        }
    
        fprintf( file, "\t</joint_list>\n\n" );
        fprintf (file, "</joint_animation>\n\n");                                                
    }

    fclose (file);




	/***********************************************************************************************	
	************************************************************************************************
	*	Binary File Exporter
	************************************************************************************************
	***********************************************************************************************/

	// If we're exporting a mesh file, go into here
	if( meshOnlyExport )
	{
		char rssExtension[] = {'r', 's', 's', '\0'};

		for( int i = 0; i < MS_MAX_PATH; i++ )
		{
			int j = 0;

			if( szFile[i] == '.' )
			{
				//atFileExtension = true;
				strcpy( &szFile[i+1], rssExtension );

				break;
			}
		}

		// Open the file / create it
		file = fopen (szFile, "wb");
		if (!file)
			return -1;

		CSpriteBinaryFileHeader header;

		// Set up counts
		header.vert_count = vertVector.size();
		header.uv_count = uvVector.size();
		header.vert_norm_count = vertNormVector.size();
		header.face_group_count = faceLstVector.size();
		header.text_count = materialVector.size();
		header.joint_count = jointVector.size();

		fwrite( &header, 1, sizeof(header), file );


		///////////////////////////////////////////////
		//	Vert List
		///////////////////////////////////////////////

		// Write down the vert list tag
		fwrite( &VERT_LIST, 1, sizeof(VERT_LIST), file );

		// If there are no joints then export only the vertex information
		if( header.joint_count == 0 )
		{
			for( unsigned int i = 0; i < vertVector.size(); i++ )
			{
				fwrite( &vertVector[i].vertAry, 1, sizeof(vertVector[i].vertAry), file );
			}
		}
		// If there are joints, then export the joint indexes as well
		else
		{
			for( unsigned int i = 0; i < vertVector.size(); i++ )
			{
				fwrite( &vertVector[i].vertAry, 1, sizeof(vertVector[i].vertAry), file );
				fwrite( &vertVector[i].jointIndex, 1, sizeof(vertVector[i].jointIndex), file );
			}
		}


		///////////////////////////////////////////////
		//	UV List
		///////////////////////////////////////////////

		// If there's no materials then UVs aren't needed
		if( materialVector.size() > 0 )
		{
			// Write down the UV list tag
			fwrite( &UV_LIST, 1, sizeof(UV_LIST), file );

			for( unsigned int i = 0; i < uvVector.size(); i++ )
			{
				fwrite( &uvVector[i].uvAry, 1, sizeof(uvVector[i].uvAry), file );
			}
		}


		///////////////////////////////////////////////
		//	Vert Normal List
		///////////////////////////////////////////////

		// If there's no materials then normals aren't needed
		if( materialVector.size() > 0 )
		{
			// Write down the normal list tag
			fwrite( &VERT_NORM_LIST, 1, sizeof(VERT_NORM_LIST), file );

			for( unsigned int i = 0; i < vertNormVector.size(); i++ )
			{
				fwrite( &vertNormVector[i].vertAry, 1, sizeof(vertNormVector[i].vertAry), file );
			}
		}


		///////////////////////////////////////////////
		//	Face Group
		///////////////////////////////////////////////

		// Write the face group tag
		fwrite( &FACE_GROUP, 1, sizeof(FACE_GROUP), file );

		CBinaryFaceGroup group;

		// If there are materials then we need to write down the UV and normal information
		if( materialVector.size() > 0 )
		{
			for( unsigned int i = 0; i < faceLstVector.size(); i++ )
			{
				// Write down the face list tag
				fwrite( &FACE_LIST, 1, sizeof(FACE_LIST), file );

				group.groupFaceCount = faceLstVector[i].size();
				group.matIndex = faceLstVector[i][0].matIndex;

				fwrite( &group, 1, sizeof(group), file );

				int indexAry[9];

				// If we're doing a left handed system then we have to write down the information in a different order
				if( leftHandSystem )
				{
					for( unsigned int j = 0; j < faceLstVector[i].size(); j++ )
					{
						indexAry[0] = faceLstVector[i][j].vert[0].index;
						indexAry[1] = faceLstVector[i][j].vert[2].index;
						indexAry[2] = faceLstVector[i][j].vert[1].index;

						indexAry[3] = faceLstVector[i][j].uv[0].index;
						indexAry[4] = faceLstVector[i][j].uv[2].index;
						indexAry[5] = faceLstVector[i][j].uv[1].index;

						indexAry[6] = faceLstVector[i][j].vertNorm[0].index;
						indexAry[7] = faceLstVector[i][j].vertNorm[2].index;
						indexAry[8] = faceLstVector[i][j].vertNorm[1].index;

						fwrite( indexAry, 1, sizeof(indexAry), file );
					}
				}
				else
				{
					for( unsigned int j = 0; j < faceLstVector[i].size(); j++ )
					{
						indexAry[0] = faceLstVector[i][j].vert[0].index;
						indexAry[1] = faceLstVector[i][j].vert[1].index;
						indexAry[2] = faceLstVector[i][j].vert[2].index;

						indexAry[3] = faceLstVector[i][j].uv[0].index;
						indexAry[4] = faceLstVector[i][j].uv[1].index;
						indexAry[5] = faceLstVector[i][j].uv[2].index;

						indexAry[6] = faceLstVector[i][j].vertNorm[0].index;
						indexAry[7] = faceLstVector[i][j].vertNorm[1].index;
						indexAry[8] = faceLstVector[i][j].vertNorm[2].index;

						fwrite( indexAry, 1, sizeof(indexAry), file );
					}
				}
			}
		}
		else
		{
			for( unsigned int i = 0; i < faceLstVector.size(); i++ )
			{
				fwrite( &FACE_LIST, 1, sizeof(FACE_LIST), file );

				int faceCount = faceLstVector[i].size();
				//group.groupFaceCount = faceLstVector[i].size();
				//group.matIndex = faceLstVector[i][0].matIndex;

				fwrite( &faceCount, 1, sizeof(faceCount), file );
			
				int indexAry[3];
			
				if( leftHandSystem )
				{
					for( unsigned int j = 0; j < faceLstVector[i].size(); j++ )
					{
						indexAry[0] = faceLstVector[i][j].vert[0].index;
						indexAry[1] = faceLstVector[i][j].vert[2].index;
						indexAry[2] = faceLstVector[i][j].vert[1].index;

						fwrite( indexAry, 1, sizeof(indexAry), file );
					}
				}
				else
				{
					for( unsigned int j = 0; j < faceLstVector[i].size(); j++ )
					{
						indexAry[0] = faceLstVector[i][j].vert[0].index;
						indexAry[1] = faceLstVector[i][j].vert[1].index;
						indexAry[2] = faceLstVector[i][j].vert[2].index;

						fwrite( indexAry, 1, sizeof(indexAry), file );
					}
				}
			}
		}


		///////////////////////////////////////////////
		//	Material List
		///////////////////////////////////////////////	

		if( materialVector.size() > 0 )
		{
			// Write down the material list tag
			fwrite( &MAT_LIST, 1, sizeof(MAT_LIST), file );

			char art_path[MAT_PATH_SIZE];

			for( unsigned int i = 0; i < materialVector.size(); i++ )
			{
				if( materialVector[i].defuse.length() > MAT_PATH_SIZE-1 )
				{
					PostMsg( "Binary Export", "Material string path (%s) too long.", materialVector[i].defuse.c_str() );
					return -1;
				}

				memset( art_path, 0, sizeof(art_path) );
				strcpy( art_path, materialVector[i].defuse.c_str() );
				fwrite( art_path, 1, sizeof(art_path), file );
			}
		}


		///////////////////////////////////////////////
		//	Joint List
		///////////////////////////////////////////////	

		if( jointVector.size() > 0 )
		{
			// Write down the joint list tag
			fwrite( &JOINT_LIST, 1, sizeof(JOINT_LIST), file );

			for( unsigned int i = 0; i < jointVector.size(); i++ )
			{
				CBinaryJoint joint;

				joint.vert_count = jointVector[i]->jointVertCount;

				// Do not export if the joint name is too long
				if( jointVector[i]->name.length() > JOINT_NAME_SIZE-1 )
				{
					PostMsg( "Binary Export", "Joint string name (%s) too long.", jointVector[i]->name.c_str() );
					return -1;
				}

				// copy the string to the char array
				memset( joint.name, 0, sizeof(joint.name) );
				strcpy( joint.name, jointVector[i]->name.c_str() );
			
				// Do not export if the parent name is too long
				if( jointVector[i]->parentName.length() > JOINT_NAME_SIZE-1 )
				{
					PostMsg( "Binary Export", "Joint string parent name (%s) too long.", jointVector[i]->parentName.c_str() );
					return -1;
				}

				// copy the string to the char array
				memset( joint.parentName, 0, sizeof(joint.parentName) );
				strcpy( joint.parentName, jointVector[i]->parentName.c_str() );

				joint.x = jointVector[i]->bx;
				joint.y = jointVector[i]->by;
				joint.z = jointVector[i]->bz;

				fwrite( &joint, 1, sizeof(joint), file );
			}
		}

		fclose (file);
	}
	else
	{
		/***********************************************************************************************	
		************************************************************************************************
		*	Joint Animation File
		************************************************************************************************
		***********************************************************************************************/

		char rsaExtension[] = {'r', 's', 'a', '\0'};

		for( int i = 0; i < MS_MAX_PATH; i++ )
		{
			int j = 0;

			if( szFile[i] == '.' )
			{
				strcpy( &szFile[i+1], rsaExtension );

				break;
			}
		}

		// Open the file
		file = fopen (szFile, "wb");
		if (!file)
			return -1;

		CSpriteAnimationBinaryFileHeader header;

		header.joint_count = jointVector.size();
		header.frame_count = msModel_GetTotalFrames(pModel);

		// Write down the header
		fwrite( &header, 1, sizeof(header), file );


		///////////////////////////////////////////////
		//	Joint
		///////////////////////////////////////////////	

		// Write down the joint tag
		fwrite( &JOINT, 1, sizeof(JOINT), file );

		for( unsigned int i = 0; i < jointVector.size(); i++ )
		{
			CBinaryJointAnimation joint;	

			joint.keyframe_count = jointVector[i]->keyFrame.size();
			
			// copy the string to the char array
			memset( joint.name, 0, sizeof(joint.name) );
			strcpy( joint.name, jointVector[i]->name.c_str() );

			// copy the string to the char array
			memset( joint.parentName, 0, sizeof(joint.parentName) );
			strcpy( joint.parentName, jointVector[i]->parentName.c_str() );

			fwrite( &joint, 1, sizeof(joint), file );

			// Write down the key frame tag
			fwrite( &KEY_FRAME, 1, sizeof(KEY_FRAME), file );

			for( unsigned int j = 0; j < jointVector[i]->keyFrame.size(); j++ )
			{
				CBinaryKeyFrame keyframe;		

				keyframe.time = jointVector[i]->keyFrame[j].btime;

				keyframe.x = jointVector[i]->keyFrame[j].bx;
				keyframe.y = jointVector[i]->keyFrame[j].by;
				keyframe.z = jointVector[i]->keyFrame[j].bz;

				keyframe.rx = jointVector[i]->keyFrame[j].brx;
				keyframe.ry = jointVector[i]->keyFrame[j].bry;
				keyframe.rz = jointVector[i]->keyFrame[j].brz;

				fwrite( &keyframe, 1, sizeof(keyframe), file );
			}
		}

		fclose (file);
	}

    // dont' forget to destroy the model
    msModel_Destroy (pModel);

	// Delete all pointers in jointVector
	for( unsigned int i = 0; i < jointVector.size(); i++ )
	{
		if( jointVector[i] != NULL )
		{
			delete jointVector[i];
			jointVector[i] = NULL;
		}
	}

    return 0;
}


int cPlugIn::GetPathTrimIndex( char * path )
{
    int result = 0;

    while( path[result] == '.' || path[result] == '\\' )
    {
        result++;
    }

    return result;
}

/*void cPlugIn::ReplaceChar( char * path, char charToFind, char charToReplace )
{
    // Get the string length
    int length = strlen( path );

    for( int i = 0; i < length; i++ )
    {
        if( path[i] == charToFind )
        {
                path[i] = charToReplace;
        }
    }
}*/

void cPlugIn::find_and_replace( string &source, const string find, string replace )
{
    size_t j;

    for ( ; (j = source.find( find )) != string::npos ; )
    {
        source.replace( j, find.length(), replace );
    }

}

/*CVert & cPlugIn::CalcSurfaceNormal( vector<CVert> & vertVect, int v1, int v2, int v3 )
{
    CVert norm;

    double X1( vertVect[v1].x );
    double Y1( vertVect[v1].y );
    double Z1( vertVect[v1].z );

    double X2( vertVect[v2].x );
    double Y2( vertVect[v2].y );
    double Z2( vertVect[v2].z );

    double X3( vertVect[v3].x );
    double Y3( vertVect[v3].y );
    double Z3( vertVect[v3].z );

    // Calculate the cross product
    double x( Y1 * ( Z2 - Z3 ) + Y2 * ( Z3 - Z1 ) + Y3 * ( Z1 - Z2 ) );
    double y( Z1 * ( X2 - X3 ) + Z2 * ( X3 - X1 ) + Z3 * ( X1 - X2 ) );
    double z( X1 * ( Y2 - Y3 ) + X2 * ( Y3 - Y1 ) + X3 * ( Y1 - Y2 ) );

    // Calculate the length of the vector
    double length( sqrt( (x*x) + (y*y) + (z*z) ) );

    // length can't be zero otherwise you'll get a divided by 0 error.

    // The normalizing part is the (n.x / length)
    if( length != 0.0 )
    {
        // Normalize the normal and make it a point. Sounds silly hu?
        norm.x = ( x / length );
        norm.y = ( y / length );
        norm.z = ( z / length );
    }

    return norm;

}   // CalcSurfaceNormal */
