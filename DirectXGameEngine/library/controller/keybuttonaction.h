
/************************************************************************
*    FILE NAME:       keybuttonaction.h
*
*    DESCRIPTION:     Template class to convert the state of a button
*                     or key press into an up, down or hold state
*                     for the given action
************************************************************************/

#ifndef __classtemplate_h__
#define __classtemplate_h__

// Game lib dependencies
#include <controller/controllerdefs.h>
#include <common/defs.h>

template <class type>
class CKeyButtonAction
{
public:
    CKeyButtonAction(type _id):counter(0),lastCount(0),queryActive(false)
    {
        id.push_back(_id);
    }

    // Reset the counter
    void Reset()
    {
        lastCount = counter;
        counter = 0;

        // Set the query to active here because
        // this is the button up check
        if( lastCount > counter )
            queryActive = true;
    }

    // Inc the counter
    void Inc()
    {
        lastCount = counter;
        ++counter;
        queryActive = true;
    }

    // Get the result based on the counter and the last count
    NDevice::EActionPress GetResult()
    {
        if( (lastCount > 0) && (counter > 0) )
            return NDevice::EAP_HOLD;

        else if( (lastCount == 0) && (counter == 1) )
            return NDevice::EAP_DOWN;

        else if( (lastCount > 0) && (counter == 0) )
            return NDevice::EAP_UP;

        else
            return NDevice::EAP_IDLE;
    }

    // Get the id the action is associated with
    type GetId( unsigned int index = 0 )
    {
        return id[index];
    }

    // Get the id the action is associated with
    type GetSecondId()
    {
        return secondId;
    }

    // Do we have a second id?
    size_t GetIdCount()
    {
        return id.size();
    }

    // Set the second id
    void SetId( type _id )
    {
        id.push_back(_id);
    }

    // Is the Query active
    bool IsQueryActive()
    {
        return queryActive;
    }

    // Set the second id
    void ClearQuery()
    {
        queryActive = false;
    }

protected:

    // Controller specific type
    std::vector<type> id;

    // Active counter
    uint counter;

    // Holds the last counter value for comparison
    uint lastCount;

    // querry flag
    bool queryActive;
};

#endif  // __classtemplate_h__


