#ifndef WARBL_H_
#define WARBL_H_

#include <stdint.h>


extern int lowerThreshold();
extern uint8_t hysteresisConfig();
extern void sendUSBMIDI(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2);
extern uint8_t newNote;

#endif // WARBL_H_
