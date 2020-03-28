//*****************************************************************************
/*!
 *  \file   tagfileparser.h
 *
 *  \brief  Designed to read in a tag file and parse the information into a
 *          data definition node tree based on the definition file supplied.
 *
 *****************************************************************************/

#ifndef _tagfileparser_h
#define _tagfileparser_h

#include <map>
#include <string>
#include "tagtokenparser.h"
#include "setdefs.h"

// Forward declarations
class CDataDefNode;

//*****************************************************************************
/*!
 *  \class  CTagFileParser
 *
 *  \brief  This class implements a singleton object responsible for caching
 *          data def node trees base on a tagged configuration file.  It will
 *          maintain internal copies and create clones for clients to use and
 *          discard.
 *****************************************************************************/
class CTagFileParser
{
public: // static class methods	to use parser as a singleton

    // Produces a data definition tree from a config file and a tag defs file.
    CDataDefNode * SingletonParseFile( const std::string & tagFilename,
                                       const std::string & definitionFilename = "");

    // Retrieve a reference to a static singleton CTagFileParser object.
    static CTagFileParser & GetInstance();

public: // methods

    // Class destructor.
    ~CTagFileParser();

    // Non singleton method.
    // Produces a data definition tree from a config file and a tag defs file.
    CDataDefNode * ParseFile( const std::string & tagFilename,
                              const std::string & definitionFilename = "");

    // Retrieve a cached data node tree based on the specified filename.
    CDataDefNode * RetrieveFile(const std::string & tagFilename);

    // Cache a data node tree built from the specified filename.
    void CacheTagFile(const std::string & tagFilename,
                      const std::string & definitionFilename = "");

    // Purge all cached data def node trees.
    void PurgeCache();

    // Remove one tag file's data def node tree.
    void PurgeCacheOfTagFile(const std::string & tagFilename);

private: // methods

    // Class constuctor, made private for singleton.
    CTagFileParser() {}

    // These default methods should not be called on a singleton.
    CTagFileParser(const CTagFileParser & copy);
    CTagFileParser & operator=(const CTagFileParser & rhs);

    // Retrieve the information about tags that will be understood.
    void RetrieveTagInfo(TTagTypeMap & knownTags,
                         const std::string & definitionFilename,
                         TStringSet & previouslyParsedFiles);

    // Converts the definition file into a tagName -> tagType mapping.
    void ConvertTokensToTagInfo(TTagTypeMap & knownTags,
                                const TTokenList & definitionTokens,
                                TStringSet & previouslyParsedFiles);

	// Tests for the existence of a file with the specified name
	const bool IsFileAvailable(const std::string & filename);

private: // members

    // A map to hold cached data def node trees, associated with the filename.
    std::map<std::string, CDataDefNode *> cachedNodes;
};

#endif  // _tag_file_parser_h_
