#include <limits.h>
#include "warbl.h"
#include "debug.h"
#include "fingering.h"
typedef struct {
    byte midiNote;
    byte registerControl;
    byte pattern[(MAX_WARBL_FINGER_HOLES + 1)/2];
} fingerPattern_t;


static fingerPattern_t fingerings[MAX_FINGER_POSITIONS];
// These variables are private to this module/file.
static byte packetLen = 0;
static byte rxIndex = 0;
static byte numberOfPatterns = 0;
static byte packetId = 0;

bool isCustomFingeringEnabled() {
    return numberOfPatterns > 0;
}

// Return a MIDI note number (0-127) based on the current fingering.
// The analog readings of the 9 hole sensors are also stored in the tempToneholeRead variable for later use.
int customMidiNote(unsigned int fingerPattern)
{
    for (int i = 0; i < numberOfPatterns; i++) {
        fingerPattern_t * fingering = &fingerings[i];
        int fingerMask = 0b100000000; // start with the thumb hole
        bool match = true;
        for (int j = 0; j < MAX_WARBL_FINGER_HOLES; j++) {
            int k = j / 2;
            int holeValue = (j % 2 == 0)? fingering->pattern[k] & 0xf : fingering->pattern[k] >> 4;

            if (holeValue != FINGER_DONT_CARE) {
                bool holeCovered = ((fingerPattern & fingerMask) == 0)? false : true;
                if (!(holeCovered && holeValue ==  FINGER_COVERED || !holeCovered && holeValue >= FINGER_OPEN)) {
                    match = false;
                    break;
                }
            }

            fingerMask >>= 1;

        }
        if (match) {
            return fingering->midiNote;
        }
    }
    return 0;
}

void resetFingerings() {
    numberOfPatterns = 0;
    rxIndex = 0;
    packetId = 0;
    packetLen = 0;
    for (int i = 0; i < arraySize(fingerings); i++) {
        fingerPattern_t * fingering = &fingerings[i];
        fingering->midiNote = 0;
        fingering->registerControl = REGISTER_BOTH;
        for (int j = 0; j < arraySize(fingering->pattern); j++) {
            fingering->pattern[j] = FINGER_DONT_CARE;
        }
    }
}

void rxFingeringConfig(byte data) {
    if (packetLen == 0) {
        packetLen = data;
        rxIndex = 0;
    } else {
        if (rxIndex == 0) {
            packetId = data;
            if (packetId == FINGERING_PACKET_START) {
                resetFingerings();
            }
        } else if (rxIndex == 1) {
            fingerings[numberOfPatterns].midiNote = data;
        } else if (rxIndex == 2) {
            fingerings[numberOfPatterns].registerControl = data;
        } else {
            if (packetId == FINGERING_PACKET_PATTERN) {
                int k = (rxIndex - 3) / 2;
                if (rxIndex % 2 != 0) {
                    fingerings[numberOfPatterns].pattern[k] = data;
                } else {
                    fingerings[numberOfPatterns].pattern[k] |= data << 4;
                }
            }
        }
        rxIndex++;
        if (rxIndex == packetLen) {
            packetLen = 0;
            if (numberOfPatterns + 1 < MAX_FINGER_POSITIONS) {
                numberOfPatterns++;
            }
        }
    }
}


