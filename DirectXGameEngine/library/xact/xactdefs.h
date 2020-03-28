/************************************************************************
*    FILE NAME:       xactdefs.h
*
*    DESCRIPTION:     xact defines
************************************************************************/

#ifndef __xact_defs_h__
#define __xact_defs_h__

// DirectX lib dependencies
#include <xact3.h>

// Boost lib dependencies
#include <boost/scoped_array.hpp>

class CWaveBankFileHeader
{
public:
    char header[4];
    int version;
    int headerSize1;
    int headerSize2;
    int offsetDetailsDir;
    int lengthDetailsDir;
    int offsetFileNameDir;
    int lengthFileNameDir;
    int firstFileOffset;
    int unknown1;
    short int unknown2[6];
    short int streaming;
    short int unknown3;
    int noOfFiles;
    char fileName[16];
    int lengthDetailEntry;
    int lengthFilenameEntry;
    int paddingBetweenFiles;
    int nullValue;
};


class CWaveBank
{
public:

    CWaveBank() : pWaveBank(NULL),
                  pMemMapBuffer(NULL),
                  streamFileHandle(NULL)
    { }

    // pointer to wav bank
    IXACT3WaveBank * pWaveBank;

    // pointer to memory mapped buffer
    VOID * pMemMapBuffer;

    // handle to stream file
    HANDLE streamFileHandle;
};


class CSoundBank
{
public:

    CSoundBank() : pSoundBank(NULL)
    { }

    // Pointer to sound bank
    IXACT3SoundBank * pSoundBank;

    // Allocated sound bank buffer
    boost::scoped_array<unsigned char> spSoundBankBuffer;
};


#endif  // __xact_defs_h__
