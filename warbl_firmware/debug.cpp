#include "warbl.h"
#include "debug.h"


#if DEBUG_ENABLED


#define DEBUG_MIDI_CHANNEL 8 // Send the midi debug data on this channel
#define KEY_PRESSURE 0xA0 // 160

static byte previousLineInfo = -1;
static bool debugEnableOnce = false;

void debugValue(byte key, byte value)
{
    sendUSBMIDI(KEY_PRESSURE, DEBUG_MIDI_CHANNEL, key & 0x7F , value & 0x7F );
}

void debugSetOnce(bool enable) {
    debugEnableOnce = enable;
}

void debugInt(byte key, int value)
{
    debugValue(key, value & 0x7F );
    debugValue(key + 1, (value>>7) & 0x7F );
}

void debugIntOnce(byte key, int value) {
    if ( debugEnableOnce ) {
        debugInt(key, value);
    }
}

// Debug trace
void debugTrace(byte lineInfo, byte data)
{

    if (previousLineInfo != lineInfo) {
        //sendUSBMIDI(CC, 8, 99, lineInfo); // send using Non-Registered Parameter Number (NRPN)
        debugValue(lineInfo, data);
        previousLineInfo = lineInfo;

    }
}

#endif
