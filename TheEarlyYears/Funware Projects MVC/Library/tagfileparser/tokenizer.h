//*****************************************************************************
/*!
 *  \file   tokenizer.h
 *
 *  \brief  This class is designed to break input into 'tokens'.  These tokens
 *          are delimited within the input by whitespace.  Contains
 *          functionality tokenize a file, or a string.
 *
 *****************************************************************************/
#ifndef _tokenizer_h_
#define _tokenizer_h_

#include <list>
#include <string>

typedef std::string TToken;
typedef std::list<TToken> TTokenList;
typedef TTokenList::iterator TTokenListIter;
typedef TTokenList::const_iterator TTokenListConstIter;

//*****************************************************************************
/*!
 *  \class  CTokenizer.
 *
 *  \brief  Break input into "tokens".  Tokens are delimited by whitespace.
 *
 *****************************************************************************/
class CTokenizer
{
public:
    // Break an entire file into tokens.  Optionally ignore ';' style comments.
    static void TokenizeFile(TTokenList & tokens,
                             const std::string & filename);

    // Break an input string into tokens based on the supplied delimiters.
    static void TokenizeString(TTokenList & tokens,
                               const char * pStringBuffer,
                               const char * pDelimiters);
};

#endif
