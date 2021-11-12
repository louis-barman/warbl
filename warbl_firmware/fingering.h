
#ifndef FINGERING_H_
#define FINGERING_H_

#define WARBL_CFG_FINGERING_ID 118

#define MAX_WARBL_FINGER_HOLES 9
#define MAX_FINGER_POSITIONS 50

#define FINGER_DONT_CARE    0
#define FINGER_COVERED      1
#define FINGER_OPEN         2

#define FINGERING_PACKET_START      100
#define FINGERING_PACKET_PATTERN    101
#define FINGERING_PACKET_END        102

#define REGISTER_BOTH       0
#define REGISTER_HIGH_ONLY  1
#define REGISTER_LOW_ONLY   2

extern int customMidiNote(unsigned int fingerPattern);
extern void rxFingeringConfig(byte data);
extern bool isCustomFingeringEnabled();


#endif // FINGERING_H_
