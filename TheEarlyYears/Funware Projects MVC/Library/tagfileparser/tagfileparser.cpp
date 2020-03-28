//*****************************************************************************
/*!
 *  \file   tagfileparser.cpp
 *
 *  \brief  Designed to read in a tag file and parse the information into a
 *          data definition node tree based on the definition file supplied.
 *
 *****************************************************************************/

#include "windows.h"
#include <iostream>
#include "tagfileparser.h"
#include "datadefnode.h"
#include "deletefuncs.h"
#include <sys/stat.h>
#include <cassert>


//*****************************************************************************
/*!
 *  \brief  The definition file contains information about the tags that can be
 *          decoded by this function call.  The tag file contains the
 *          information stored in a <Tag> Info </Tag> format.  This function
 *          converts the tag file information into run time storage in the form
 *          of a node tree.  The node tree is stored in a cache, which is
 *          searched before any parsing begins.  If a cached node tree is
 *          found, is is cloned and returned to the caller.
 *
 *  \param  const string & tagFilename - The file containing the tags to be
 *          decoded.
 *  \param  const string & definitionFilename - The definition file.  Contains
 *          information about the different tags that this parsing routine can
 *          decode.
 *
 *  \return CDataDefNode * - the root node of the new data definition tree.
 *
 *****************************************************************************/
CDataDefNode * CTagFileParser::SingletonParseFile(const std::string & tagFilename,
                                                  const std::string & definitionFilename)
{
	// assert to see if the file actually exists
    assert( IsFileAvailable( tagFilename ) );

    CDataDefNode * pDataDefNodeTree =
        CTagFileParser::GetInstance().RetrieveFile(tagFilename);

    if (pDataDefNodeTree == NULL)
    {
        CTagFileParser::GetInstance().CacheTagFile(tagFilename, definitionFilename);

        pDataDefNodeTree =
            CTagFileParser::GetInstance().RetrieveFile(tagFilename);
    }

    return pDataDefNodeTree;
}


//*****************************************************************************
/*!
 *  \brief  Non singleton method.
 *          
 *          The definition file contains information about the tags that can be
 *          decoded by this function call.  The tag file contains the
 *          information stored in a <Tag> Info </Tag> format.  This function
 *          converts the tag file information into run time storage in the form
 *          of a node tree.  The node tree is stored in a cache, which is
 *          searched before any parsing begins.  If a cached node tree is
 *          found, is is cloned and returned to the caller.
 *
 *  \param  const string & tagFilename - The file containing the tags to be
 *          decoded.
 *  \param  const string & definitionFilename - The definition file.  Contains
 *          information about the different tags that this parsing routine can
 *          decode.
 *
 *  \return CDataDefNode * - the root node of the new data definition tree.
 *
 *****************************************************************************/
CDataDefNode * CTagFileParser::ParseFile(const std::string & tagFilename,
                                         const std::string & definitionFilename)
{
	// assert to see if the file actually exists
    assert( IsFileAvailable( tagFilename ) );

    CDataDefNode * pDataDefNodeTree = RetrieveFile(tagFilename);

    if (pDataDefNodeTree == NULL)
    {
        CacheTagFile(tagFilename, definitionFilename);

        pDataDefNodeTree = RetrieveFile(tagFilename);
    }

    return pDataDefNodeTree;
}


//*****************************************************************************
/*!
 *  \brief  Retrieve a reference to a static singleton CTagFileParser object.
 *
 *  \return CTagFileParser & - A reference to the singleton object.
 *
 *****************************************************************************/
CTagFileParser & CTagFileParser::GetInstance()
{
    // Create the singleton object if it does not exist.
    static CTagFileParser theParser;

    // Return a reference to the singleton.
    return theParser;
}


//*****************************************************************************
/*!
 *  \brief  Class destructor.
 *
 *****************************************************************************/
CTagFileParser::~CTagFileParser()
{
    PurgeCache();
}


//*****************************************************************************
/*!
 *  \brief  Retrieve a cached data node tree based on the specified filename.
 *
 *  \param  const string & tagFilename - The filename from which a cached node
 *              tree was built.
 *
 *  \return CDataDefNode * - A cloned version of the node tree, if found in the
 *              cache, otherwise NULL.  Note that the caller is responsible for
 *              deleting the cloned object from memory.
 *
 *****************************************************************************/
CDataDefNode * CTagFileParser::RetrieveFile(const std::string & tagFilename)
{
    CDataDefNode * pCached = NULL;

    if( IsFileAvailable( const_cast<char *>(tagFilename.c_str()) ) )
    {
        std::string newTagFilename( tagFilename );

        // Determine if this file has already been parsed.  If so, return a copy of
        // the cached data def node tree.
        std::map<std::string, CDataDefNode *>::iterator cachedNodesIterator = cachedNodes.find(newTagFilename);
        if (cachedNodesIterator != cachedNodes.end())
        {
            pCached = cachedNodesIterator->second->Clone();
        }
    }

    return pCached;
}


//*****************************************************************************
/*!
 *  \brief  Cache a data node tree built from the specified filename.
 *
 *  \param  const string & tagFilename - The filename to be parsed into a node
 *              tree.
 *  \param  const string & definitionFilename - The tag definition file to be
 *              used in the tag file parsing.
 *
 *****************************************************************************/
void CTagFileParser::CacheTagFile(const std::string & tagFilename,
                                  const std::string & definitionFilename)
{
	// assert to see if the file actually exists
    assert( IsFileAvailable( tagFilename ) );

    // Remove the cached version if we're recaching.
    PurgeCacheOfTagFile(tagFilename);

    // Copy the tag definition filename so we can change it if needed.
    std::string tagDefinitionFile(definitionFilename);

    // Storage for the tokens produced by reading the tag file.
    TTokenList tokens;
    // Convert the file specified into tokens.
    CTokenizer::TokenizeFile(tokens, tagFilename);

    assert(!tokens.empty());

    // Check if the tag definition file was specified on the first line.  Note
    // that the presence of a #tagdef line in the config file obsoletes the
    // second parameter.
    if(tokens.front() == "#tagdef")
    {
        tokens.pop_front();
        tagDefinitionFile = tokens.front();
        tokens.pop_front();
    }

    if(tagDefinitionFile == "")
    {
        std::cerr << "Error: No tag definition file specified" << std::endl;
        assert (false);
    }

    // List of files that have previously been parsed.  Avoids recursion.
    TStringSet previouslyParsedFiles;

    // Storage for tags that can be parsed by this function call.
    TTagTypeMap knownTags;

    // Turn the definition file into a mapping of tagName -> tagType.
    RetrieveTagInfo(knownTags, tagDefinitionFile, previouslyParsedFiles);

    // If no definitions were created, the system will be halted..
    if (knownTags.empty())
    {
        std::cerr << "Error reading definition file: " << tagDefinitionFile << std::endl;
        assert (false);
    }

    // Create a root node for the new definition tree.
    CDataDefNode * pRootNode = new CDataDefNode("RootNode");

    // Create a local parser to decode the tokens.
    CTagTokenParser tokenParser(knownTags);

    // Parse the tokens according to the defs, attaching info to the root node.
    tokenParser.ParseTokens(pRootNode, tokens);

    // Cache the data def node tree by filename.
    cachedNodes[tagFilename] = pRootNode;
}


//*****************************************************************************
/*!
 *  \brief  Purge all cached data def node trees.
 *
 *****************************************************************************/
void CTagFileParser::PurgeCache()
{
    DeleteMapPointers( cachedNodes );
}


//*****************************************************************************
/*!
 *  \brief  Purge a particular file from the cached data.
 *
 *  \param  const string & tagFilename - The filename to be purged from the
 *          cached data.
 *
 *****************************************************************************/
void CTagFileParser::PurgeCacheOfTagFile(const std::string & tagFilename)
{
	// assert to see if the file actually exists
    assert( IsFileAvailable( tagFilename ) );

    // Remove the cached version if it exists.
    std::map<std::string, CDataDefNode *>::iterator cachedNodesIterator = cachedNodes.find(tagFilename);
    if (cachedNodesIterator != cachedNodes.end())
    {
        delete cachedNodesIterator->second;
        cachedNodes.erase(cachedNodesIterator);
    }
}


//*****************************************************************************
/*!
 *  \brief  Reads the tag definition file and creates the mapping from tag name
 *          to tag type.
 *
 *  \param  TTagTypeMap & knownTags - the mapping from tag name to tag type.
 *  \param  const string & definitionFilename - name of the file to read.
 *  \param  TStringSet & previouslyParsedFiles - the set of strings
 *          representing the files that have been previously read in.  This
 *          list is maintained to avoid recursion in the #include hierarchy.
 *
 *****************************************************************************/
void CTagFileParser::RetrieveTagInfo(TTagTypeMap & knownTags,
                                     const std::string & definitionFilename,
                                     TStringSet & previouslyParsedFiles)
{
	// assert to see if the file actually exists
    assert( IsFileAvailable( definitionFilename ) );

    // Storage for the tokenized file information.
    TTokenList definitionFileTokens;

    // Convert the definition file into tokens.
    CTokenizer::TokenizeFile(definitionFileTokens, definitionFilename);

    previouslyParsedFiles.insert(definitionFilename);

    // Convert the tokenized file into the tagName -> tagType mapping.
    ConvertTokensToTagInfo(knownTags, definitionFileTokens, previouslyParsedFiles);
}


//*****************************************************************************
/*!
 *  \brief  Convert the tokenized definition file into the mapping that
 *          represents the set of known tags.
 *
 *  \param  TTagTypeMap & knownTags - the destination mapping.
 *  \param  const TTokenList & definitionTokens - the tokens representing the
 *          definition file.
 *  \param  TStringSet & previouslyParsedFiles - the set of strings
 *          representing the files that have been previously read in.  This
 *          list is maintained to avoid recursion in the #include hierarchy.
 *
 *****************************************************************************/
void CTagFileParser::ConvertTokensToTagInfo(TTagTypeMap & knownTags,
                                            const TTokenList & definitionTokens,
                                            TStringSet & previouslyParsedFiles)
{
    // Iterate through the list of supplied tokens.
    for (TTokenListConstIter definitionIter = definitionTokens.begin(); definitionIter != definitionTokens.end(); )
    {
        // Retrieve the tag name from the iterator;
        std::string tagName = *definitionIter;
        // Move the iterator along to the tag type.
        definitionIter++;

        // Make sure the iterator is valid.
        if (definitionIter != definitionTokens.end())
        {
            // Retrieve the tag type information.
            std::string tagType = *definitionIter;
            // Move the iterator along to the next pair.
            definitionIter++;

            if (tagName == "#include")
            {
				// assert to see if the file actually exists
				assert( IsFileAvailable( tagType ) );

                if (previouslyParsedFiles.find(tagType) == previouslyParsedFiles.end())
                {
                    RetrieveTagInfo(knownTags, tagType, previouslyParsedFiles);
                }
            }
            else
            {
                // Store the tagName->tagType pairing.
                knownTags.insert(make_pair(tagName, tagType));
            }
        }
    }
}

//*****************************************************************************
/*!
 *  \brief  Tests for the existence of a file with the specified name.
 *
 *  \param  const string & filename - the name of the file to test.
 *
 *  \return const bool - true if the file could be found, false otherwise.
 *
 *****************************************************************************/
const bool CTagFileParser::IsFileAvailable(const std::string & filename)
{
    struct stat statBuf;

    return (stat( filename.c_str(), &statBuf ) == 0);
}


