#ifndef WARBL_MOCK_H_
#define WARBL_MOCK_H_
#include "warbl.h"

namespace WarblMock {
    extern void resetWarblMock();
    extern void setUpperThreshold(byte value);
    extern void setLowerThreshold(byte value);
    extern void enableOverblow(bool enable);
    extern void setHysteresisConfig(byte value);
}

#endif // WARBL_MOCK_H_
