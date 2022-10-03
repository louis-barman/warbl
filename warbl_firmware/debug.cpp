#include "warbl.h"
#include "debug.h"


#if DEBUG_ENABLED


#define DEBUG_MIDI_CHANNEL 8 // Send the midi debug data on this channel
#define KEY_PRESSURE 0xA0 // 160

static uint8_t previousLineInfo = -1;
static bool debugEnableOnce = false;

void debugValue(uint8_t key, uint8_t value) {
    sendUSBMIDI(KEY_PRESSURE, DEBUG_MIDI_CHANNEL, key & 0x7F , value & 0x7F );
}

void debugSetOnce(bool enable) {
    debugEnableOnce = enable;
}

void debugInt(uint8_t key, int value) {
    debugValue(key, value & 0x7F );
    debugValue(key + 1, (value>>7) & 0x7F );
}

void debugIntOnce(uint8_t key, int value) {
    if ( debugEnableOnce ) {
        debugInt(key, value);
    }
}

// Debug trace
void debugTrace(uint8_t lineInfo, uint8_t data) {
    if (previousLineInfo != lineInfo) {
        //sendUSBMIDI(CC, 8, 99, lineInfo); // send using Non-Registered Parameter Number (NRPN)
        debugValue(lineInfo, data);
        previousLineInfo = lineInfo;

    }
}

#endif
