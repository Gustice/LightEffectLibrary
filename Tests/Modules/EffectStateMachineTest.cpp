
#include "EffectStateMachine.h"
#include "EffectWaveforms.h"
#include "MyUtils.h"

using namespace Effect;

/// Demo Effect Macro
EffMacro_t eff_TestStdPulse[] = {
    {Light_Idle, (uint8_t *)0, 0, 32, &color_ColdWhite, 0, 1},
    {Light_Wave, gau8_offsetPulse, 0xFF, 32, USEOLD_COLOR, 0, 2},
    {Light_Idle, (uint8_t *)0, 0, 32, USEOLD_COLOR, 0, 0},
};

static const uint8_t gau8_testWave1[]               = {1, 2, 3, 4, 5, 6, 7, 8};
static const uint8_t u8_testWaveLen                 = sizeof(gau8_testWave1);
static const uint8_t gau8_testWave2[u8_testWaveLen] = {11, 21, 31, 41, 51, 61, 71, 81};
EffMacro_t           eff_Demo1[]                     = {
    // State      WV            FS    N  Start         R  Next
    {Light_Blank, (uint8_t *)0, 0xFF, 4, &color_Black, 0, 1},
    {Light_Wave, gau8_testWave1, 0xFF, 4, &color_Red, 0, 2},
    {Light_Wave, gau8_testWave2, 0xFF, 4, &color_Blue, 0, 1}, // <- Back to 1
};
EffMacro_t           eff_Demo2[]                     = {
    // State      WV            FS    N  Start         R  Next
    {Light_Idle, (uint8_t *)0, 0xFF, 4, &color_Blue, 0, 1},
    {Light_Wave, gau8_testWave1, 0xFF, 4, &color_Red, 0, 1},
};
EffMacro_t           eff_Demo3[]                     = {
    // State      WV            FS    N  Start         R  Next
    {Light_Idle, (uint8_t *)0, 0xFF, 4, &color_Blue, 0, 1},
    {Light_Idle, (uint8_t *)0, 0xFF, 4, &color_Green, 0, 1},
    {Light_Idle, (uint8_t *)0, 0xFF, 4, &color_Red, 0, 1},
};


TEST_CASE("Constructing Effect StateMachine", "[EffectStateMachine, Constructor]") {
    static EffectSM dut(u8_testWaveLen, 0, 3);
    REQUIRE(true);
}

bool CheckColor(Color c1, Color c2) {
    if (c1 == c2)
        return true;

    Color_t v1 = c1.GetColor();
    Color_t v2 = c2.GetColor();
    INFO(
        printf(" rgbw: 0x%02x 0x%02x 0x%02x 0x%02x != 0x%02x 0x%02x 0x%02x 0x%02x\n", v1.red, v1.green, v1.blue, v1.white, v2.red, v2.green, v2.blue, v2.white))
    return false;
}

TEST_CASE("Running Effect StateMachine with different Setups", "[EffectStateMachine]") {
    SECTION("Minimal setup, only Template Length defined") {
        static EffectSM dut(u8_testWaveLen);
        dut.SetEffect(eff_Demo1);

        CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(1, 0, 0, 0), *dut.Tick())); // <- Switching to next waveform
        CHECK(CheckColor(Color(3, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(5, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(7, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 11, 0), *dut.Tick())); // <- Switching to next waveform
        CHECK(CheckColor(Color(0, 0, 31, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 51, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 71, 0), *dut.Tick()));
    }
    SECTION("Running with full intensity(applies for idle state)") {
        static EffectSM dut(u8_testWaveLen, 0xFF,0);
        dut.SetEffect(eff_Demo2);

        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(1, 0, 0, 0), *dut.Tick())); // <- Switching to next waveform
    }
    SECTION("Running with half intensity(applies for idle state)") {
        static EffectSM dut(u8_testWaveLen, 0x80,0);
        dut.SetEffect(eff_Demo2);

        CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
        CHECK(CheckColor(Color(1, 0, 0, 0), *dut.Tick())); // <- Switching to next waveform
    }
    SECTION("Running with cross fadeing 2 Steps") {
        static EffectSM dut(u8_testWaveLen, 0xFF,2);
        dut.SetEffect(eff_Demo3);

        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    }

}

// const static Effect1 gau8_testSeqWave[] = {
//     Effect1(Light_Idle, 32, 1),
//     Effect2(Light_Wave, 32, 2, gau8_offsetPulse, gu8_dynamicRange, 0),
//     Effect1(Light_Idle, 32, 0),
// };
// const static Effect3 init(Light_Blank, 1,2, nullptr, 0, 0, 0, 0, NO_COLOR, 128, 10);
// EffectMacro someScene(&init, gau8_testSeqWave, 3);
// void SequenceMachinDemo(void) {
//     static SequenceSM Eff1(&init);
//     static SequenceSM Eff2(&init);

//     Eff1.SetEffect(&someScene);
//     Eff2.SetEffect(&someScene, 3);

// }

TEST_CASE("Setting EffectSM by Parameter or by Structure", "[EffectStateMachine, Constructor]") {

    SECTION("Setting EffectSM Object by Parameter") {}
    SECTION("Setting EffectSM Object by Structure") {}
}
