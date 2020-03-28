
/************************************************************************
*    FILE NAME:       progressbar.h
*
*    DESCRIPTION:     Class data
************************************************************************/

#ifndef __progress_bar_h__
#define __progress_bar_h__

// Boost lib dependencies
#include <boost/serialization/nvp.hpp>

class CProgressBar
{
public:

    CProgressBar() : currentValue(0), maxValue(0)
    {}

    CProgressBar(float curVal, float maxVal) : currentValue(curVal), maxValue(maxVal)
    {}

    // Serialize the point
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar 
            & BOOST_SERIALIZATION_NVP(currentValue)
            & BOOST_SERIALIZATION_NVP(maxValue)
            ;

    }	// serialize

    // set/get functions
    void SetCurrentValue( float value ) {currentValue = value;}
    float GetCurrentValue() const {return currentValue;}

    void SetMaxValue( float value ) {maxValue = value;}
    float GetMaxValue() const {return maxValue;}

private:

    // current value of progress bar
    float currentValue;

    // Max value of progress bar
    float maxValue;

};

#endif  // __progress_bar_h__


