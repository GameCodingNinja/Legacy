
/************************************************************************
*    FILE NAME:       savefile.cpp
*
*    DESCRIPTION:     Class for saving game saves
************************************************************************/

// Physical component dependency
#include "savefile.h"

// Standard lib dependencies
#include <fstream>
#include <ctime>

// Boost lib dependencies
#include <boost/format.hpp>

// Boost lib dependencies
#include <boost/archive/xml_oarchive.hpp> // saving
#include <boost/archive/xml_iarchive.hpp> // loading
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/nondet_random.hpp>

// Game lib dependencies
#include <misc/settings.h>
#include <utilities/exceptionhandling.h>

// Required namespace(s)
using namespace std;


/************************************************************************
*    desc:  Constructer
************************************************************************/
CSaveFile::CSaveFile()
{
	SetFileExtension(".game_save");

	// There's 4 different ammo tiers
	shipAmmoTierVec.resize(EAT_MAX_AMMO_TIERS);

}   // constructor


/************************************************************************
*    desc:  Init the new game file
************************************************************************/
void CSaveFile::InitNewGameFile()
{
	// Open the save file with the default values for a new game
	//std::ifstream ifs( "data/game/init.game_save" );
    //boost::archive::xml_iarchive ia(ifs);

    // load data from archive
	//ia >> boost::serialization::make_nvp("game_save", *this);

	// Set the world seed
	worldSeed = boost::random::random_device()();

	// Seed the boost random number generator
	randGen.seed( boost::random::random_device()() );

	const uint MIN_DIST_CENTER_UNIVERSE(10483648);
	const uint MAX_DIST_CENTER_UNIVERSE(1547483648);

	// Randomly generate the ship position opposite it's home world position
	for( uint i = 0; i < EP2D_MAX; ++i )
	{
		boost::random::uniform_int_distribution<> distPos( MIN_DIST_CENTER_UNIVERSE, MAX_DIST_CENTER_UNIVERSE);
		boost::random::uniform_int_distribution<> distSigned(0, 1);

		if( distSigned(randGen) )
		{
			if( EP2D_X == i )
			{
				playerPos.x.i = distPos(randGen);
				homeWorldPos.x.i = -playerPos.x.i;
			}
			else
			{
				playerPos.y.i = distPos(randGen);
				homeWorldPos.y.i = -playerPos.y.i;
			}
		}
		else
		{
			if( EP2D_X == i )
			{
				homeWorldPos.x.i = distPos(randGen);
				playerPos.x.i = -homeWorldPos.x.i;
			}
			else
			{
				homeWorldPos.y.i = distPos(randGen);
				playerPos.y.i = -homeWorldPos.y.i;
			}
		}
	}

}	// InitNewGameFile


/************************************************************************
*    desc:  Save the file
************************************************************************/
void CSaveFile::Save()
{
	size_t dashPos = GetFileName().find("_");
	size_t dotPos = GetFileName().find(".");

	string saveNo = GetFileName().substr(0, dashPos);
	string saveDuration = GetFileName().substr(dashPos+1, dotPos-dashPos-1);

	// Convert the string duration to the actual duration class
	boost::posix_time::time_duration startDur( boost::posix_time::duration_from_string( saveDuration ) );

	// Inc the ongoing game duration
	SetDuration( startDur );

	// Add to the ongoing game duration
	StopGameTime();

	// Convert the new duration to a deliminated string
	string newDurStr = boost::posix_time::to_simple_string( GetDuration() );

	// Replace the ":" to "-", a character that can be used in a file name
	std::replace( newDurStr.begin(), newDurStr.end(), ':', '-');

	// construct the new file name with the new duration time
	string newFileName = saveNo + "_" + newDurStr + GetFileExtension();

	// Open and save the file
	std::ofstream ofs( GetSaveFilePath() + newFileName);
    boost::archive::xml_oarchive oa(ofs);

    // write data to archive
	oa << boost::serialization::make_nvp("game_save", *this);

	// Delete the old file
	if( newFileName != GetFileName() )
		boost::filesystem::remove( GetSaveFilePath() + GetFileName() );

	// Set the new file name
	SetFileName( newFileName );

	// Start the game time
	StartGameTime();

}	// Save


/************************************************************************
*    desc:  Save the file
************************************************************************/
void CSaveFile::NewSave()
{
	// Init the save file data
	InitNewGameFile();

	// Get the file name map to determin the new save no.
	const std::multimap<std::time_t, std::string> & fileMap = GetFileNameMap();

	uint highestSaveNoFound(0);

	// Find the highest save no.
	if( fileMap.size() > 0 )
	{
		for( std::multimap<std::time_t, std::string>::const_iterator it = fileMap.begin(); 
			 it != fileMap.end();
			 ++it )
		{
			// Find the file name deliminators
			size_t dashPos = it->second.find("_");

			// Get the save no. and the play time duration
			string saveNoStr = it->second.substr(0, dashPos);

			uint saveNo = boost::lexical_cast<uint>( saveNoStr );

			if( saveNo > highestSaveNoFound )
				highestSaveNoFound = saveNo;
		}
	}

	// Add one to the highest no found
	string fileNo = boost::lexical_cast<string>( highestSaveNoFound + 1 );

	// Set the new file name
	SetFileName(fileNo + "_00-00-00" + GetFileExtension());

	std::ofstream ofs( GetSaveFilePath() + GetFileName());
    boost::archive::xml_oarchive oa(ofs);

    // write data to archive
	oa << boost::serialization::make_nvp("game_save", *this);

	// Start the game time
	StartGameTime();

}	// NewSave


/************************************************************************
*    desc:  Save the file by overwritting another
************************************************************************/
void CSaveFile::OverwriteSave( const string & fileToOverwrite )
{
	// Init the save file data
	InitNewGameFile();

	// Get the save file number from the file to be overwritten
	size_t dashPos = fileToOverwrite.find("_");
	string saveNo = fileToOverwrite.substr(0, dashPos);

	// Set the new file name
	SetFileName(saveNo + "_00-00-00" + GetFileExtension());

	std::ofstream ofs( GetSaveFilePath() + GetFileName());
    boost::archive::xml_oarchive oa(ofs);

    // write data to archive
	oa << boost::serialization::make_nvp("game_save", *this);

	// Delete the overwritten file
	if( fileToOverwrite != GetFileName() )
		boost::filesystem::remove( GetSaveFilePath() + fileToOverwrite );

	// Start the game time
	StartGameTime();

}	// NewSave


/************************************************************************
*    desc:  Load the file
************************************************************************/
void CSaveFile::Load()
{
	// Load the save file
	std::ifstream ifs( GetSaveFilePath() + GetFileName());
    boost::archive::xml_iarchive ia(ifs);

    // load data from archive
	ia >> boost::serialization::make_nvp("game_save", *this);

	size_t dashPos = GetFileName().find("_");
	size_t dotPos = GetFileName().find(".");

	string saveNo = GetFileName().substr(0, dashPos);
	string saveDuration = GetFileName().substr(dashPos+1, dotPos-dashPos-1);

	// Convert the string duration to the actual duration class
	boost::posix_time::time_duration startDur( boost::posix_time::duration_from_string( saveDuration ) );

	// Inc the ongoing game duration
	SetDuration( startDur );

	// Start the game time
	StartGameTime();

}	// Load


/************************************************************************
*    desc:  Set/Get the player position
************************************************************************/
void CSaveFile::SetPlayerPos( const CWorldPoint & pos )
{
	playerPos = pos;

}	// SetPlayerPos

const CWorldPoint & CSaveFile::GetPlayerPos() const
{
	return playerPos;

}	// SetPlayerPos


/************************************************************************
*    desc:  Get the home world position
************************************************************************/
const CWorldPoint & CSaveFile::GetHomeWorldPos() const
{
	return homeWorldPos;

}	// GetHomeWorldPos


/************************************************************************
*    desc:  Set/Get the ship health
************************************************************************/
void CSaveFile::SetShipHealth( const CProgressBar & progressBar )
{
	shipHealth = progressBar;

}	// SetShipHealth

const CProgressBar & CSaveFile::GetShipHealth() const
{
	return shipHealth;

}	// GetShipHealth


/************************************************************************
*    desc:  Set/Get the ship shields
************************************************************************/
void CSaveFile::SetShipShields( const CProgressBar & progressBar )
{
	shipShields = progressBar;

}	// SetShipShields

const CProgressBar & CSaveFile::GetShipShields() const
{
	return shipShields;

}	// GetShipShields



/************************************************************************
*    desc:  Set/Get the ship fuel
************************************************************************/
void CSaveFile::SetShipFuel( const CProgressBar & progressBar )
{
	shipFuel = progressBar;

}	// SetShipFuel

const CProgressBar & CSaveFile::GetShipFuel() const
{
	return shipFuel;

}	// GetShipFuel


/************************************************************************
*    desc:  Set/Get the ship ammo
************************************************************************/
void CSaveFile::SetShipAmmoTier( const CProgressBar & progressBar, int tier )
{
	shipAmmoTierVec[tier] = progressBar;

}	// SetShipAmmoTier

const CProgressBar & CSaveFile::GetShipAmmoTier1() const
{
	return shipAmmoTierVec[EAT_AMMO_TIER_1];

}	// GetShipAmmoTier1

const CProgressBar & CSaveFile::GetShipAmmoTier2() const
{
	return shipAmmoTierVec[EAT_AMMO_TIER_2];

}	// GetShipAmmoTier2

const CProgressBar & CSaveFile::GetShipAmmoTier3() const
{
	return shipAmmoTierVec[EAT_AMMO_TIER_3];

}	// GetShipAmmoTier3

const CProgressBar & CSaveFile::GetShipAmmoTier4() const
{
	return shipAmmoTierVec[EAT_AMMO_TIER_4];

}	// GetShipAmmoTier1

uint CSaveFile::GetWorldSeed() const
{
	return worldSeed;

}	// GetWorldSeed


/************************************************************************
*    desc:  Check to see if this recipe has been found
************************************************************************/
bool CSaveFile::RecipeFound( const string & recipe )
{
	return (recipiesSet.find( recipe ) != recipiesSet.end());

}	// RecipeFound

/************************************************************************
*    desc:  Add this recipe to the known list of recipes
************************************************************************/
void CSaveFile::SetRecipe( const string & recipe )
{
	recipiesSet.insert(recipe);

}	// SetRecipe


/************************************************************************
*    desc:  Add or subtract from the inventory item
*
*    ret: True if the inventory item is in supply, false if depleted
************************************************************************/
bool CSaveFile::AdjustInventory( const string & item, int amount )
{
	bool result(true);

	// See if this item is in the inventory
    inventoryIt = inventoryMap.find( item );

	// If the item is found, adjust the amount
    if( inventoryIt != inventoryMap.end() )
	{
		// Adjust the amount if not depleted
		if( (inventoryIt->second + amount) > 0 )
		{
			inventoryIt->second += amount;
		}
		// Delete this inventory item if depleted
		else
		{
			inventoryMap.erase( inventoryIt );
			result = false;
		}
	}
	else
	{
		// Throw an exception if the amount is less then 1
		if( amount < 1 )
			throw NExcept::CCriticalException("Error adding inventory item",
				boost::str( boost::format("Inventory item must be a positive number (%s, %d).\n\n%s\nLine: %s") % item % amount % __FUNCTION__ % __LINE__ ));

		inventoryMap.insert( make_pair(item, amount) );
	}

	return result;

}	// AdjustInventory


/************************************************************************
*    desc:  Get the amount of items of this type in the inventory
*
*    ret: The amount
************************************************************************/
int CSaveFile::GetInventoryAmount( const string & item )
{
	int result(0);

	// See if this item is in the inventory
    inventoryIt = inventoryMap.find( item );

	// If the item is found, adjust the amount
    if( inventoryIt != inventoryMap.end() )
		result = inventoryIt->second;

	return result;

}	// GetInventoryAmount







