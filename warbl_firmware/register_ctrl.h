
#ifndef REGISTER_CTRL_H_
#define REGISTER_CTRL_H_

#define SILENCE         1
#define BOTTOM_REGISTER 2
#define TOP_REGISTER    3

typedef enum {JUMP, DROP} jumpDrop_t;


extern byte registerControl(int pressure, int upperBound);

extern int getRegisterHoldoffTime(jumpDrop_t jumpDrop);
extern int calcHysteresis(int currentUpperBound, bool high);
extern int pressureRateChange(int pressure);

#ifdef UNIT_TESTS
extern void resetRegisterCtrl();
#endif

#endif // REGISTER_CTRL_H_
