/************************************************************************
*    FILE NAME:       gameobjectmanager.h
*
*    DESCRIPTION:     Game specific object manager class
************************************************************************/

#ifndef __gameobjectmanager_h__
#define __gameobjectmanager_h__

// Physical component dependency
#include "3d\\objectmanager.h"

// Game dependencies
#include "charactermanager.h"

class CGameObjectManager : public CObjectManager
{
public:

    CGameObjectManager();
    ~CGameObjectManager();

    void Update();

    // Change the active characters
    void ChangeCharacters();

    // Loads the character lists
    void LoadCharacterLists( std::string & filePath );

    // Renders the scene
    void Render( CMatrix & matrix );

private:

    CCharacterManager charManager;
};
#endif  // __gameobjectmanager_h__