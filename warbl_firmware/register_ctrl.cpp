#include <limits.h>
#include "warbl.h"
#include "debug.h"
#include "register_ctrl.h"

#define SILENCE_HYSTERESIS 1


// These variables are private to this module/file.
static int currentState;
static bool holdoffActive = false;
static int holdoffCounter = 0;
static int rateChangeIdx = 0;
static int previousPressure = 0;
static int previousAverage1 = 0;
static int previousAverage2 = 0;


/**
 * @brief calcHysteresis the amount of hysteresis when entering or leaving the top register
 * @param currentUpperBound
 * @param high true for threshold when entering the top register otherwise false for the low threashold
 * @return the low or high UpperBound
 */
int calcHysteresis(int currentUpperBound, bool high)
{
    int hysteresisPercent = hysteresisConfig();
    if (hysteresisPercent == 0 ) {
        return currentUpperBound;
    }
    int range = currentUpperBound - lowerThreshold();
    int newUpperBound;
    if (high) {
        newUpperBound = currentUpperBound + (range *  hysteresisPercent )/400;
    } else {
        newUpperBound = currentUpperBound - (range *  hysteresisPercent * 3 )/400;
    }
    return newUpperBound;
}


void rateChangeReset() {
    rateChangeIdx = 0;
    previousPressure = 0;
    previousAverage1 = 0;
    previousAverage2 = 0;
}


/**
 * @brief pressureRateChange caculates how fast (the rate) pressures is changing
 *
 * The averaging and FIFO of averages are needed to remove some noise from the pressure readings
 *
 * @param pressure
 * @return the rate of pressure change or SHRT_MIN if the reading is not valid
 */
int pressureRateChange(int pressure) {
    int rateChange = SHRT_MIN;
    if (rateChangeIdx == 0) {
        rateChangeIdx = 1;
        previousPressure = pressure;
    } else {
        rateChangeIdx = 0;
        // no need to divide the average by the number of entres so we dont loose resolution
        int average = pressure + previousPressure;

        // check we have enough readings to be valid
        if (previousAverage2 > 0) {
            rateChange = average - previousAverage2;
        }

        // this works as a very fast FIFO (first in first out) with a length 3
        previousAverage2 = previousAverage1;
        previousAverage1 = average;

    }
    return rateChange;
}

/**
 * @brief delayStateChange delay the overblow state until either it has timedout or the pressure has leveled off.
 * @param jumpDrop either JUMP, DROP
 * @param pressure
 * @param upperBound
 * @return the current state to be used
 */
byte delayStateChange(jumpDrop_t jumpDrop, int pressure, int upperBound) {
    if (!holdoffActive) {
        holdoffActive = true;
        holdoffCounter = getRegisterHoldoffTime(jumpDrop);
        rateChangeReset();
    }
    if (holdoffCounter > 0) {
        holdoffCounter--;

        bool exitEarly = false;
        int rateChange = pressureRateChange(pressure);
        if (rateChange != SHRT_MIN) {
            if (jumpDrop == JUMP && rateChange <= 0) {
                exitEarly = true;
            } else if (jumpDrop == DROP && rateChange >= 0) {
                exitEarly = true;
            }
        }


        if (holdoffCounter == 0 || exitEarly) {
            holdoffActive = false;
            return BOTTOM_REGISTER;
        }
    }
    return currentState;
}


/**
 * @brief registerControl determine the overblowing register control
 * @param pressure the latest breath pressure reading
 * @param upperBound -1 if the upperBound is disabled otherwise the adjusted upperBound with the muliplier
 * @return the new overblowing state to be used
 */
byte registerControl(int pressure, int upperBound) {
    debugPressure(pressure);


    byte newState = currentState;
    if (pressure <= lowerThreshold()) {
        newState = SILENCE;
        holdoffActive = false;
        debugSetOnce(true);
    }
    else if (pressure > lowerThreshold() + SILENCE_HYSTERESIS) {
        // added very small amount of hysteresis for state 2, 4/25/20
        if (currentState == SILENCE) {
            newState = BOTTOM_REGISTER;
        }
        if (upperBound != -1 ) {
            int upperBoundHigh = calcHysteresis(upperBound, true);
            int upperBoundLow = calcHysteresis(upperBound, false);
            if (pressure > upperBoundHigh) {
                newState = TOP_REGISTER;
                // debugTrace(1, upperBoundHigh); //ZZ LB Debug
                holdoffActive = false;
            } else if (pressure <= upperBoundLow) {
                newState = BOTTOM_REGISTER;
                // debugTrace(2, upperBoundLow); //ZZ LB Debug
            }


            if (currentState == SILENCE && newState == BOTTOM_REGISTER) {
                newState = delayStateChange(JUMP, pressure, upperBoundHigh);
            } else if (currentState == TOP_REGISTER && newState == BOTTOM_REGISTER) {
                newState = delayStateChange(DROP, pressure, upperBoundLow);
            }

            // Set DEBUG_ENABLED to enable or disable these messages
            debugIntOnce(DEBUG_LOWER_THRESHOLD, lowerThreshold());
            debugIntOnce(DEBUG_UPPERBOUND, upperBound);
            debugIntOnce(DEBUG_UPPERBOUND_HIGH, upperBoundHigh);
            debugIntOnce(DEBUG_UPPERBOUND_LOW, upperBoundLow);
            debugSetOnce(false);

        }

    }
    currentState = newState;
    return newState;
}

#ifdef UNIT_TESTS
void resetRegisterCtrl(){
    currentState = SILENCE;
    rateChangeReset();
}
#endif

#if DEBUG_ENABLED
void debugPressure(int pressure) {
    static int previousPressure = 0;

    if (pressure > lowerThreshold()) {
        if (previousPressure!= pressure) {
            previousPressure = pressure;
            debugInt(DEBUG_PRESSURE, pressure);
        }
    }
}
#endif
