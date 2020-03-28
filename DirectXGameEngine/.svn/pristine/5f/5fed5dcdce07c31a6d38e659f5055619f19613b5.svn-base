
/************************************************************************
*    FILE NAME:       actormanager.cpp
*
*    DESCRIPTION:     CActorManager Class
************************************************************************/

// Physical component dependency
#include <managers/actormanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Game lib dependencies
#include <2d/actorsprite2d.h>
#include <2d/iaibase.h>
#include <utilities/exceptionhandling.h>

// Required namespace(s)
using namespace std;

/************************************************************************
*    desc:  Constructer
************************************************************************/
CActorManager::CActorManager()
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CActorManager::~CActorManager()
{
}	// destructer


/************************************************************************
*    desc:  preload an actor sprite 2D to force a load of mesh and textures
*
*    param: name - actor name
************************************************************************/
void CActorManager::PreloadActor2D( const string & name )
{
    CActorSprite2D actorSprite2D;

    actorSprite2D.Create( name );

}	// PreloadActor2D


/************************************************************************
*    desc:  create a unique actor sprite 2D
*
*    param: name - actor name
************************************************************************/
void CActorManager::CreateActor2D( const string & name, CWorldPoint pos, CPoint rot, CPoint scale )
{
    // See if this actor has already been created
    actor2DMapIter = actor2DMap.find( name );

    // Insert it into the map
    if( actor2DMapIter == actor2DMap.end() )
        actor2DMap.insert( name, CreateActorPtr2D( name, pos, rot, scale ) );

}	// CreateActor2D


/************************************************************************
*    desc:  delete a unique actor sprite 2D
*
*    param: name - actor name
************************************************************************/
void CActorManager::DeleteActor2D( const string & name )
{
    // See if this actor has already been created
    actor2DMapIter = actor2DMap.find( name );

    // Delete it from the map
    if( actor2DMapIter != actor2DMap.end() )
        actor2DMap.erase( actor2DMapIter );

}	// DeleteUnique2D


/************************************************************************
*    desc:  Clear out all the unique actors
************************************************************************/
void CActorManager::ClearAllActor2D()
{
    actor2DMap.clear();

}	// ClearAllActor2D


/************************************************************************
*    desc:  Clear out all the actors in the given vector
************************************************************************/
void CActorManager::ClearActor2DVec( const string & name )
{
    // See if this actor has already been created
    actor2DVecMapIter = actor2DVecMap.find( name );

    // Delete it from the map
    if( actor2DVecMapIter != actor2DVecMap.end() )
        actor2DVecMapIter->second->clear();

}	// ClearAllActor2DVec


/************************************************************************
*    desc:  Clear out all the map vectors
************************************************************************/
void CActorManager::ClearAllActor2DVecMap()
{
    actor2DVecMap.clear();

}	// ClearAllActor2DVecMap


/************************************************************************
*    desc:  Clear out everything
************************************************************************/
void CActorManager::ClearAll()
{
    ClearAllActor2D();
    ClearAllActor2DVecMap();

}	// ClearAll


/************************************************************************
*    desc:  Create the actor and return it.
*
*    param: name - actor name
************************************************************************/
CActorSprite2D * CActorManager::CreateActorPtr2D( const string & name, 
                                                  const CWorldPoint & pos, 
                                                  const CPoint & rot, 
                                                  const CPoint & scale )
{
    // Allocate the actor
    CActorSprite2D * pActorSprite = new CActorSprite2D;

    // Create it and get the ai name
    string aiName = pActorSprite->Create( name, true );

    if( !pos.IsEmpty() )
        pActorSprite->SetPos(pos);

    if( !rot.IsEmpty() )
        pActorSprite->SetRot(rot);

    if( scale != pActorSprite->GetScale() )
        pActorSprite->SetScale(scale);

    // Send out a creat signal
    signal( aiName, pActorSprite );

    return pActorSprite;

}	// CreateActorPtr2D


/************************************************************************
*    desc:  create an actor sprite 2D
*
*    param: name - actor name
************************************************************************/
CActorSprite2D * CActorManager::GetActor2D( const string & name )
{
    // See if this actor has already been created
    actor2DMapIter = actor2DMap.find( name );

    if( actor2DMapIter != actor2DMap.end() )
        return actor2DMapIter->second;

    throw NExcept::CCriticalException("Get Actor 2D Error!",
                boost::str( boost::format("Actor 2D name can't be found (%s).\n\n%s\nLine: %s") % name.c_str() % __FUNCTION__ % __LINE__ ));

    return NULL;

}	// GetActor2D


/************************************************************************
*    desc:  create an actor sprite 2D vector
*
*    param: name - actor name
************************************************************************/
void CActorManager::CreateActorVec2D( const string & name )
{
    // See if this actor has already been created
    actor2DVecMapIter = actor2DVecMap.find( name );

    // Insert it into the map
    if( actor2DVecMapIter == actor2DVecMap.end() )
        actor2DVecMap.insert( name, new boost::ptr_vector<CActorSprite2D> );

}	// CreateActorVec2D


/************************************************************************
*    desc:  Delete an actor sprite 2D vector
*
*    param: name - actor name
************************************************************************/
void CActorManager::DeleteActorVec2D( const string & name )
{
    // See if this actor has already been created
    actor2DVecMapIter = actor2DVecMap.find( name );

    // Delete it from the map
    if( actor2DVecMapIter != actor2DVecMap.end() )
        actor2DVecMap.erase( actor2DVecMapIter );

}	// DeleteActorVec2D


/************************************************************************
*    desc:  Create an actor sprite 2D to add to a vector
*
*    param: name - actor name
************************************************************************/
void CActorManager::AddActorToVec2D( const string & vecName, 
                                     const string & actorName, 
                                     CWorldPoint pos, 
                                     CPoint rot, 
                                     CPoint scale )
{
    // See if this actor has already been created
    actor2DVecMapIter = actor2DVecMap.find( vecName );

    // Insert it into the map
    if( actor2DVecMapIter != actor2DVecMap.end() )
        actor2DVecMapIter->second->push_back( CreateActorPtr2D(actorName, pos, rot, scale) );

}	// CreateActorVec2D


/************************************************************************
*    desc:  Add a Created actor to a vector
*
*    param: name - actor name
*           pActorSprite - sprite pointer
************************************************************************/
void CActorManager::AddActorToVec2D( const string & vecName, CActorSprite2D * pActorSprite )
{
    // See if this actor has already been created
    actor2DVecMapIter = actor2DVecMap.find( vecName );

    // Insert it into the map
    if( actor2DVecMapIter != actor2DVecMap.end() )
        actor2DVecMapIter->second->push_back( pActorSprite );

    else
        throw NExcept::CCriticalException("Add Actor 2D Error!",
                boost::str( boost::format("Actor 2D has not been created (%s).\n\n%s\nLine: %s") % vecName % __FUNCTION__ % __LINE__ ));

}	// CreateActorVec2D


/************************************************************************
*    desc:  Connect to the AI signal slot
*  
*    param: slot - boost slot
************************************************************************/
void CActorManager::Connect( const CreateAISignal::slot_type & slot )
{
    signal.connect(slot);

}	// SetCreateAICallback


/***************************************************************************
*    decs:  Act upon what the user is doing
****************************************************************************/
void CActorManager::HandleGameInput2D()
{
    for( actor2DMapIter = actor2DMap.begin();
         actor2DMapIter != actor2DMap.end();
         ++actor2DMapIter )
    {
         actor2DMapIter->second->HandleGameInput();
    }

}	// HandleGameInput2D


/***************************************************************************
*    desc:  Check for collision and react to it
****************************************************************************/
void CActorManager::ReactToCollision2D()
{
    for( actor2DMapIter = actor2DMap.begin();
         actor2DMapIter != actor2DMap.end();
         ++actor2DMapIter )
    {
         actor2DMapIter->second->ReactToCollision();
    }

    for( actor2DVecMapIter = actor2DVecMap.begin();
         actor2DVecMapIter != actor2DVecMap.end();
         ++actor2DVecMapIter )
    {
        for( actor2DVecIter = actor2DVecMapIter->second->begin();
             actor2DVecIter != actor2DVecMapIter->second->end();
             ++actor2DVecIter )
        {
             actor2DVecIter->ReactToCollision();
        }
    }

}	// ReactToCollision2D


/************************************************************************
*    desc:  Transform the actor manager's components
************************************************************************/
void CActorManager::Transform()
{
    for( actor2DMapIter = actor2DMap.begin();
         actor2DMapIter != actor2DMap.end();
         ++actor2DMapIter )
    {
         actor2DMapIter->second->Transform();
    }

    for( actor2DVecMapIter = actor2DVecMap.begin();
         actor2DVecMapIter != actor2DVecMap.end();
         ++actor2DVecMapIter )
    {
        for( actor2DVecIter = actor2DVecMapIter->second->begin();
             actor2DVecIter != actor2DVecMapIter->second->end();
             ++actor2DVecIter )
        {
             actor2DVecIter->Transform();
        }
    }
    
}	// Transform


/************************************************************************
*    desc:  Transform the actor manager's components
*
*	 param: const CMatrix & _scaledMatrix  - passed in relative scaled rotation matrix
*			const CMatrix & _unscaledMatrix - passed in relative unscaled rotation matrix
*			const CWorldPoint & point      - passed in relative position
************************************************************************/
void CActorManager::Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                               const CWorldPoint & point )
{
    for( actor2DMapIter = actor2DMap.begin();
         actor2DMapIter != actor2DMap.end();
         ++actor2DMapIter )
    {
         actor2DMapIter->second->Transform( _scaledMatrix, _unscaledMatrix, point );
    }

    for( actor2DVecMapIter = actor2DVecMap.begin();
         actor2DVecMapIter != actor2DVecMap.end();
         ++actor2DVecMapIter )
    {
        for( actor2DVecIter = actor2DVecMapIter->second->begin();
             actor2DVecIter != actor2DVecMapIter->second->end();
             ++actor2DVecIter )
        {
             actor2DVecIter->Transform( _scaledMatrix, _unscaledMatrix, point );
        }
    }
    
}	// Transform


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CActorManager::Update2D()
{
    for( actor2DMapIter = actor2DMap.begin();
         actor2DMapIter != actor2DMap.end(); )
    {
         actor2DMapIter->second->Update();

         // Check if this actor is valid and delete if not
         if( !actor2DMapIter->second->IsValid() )
             actor2DMapIter = actor2DMap.erase( actor2DMapIter );

         else
            ++actor2DMapIter;
    }

    for( actor2DVecMapIter = actor2DVecMap.begin();
         actor2DVecMapIter != actor2DVecMap.end();
         ++actor2DVecMapIter )
    {
        for( actor2DVecIter = actor2DVecMapIter->second->begin();
             actor2DVecIter != actor2DVecMapIter->second->end(); )
        {
            actor2DVecIter->Update();

            // Check if this actor is invalid and delete if not
            if( !actor2DVecIter->IsValid() )
                actor2DVecIter = actor2DVecMapIter->second->erase( actor2DVecIter );

            else
                ++actor2DVecIter;
        }
    }

}	// Update2D


/***************************************************************************
*    desc:  Do the actor2D rendering
****************************************************************************/
void CActorManager::Render2D()
{
    for( actor2DMapIter = actor2DMap.begin();
         actor2DMapIter != actor2DMap.end();
         ++actor2DMapIter )
    {
         actor2DMapIter->second->Render();
    }

    for( actor2DVecMapIter = actor2DVecMap.begin();
         actor2DVecMapIter != actor2DVecMap.end();
         ++actor2DVecMapIter )
    {
        for( actor2DVecIter = actor2DVecMapIter->second->begin();
             actor2DVecIter != actor2DVecMapIter->second->end();
             ++actor2DVecIter )
        {
             actor2DVecIter->Render();
        }
    }

}	// Render2D


