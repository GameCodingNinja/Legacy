
/************************************************************************
*    FILE NAME:       iaibase.h
*
*    DESCRIPTION:     iAIBase Class
************************************************************************/

#ifndef __ai_base_h__
#define __ai_base_h__

// Forward declaration(s)
class CActorSprite2D;

class iAIBase
{
public:

    // Constructor
    // this class does not own the CActorSprite pointer so don't delete it
    explicit iAIBase( CActorSprite2D * _pActor );

    // Destructor
    virtual ~iAIBase(){}

    // React to what the player is doing
    virtual void HandleGameInput(){}

    // Check for collision and react to it
    virtual void ReactToCollision(){}

    // Update animations, Move sprites
    virtual void Update(){}

    // Is this sprite valid?
    bool IsValid(){ return validFlag; }

    // Set the actor as invalid
    void Invalidate(){ validFlag = false; }

    // Set the actor as valid
    void Validate(){ validFlag = true; }

protected:

    // Pointer to actor class.
    // NOTE: We do not own this pointer. Don't try to free it!
    CActorSprite2D * pActor;

    // Flag to indicate if the sprite is valid
    bool validFlag;

};

#endif  // __ai_base_h__


