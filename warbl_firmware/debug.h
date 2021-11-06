
#ifndef DEBUG_H_
#define DEBUG_H_

#ifndef DEBUG_ENABLED
#define DEBUG_ENABLED   0 // Set to 1 to enable midi debuging (Normally keep as 0)
#endif

#if DEBUG_ENABLED

    #define DEBUG_PRESSURE         10
    #define DEBUG_LOWER_THRESHOLD  20
    #define DEBUG_UPPERBOUND       22
    #define DEBUG_UPPERBOUND_LOW   24
    #define DEBUG_UPPERBOUND_HIGH  26

    extern void debugTrace(byte lineInfo, byte data);
    extern void debugValue(byte key, byte value);
    extern void debugInt(byte key, int value);
    extern void debugSetOnce(bool enable);
    extern void debugIntOnce(byte key, int value);
    extern void debugPressure(int pressure);

#else

    // Does nothing and takes up no space when DEBUG_ENABLED is 0
    #define debugTrace(lineInfo, data)
    #define debugValue(key, value)
    #define debugInt(key, value)
    #define debugSetOnce(enable)
    #define debugIntOnce(key, value)
    #define debugPressure(pressure)

#endif

#endif // DEBUG_H_
