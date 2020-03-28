
/************************************************************************
*    FILE NAME:       generatormanager.cpp
*
*    DESCRIPTION:     Generator Manager
************************************************************************/

// Physical component dependency
#include <managers/generatormanager.h>

// Standard lib dependencies

// Boost lib dependencies

// Game lib dependencies
#include <common/matrix.h>
#include <2d/generator2d.h>
#include <2d/objectdata2d.h>
#include <2d/objectdatalist2d.h>


/************************************************************************
*    desc:  Constructer
************************************************************************/
CGeneratorMgr::CGeneratorMgr()
{
}   // constructor


/************************************************************************
*    desc:  Destructer
************************************************************************/
CGeneratorMgr::~CGeneratorMgr()
{
}   // destructor


/************************************************************************
*    desc:  Create the generators
************************************************************************/
void CGeneratorMgr::CreateGenerators( std::string & filePath, CPointInt & _focus )
{
    focus = _focus;

    // this open and parse the XML file:
    const XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "generatorLst" );

    for( int i = 0; i < mainNode.nChildNode(); ++i )
    {
        const XMLNode genNode = mainNode.getChildNode( "generator", i );
        const XMLNode objDataNode = genNode.getChildNode( "objectData" );
        const XMLNode sectorNode = genNode.getChildNode( "sector" );
        const XMLNode countNode = genNode.getChildNode( "count" );
        const XMLNode depthNode = genNode.getChildNode( "depth" );
        const XMLNode rotateNode = genNode.getChildNode( "rotate" );
        const XMLNode scaleNode = genNode.getChildNode( "scale" );
        const XMLNode colorNode = genNode.getChildNode( "color" );
        const XMLNode frmDisplyNode = genNode.getChildNode( "frameDisplay" );

        // Create the generator
        generator.push_back( new CGenerator2D( 
            CObjectDataList2D::Instance().GetData( objDataNode.getAttribute( "group" ), objDataNode.getAttribute( "type" ) ),
            atoi(sectorNode.getAttribute( "size" )),
            atoi(sectorNode.getAttribute( "dimensions" )),
            atoi(countNode.getAttribute( "min" )),
            atoi(countNode.getAttribute( "max" )),
            atoi(depthNode.getAttribute( "min" )),
            atoi(depthNode.getAttribute( "max" )),
            atoi(rotateNode.getAttribute( "min" )),
            atoi(rotateNode.getAttribute( "max" )),
            atoi(scaleNode.getAttribute( "min" )),
            atoi(scaleNode.getAttribute( "max" )),
            atof(colorNode.getAttribute( "r_min" )),
            atof(colorNode.getAttribute( "r_max" )),
            atof(colorNode.getAttribute( "g_min" )),
            atof(colorNode.getAttribute( "g_max" )),
            atof(colorNode.getAttribute( "b_min" )),
            atof(colorNode.getAttribute( "b_max" )),
            atof(colorNode.getAttribute( "a_min" )),
            atof(colorNode.getAttribute( "a_max" )),
            atoi(colorNode.getAttribute( "colorGridSize" )),
            atoi(colorNode.getAttribute( "hueShiftMin" )),
            atoi(colorNode.getAttribute( "hueShiftMax" )),
            (std::string("random") == frmDisplyNode.getAttribute( "displayType" ) ? CGenerator2D::EFD_RANDOM : CGenerator2D::EFD_SEQUENTIALLY ),
            atoi(genNode.getAttribute( "seed" )),
            focus,
            (std::string("true") == genNode.getAttribute( "instance" ) ? true : false ) ));
    }

}	// CreateGenerators


/************************************************************************
*    desc:  Handle the generation
*
*	 param: bool forceGenerate - forces the generators to create their
*								 sectors and sprites as opposed to waiting
*								 for a difference in focus
************************************************************************/
void CGeneratorMgr::HandleGeneration( CPointInt & newFocus, bool forceGenerate )
{
    // If the current focus doesn't match the last one, we remove some sprites and generate new ones
    if( focus != newFocus || forceGenerate )
    {
        for( size_t i = 0; i < generator.size(); ++i )
            generator[i].Generate( focus, newFocus, forceGenerate );

        focus = newFocus;
    }

}   // HandleSpaceGeneration


/************************************************************************
*    desc:  Transform the world generator's components
************************************************************************/
void CGeneratorMgr::Transform()
{
    for( size_t i = 0; i < generator.size(); ++i )
        generator[i].Transform();

}	// Transform


/************************************************************************
*    desc:  Transform the world generator's components
*
*	 param: const CMatrix & _scaledMatrix  - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point      - passed in relative position
************************************************************************/
void CGeneratorMgr::Transform( const CMatrix & _scaledMatrix,
                                     const CMatrix & _unscaledMatrix, 
                                     const CWorldPoint & point )
{
    for( size_t i = 0; i < generator.size(); ++i )
        generator[i].Transform( _scaledMatrix, _unscaledMatrix, point );
    
}	// Transform


/************************************************************************
*    desc:  Update the sectors in the world
************************************************************************/
void CGeneratorMgr::Update()
{
    for( size_t i = 0; i < generator.size(); ++i )
        generator[i].Update();

}	// Update


/************************************************************************
*    desc:  Render the sectors in the world
*
*	 param:	const CMatrix & matrix    - passed in relative rotation matrix
*			const CWorldPoint & point - passed in relative position
***********************************************************************/
void CGeneratorMgr::Render()
{
    for( size_t i = 0; i < generator.size(); ++i )
        generator[i].Render();

}	// Render



/************************************************************************
*    desc:  Clear the contents of the generator
************************************************************************/
void CGeneratorMgr::Clear()
{
    generator.clear();

}	// Clear