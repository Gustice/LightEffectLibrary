
#include "StateMachineDemo.h"
/// Demo Effect Macro
EffMacro_type eff_TestStdPulse[] = {
    {Light_Idle, (uint8_t *)0, 0, 32, &color_ColdWhite, 0, 1},
    {Light_Wave, gau8_offsetPulse, 0xFF, 32, USEOLD_COLOR, 0, 2},
    {Light_Idle, (uint8_t *)0, 0, 32, USEOLD_COLOR, 0, 0},
};

static const uint8_t gau8_testWave[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
EffMacro_type        eff_Demo[]      = {
    // EffectState  // WafeformRef  FS-Val  Duration   Color           Repeats  Next
    {Light_Blank, (uint8_t *)0, 0xFF, 4, &color_Black, 0, 1},
    {Light_Wave, gau8_testWave, 0xFF, 6, &color_Black, 0, 1},
};
static const uint8_t u8_testWaveLen = sizeof(gau8_testWave);

void StateMachinDemo(void) {
    PrintLine("## StateMachine Demo");

    static EffectSM Eff1(u8_testWaveLen, 0, 3);
    static EffectSM Eff2(u8_testWaveLen, 0, 4);
    Eff1.SetEffect(eff_Demo);
    Eff2.SetEffect(eff_Demo, NO_COLOR, 3);

    PrintLine("  No  Effect1            Effect2 (delayed)");
    PrintLine("  No  tick index fade    tick index fade    ");
    char stream[100];

    for (uint8_t i = 0; i < 16; i++) {
        uint8_t tick1 = Eff1.Tick();
        uint8_t tick2 = Eff2.Tick();
        sprintf(stream, "  %2d  %4d  %4d  %3d      %4d  %4d  %3d    ", i, tick1, Eff1.GetWaveformIdx(), Eff1.GetDissolveRatio(), tick2, Eff2.GetWaveformIdx(),
                Eff2.GetDissolveRatio());
        PrintLine(stream);
    }
}