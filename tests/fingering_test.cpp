#include <string>
using namespace std;

#include "gtest/gtest.h"
#include "warbl.h"
#include "fingering.h"
#include "warbl_mock.h"


namespace {

void resetTests() {
    WarblMock::resetWarblMock();
    // send the two byte fingering start packet
    rxFingeringConfig(2);
    rxFingeringConfig(FINGERING_PACKET_START);
}

int internalNoteNameToMidi (const string noteName) {
    int midiNote = 0;

    for(const char& c : noteName) {
        if (c == ' ') {
            continue;
        }
        if (c >= 'A' && c <= 'G') {
            switch(c) {
            case 'C':
                midiNote = 12;
                break;
            case 'D':
                midiNote = 14;
                break;
            case 'E':
                midiNote = 16;
                break;
            case 'F':
                midiNote = 17;
                break;
            case 'G':
                midiNote = 19;
                break;
            case 'A':
                midiNote = 21;
                break;
            case 'B':
                midiNote = 23;
                break;
             }
        }
        if (c == '#') {
            midiNote++;
        }
        if (c == 'b') {
            midiNote--;
        }
        if (c >= '0' && c <= '9') {
            int octive = c - '0';
            midiNote += octive * 12;
        }

    }
    return midiNote;

}

int findPacketLength(const string fingers) {
    int len = 0;
    for(const char& c : fingers) {
        if (c == ' ') {
            continue;
        }
        len++;
    }
    return len;

}

void testFingering(const string fingering) {

    const size_t sep = fingering.find(":");
    if (sep != string::npos && sep > 0) {
        const string fingers = fingering.substr(sep + 1 );
        int packetLen = findPacketLength(fingers);
        rxFingeringConfig(packetLen + 2);
        rxFingeringConfig(FINGERING_PACKET_PATTERN);
        const int midiNote = internalNoteNameToMidi(fingering.substr(0, sep));
        rxFingeringConfig(midiNote);
        rxFingeringConfig(REGISTER_BOTH);

        for(const char& c : fingers) {
            switch (c) {
            case 'x':
                rxFingeringConfig(FINGER_COVERED);
                break;
            case 'o':
                rxFingeringConfig(FINGER_OPEN);
                break;
            case '-':
                rxFingeringConfig(FINGER_DONT_CARE);
                break;
            case ' ':
                break;

            }
        }

    }

}

TEST(Fingering, internalNoteNameToMidi) {
    EXPECT_EQ(46, internalNoteNameToMidi("A#2"));
    EXPECT_EQ(46, internalNoteNameToMidi("Bb2"));
    EXPECT_EQ(47, internalNoteNameToMidi("B2"));
    EXPECT_EQ(59, internalNoteNameToMidi("B3"));
    EXPECT_EQ(60, internalNoteNameToMidi("C4")); // Middle c
    EXPECT_EQ(61, internalNoteNameToMidi("C#4"));
    EXPECT_EQ(62, internalNoteNameToMidi("D4"));
    EXPECT_EQ(63, internalNoteNameToMidi("Eb4"));
    EXPECT_EQ(64, internalNoteNameToMidi("E4"));
    EXPECT_EQ(65, internalNoteNameToMidi("F4"));
    EXPECT_EQ(66, internalNoteNameToMidi("F#4"));
    EXPECT_EQ(67, internalNoteNameToMidi("G4"));
    EXPECT_EQ(68, internalNoteNameToMidi("G#4"));
    EXPECT_EQ(69, internalNoteNameToMidi("A4"));
    EXPECT_EQ(70, internalNoteNameToMidi("Bb4"));
    EXPECT_EQ(71, internalNoteNameToMidi("B4"));
    EXPECT_EQ(72, internalNoteNameToMidi("C5"));
    EXPECT_EQ(73, internalNoteNameToMidi("C#5"));
    EXPECT_EQ(74, internalNoteNameToMidi("D5"));
    EXPECT_EQ(75, internalNoteNameToMidi("Eb5"));
    EXPECT_EQ(76, internalNoteNameToMidi("E5"));
    EXPECT_EQ(77, internalNoteNameToMidi("F5"));
    EXPECT_EQ(78, internalNoteNameToMidi("F#5"));
    EXPECT_EQ(79, internalNoteNameToMidi("G5"));
    EXPECT_EQ(80, internalNoteNameToMidi("G#5"));
    EXPECT_EQ(81, internalNoteNameToMidi("A5"));
    EXPECT_EQ(82, internalNoteNameToMidi("Bb5"));
    EXPECT_EQ(83, internalNoteNameToMidi("B5"));
    EXPECT_EQ(84, internalNoteNameToMidi("C6"));
}


TEST(Fingering, withNoConfig) {
    resetTests();
    EXPECT_FALSE(isCustomFingeringEnabled());
    EXPECT_EQ(-1, customMidiNote(0b011111110));
}


TEST(Fingering, withLowD4) {
    resetTests();
    testFingering("D4 : - xxx xxx- -");
    EXPECT_TRUE(isCustomFingeringEnabled());
    EXPECT_EQ(62, customMidiNote(0b011111100));
    EXPECT_EQ(62, customMidiNote(0b111111111));
    EXPECT_EQ(-1, customMidiNote(0b011111000));
}


TEST(Fingering, withLowE4) {
    resetTests();
    testFingering("E4 : - xxx xxo- -");
    EXPECT_EQ(64, customMidiNote(0b011111000));
    EXPECT_EQ(64, customMidiNote(0b111111011));
    EXPECT_EQ(-1, customMidiNote(0b011110000));
    EXPECT_EQ(-1, customMidiNote(0b011111100));
}


TEST(Fingering, withDandE) {
    resetTests();
    testFingering("E4 : - xxx xxo- -");
    testFingering("D4 : - xxx xxx- -");
    EXPECT_EQ(62, customMidiNote(0b011111100)); // D4
    EXPECT_EQ(62, customMidiNote(0b111111111));
    EXPECT_EQ(64, customMidiNote(0b011111000)); // E4
    EXPECT_EQ(64, customMidiNote(0b111111011));
    EXPECT_EQ(-1, customMidiNote(0b011110000));
}


TEST(Fingering, withLowScale) {
    resetTests();
    testFingering("D4 : - xxx xxx- -");
    testFingering("E4 : - xxx xxo- -");
    testFingering("F4 : - xxx xox- -");
    testFingering("F#4: - xxx xo-- -");
    testFingering("G4 : - xxx o--- -");
    testFingering("A4 : - xxo ---- -");
    testFingering("Bb4: - xox ---- -");
    testFingering("B4 : - xoo ---- -");
    testFingering("C#5: - oxx o--- -");
    testFingering("C#5: - oxo o--- -");
    testFingering("C5 : - ooo ooo- -");
    testFingering("D5 : - oxx xxx- -");
    EXPECT_EQ(62, customMidiNote(0b011111100)); // D4
    EXPECT_EQ(62, customMidiNote(0b111111111));
    EXPECT_EQ(64, customMidiNote(0b011111000));
    EXPECT_EQ(64, customMidiNote(0b111111011)); // E4
    EXPECT_EQ(64, customMidiNote(0b011111000));
    EXPECT_EQ(65, customMidiNote(0b011110100)); // F4
    EXPECT_EQ(66, customMidiNote(0b011110000)); // F#4
}


}  // namespace
