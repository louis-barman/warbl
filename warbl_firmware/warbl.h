
#ifndef WARBL_H_
#define WARBL_H_


typedef unsigned char byte;

#define arraySize(arr) (sizeof(arr)/sizeof(arr[0]))

extern int lowerThreshold();
extern byte hysteresisConfig();
extern void sendUSBMIDI(byte m, byte c, byte d1, byte d2);

#endif // WARBL_H_
