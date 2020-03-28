//*****************************************************************************
/*!
 *  \file   tagtokenparser.h
 *
 *  \brief  Designed to take a stream of tokens and convert it into the
 *          definition information it represents.
 *
 *****************************************************************************/
#ifndef _tokenparser_h_
#define _tokenparser_h_

#include <map>
#include <string>
#include "tokenizer.h"

// Forward declaration of used classes.
class CDataDefNode;


typedef std::map<std::string, std::string> TTagTypeMap;

/*! \enum   EPErrorStatus
 *
 *  \brief  The error status of the attempted parsing operation.
 */
enum EPErrorStatus
{
    EPES_NO_ERROR,                  //!< No errors.
    EPES_UNKNOWN_TAG,               //!< Encountered an unknown tag.
    EPES_UNEXPECTED_TOKEN,          //!< Expected a tag, didn't find one.
    EPES_BADLY_FORMED_VALUE,        //!< String missing quote, non numeric int.
    EPES_BADLY_FORMED_DEFINITION,   //!< The type in the definition is unknown.
    EPES_CLOSE_TAG_NOT_FOUND,       //!< Expected a close tag, didn't find one.
    EPES_OPTIONAL_TYPE_NOT_FOUND,   //!< Didn't find an optional tag.
};

/*! \enum   EPType
 *
 *  \brief  The type of value produced by parsing the token(s)..
 */
enum EPType
{
    EPT_INTEGER,        //!< Valid characters: -0123456789
    EPT_STRING,         //!< Anything surrounded by " characters.
    EPT_OLD_STRING,     //!< Anything except the <> characters.
    EPT_DOUBLE,         //!< Valid characters: -0123456789.
    EPT_FLOAT,          //!< Valid characters: -0123456789.
    EPT_FILENAME,       //!< String representation.  Can be existence tested.
    EPT_COMPOUND,       //!< Tag constructed from other tags.
    EPT_ARRAY,          //!< An array of nodes.
    EPT_OPTIONAL,       //!< The current type is NOT required.
    EPT_ONE_OF,         //!< Used to implement OR handling.
    EPT_AT_LEAST_ONE_OF,//!< Used to implement "at least one of" OR handling.
    EPT_UNKNOWN_TYPE,   //!< The type string contained unknown elements.
};

//*****************************************************************************
/*!
 *  \class
 *
 *  \brief
 *
 *****************************************************************************/
class CTagTokenParser
{
public:

    // Constructor.
    CTagTokenParser(const TTagTypeMap & knownTagDefs);

    // Convert tokens into CDataDefNodes and attach them to the root node.
    void ParseTokens(CDataDefNode * const pRootNode,
                     const TTokenList & tokens);

private:
    // Convert tokens forming a tag into data and attach to the parent node.
    void ReadTag(CDataDefNode * const pParentNode,
                 bool required = true);

    // Convert a tag's value tokens into data and attach to the parent node.
    void ReadTagValues(CDataDefNode * const pParentNode,
                       const std::string & typeString,
                       bool required = true);

    // Convert a compound's tokens into data and attach to the parent node.
    void ReadCompound(CDataDefNode * const pParentNode,
                      const std::string & compoundName,
                      bool required = true);

    // Convert tokens forming an array into data and attach to the parent node.
    void ReadArray(CDataDefNode * const pParentNode,
                   const std::string & arrayType,
                   bool required = true);

    // Read until one of the supplied types is matched.
    void ReadOneOf(CDataDefNode * const pParentNode,
                   const std::string & sourceTypeString,
                   bool required = true);

    // Convert an int's tokens into data and attach to the parent node.
    void ReadInt(CDataDefNode * const pParentNode,
                 bool required = true);

    // Convert a float's tokens into data and attach to the parent node.
    void ReadFloat(CDataDefNode * const pParentNode,
                   bool required = true);

    // Convert a double's tokens into data and attach to the parent node.
    void ReadDouble(CDataDefNode * const pParentNode,
                    bool required = true);

    // Convert a string's tokens into data and attach to the parent node.
    void ReadString(CDataDefNode * const pParentNode,
                    bool required = true);

    // Convert an old string's tokens into data and attach to the parent node.
    void ReadOldString(CDataDefNode * const pParentNode,
                       bool required = true);

    void ReadFilename(CDataDefNode * const pParentNode, bool required);

    // Skip over any tokens that are part of a well formed, unknown tag.
    void SkipUnknownTag();

    // Return true if the token supplied is a valid tag.
    inline bool IsATag(const TToken & token) const;

    // Extract the name of a tag from the token containing it.
    const std::string ExtractTagName(const TToken & tag);

    // Create a close tag, based on the supplied open tag.
    const TToken CreateCloseTag(const TToken & openTag);

    // Extract the first full type string from the source supplied.
    // Manipulates the values of both supplied strings.
    EPType ExtractFirstTypeString(std::string & extractedTypeString,
                                  std::string & sourceTypeString,
                                  bool allowDelimiterStrip = true);

    // Calculate a substring length (delimited by the characters supplied).
    unsigned int CalculateDelimitedLength(const std::string & searchString,
                                          const std::string & delimiters);

    // Tests for the existence of a file with the specified name
    const bool IsFileAvailable(const std::string & filename);

    // Abort the parsing and halt execution with a reason.
    void Abort(const std::string & reason) const;

    // Two iterators into the token list supplied to the constructor.
    // These iterators are set to list.begin and list.end respectively.
    TTokenListConstIter tokenIter;
    TTokenListConstIter tokenEndIter;

    // Global error status flag for all parsing operations.
    EPErrorStatus errorStatus;

    // A reference to the map holding all tag definitions.
    // Member reference for speed.
    const TTagTypeMap & knownTags;

    // The "main" parent for the parsing. Used in Abort();
    CDataDefNode * pMasterRootNode;
};


//*****************************************************************************
/*!
 *  \brief  Tests to determine whether the token supplied is a well formed tag.
 *
 *  \param  const TToken & token - the token to test.
 *
 *  \return bool - true if the token is a tag, false otherwise.
 *
 *****************************************************************************/
bool CTagTokenParser::IsATag(const TToken & token) const
{
    unsigned int tokenLength = token.length();

    // Must be of the form <X> to be a tag.  X is one or more characters.
    return ((tokenLength > 2) && (token[0] == '<') && (token[tokenLength - 1] == '>'));
}

#endif

