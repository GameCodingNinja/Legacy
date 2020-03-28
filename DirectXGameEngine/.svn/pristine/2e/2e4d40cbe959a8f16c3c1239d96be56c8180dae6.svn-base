
/************************************************************************
*    FILE NAME:       actormanager.h
*
*    DESCRIPTION:     CActorManager Class
************************************************************************/

#ifndef __actor_manager_h__
#define __actor_manager_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/function.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/signal.hpp>

// Game lib dependencies
#include <3d/worldcamera.h>

// Forward declaration(s)
class iAIBase;
class CActorSprite2D;

class CActorManager
{
public:

    // Define the boost signal
    typedef boost::signal<void (std::string &, CActorSprite2D *)> CreateAISignal;

    // Get the instance of the singleton class
    static CActorManager & Instance()
    {
        static CActorManager actorManager;
        return actorManager;
    }

    // Connect to the shader signal
    void Connect( const CreateAISignal::slot_type & slot );

    // preload an actor sprite 2D to force a load of mesh and textures
    void PreloadActor2D( const std::string & name );

    // create a unique actor sprite 2D
    void CreateActor2D( const std::string & name,
                        CWorldPoint pos = CWorldPoint(),
                        CPoint rot = CPoint(),
                        CPoint scale = CPoint(1,1,1) );

    // delete a unique actor sprite 2D
    void DeleteActor2D( const std::string & name );

    // Clear out all the unique actors
    void ClearAllActor2D();

    // Clear out all the actors in the given vector
    void ClearActor2DVec( const std::string & name );

    // Clear out all the map vectors
    void ClearAllActor2DVecMap();

    // Clear out everything
    void ClearAll();

    // Get the unique actor
    CActorSprite2D * GetActor2D( const std::string & name );

    // create an actor sprite 2D vector
    void CreateActorVec2D( const std::string & name );

    // Delete an actor sprite 2D vector
    void DeleteActorVec2D( const std::string & name );

    // Create an actor sprite 2D vector
    void AddActorToVec2D( const std::string & vecName,
                          const std::string & actorName,
                          CWorldPoint pos = CWorldPoint(),
                          CPoint rot = CPoint(),
                          CPoint scale = CPoint(1,1,1) );

    void AddActorToVec2D( const std::string & vecName,
                          CActorSprite2D * pActorSprite );

    // Act upon what the user is doing
    void HandleGameInput2D();

    // Check for collision and react to it
    void ReactToCollision2D();

    // Transform the actor manager's components
    void Transform();
    void Transform( const CMatrix & _scaledMatrix, const CMatrix & _unscaledMatrix, 
                    const CWorldPoint & point );

    // Update objects that require them
    void Update2D();

    // Do the rendering
    void Render2D();

    // Create the actor and return it.
    CActorSprite2D * CreateActorPtr2D( const std::string & name,
                                       const CWorldPoint & pos,
                                       const CPoint & rot,
                                       const CPoint & scale = CPoint(1,1,1) );

private:

    // Constructor
    CActorManager();

    // Destructor
    ~CActorManager();

private:

    // Unique actor sprite 2D map
    boost::ptr_map< const std::string, CActorSprite2D > actor2DMap;
    boost::ptr_map< const std::string, CActorSprite2D >::iterator actor2DMapIter;

    // Vector group actor sprite 2D map
    boost::ptr_map< const std::string, boost::ptr_vector<CActorSprite2D> > actor2DVecMap;
    boost::ptr_map< const std::string, boost::ptr_vector<CActorSprite2D> >::iterator actor2DVecMapIter;
    boost::ptr_vector<CActorSprite2D>::iterator actor2DVecIter;

    // Boost signal
    CreateAISignal signal;

};

#endif  // __actor_manager_h__


