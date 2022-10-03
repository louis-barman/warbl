#ifndef WARBL_MOCK_H_
#define WARBL_MOCK_H_
#include "warbl.h"

namespace WarblMock {
    extern void resetWarblMock();
    extern void setUpperThreshold(uint8_t value);
    extern void setLowerThreshold(uint8_t value);
    extern void enableOverblow(bool enable);
    extern void setHysteresisConfig(uint8_t value);
}

#endif // WARBL_MOCK_H_
