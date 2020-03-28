
/************************************************************************
*    FILE NAME:       savefile.h
*
*    DESCRIPTION:     Class for saving game saves
************************************************************************/

#ifndef __save_file_h__
#define __save_file_h__

// Physical component dependency
#include <managers/basesavefile.h>

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/serialization/nvp.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/map.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

// Game lib dependencies
#include <common/worldpoint.h>
#include <common/progressbar.h>
#include <common/defs.h>

enum EAmmoTier
{
    EAT_AMMO_TIER_1,
    EAT_AMMO_TIER_2,
    EAT_AMMO_TIER_3,
    EAT_AMMO_TIER_4,
    EAT_MAX_AMMO_TIERS

};

class CSaveFile : public CBaseSaveFile
{
public:

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar 
            & BOOST_SERIALIZATION_NVP(worldSeed)
            & BOOST_SERIALIZATION_NVP(playerPos)
            & BOOST_SERIALIZATION_NVP(homeWorldPos)
            & BOOST_SERIALIZATION_NVP(shipHealth)
            & BOOST_SERIALIZATION_NVP(shipShields)
            & BOOST_SERIALIZATION_NVP(shipFuel)
            & BOOST_SERIALIZATION_NVP(shipAmmoTierVec)
            & BOOST_SERIALIZATION_NVP(recipiesSet)
            & BOOST_SERIALIZATION_NVP(inventoryMap)

            ;
    }

    // Get the instance of the singleton class
    static CSaveFile & Instance()
    {
        static CSaveFile saveFile;
        return saveFile;
    }

    // Save/Load the file
    void Save();
    void Load();

    // New save file
    void NewSave();

    // Save the file by overwritting another
    void OverwriteSave( const std::string & fileToOverwrite );

    // Set/Get player pos
    void SetPlayerPos( const CWorldPoint & pos );
    const CWorldPoint & GetPlayerPos() const;

    // Get the home world position
    const CWorldPoint & GetHomeWorldPos() const;

    // Set/Get ship health
    void SetShipHealth( const CProgressBar & progressBar );
    const CProgressBar & GetShipHealth() const;

    // Set/Get ship shields
    void SetShipShields( const CProgressBar & progressBar );
    const CProgressBar & GetShipShields() const;

    // Set/Get ship fuel
    void SetShipFuel( const CProgressBar & progressBar );
    const CProgressBar & GetShipFuel() const;

    // Get the world seed
    uint GetWorldSeed() const;

    // Set/Get the ship ammo tier
    void SetShipAmmoTier( const CProgressBar & progressBar, int tier );
    const CProgressBar & GetShipAmmoTier1() const;
    const CProgressBar & GetShipAmmoTier2() const;
    const CProgressBar & GetShipAmmoTier3() const;
    const CProgressBar & GetShipAmmoTier4() const;

    // Check to see if this recipe has been found
    bool RecipeFound( const std::string & recipe );

    // Add this recipe to the known list of recipes
    void SetRecipe( const std::string & recipe );

    // Add or subtract from the inventory item
    bool AdjustInventory( const std::string & item, int amount );

    // Get the amount of items of this type in the inventory
    int GetInventoryAmount( const std::string & item );

private:

    // Constructor
    CSaveFile();

    // Init the new game file
    void InitNewGameFile();

private:

    // World Seed
    uint worldSeed;

    // Player position
    CWorldPoint playerPos;

    // Player home world position
    CWorldPoint homeWorldPos;

    // Progress bars
    CProgressBar shipHealth;
    CProgressBar shipShields;
    CProgressBar shipFuel;
    std::vector<CProgressBar> shipAmmoTierVec;

    // Recipes found
    std::set<std::string> recipiesSet;

    // Inventory
    std::map< const std::string, int > inventoryMap;
    std::map< const std::string, int >::iterator inventoryIt;

    // Boost random number generator
    boost::random::mt19937 randGen;

};

#endif  // __save_file_h__


