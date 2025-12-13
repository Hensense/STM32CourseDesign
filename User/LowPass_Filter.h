#ifndef _LOWPASS_FILTER_H
#define _LOWPASS_FILTER_H

/**
 *  Low pass filter class
 */
class LowPassFilter
{
public:
    /**
     * @param Tf - Low pass filter time constant
     */
	LowPassFilter(){}
    LowPassFilter(float Tf);
    ~LowPassFilter(){}

    float operator()(float x);
    float Tf; //!< Low pass filter time constant

protected:
    unsigned long timestamp_prev;  //!< Last execution timestamp
    float y_prev; //!< filtered value in previous execution step 
};

#endif
