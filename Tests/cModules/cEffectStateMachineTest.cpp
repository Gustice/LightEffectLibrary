
#include "EffectStateMachine.h"
#include "EffectWaveforms.h"
#include "MyUtils.h"

static const uint8_t gau8_testWave1[]               = {1, 2, 3, 4, 5, 6, 7, 8};
static const uint8_t u8_testWaveLen                 = sizeof(gau8_testWave1);
static const uint8_t gau8_testWave2[u8_testWaveLen] = {11, 21, 31, 41, 51, 61, 71, 81};

static bool CheckColor(Color_t c1, const Color_t c2) {
    if (ColorIsEqual(&c1, c2))
        return true;

    INFO(printf(" rgbw: 0x%02x 0x%02x 0x%02x 0x%02x != 0x%02x 0x%02x 0x%02x 0x%02x\n", c1.red, c1.green, c1.blue,
                c1.white, c2.red, c2.green, c2.blue, c2.white));
    INFO(printf(" rgbw: %d %d %d %d != %d %d %d %d\n", c1.red, c1.green, c1.blue, c1.white, c2.red, c2.green, c2.blue,
                c2.white));
    return false;
}

TEST_CASE("Running cEffect StateMachine with different Setups", "[cEffectStateMachine]") {
    SECTION("Running with different intensities(applies for idle state)") {
        EffMacro_t effDemo[] = {
            // State      WV            FS    N  Start         R  Next
            {Light_Idle, NO_WAVE, 0xFF, 4, &cBlue, 0, 1},
            {Light_Idle, NO_WAVE, 0xFF, 4, &cGreen, 0, 2},
        };
        uint8_t        fullIntens = 0xFFu;
        uint8_t        halfIntens = 0x80u;
        static Color_t dutColor;

        SM_ParameterValues_t param{
            .idleIntens     = 0xFF,
            .dynamicRange   = 30,
            .fadeSteps      = 0,
            .templateLength = u8_testWaveLen,
        };

        SECTION("Running with full intensity(applies for idle state)") {
            static EffectSM_t dut;
            param.idleIntens = 0xFF;
            SM_Construct(&dut, param, &dutColor);
            SM_SetEffect(&dut, effDemo, NO_COLOR, NULL, 0);

            CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *SM_Tick(&dut)));
            CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *SM_Tick(&dut)));
            CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *SM_Tick(&dut)));
            CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *SM_Tick(&dut)));
            CHECK(CheckColor(Color_t{0, 0xFF, 0, 0}, *SM_Tick(&dut))); // <- Switching to next waveform
        }

        SECTION("Running with half default idle intensity(applies for idle state)") {
            static EffectSM_t dut;
            param.idleIntens = 0x80;
            SM_Construct(&dut, param, &dutColor);
            SM_SetEffect(&dut, effDemo, NO_COLOR, NULL, 0);

            CHECK(CheckColor(Color_t{0, 0, 0x80, 0}, *SM_Tick(&dut)));
            CHECK(CheckColor(Color_t{0, 0, 0x80, 0}, *SM_Tick(&dut)));
            CHECK(CheckColor(Color_t{0, 0, 0x80, 0}, *SM_Tick(&dut)));
            CHECK(CheckColor(Color_t{0, 0, 0x80, 0}, *SM_Tick(&dut)));
            CHECK(CheckColor(Color_t{0, 0x80, 0, 0}, *SM_Tick(&dut))); // <- Switching to next waveform
        }
        SECTION("Define other start intensity") {
            static EffectSM_t dut;
            SM_Construct(&dut, param, &dutColor);
            SM_SetEffect(&dut, effDemo, NO_COLOR, &halfIntens, 0);

            CHECK(CheckColor(Color_t{0, 0, 0x80, 0}, *SM_Tick(&dut)));
            SM_Tick(&dut);
            SM_Tick(&dut);
            CHECK(CheckColor(Color_t{0, 0, 0x80, 0}, *SM_Tick(&dut)));
            CHECK(CheckColor(Color_t{0, 0x80, 0, 0}, *SM_Tick(&dut))); // <- Switching to next waveform
        }
    }

    SECTION("Starting Effect with different colors") {
        EffMacro_t effDemo[] = {
            // State     WV       FS    N  Start         R  Next
            {Light_Idle, NO_WAVE, 0xFF, 4, &cBlue, 0, 1},
            {Light_Idle, NO_WAVE, 0xFF, 4, &cGreen, 0, 2},
            {Light_Idle, NO_WAVE, 0xFF, 4, &cRed, 0, 0},
        };

        static Color_t       dutColor;
        SM_ParameterValues_t param{
            .idleIntens     = 0xFF,
            .dynamicRange   = 30,
            .fadeSteps      = 0,
            .templateLength = u8_testWaveLen,
        };
        static EffectSM_t dut;
        SM_Construct(&dut, param, &dutColor);

        SECTION("Not Changing colors") {
            SM_SetEffect(&dut, effDemo, NO_COLOR, NULL, 0);
            CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *SM_Tick(&dut)));
            SM_Tick(&dut);
            SM_Tick(&dut);
            CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *SM_Tick(&dut)));
            CHECK(CheckColor(Color_t{0, 0xFF, 0, 0}, *SM_Tick(&dut))); // <- Switching to next waveform
        }
        SECTION("Define other Start Color") {
            SM_SetEffect(&dut, effDemo, &cRed, NULL, 0);
            CHECK(CheckColor(Color_t{0xFF, 0, 0, 0}, *SM_Tick(&dut)));
            SM_Tick(&dut);
            SM_Tick(&dut);
            CHECK(CheckColor(Color_t{0xFF, 0, 0, 0}, *SM_Tick(&dut)));
            CHECK(CheckColor(Color_t{0, 0xFF, 0, 0}, *SM_Tick(&dut))); // <- Switching to next waveform
        }
    }
    SECTION("Delayed start of Effects") {
        EffMacro_t effDemo[] = {
            // State      WV            FS    N  Start         R  Next
            {Light_Idle, NO_WAVE, 0xFF, 4, &cBlue, 0, 1},
            {Light_Idle, NO_WAVE, 0xFF, 4, &cGreen, 0, 2},
        };

        Color_t              dutColor;
        SM_ParameterValues_t param{
            .idleIntens     = 0xFF,
            .dynamicRange   = 30,
            .fadeSteps      = 0,
            .templateLength = u8_testWaveLen,
        };
        EffectSM_t dut;
        SM_Construct(&dut, param, &dutColor);
        SM_SetEffect(&dut, effDemo, NO_COLOR, NULL, 2);

        CHECK(CheckColor(Color_t{0, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *SM_Tick(&dut)));
        SM_Tick(&dut);
        SM_Tick(&dut);
        CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0xFF, 0, 0}, *SM_Tick(&dut))); // <- Switching to next waveform
    }

    // SECTION("Running with cross fadeing 2 Steps") { // @todo Cross-Fade
    //     static Color_t       dutColor;
    //     SM_ParameterValues_t param{
    //         .idleIntens     = 0xFF,
    //         .dynamicRange   = 30,
    //         .fadeSteps      = 0,
    //         .templateLength = u8_testWaveLen,
    //     };
    //     static EffectSM_t dut;
    //     SM_Construct(&dut, param, &dutColor);
    //     SM_SetEffect(&dut, effDemo, NO_COLOR, NULL, 0);

    //     static EffectSM dut(u8_testWaveLen, 0xFF, 2);
    //     dut.SetEffect(effDemo);

    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *SM_Tick(&dut)));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *SM_Tick(&dut)));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *SM_Tick(&dut)));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *SM_Tick(&dut)));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *SM_Tick(&dut)));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *SM_Tick(&dut)));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *SM_Tick(&dut)));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *SM_Tick(&dut)));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *SM_Tick(&dut)));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *SM_Tick(&dut)));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *SM_Tick(&dut)));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *SM_Tick(&dut)));
    // }
}

TEST_CASE("Processing different cEffectMacros correctly", "[cEffectStateMachine]") {
    SECTION("Minimal setup, Processing Next-Index correctly") {
        EffMacro_t effDemo[] = {
            // State      WV            FS    N  Start         R  Next
            {Light_Blank, (uint8_t *)0, 0xFF, 4, &cBlack, 0, 1},
            {Light_Wave, gau8_testWave1, 0xFF, 4, &cRed, 0, 2},
            {Light_RevWave, gau8_testWave1, 0xFF, 4, &cBlue, 0, 1}, // <- Back to 1
        };

        Color_t              dutColor;
        SM_ParameterValues_t param{
            .idleIntens     = 0xFF,
            .dynamicRange   = 30,
            .fadeSteps      = 0,
            .templateLength = u8_testWaveLen,
        };
        EffectSM_t dut;
        SM_Construct(&dut, param, &dutColor);
        SM_SetEffect(&dut, effDemo, NO_COLOR, NULL, 0);

        CHECK(CheckColor(Color_t{0, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0, 0, 0}, *SM_Tick(&dut))); // => Next 1
        CHECK(CheckColor(Color_t{1, 0, 0, 0}, *SM_Tick(&dut))); // <- Switching to next waveform
        CHECK(CheckColor(Color_t{3, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{5, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{7, 0, 0, 0}, *SM_Tick(&dut))); // => Next 2
        CHECK(CheckColor(Color_t{0, 0, 8, 0}, *SM_Tick(&dut))); // <- Switching to next waveform
        CHECK(CheckColor(Color_t{0, 0, 6, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0, 4, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0, 2, 0}, *SM_Tick(&dut))); // => Next 1
        CHECK(CheckColor(Color_t{1, 0, 0, 0}, *SM_Tick(&dut))); // <- Switching to next waveform
        CHECK(CheckColor(Color_t{3, 0, 0, 0}, *SM_Tick(&dut)));
    }
    SECTION("Stretching and compressing waveforms correctly") {
        EffMacro_t effDemo[] = {
            // State      WV            FS    N  Start         R  Next
            {Light_Blank, (uint8_t *)0, 0xFF, 1, &cBlack, 0, 1},
            {Light_Wave, gau8_testWave1, 0xFF, 2, &cRed, 0, 2},
            {Light_Wave, gau8_testWave1, 0xFF, 4, &cRed, 0, 3},
            {Light_Wave, gau8_testWave1, 0xFF, 8, &cRed, 0, 0},
        };

        Color_t              dutColor;
        SM_ParameterValues_t param{
            .idleIntens     = 0xFF,
            .dynamicRange   = 30,
            .fadeSteps      = 0,
            .templateLength = u8_testWaveLen,
        };
        EffectSM_t dut;
        SM_Construct(&dut, param, &dutColor);
        SM_SetEffect(&dut, effDemo, NO_COLOR, NULL, 0);

        CHECK(CheckColor(Color_t{0, 0, 0, 0}, *SM_Tick(&dut))); // => Next 1
        CHECK(CheckColor(Color_t{1, 0, 0, 0}, *SM_Tick(&dut))); // <- Switching to next waveform
        CHECK(CheckColor(Color_t{5, 0, 0, 0}, *SM_Tick(&dut))); // => Next 2

        CHECK(CheckColor(Color_t{1, 0, 0, 0}, *SM_Tick(&dut))); // <- Switching to next waveform
        CHECK(CheckColor(Color_t{3, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{5, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{7, 0, 0, 0}, *SM_Tick(&dut))); // => Next 3

        CHECK(CheckColor(Color_t{1, 0, 0, 0}, *SM_Tick(&dut))); // <- Switching to next waveform
        CHECK(CheckColor(Color_t{2, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{3, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{4, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{5, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{6, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{7, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{8, 0, 0, 0}, *SM_Tick(&dut))); // => Next 0

        CHECK(CheckColor(Color_t{0, 0, 0, 0}, *SM_Tick(&dut))); // => Next 1
    }
    SECTION("Minimal setup, Processing repeats correctly") {
        EffMacro_t effDemo[] = {
            // State      WV            FS    N  Start         R  Next
            {Light_Blank, (uint8_t *)0, 0xFF, 2, &cBlack, 0, 1},
            {Light_Wave, gau8_testWave1, 0xFF, 4, &cRed, 1, 2},
            {Light_RevWave, gau8_testWave1, 0xFF, 2, &cBlue, 2, 1}, // <- Back to 1
        };

        Color_t              dutColor;
        SM_ParameterValues_t param{
            .idleIntens     = 0xFF,
            .dynamicRange   = 30,
            .fadeSteps      = 0,
            .templateLength = u8_testWaveLen,
        };
        EffectSM_t dut;
        SM_Construct(&dut, param, &dutColor);
        SM_SetEffect(&dut, effDemo, NO_COLOR, NULL, 0);

        CHECK(CheckColor(Color_t{0, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0, 0, 0}, *SM_Tick(&dut))); // => Next 1

        CHECK(CheckColor(Color_t{1, 0, 0, 0}, *SM_Tick(&dut))); // <- Switching to next waveform
        CHECK(CheckColor(Color_t{3, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{5, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{7, 0, 0, 0}, *SM_Tick(&dut))); // => Repeat
        CHECK(CheckColor(Color_t{1, 0, 0, 0}, *SM_Tick(&dut))); // <-
        CHECK(CheckColor(Color_t{3, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{5, 0, 0, 0}, *SM_Tick(&dut)));
        CHECK(CheckColor(Color_t{7, 0, 0, 0}, *SM_Tick(&dut))); // => Next 2

        CHECK(CheckColor(Color_t{0, 0, 8, 0}, *SM_Tick(&dut))); // <- Switching to next waveform
        CHECK(CheckColor(Color_t{0, 0, 4, 0}, *SM_Tick(&dut))); // => Repeat
        CHECK(CheckColor(Color_t{0, 0, 8, 0}, *SM_Tick(&dut))); // <-
        CHECK(CheckColor(Color_t{0, 0, 4, 0}, *SM_Tick(&dut))); // => Repeat
        CHECK(CheckColor(Color_t{0, 0, 8, 0}, *SM_Tick(&dut))); // <-
        CHECK(CheckColor(Color_t{0, 0, 4, 0}, *SM_Tick(&dut))); // => Next

        CHECK(CheckColor(Color_t{1, 0, 0, 0}, *SM_Tick(&dut))); // <- Switching to next waveform
        CHECK(CheckColor(Color_t{3, 0, 0, 0}, *SM_Tick(&dut)));
    }
}

TEST_CASE("Setting All different effects of cEffectSM", "[cEffectStateMachine, Constructor]") {
    EffMacro_t effDemo[] = {
        // State      WV                FS    N   Start         R  Next
        {Light_Blank, NO_WAVE, 0xFF, 2, &cRed, 0, 1},
        {Light_Idle, NO_WAVE, 0xFF, 4, USEOLD_COLOR, 0, 2},
        {Light_Blank, NO_WAVE, 0xFF, 2, USEOLD_COLOR, 0, 3},
        {Light_Wave, gau8_testWave1, 0xFF, 4, USEOLD_COLOR, 0, 4},
        {Light_Freeze, NO_WAVE, 0xFF, 4, USEOLD_COLOR, 0, 5},
        {Light_RevWave, gau8_testWave1, 0xFF, 4, USEOLD_COLOR, 0, 6},
        {Light_Flicker, NO_WAVE, 0x80, 16, USEOLD_COLOR, 0, 0}, // <- Back to 1
    };

    Color_t              dutColor;
    SM_ParameterValues_t param{
        .idleIntens     = 0x80,
        .dynamicRange   = 30,
        .fadeSteps      = 0,
        .templateLength = u8_testWaveLen,
    };
    EffectSM_t dut;
    SM_Construct(&dut, param, &dutColor);
    SM_SetEffect(&dut, effDemo, NO_COLOR, NULL, 0);

    CHECK(CheckColor(Color_t{0, 0, 0, 0}, *SM_Tick(&dut))); // <- Blank
    CHECK(CheckColor(Color_t{0, 0, 0, 0}, *SM_Tick(&dut))); // => Next 1

    CHECK(CheckColor(Color_t{0x80, 0, 0, 0}, *SM_Tick(&dut))); // <- Switch to Idle
    CHECK(CheckColor(Color_t{0x80, 0, 0, 0}, *SM_Tick(&dut)));
    CHECK(CheckColor(Color_t{0x80, 0, 0, 0}, *SM_Tick(&dut)));
    CHECK(CheckColor(Color_t{0x80, 0, 0, 0}, *SM_Tick(&dut))); // => Next 2

    CHECK(CheckColor(Color_t{0, 0, 0, 0}, *SM_Tick(&dut))); // <- Blank
    CHECK(CheckColor(Color_t{0, 0, 0, 0}, *SM_Tick(&dut))); // => Next 3

    CHECK(CheckColor(Color_t{1, 0, 0, 0}, *SM_Tick(&dut))); // <- Switching to next waveform
    CHECK(CheckColor(Color_t{3, 0, 0, 0}, *SM_Tick(&dut)));
    CHECK(CheckColor(Color_t{5, 0, 0, 0}, *SM_Tick(&dut)));
    CHECK(CheckColor(Color_t{7, 0, 0, 0}, *SM_Tick(&dut))); // => Next 4

    CHECK(CheckColor(Color_t{7, 0, 0, 0}, *SM_Tick(&dut))); // <- Switching to Freeze
    CHECK(CheckColor(Color_t{7, 0, 0, 0}, *SM_Tick(&dut)));
    CHECK(CheckColor(Color_t{7, 0, 0, 0}, *SM_Tick(&dut)));
    CHECK(CheckColor(Color_t{7, 0, 0, 0}, *SM_Tick(&dut))); // => Next 5

    CHECK(CheckColor(Color_t{8, 0, 0, 0}, *SM_Tick(&dut))); // <- Switching to next waveform
    CHECK(CheckColor(Color_t{6, 0, 0, 0}, *SM_Tick(&dut)));
    CHECK(CheckColor(Color_t{4, 0, 0, 0}, *SM_Tick(&dut)));
    CHECK(CheckColor(Color_t{2, 0, 0, 0}, *SM_Tick(&dut))); // => Next 6

    // Dynamic range is set to 30 internally // @todo change someday
    for (size_t i = 0; i < 16; i++) {
        const Color_t ch = *SM_Tick(&dut);
        // INFO(printf("  %d = %d", i, ch));
        CHECK(((ch.red >= 0x80) && (ch.red <= (0x80 + 30))));
    }
}

TEST_CASE("cEffectSM Plots for manual investigation", "[cEffectStateMachine, Constructor]") {
    EffMacro_t effDemo[] = {
        // State      WV                  FS    N   Start         R  Next
        {Light_Blank, NO_WAVE, 0xFF, 4, &cRed, 0, 1},
        {Light_Wave, gau8_fullPulse, 0xFF, 128, USEOLD_COLOR, 0, 2},
        {Light_Blank, NO_WAVE, 0xFF, 4, USEOLD_COLOR, 0, 3},
        {Light_Wave, gau8_fullSlope, 0xFF, 128, USEOLD_COLOR, 0, 4},
        {Light_Blank, NO_WAVE, 0xFF, 4, USEOLD_COLOR, 0, 5},
        {Light_Wave, gau8_initSlope, 0xFF, 128, USEOLD_COLOR, 0, 6},
        {Light_Blank, NO_WAVE, 0xFF, 4, USEOLD_COLOR, 0, 7},
        {Light_Wave, gau8_offsetPulse, 0xFF, 128, USEOLD_COLOR, 0, 8},
        {Light_Blank, NO_WAVE, 0xFF, 4, USEOLD_COLOR, 0, 9},
        {Light_Wave, gau8_offsetSlope, 0xFF, 128, USEOLD_COLOR, 0, 10},
        {Light_Blank, NO_WAVE, 0xFF, 4, USEOLD_COLOR, 0, 11},
        {Light_Flicker, NO_WAVE, 0xFF, 64, USEOLD_COLOR, 0, 0},
    };

    Color_t              dutColor;
    SM_ParameterValues_t param{
        .idleIntens     = 0x80,
        .dynamicRange   = 30,
        .fadeSteps      = 0,
        .templateLength = cu16_TemplateLength,
    };
    EffectSM_t dut;
    SM_Construct(&dut, param, &dutColor);
    SM_SetEffect(&dut, effDemo, NO_COLOR, NULL, 0);

    std::ofstream plotFile;
    char          stream[100];
    uint8_t       cStream[4];
    plotFile.open(TSTOUTPUT_DIR "cEffectStateMachine.csv");
    plotFile << "cycle,value" << std::endl;

    for (int i = 0; i < 728; i++) {
        const Color_t * c = SM_Tick(&dut);
        sprintf(stream, "%3d,%d", i, c->red);
        plotFile << stream << std::endl;
    }
    plotFile.close();
    REQUIRE(true);
}