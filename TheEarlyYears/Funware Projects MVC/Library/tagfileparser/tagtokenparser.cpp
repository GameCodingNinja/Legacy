//*****************************************************************************
/*!
 *  \file   tagtokenparser.cpp
 *
 *  \brief  Designed to take a stream of tokens and convert it into the
 *          definition information it represents.
 *
 *****************************************************************************/

#include "tagtokenparser.h"

#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <cassert>

#include "datadefarraynode.h"
#include "datadefstringnode.h"
#include "datadefintegernode.h"
#include "datadeffloatnode.h"
#include "datadefdoublenode.h"

#include "minTools.h"

//*****************************************************************************
/*!
 *  \brief   Create the parser with the tags it knows how to parse.
 *
 *****************************************************************************/
CTagTokenParser::CTagTokenParser(const TTagTypeMap & knownTagDefs) :
    errorStatus(EPES_NO_ERROR),
    knownTags(knownTagDefs),
    pMasterRootNode(NULL)
{
}


//*****************************************************************************
/*!
 *  \brief  Traverse the tokens supplied creating a tree of data definition
 *          nodes which reflect the tokenized info.  This data definition node
 *          hierarchy can then be used to instantiate objects.
 *
 *  \param  CDataDefNode * const pRootNode - Node to which all others attach.
 *  \param  const TTokenList & tokens - Tokens to be parsed.
 *
 *****************************************************************************/
void CTagTokenParser::ParseTokens(CDataDefNode * const pRootNode,
                                  const TTokenList & tokens)
{
    assert(pRootNode != NULL);

    // Initialize the member iterators used during the parsing.
    tokenIter = tokens.begin();
    tokenEndIter = tokens.end();

    // Set the 'master' root node.
    pMasterRootNode = pRootNode;

    // Loop while there are more tokens to look at.
    while (tokenIter != tokenEndIter)
    {
        // Test the first token to see if it is a tag, if not throw an
        // exception.
        if (!IsATag(*tokenIter))
        {
			Abort(std::string("ParseTokens - File error - unexpected token: ") + *tokenIter);
        }

        // Read the info from this tag into the definition node tree.
        ReadTag(pRootNode);
    }
}


//*****************************************************************************
/*!
 *  \brief  This function reads through a list of tokens representing a tag,
 *          extracting the definition information.  The extracted information
 *          is attached to the data definition node supplied.
 *
 *  \param  CDataDefNode * const pParentNode - The parent node to which the
 *          definition information yielded by reading this tag will be
 *          attached.
 *  \param  bool required - a true value indicates that this tag is required
 *          and that if it isn't found, an error should be generated.  A value
 *          of false will allow this function to return an
 *          EPES_OPTIONAL_TYPE_NOT_FOUND result.
 *
 *****************************************************************************/
void CTagTokenParser::ReadTag(CDataDefNode * const pParentNode,
                              bool required)
{
    assert(pParentNode != NULL);

    // Make sure we haven't passed the end.
    if (tokenIter == tokenEndIter)
    {
		Abort("ReadTag - Passed-in Iterator == EndIterator");
    }

    // Retrieve the opening tag.
    const TToken & openTag = *tokenIter++;

    // Create the closing tag based on the open tag so that we know what to look for.
    const TToken closeTag = CreateCloseTag(openTag);

    // Extract the name of the current tag from the open tag.
    const std::string tagName = ExtractTagName(openTag);

    // Attempt to locate the type of the current tag.
    TTagTypeMap::const_iterator tagIter = knownTags.find(tagName);

    // Test to see if the current tag was known.
    if (tagIter == knownTags.end())
    {
        std::cerr << "WARNING: Unknown tag(" << *tokenIter << ")" << std::endl;
        SkipUnknownTag();
    }
    else
    {
        // Create some storage for the definition info pertaining to this tag.
        CDataDefNode * pNewNode = new CDataDefNode(tagName);

        // Attach the newly created node to the supplied parent.
        pParentNode->AddChild(pNewNode);

        // Read in the definition information stored within this tag's values.
        ReadTagValues(pNewNode, tagIter->second, required);

        // Make sure we haven't passed the end.
        if (tokenIter == tokenEndIter)
        {
			Abort(std::string("ReadTag - Iterator == EndIterator after ReadTagValues was called"));
        }

        // Test to make sure the close tag was found.
        if (*tokenIter != closeTag)
        {
			Abort(std::string("ReadTag - Looking for the close tag(") + closeTag + ") but found (" + *tokenIter + ")");
        }

        // Throw away the close tag, everything's okay.
        ++tokenIter;
    }
}


//*****************************************************************************
/*!
 *  \brief  Based on the supplied type this function will decode the token
 *          stream and add any resulting definition nodes to the parent node.
 *
 *  \param  CDataDefNode * const pParentNode - Node to attach def info to.
 *  \param  const string & typeString - The type string describes the values
 *          that should be expected when reading the tokens.  The
 *          representation  for different types can be found in the
 *          ExtractFirstTypeString function in this file.
 *  \param  bool required - a true value indicates that this tag is required
 *          and that if it isn't found, an error should be generated.  A value
 *          of false will allow this function to return an
 *          EPES_OPTIONAL_TYPE_NOT_FOUND result.
 *
 *****************************************************************************/
void CTagTokenParser::ReadTagValues(CDataDefNode * const pParentNode,
                                    const std::string & typeString,
                                    bool required)
{
    // Create a local copy of the type string to be manipulated.
    std::string localTypeString(typeString);

    // Looping while error free and more data types nead to be dealt with.
    while (!localTypeString.empty())
    {
        // Get the current type string from the full type string.
        std::string currentTypeString;
        EPType currentType = ExtractFirstTypeString(currentTypeString, localTypeString);

        switch (currentType)
        {

            case EPT_INTEGER:
            {
                ReadInt(pParentNode, required);
                break;
            }

            case EPT_STRING:
            {
                ReadString(pParentNode, required);
                break;
            }

            case EPT_OLD_STRING:
            {
                ReadOldString(pParentNode, required);
                break;
            }

            case EPT_DOUBLE:
            {
                ReadDouble(pParentNode, required);
                break;
            }

            case EPT_FLOAT:
            {
                ReadFloat(pParentNode, required);
                break;
            }

            case EPT_FILENAME:
            {
                ReadFilename(pParentNode, required);
                break;
            }

            case EPT_COMPOUND:
            {
                // Read the compound information into storage.
                ReadCompound(pParentNode, currentTypeString, required);
                break;
            }

            case EPT_ARRAY:
            {
                // Read the array information into storage.
                ReadArray(pParentNode, currentTypeString, required);
                break;
            }

            case EPT_OPTIONAL:
            {
                // Read the array information into storage.
                ReadTagValues(pParentNode, currentTypeString, false);
                break;
            }

            case EPT_ONE_OF:
            {
                // Read the array information into storage.
                ReadOneOf(pParentNode, currentTypeString, false);
                break;
            }

            case EPT_AT_LEAST_ONE_OF:
            {
                // Read the array information into storage.
                // We must find at least one value from this type.
                ReadOneOf(pParentNode, currentTypeString, required);
                break;
            }

            default:
            {
				Abort(std::string("ReadTagValues - Unknown type in type string \"") + typeString + "\"");
                break;
            }
        }

        // After reading one matching element, subsequent elements are required.
        required = true;
    }
}


//*****************************************************************************
/*!
 *  \brief  Decode the tokens comprising a tag that may contain other tags.  A
 *          compound may reference other primitive elements or, in turn,
 *          further compounds.
 *
 *  \param  CDataDefNode * const pParentNode - Node to attach def info to.
 *  \param  const string & compoundName - Name of the compound being parsed..
 *  \param  bool required - a true value indicates that this tag is required
 *          and that if it isn't found, an error should be generated.  A value
 *          of false will allow this function to return an
 *          EPES_OPTIONAL_TYPE_NOT_FOUND result.
 *
 *****************************************************************************/
void CTagTokenParser::ReadCompound(CDataDefNode * const pParentNode,
                                   const std::string & compoundName,
                                   bool required)
{
    // The expected token is the open tag corresponding to compoundName.
    const std::string expectedTag = "<" + compoundName + ">";

    // Test the first token to see if it's the one that was expected.
    if (*tokenIter == expectedTag)
    {
        // The expected open tag has been found, decode it.
        // Once we have found the open tag then the rest of the compound tag
        // must be found so we pass it 'true' for the required parameter.
        ReadTag(pParentNode, true);

        // Set the error status to NO ERROR
        errorStatus = EPES_NO_ERROR;
    }
    else
    {
        // Value was not found, determine if it was required..
        if (required)
        {
			// Required search failed, indicate that an error occured.
            Abort(std::string("Unexpected token (") + *tokenIter + ") when expecting(" + expectedTag + ")");
        }
        else
        {
            // Set the error status to TYPE NOT FOUND
            errorStatus = EPES_OPTIONAL_TYPE_NOT_FOUND;
        }
    }
}


//*****************************************************************************
/*!
 *  \brief  Read in an Array of the specified type.
 *
 *  \param  CDataDefNode * const pParentNode - Node to attach def info to.
 *  \param  const string & arrayType - Type of the array being read in.
 *  \param  bool required - a true value indicates that this tag is required
 *          and that if it isn't found, an error should be generated.  A value
 *          of false will allow this function to return an
 *          EPES_OPTIONAL_TYPE_NOT_FOUND result.
 *
 *****************************************************************************/
void CTagTokenParser::ReadArray(CDataDefNode * const pParentNode,
                                const std::string & arrayType,
                                bool required)
{
    // Create some storage for the definition info pertaining to this tag.
    CDataDefNode * pArrayNode = new CDataDefArrayNode();

    // Attach the newly created node to the supplied parent.
    pParentNode->SetData(pArrayNode);

    // Read one element first.
    ReadTagValues(pArrayNode, arrayType, required);

    // Continue to loop while there are no errors.
    while (errorStatus == EPES_NO_ERROR)
    {
        // In an array, only one element is required, the rest are optional.
        ReadTagValues(pArrayNode, arrayType, false);
    }

    // If we got here we only had a OPTIONAL TYPE NOT FOUND error
    // and therefor we can reset the error status to NO ERROR
    errorStatus = EPES_NO_ERROR;
}


//*****************************************************************************
/*!
 *  \brief
 *
 *  \param  CDataDefNode * const pParentNode - Node to attach def info to.
 *  \param  const string & sourceTypeString
 *  \param  bool required - a true value indicates that this tag is required
 *          and that if it isn't found, an error should be generated.  A value
 *          of false will allow this function to return an
 *          EPES_OPTIONAL_TYPE_NOT_FOUND result.
 *
 *****************************************************************************/
void CTagTokenParser::ReadOneOf(CDataDefNode * const pParentNode,
                                const std::string & sourceTypeString,
                                bool required)
{
    // Set the error status to NO ERROR
    errorStatus = EPES_NO_ERROR;

    // Used to indicate a successful search.  Defaults to the failure case.
    bool found = false;

    // Duplicate the supplied string so it can be manipulated safely.
    std::string localTypeString(sourceTypeString);

    // Search while there are tokens available and no errors
    while (!localTypeString.empty() && !found)
    {
        // Storage for the first type from the local typeString.
        std::string currentTypeString;

        // Get the first type string from the supplied type string.
        ExtractFirstTypeString(currentTypeString, localTypeString, false);

        // Attempt to find an item matching the current type string.
        ReadTagValues(pParentNode, currentTypeString, false);

        // Tes to see if the search was performed with no errors.
        if (errorStatus == EPES_NO_ERROR)
        {
            // No errors means the item has been found.
            found = true;
        }
    }

    // If we didn't find any of the tags we check if it was an optional field
    // or if we should abort the parsing at this point.
    if (!found)
    {
        // Value was not found, determine if it was required..
        if (required)
        {
			// Required search failed, indicate that an error occured.
            Abort(std::string("Did not find OneOf: ") + sourceTypeString);
        }
        else
        {
            // Set the error status to TYPE NOT FOUND
            errorStatus = EPES_OPTIONAL_TYPE_NOT_FOUND;
        }
    }
}


//*****************************************************************************
/*!
 *  \brief  Read the integer value primitive.
 *
 *  \param  CDataDefNode * const pParentNode - Node to attach def info to.
 *  \param  bool required - a true value indicates that this primitive is
 *          required and that if it isn't found, an error should be generated.
 *          A value of false will allow this function to return an
 *          EPES_OPTIONAL_TYPE_NOT_FOUND result.
 *
 *****************************************************************************/
void CTagTokenParser::ReadInt(CDataDefNode * const pParentNode,
                              bool required)
{
    // Retrieve the first token from those supplied.
    const TToken & token = *tokenIter;

    // Test to see if there are any "non int" characters.
    if (token.find_first_not_of("-0123456789") == std::string::npos)
    {
        // All characters were acceptable as part of an integer, process it.

        // Create a new definition node and add it to the tree.
        pParentNode->SetData(new CDataDefIntegerNode(atoi(token.c_str())));

        // Set the error status to NO ERROR
        errorStatus = EPES_NO_ERROR;

        // The first token supplied has been dealt with, throw it away.
        ++tokenIter;
    }
    else
    {
        // Invalid characters detected, determine error status.
        // Value was not found, determine if it was required..
        if (required)
        {
			// Required search failed, indicate that an error occured.
            Abort(std::string("ReadInt - Badly formed integer (") + token + ")");
        }
        else
        {
            // Set the error status to TYPE NOT FOUND
            errorStatus = EPES_OPTIONAL_TYPE_NOT_FOUND;
        }
    }
}


//*****************************************************************************
/*!
 *  \brief  Read the float value primitive.
 *
 *  \param  CDataDefNode * const pParentNode - Node to attach def info to.
 *  \param  bool required - a true value indicates that this primitive is
 *          required and that if it isn't found, an error should be generated.
 *          A value of false will allow this function to return an
 *          EPES_OPTIONAL_TYPE_NOT_FOUND result.
 *
 *****************************************************************************/
void CTagTokenParser::ReadFloat(CDataDefNode * const pParentNode,
                                bool required)
{
    // Retrieve the first token from those supplied.
    const TToken & token = *tokenIter;

    // Test to see if there are any "non int" characters.
    if (token.find_first_not_of("-.0123456789") == std::string::npos)
    {
        // All characters were acceptable as part of an integer, process it.

        // Create a new definition node and add it to the tree.
        pParentNode->SetData( new CDataDefFloatNode( static_cast<float>(atof(token.c_str())) ) );

        // Set the error status to NO ERROR
        errorStatus = EPES_NO_ERROR;

        // The first token supplied has been dealt with, throw it away.
        ++tokenIter;
    }
    else
    {
        // Invalid characters detected, determine error status.
        // Value was not found, determine if it was required..
        if (required)
        {
			// Required search failed, indicate that an error occured.
            Abort(std::string("ReadFloat - Badly formed float (") + token + ")");
        }
        else
        {
            // Set the error status to TYPE NOT FOUND
            errorStatus = EPES_OPTIONAL_TYPE_NOT_FOUND;
        }
    }
}


//*****************************************************************************
/*!
 *  \brief  Read the integer value primitive.
 *
 *  \param  CDataDefNode * const pParentNode - Node to attach def info to.
 *  \param  bool required - a true value indicates that this primitive is
 *          required and that if it isn't found, an error should be generated.
 *          A value of false will allow this function to return an
 *          EPES_OPTIONAL_TYPE_NOT_FOUND result.
 *
 *****************************************************************************/
void CTagTokenParser::ReadDouble(CDataDefNode * const pParentNode,
                                 bool required)
{
    // Retrieve the first token from those supplied.
    const TToken & token = *tokenIter;

    // Test to see if there are any "non int" characters.
    if (token.find_first_not_of("-.0123456789") == std::string::npos)
    {
        // All characters were acceptable as part of an integer, process it.

        // Create a new definition node and add it to the tree.
        pParentNode->SetData(new CDataDefDoubleNode(atof(token.c_str())));

        // Set the error status to NO ERROR
        errorStatus = EPES_NO_ERROR;

        // The first token supplied has been dealt with, throw it away.
        ++tokenIter;
    }
    else
    {
        // Invalid characters detected, determine error status.
        // Value was not found, determine if it was required..
        if (required)
        {
			// Required search failed, indicate that an error occured.
            Abort(std::string("ReadInt - Badly formed integer (") + token + ")");
        }
        else
        {
            // Set the error status to TYPE NOT FOUND
            errorStatus = EPES_OPTIONAL_TYPE_NOT_FOUND;
        }
    }
}


//*****************************************************************************
/*!
 *  \brief  Read the string value primitive.  A string must be delimited, on
 *          both ends, by the " character.
 *
 *  \param  CDataDefNode * const pParentNode - Node to attach def info to.
 *  \param  bool required - a true value indicates that this primitive is
 *          required and that if it isnt found, an error should be generated.
 *          A value of false will allow this function to return an
 *          EPES_OPTIONAL_TYPE_NOT_FOUND result.
 *
 *****************************************************************************/
void CTagTokenParser::ReadString(CDataDefNode * const pParentNode,
                                 bool required)
{
    // Retrieve the first token from those supplied.
    TToken token = *tokenIter;

    // Check to make sure that the conditions for starting a "string" are met.
    if (token[0] == '\"')
    {
        // Get rid of the opening " character.
        token.erase(0, 1);

        // Storage used to reconstruct the full string.
        std::string data;
        // Keep looping while there are more tokens and the closing " has not been found.
        while (tokenIter != tokenEndIter && token[token.length()-1] != '\"')
        {
            // This is part of the string, store it.
            data += token;
            // The token has been dealth with, throw it away.
            ++tokenIter;
            // Prepend a space to the next token, enforcing spaces between each "word".
            token = " " + *tokenIter;
        }

        // If the token storage has been emptied then there is an error conditions
        if (tokenIter != tokenEndIter)
        {
            // Get rid of the closing " character.
            token.erase(token.length()-1, 1);
            // Append the final token to the reconstructed string.
            data += token;
            // The token has been dealt with, throw it away.
            tokenIter++;
            // Create a new string definition node and insert it into the tree.
            pParentNode->SetData(new CDataDefStringNode(data));

            // Set the error status to NO ERROR
            errorStatus = EPES_NO_ERROR;
        }
        else
        {
			// Required search failed, indicate that an error occured.
            Abort(std::string("ReadString - Closing \" not found in string (") + token + ")");
        }
    }
    else
    {
        // Invalid delimiting characters detected, determine error status.
        // Value was not found, determine if it was required..
        if (required)
        {
			// Required search failed, indicate that an error occured.
            Abort(std::string("ReadString - Opening \" not found in string (") + token + ")");
        }
        else
        {
            // Set the error status to TYPE NOT FOUND
            errorStatus = EPES_OPTIONAL_TYPE_NOT_FOUND;
        }
    }
}


//*****************************************************************************
/*!
 *  \brief  Read the 'old style' string value primitive.  An 'old style' string
 *          consists of any characters except the tag delimiters '<' and '>'.
 *
 *  \param  CDataDefNode * const pParentNode - Node to attach def info to.
 *  \param  bool required - a true value indicates that this primitive is
 *          required and that if it isn't found, an error should be generated.
 *          A value of false will allow this function to return an
 *          EPES_OPTIONAL_TYPE_NOT_FOUND result.
 *
 *  \return EPErrorStatus - indicates the status of the processing attempt.
 *
 *****************************************************************************/
void CTagTokenParser::ReadOldString(CDataDefNode * const pParentNode,
                                    bool required)
{
    // Retrieve the first token from those supplied.
    const TToken & token = *tokenIter;

    if (token.find_first_of("<>") == std::string::npos)
    {
        pParentNode->SetData(new CDataDefStringNode(token));

        // Delete the converted token.
        ++tokenIter;

        // Set the error status to NO ERROR
        errorStatus = EPES_NO_ERROR;
    }
    else
    {
        // Value was not found, determine if it was required..
        if (required)
        {
			// Required search failed, indicate that an error occured.
            Abort(std::string("ReadOldString - Closing \" not found in string (") + token + ")");
        }
        else
        {
            // Set the error status to TYPE NOT FOUND
            errorStatus = EPES_OPTIONAL_TYPE_NOT_FOUND;
        }
    }
}


//*****************************************************************************
/*!
 *  \brief  Read in a Filename from the configuration file.  The format of the
 *          Filename primitive is the same as that of the 'old string'
 *          primitive.  The Filename will be used to test for the existence of
 *          the specified file.
 *
 *  \param  CDataDefNode * const pParentNode - Node to attach def info to.
 *  \param  bool required - a true value indicates that this primitive is
 *          required and that if it isn't found, an error should be generated.
 *          A value of false will allow this function to return an
 *          EPES_OPTIONAL_TYPE_NOT_FOUND result.
 *
 *****************************************************************************/
void CTagTokenParser::ReadFilename(CDataDefNode * const pParentNode,
                                   bool required)
{
    // Retrieve the first token from those supplied.
    const TToken & token = *tokenIter;

    if (token.find_first_of("<>") == std::string::npos)
    {
        // Store the filename as a string node.
        pParentNode->SetData(new CDataDefStringNode(token));

        // Delete the converted token.
        ++tokenIter;

        // The Filename was correctly decoded, test for its existence.
        if (!IsFileAvailable(token))
        {
			Abort(std::string("ReadFilename - File (") + token + ") does not exist");
        }

        // Set the error status to NO ERROR
        errorStatus = EPES_NO_ERROR;
    }
    else
    {
        // Value was not found, determine if it was required..
        if (required)
        {
			// Required search failed, indicate that an error occured.
            Abort(std::string("ReadFilename - Badly formed Filename string (") + token + ")");
        }
        else
        {
            // Set the error status to TYPE NOT FOUND
            errorStatus = EPES_OPTIONAL_TYPE_NOT_FOUND;
        }
    }
}


//*****************************************************************************
/*!
 *  \brief  Skip over an unknown tag.  Enforces the condition that the tag be
 *          well formed.  This allows older parsing routines to ignore features
 *          that may be added in the future.
 *
 *****************************************************************************/
void CTagTokenParser::SkipUnknownTag()
{
    // Retrieve the first token from those supplied.
    const TToken openTag = *tokenIter++;

    // Create a closing tag based on the open tag.
    const TToken closeTag = CreateCloseTag(openTag);

    // Loop while there's more tokens, the close tag hasn't been found and no errors occur.
    while ((tokenIter != tokenEndIter) && (*tokenIter != closeTag))
    {
        // Test the next token to see if it is a tag.
        if (IsATag(*tokenIter))
        {
            // Recursively skip the tag (allows for nesting).
            SkipUnknownTag();
        }
        else
        {
            // Otherwise, it's data, skip it.
            ++tokenIter;
        }
    }

    // After skipping an unknown tag, the front token should be the close tag.
    if (*tokenIter != closeTag)
    {
		Abort(std::string("SkipUnknownTag - Close tag (") + closeTag + ")not found");
    }

    // Close tag found, skip it.
    ++tokenIter;
}


//*****************************************************************************
/*!
 *  \brief  Extract the name of the tag from the string that represents the
 *          tag.
 *
 *  \param  const TToken & tag - the tag to be converted.
 *
 *  \return const string - The extracted tag
 *
 *****************************************************************************/
const std::string CTagTokenParser::ExtractTagName(const TToken & tag)
{
    // Can only extract a tag name from a valid tag.
    if (!IsATag(tag))
    {
		Abort(std::string("ExtractTagName - ") + tag + " is not a tag");
    }

    // Ignore the first character and the last character.
    return (tag.substr(1, tag.length() - 2));
}


//*****************************************************************************
/*!
 *  \brief  Create a close tag based on the supplied open tag.
 *
 *  \param  const TToken & openTag - the open tag used to create the close tag.
 *
 *  \return const TToken - The newly created closeTag
 *
 *****************************************************************************/
const TToken CTagTokenParser::CreateCloseTag(const TToken & openTag)
{
    // Can only create a close tag from a valid tag.
    if (!IsATag(openTag))
    {
		Abort(std::string("CreateCloseTag - ") + openTag + " is not a tag");
    }

    TToken closeTag(openTag);

    closeTag.insert(1,"/");

    return (closeTag);
}


//*****************************************************************************
/*!
 *  \brief  Break the supplied source type string into two parts.  The first
 *          part will contain the first complete type string found within the
 *          source string.  The second part will contain any remaining type
 *          string characters that were not part of the first complete type
 *          string.
 *          This function modifies both supplied parameters.
 *          The different types that can be understood are:
 *          i - Integer.    Any character of the set: -0123456789.
 *          s - String.     Any string of characters surrounded by quotes (").
 *          S - Old string. Any string of characters not containing <>.
 *          F - Filename.  An old string entry that represents a file name.
 *          [X] - Compound. Another tagged entry in the file.
 *          (X) - Array.    A list of things of like type.
 *          <X> - Optional. An entry in the tag file that is not required.
 *
 *          Also of note is the use of the '|' operator between types in the
 *          definition file.  This operator will specify that the tag found
 *          should be one of the two types surrounding the operator.
 *          In addition, the '||' operator between types in the definition
 *          file will specify that the tag found should be one of the two types
 *          surrounding the operator and that at least one of the types
 *          must be present.
 *
 *  \param  string & extractedTypeString - The destination string.  After the
 *          completion of this function call, this variable will contain the
 *          first complete type string found in the supplied source string.
 *  \param  string & sourceTypeString - The source string.  This string will be
 *          searched to find the first complete type string it contains.  That
 *          type string will be stored in extractedTypeString and removed from
 *          this source string.
 *
 *  \return EPType - The type of the first type string found within the source
 *          string supplied.  EPT_UNKNOWN_TYPE indicates problems.
 *
 *****************************************************************************/
EPType CTagTokenParser::ExtractFirstTypeString(std::string & extractedTypeString,
                                               std::string & sourceTypeString,
                                               bool allowDelimiterStrip)
{
    // Assume that no delimiters will require erasing.
    bool stripDelimiters = false;
    // Default to the error case, make successful processing correct this.
    EPType extractType = EPT_UNKNOWN_TYPE;
    // As a default, assume nothing will be found.
    unsigned int extractTypeLength = 0;
    // Use the first character to begin determining type.
    switch (sourceTypeString[0])
    {
        case 'i':
        {
            // An integer has been found.
            extractType = EPT_INTEGER;
            extractTypeLength = 1;
            break;
        }

        case 's':
        {
            // A string has been found.
            extractType = EPT_STRING;
            extractTypeLength = 1;
            break;
        }

        case 'S':
        {
            // An old style string has been found.
            extractType = EPT_OLD_STRING;
            extractTypeLength = 1;
            break;
        }

        case 'd':
        {
            // A double has been found.
            extractType = EPT_DOUBLE;
            extractTypeLength = 1;
            break;
        }
        
        case 'f':
        {
            // A float has been found.
            extractType = EPT_FLOAT;
            extractTypeLength = 1;
            break;
        }

        case 'F':
        {
            // A filename has been found..
            extractType = EPT_FILENAME;
            extractTypeLength = 1;
            break;
        }

        case '[':
        {
            extractType = EPT_COMPOUND;
            extractTypeLength = CalculateDelimitedLength(sourceTypeString, "]");
            stripDelimiters = true;
            break;
        }

        case '(':
        {
            extractType = EPT_ARRAY;
            extractTypeLength = CalculateDelimitedLength(sourceTypeString, ")");
            stripDelimiters = true;
            break;
        }

        case '<':
        {
            extractType = EPT_OPTIONAL;
            extractTypeLength = CalculateDelimitedLength(sourceTypeString, ">");
            stripDelimiters = true;
            break;
        }

        default:
        {
            // Indicate to the client that this was an unknown type.
            extractType = EPT_UNKNOWN_TYPE;
            break;
        }
    }

    if (extractTypeLength > 0)
    {
        // Retrieve the compound type string with it's delimiting characters.
        extractedTypeString = sourceTypeString.substr(0, extractTypeLength);
        // Indicate that the type extraction succeeded.
        sourceTypeString.erase(0, extractTypeLength);
        // If the next character is the '|' then this is a grouped series.
        if (!sourceTypeString.empty() && sourceTypeString[0] == '|')
        {
            // Delete the leading '|' character.
            sourceTypeString.erase(0,1);

	        // If the next character is also a "|'
	        if (sourceTypeString[0] == '|')
	        {
		        // Delete the leading '|' character.
		        sourceTypeString.erase(0,1);

		        // Indicate that this type represents a group, from which at least one is required.
		        extractType = EPT_AT_LEAST_ONE_OF;
	        }
	        else
	        {
	            // Indicate that this type represents a group, from which one is required.
	            extractType = EPT_ONE_OF;
	        }
            // Local Storage for the remaining entries to be retrieved from this group.
            std::string remainingTypeString;
            // Get the first complete type, do not strip the delimiters.  The type
            // characters need to remain in the string for future processing.
            if (ExtractFirstTypeString(remainingTypeString, sourceTypeString, false) != EPT_UNKNOWN_TYPE)
            {
                // Add the remaining elements to the first element.
                extractedTypeString += remainingTypeString;
            }
            else
            {
                // Propagate the unknown type value, to let the client know.
                extractType = EPT_UNKNOWN_TYPE;
            }

            // The extracted type is now a group, does not make sense to strip delimiters.
            stripDelimiters = false;
        }

        // Determine whether any delimiting characters need to be erased.
        if (allowDelimiterStrip && stripDelimiters)
        {
            if (extractedTypeString.length() > 2)
            {
                // Strip the first character from the extracted string.
                extractedTypeString.erase(0, 1);
                // Strip the last character from the extracted string.
                extractedTypeString.erase(extractedTypeString.length() - 1, 1);
            }
            else
            {
                std::cerr << "ERROR: Invalid length to extract delimiters.  Extracted:  " << extractedTypeString << " ";
                std::cerr << "Source: " << sourceTypeString << std::endl;
            }
        }
    }
    else
    {
        std::cerr << "ERROR: Invalid type string: " << sourceTypeString << std::endl;
    }

    return extractType;
}


//*****************************************************************************
/*!
 *  \brief  Searches the source string for occurences of the characters in
 *          the supplied delimiters.  Generates warning messages if the
 *          delimiter characters can't be found.  Uses the occurence of the
 *          delimiters to determine the "Delimited Length", and returns that.
 *          Will return a length of zero if no delimiters were found.
 *
 *  \param  const string & searchString - the string to be searched.
 *  \param  const string & delimiters - the delimiters to search for.
 *
 *  \return unsigned int - the length of the delimited string, zero if no delimiters
 *          were found.
 *
 *****************************************************************************/
unsigned int CTagTokenParser::CalculateDelimitedLength(const std::string & searchString,
                                                       const std::string & delimiters)

{
    // Default the return value (length) to zero, in case nothing is found.
    unsigned int currentTypeLength = 0;

    // Search for the closing delimiter in the type string.
    unsigned int delimiterIndex = searchString.find_first_of(delimiters);

    // Test if the closing delimiter was found.
    if (delimiterIndex != std::string::npos)
    {
        // Example is (xxx).  Bracket at position 4, type string length of 5.
        currentTypeLength = delimiterIndex + 1;
    }
    else
    {
        std::cerr << "Delimiter (One of " << delimiters << ") not found within string \"" << searchString << "\"" << std::endl;
    }

    return (currentTypeLength);
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
const bool CTagTokenParser::IsFileAvailable(const std::string & filename)
{
    struct stat statBuf;

    return (stat( filename.c_str(), &statBuf ) == 0);
}


//*****************************************************************************
/*!
 *  \brief  Abort the parsing and halt execution with a printed out reason.
 *
 *  \param  const string & reason - The reason for aborting
 *
 *****************************************************************************/
void CTagTokenParser::Abort(const std::string & reason) const
{
    PostMsg( "error", (char *)reason.c_str() );
	
	std::cerr << std::string(80, '-') << std::endl << std::endl;

    pMasterRootNode->Dump();

    std::cerr << std::endl << "ERROR in CTagTokenParser::" << reason << std::endl;

    std::cerr << std::endl << std::string(80, '-') << std::endl;

    assert(false);
}

