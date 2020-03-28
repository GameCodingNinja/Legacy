
/************************************************************************
*
*    FILE NAME:       CPathNode.cpp
*
*    DESCRIPTION:     Dynamic linked tree list node
*
************************************************************************/

#include "CMatrix.h"         // Matrix class
#include "CPathNode.h"

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

// init the static class member
float CPathNode::mainNodeKeyCounter = 0.0;
float CPathNode::subNodeCounter = 0.0;
float CPathNode::lastMainKey = 0.0;

/************************************************************************
*    DESCRIPTION:   Constructer                                                             
************************************************************************/
CPathNode::CPathNode( CPoint &point )
	                : pos(point),
	                  prevNode(NULL),
	                  key(0.0),
					  radius(30),
					  mainNode(false)
{
}


/************************************************************************
*    DESCRIPTION:  Get the next node. First node is considered 
*                  to be the main node
*
*    Input:    unsigned int index - node index
************************************************************************/
CPathNode * CPathNode::GetNextNode( unsigned int index )
{ 
	CPathNode * result = NULL;

	if( index < nextNode.size() )
	{
		result = nextNode[index];
	}

	return result;

}	// GetNextNode


/************************************************************************
*    DESCRIPTION:  Reads a line of text from a text file
*
*    Input:    FILE *hFile - File handle
*              char *Str - text read up to the new-line character
*
*    Output:   int - Number of characters read in
************************************************************************/
int CPathNode::GetLine( FILE *hFile, char *str )
{
    // Reads a line of text from a text file
    char nextChar;
    int index = 0;

    // Reads a line of text from a text file
    nextChar = (char)fgetc( hFile );

    // Loop until we reach a new-line character
    while( nextChar != '\n' && index < MAX_LINE-2 )
    {
        // Check for end of file:
        if( nextChar == EOF )
        {
            // If found, close off string
            // and return EOF:
            str[ index ] = '\0';
            return EOF;
        }

        // Record the text
        if( nextChar == ' ' || nextChar == ',' )
           str[ index++ ] = '\0';
        else if( nextChar != '\r' && nextChar != '\n' )
           str[ index++ ] = nextChar;
            
        // Get the next character
        nextChar = (char)fgetc( hFile );
    }

    // Close off the string.
    if( index )
    {
        str[ index++ ] = '\0';
        str[ index ] = '\0';
    }

    return index;

}   // GetLine


/************************************************************************
*    DESCRIPTION:  decode node from passed string
*
*    Input:  char *strData    - Line of parsed characters
*            int strCount     - Number of characters in strData
*            std::vector<CPathNode *> nodeLst -  list of nodes
*
*    Output:   bool - true on success, false on fail
************************************************************************/   
bool CPathNode::Decode_Node( char *strData, int strCount, std::vector<CPathNode *> &nodeLst )
{
	bool result = false;
	float vert[3];
	int count = 0;
	CMatrix matrix;
	CPoint point;

	// Flip it around so that it's not upside down and backwards
	matrix.Rotate( 0, 180, 180 );

    while( strData[count] )
	{
	    // Decode each vertice into the allocated class
	    for( int i = 0; i < 3; ++i )
	    {
	        // Do a sanity check to ensure we are not reading junk
	        if( strData[count] < 45 || strData[count] > 57 || count > strCount )
	            goto HANDLE_ERROR;

	        // Decode the three vertices
	        vert[i] = atof( &strData[count] );

	        // Jump past the null
	        while( strData[count++] )
	        {}
	    }

		// Copy the vertex to the point
	    point.x = vert[0];
	    point.y = vert[1];
	    point.z = vert[2];

	    matrix.Transform( point );

		// Invert the values and switch the y and z
		point.x = -point.tx;
		point.y = -point.tz;
		point.z = -point.ty;

		point.tx = point.x;
		point.ty = point.y;
		point.tz = point.z;

		nodeLst.push_back( new CPathNode(point) );
	}

    // See if this is a main node
	if( nodeLst.size() == 1 )
	{
		nodeLst[0]->SetMainFlag( true );
	}
	
	// If we got this far, everything's ok
    result = true;	
	
HANDLE_ERROR:;

    return result;

}	// Decode_Node


/************************************************************************
*    DESCRIPTION:  Print this nodes data to a file
*
*    Input:    FILE *hFile - File handle
************************************************************************/
void CPathNode::PrintDataToFile( FILE *hFile )
{
	fprintf( hFile, "main (%f): %0.3f, %0.3f, %0.3f\n", key, pos.x, pos.y, pos.z );

	if( prevNode == NULL )
	{
		fprintf( hFile, "  prev: NULL\n" );
	}
	else
	{
		fprintf( hFile, "  prev (%f): %0.3f, %0.3f, %0.3f\n",
		         prevNode->GetKey(), 
		         prevNode->GetX(),
		         prevNode->GetY(),
		         prevNode->GetZ() );
	}

	if( GetNextNode() == NULL )
	{
		fprintf( hFile, "  next: NULL\n" );
	}
	else
	{
		for( unsigned int i = 0; i < GetNextNodeCount(); ++i )
		{
			fprintf( hFile, "  next (%f): %0.3f, %0.3f, %0.3f\n", 
			         GetNextNode(i)->GetKey(),
			         GetNextNode(i)->GetX(),
			         GetNextNode(i)->GetY(),
			         GetNextNode(i)->GetZ() );
		}
	}

	fprintf( hFile, "\n" );

}	// PrintDataToFile


/************************************************************************
*    DESCRIPTION:  Scale the node
*
*    Input:    float x, y, z: Scale amounts
************************************************************************/
void _fastcall CPathNode::Scale( float x, float y, float z )
{
	CMatrix m;

	// Create a matrix with the scaling factors
    m.Scale( x, y, z );

    // Transform all the vertices
    m.Transform( pos );

    // Copy it over and make it perminate        
    pos.x = pos.tx;
    pos.y = pos.ty;
    pos.z = pos.tz;                 

}	// Scale


/************************************************************************
*    DESCRIPTION:  Generate sub key
*
*    Input:    float mainKey - mainkey
************************************************************************/
float CPathNode::GenerateSubKey( float mainKey )
{
	// De we need to reset the key
	if( lastMainKey != mainKey )
	{
		subNodeCounter = 0.0;
		lastMainKey = mainKey;
	}

	return lastMainKey + (++subNodeCounter * 0.0001);

}	// GenerateSubKey


/************************************************************************
*    DESCRIPTION:  Does node = node
*
*    Input:   CPathNode &node - node to check against current node
*
*    Output:   bool - true on success, false on fail
************************************************************************/
bool CPathNode::operator == ( CPathNode &node )
{
    if( node.GetX() == pos.x )
    {
        if( node.GetY() == pos.y )
        {
            if( node.GetZ() == pos.z )
            {
                return true;
            }
        }
    }

    return false;

}	// operator ==


/************************************************************************
*    DESCRIPTION:  The inequality operator
*
*    Input:   CPathNode &node - node to check against current node
*
*    Output:   bool - true on success, false on fail
************************************************************************/
bool CPathNode::operator != ( CPathNode *node )
{
	if( ( node->GetX() != pos.x ) || ( node->GetY() != pos.y ) || ( node->GetZ() != pos.z ) )
  	{
		return true;
  	}
  	return false;

}	// operator !=