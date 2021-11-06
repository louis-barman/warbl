#include "gtest/gtest.h"
#include "warbl.h"
#include "warbl_mock.h"
#include "register_ctrl.h"

namespace WarblMock {

static int lowerThresholdValue;
static int upperThresholdValue;
static byte hysteresisConfigValue;
static bool overblowEnabled;

void resetWarblMock(){
    lowerThresholdValue = 1;
    upperThresholdValue = 2;
    overblowEnabled = false;
    hysteresisConfigValue = 0;
}

void setLowerThreshold(byte value) {
    lowerThresholdValue = value;
}

void setUpperThreshold(byte value) { // ZZ delete me
    upperThresholdValue = value;
}

void enableOverblow(bool enable) {
    overblowEnabled = enable;
}

void setHysteresisConfig(byte value) {
    hysteresisConfigValue = value;
}

} // namespace end

byte hysteresisConfig() {
    return WarblMock::hysteresisConfigValue;
}

int lowerThreshold() {
    return WarblMock::lowerThresholdValue;
}

int upperThreshold() {
    return WarblMock::upperThresholdValue;
}

bool isOverblowEnabled() {
    //return (breathMode == kPressureBreath || (breathMode == kPressureThumb && modeSelector[mode] == kModeCustom && switches[mode][THUMB_AND_OVERBLOW]);
    return WarblMock::overblowEnabled;
}

int getRegisterHoldoffTime(jumpDrop_t jumpDrop) {
    return 1;
}



