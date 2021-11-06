#include <limits.h>
#include "gtest/gtest.h"
#include "warbl.h"
#include "register_ctrl.h"
#include "warbl_mock.h"

namespace {

void resetTests() {
    resetRegisterCtrl();
    WarblMock::resetWarblMock();
    WarblMock::setLowerThreshold(199);
    WarblMock::setHysteresisConfig(0);
}

TEST(RegisterCtrl, OverblowDisabled) {
    resetTests();
    EXPECT_EQ(SILENCE, registerControl(0, -1));
    EXPECT_EQ(SILENCE, registerControl(199, -1));
    EXPECT_EQ(SILENCE, registerControl(200, -1)); // a small amout of hysteresis
    EXPECT_EQ(BOTTOM_REGISTER, registerControl(201, -1));
    EXPECT_EQ(BOTTOM_REGISTER, registerControl(1000, -1));
    EXPECT_EQ(BOTTOM_REGISTER, registerControl(200, -1)); // a small amout of hysteresis
    EXPECT_EQ(SILENCE, registerControl(199, -1));
}


TEST(RegisterCtrl, OverblowEnabled) {
    resetTests();
    WarblMock::enableOverblow(true);
    EXPECT_EQ(SILENCE, registerControl(199, 600));
    EXPECT_EQ(BOTTOM_REGISTER, registerControl(201, 600));
    EXPECT_EQ(BOTTOM_REGISTER, registerControl(600, 600));
    EXPECT_EQ(TOP_REGISTER, registerControl(601, 600));
}

/*
TEST(RegisterCtrl, JumpDelay) {
    resetTests();
    WarblMock::enableOverblow(true);
    EXPECT_EQ(SILENCE, registerControl(199, 600));
    EXPECT_EQ(BOTTOM_REGISTER, registerControl(202, 600));
    EXPECT_EQ(BOTTOM_REGISTER, registerControl(600, 600));
    EXPECT_EQ(TOP_REGISTER, registerControl(601, 600));
}
*/

TEST(RegisterCtrl, hysteresisIsDisabled) {
    resetTests();
    WarblMock::setHysteresisConfig(0);
    EXPECT_EQ(1000, calcHysteresis(1000, true));
    EXPECT_EQ(1000, calcHysteresis(1000, false));
}

TEST(RegisterCtrl, calcHysteresis) {
    resetTests();
    WarblMock::setHysteresisConfig(10);
    EXPECT_EQ(1020, calcHysteresis(1000, true));
    EXPECT_EQ(940, calcHysteresis(1000, false));
}

TEST(RegisterCtrl, pressureRateChangeisValid) {
    resetTests();
    EXPECT_EQ(SHRT_MIN, pressureRateChange(1));
    EXPECT_EQ(SHRT_MIN, pressureRateChange(1));
    EXPECT_EQ(SHRT_MIN, pressureRateChange(1));
    EXPECT_EQ(SHRT_MIN, pressureRateChange(1));
    EXPECT_EQ(SHRT_MIN, pressureRateChange(1));
    // we need 6 readings before the rate change is valid
    EXPECT_EQ(0, pressureRateChange(1));
}

TEST(RegisterCtrl, pressureRateChangePositive) {
    resetTests();
    pressureRateChange(1);
    pressureRateChange(2);
    pressureRateChange(3);
    pressureRateChange(4);
    pressureRateChange(5);
    // the rate should be (5+6)-(1+2) = 8
    EXPECT_EQ(8, pressureRateChange(6));
}

TEST(RegisterCtrl, pressureRateChangeNegative) {
    resetTests();
    pressureRateChange(20);
    pressureRateChange(18);
    pressureRateChange(16);
    pressureRateChange(14);
    pressureRateChange(12);
    // the rate should be (12+10)-(20+18) = −16
    EXPECT_EQ(-16, pressureRateChange(10));
}

}  // namespace
