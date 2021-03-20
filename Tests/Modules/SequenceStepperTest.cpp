
#include "SequenceStepper.h"
#include "Waveforms.h"
#include "MyUtils.h"

using namespace Effect;

static const uint8_t gau8_testWave1[]               = {1, 2, 3, 4, 5, 6, 7, 8};
static const uint8_t u8_testWaveLen                 = sizeof(gau8_testWave1);
static const uint8_t gau8_testWave2[u8_testWaveLen] = {11, 21, 31, 41, 51, 61, 71, 81};

TEST_CASE("Constructing Effect SequenceMachine", "[EffectSquenceStateMachine, Constructor]") {
    static SequenceSM dut(u8_testWaveLen, 1, 0, 3);
    REQUIRE(true);
}

static bool CheckColor(Color c1, const Color c2) {
    if (c1 == c2)
        return true;

    color_t v1 = c1.GetColor();
    color_t v2 = c2.GetColor();
    INFO(printf(" rgbw: 0x%02x 0x%02x 0x%02x 0x%02x != 0x%02x 0x%02x 0x%02x 0x%02x\n", v1.red, v1.green, v1.blue,
                v1.white, v2.red, v2.green, v2.blue, v2.white));
    INFO(printf(" rgbw: %d %d %d %d != %d %d %d %d\n", v1.red, v1.green, v1.blue, v1.white, v2.red, v2.green, v2.blue,
                v2.white));
    return false;
}

TEST_CASE("Running Effect SequenceMachine with different Setups", "[EffectSquenceStateMachine]") {
    SECTION("Running with different intensities(applies for idle state)") {
        Sequence effDemo[] = {
            // State      WV            FS    N  Start         R  Next
            // {Light_Idle, noWave, 0xFF, 4, &color_Blue, 0, 1},
            // {Light_Idle, noWave, 0xFF, 4, &color_Green, 0, 2},
            Sequence(4,1, Light_Idle, &color_Blue),
            Sequence(4,2, Light_Idle, &color_Green),
        };
        uint8_t fullIntens = 0xFFu;
        uint8_t halfIntens = 0x80u;

        SECTION("Running with full intensity(applies for idle state)") {
            static SequenceSM dut(u8_testWaveLen, 1, 0xFF, 0);
            dut.SetEffect(effDemo);

            CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0xFF, 0, 0), *dut.Tick())); // <- Switching to next waveform
        }

        SECTION("Running with half default idle intensity(applies for idle state)") {
            static SequenceSM dut(u8_testWaveLen, 1, 0x80, 0);
            dut.SetEffect(effDemo);

            CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0x80, 0, 0), *dut.Tick())); // <- Switching to next waveform
        }
        SECTION("Define other start intensity") {
            static SequenceSM dut(u8_testWaveLen, 1, 0xFF, 0);
            dut.SetEffect(effDemo, noColor, &halfIntens, 0);
            CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
            dut.Tick();
            dut.Tick();
            CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0x80, 0, 0), *dut.Tick())); // <- Switching to next waveform
        }
    }
    SECTION("Starting Effect with different colors") {
        Sequence effDemo[] = {
            // State     WV       FS    N  Start         R  Next
            // {Light_Idle, noWave, 0xFF, 4, &color_Blue, 0, 1},
            // {Light_Idle, noWave, 0xFF, 4, &color_Green, 0, 2},
            // {Light_Idle, noWave, 0xFF, 4, &color_Red, 0, 0},
            Sequence(4,1, Light_Idle, &color_Blue),
            Sequence(4,2, Light_Idle, &color_Green),
            Sequence(4,0, Light_Idle, &color_Red),
      };

        static SequenceSM dut(u8_testWaveLen, 1, 0xFF, 0);
        SECTION("Not Changing colors") {
            dut.SetEffect(effDemo);
            CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
            dut.Tick();
            dut.Tick();
            CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0xFF, 0, 0), *dut.Tick())); // <- Switching to next waveform
        }
        SECTION("Define other Start Color") {
            dut.SetEffect(effDemo, &color_Red, nullptr, 0);
            CHECK(CheckColor(Color(0xFF, 0, 0, 0), *dut.Tick()));
            dut.Tick();
            dut.Tick();
            CHECK(CheckColor(Color(0xFF, 0, 0, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0xFF, 0, 0), *dut.Tick())); // <- Switching to next waveform
        }
    }
    SECTION("Delayed start of Effects") {
        Sequence effDemo[] = {
            // State      WV            FS    N  Start         R  Next
            // {Light_Idle, noWave, 0xFF, 4, &color_Blue, 0, 1},
            // {Light_Idle, noWave, 0xFF, 4, &color_Green, 0, 2},
            Sequence(4,1, Light_Idle, &color_Blue),
            Sequence(4,2, Light_Idle, &color_Green),
      };
        static SequenceSM dut(u8_testWaveLen, 1, 0xFF, 0);
        dut.SetEffect(effDemo, noColor, 2);
        CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        dut.Tick();
        dut.Tick();
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0xFF, 0, 0), *dut.Tick())); // <- Switching to next waveform
    }

    // SECTION("Running with cross fadeing 2 Steps") { // @todo Cross-Fade
    //     static SequenceSM dut(u8_testWaveLen, 0xFF,2);
    //     dut.SetEffect(effDemo);

    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    //     CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    // }
}

TEST_CASE("Tick and GetColor on SequenceMachine yield same results", "[EffectSquenceStateMachine]") {
    Sequence effDemo[] = {
        // State        WV       FS    N  Start         R  Next
        // {Light_Idle, noWave, 0xFF, 1, &color_Blue, 0, 1},
        // {Light_Idle, noWave, 0xFF, 1, &color_Green, 0, 2},
        // {Light_Idle, noWave, 0xFF, 1, &color_Red, 0, 0},
        Sequence(1,1, Light_Idle, &color_Blue),
        Sequence(1,2, Light_Idle, &color_Green),
        Sequence(1,0, Light_Idle, &color_Red),
    };
    static SequenceSM dut(u8_testWaveLen, 1, 0xFF, 0);
    dut.SetEffect(effDemo);

    const Color * tc1 = dut.Tick();
    const Color gc1 = dut.GetColor();
    CHECK(CheckColor(Color(0, 0, 0xFF, 0), gc1));
    CHECK(CheckColor(Color(0, 0, 0xFF, 0), *tc1));
    const Color * tc2 = dut.Tick();
    const Color gc2 = dut.GetColor();
    CHECK(CheckColor(Color(0, 0xFF, 0, 0), gc2));
    CHECK(CheckColor(Color(0, 0xFF, 0, 0), *tc2));
    const Color * tc3 = dut.Tick();
    const Color gc3 = dut.GetColor();
    CHECK(CheckColor(Color(0xFF, 0, 0, 0), gc3));
    CHECK(CheckColor(Color(0xFF, 0, 0, 0), *tc3));
}

TEST_CASE("Processing different SequenceMacros correctly", "[EffectSquenceStateMachine]") {
    SECTION("Minimal setup, Processing Next-Index correctly") {
        Sequence effDemo[] = {
            // State      WV            FS    N  Start         R  Next
            // {Light_Blank, (uint8_t *)0, 0xFF, 4, &color_Black, 0, 1},
            // {Light_Wave, gau8_testWave1, 0xFF, 4, &color_Red, 0, 2},
            // {Light_RevWave, gau8_testWave1, 0xFF, 4, &color_Blue, 0, 1}, // <- Back to 1
            Sequence(4,1, Light_Blank, &color_Black),
            Sequence(4,2, gau8_testWave1, Light_Wave, &color_Red),
            Sequence(4,1, gau8_testWave1, Light_RevWave, &color_Blue),
        };

        static SequenceSM dut(u8_testWaveLen, 1);
        dut.SetEffect(effDemo); // => Start with 0
        CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick())); // => Next 1
        CHECK(CheckColor(Color(1, 0, 0, 0), *dut.Tick())); // <- Switching to next waveform
        CHECK(CheckColor(Color(3, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(5, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(7, 0, 0, 0), *dut.Tick())); // => Next 2
        CHECK(CheckColor(Color(0, 0, 8, 0), *dut.Tick())); // <- Switching to next waveform
        CHECK(CheckColor(Color(0, 0, 6, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 4, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 2, 0), *dut.Tick())); // => Next 1
        CHECK(CheckColor(Color(1, 0, 0, 0), *dut.Tick())); // <- Switching to next waveform
        CHECK(CheckColor(Color(3, 0, 0, 0), *dut.Tick()));
    }
    SECTION("Stretching and compressing waveforms correctly") {
        Sequence effDemo[] = {
            // State      WV            FS    N  Start         R  Next
            // {Light_Blank, (uint8_t *)0, 0xFF, 1, &color_Black, 0, 1},
            // {Light_Wave, gau8_testWave1, 0xFF, 2, &color_Red, 0, 2},
            // {Light_Wave, gau8_testWave1, 0xFF, 4, &color_Red, 0, 3},
            // {Light_Wave, gau8_testWave1, 0xFF, 8, &color_Red, 0, 0},
            Sequence(1,1, Light_Blank, &color_Black),
            Sequence(2,2, gau8_testWave1, Light_Wave, &color_Red),
            Sequence(4,3, gau8_testWave1, Light_Wave, &color_Red),
            Sequence(8,0, gau8_testWave1, Light_Wave, &color_Red),
        };

        static SequenceSM dut(u8_testWaveLen, 1);
        dut.SetEffect(effDemo);                            // => Start with 0
        CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick())); // => Next 1
        CHECK(CheckColor(Color(1, 0, 0, 0), *dut.Tick())); // <- Switching to next waveform
        CHECK(CheckColor(Color(5, 0, 0, 0), *dut.Tick())); // => Next 2

        CHECK(CheckColor(Color(1, 0, 0, 0), *dut.Tick())); // <- Switching to next waveform
        CHECK(CheckColor(Color(3, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(5, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(7, 0, 0, 0), *dut.Tick())); // => Next 3

        CHECK(CheckColor(Color(1, 0, 0, 0), *dut.Tick())); // <- Switching to next waveform
        CHECK(CheckColor(Color(2, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(3, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(4, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(5, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(6, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(7, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(8, 0, 0, 0), *dut.Tick())); // => Next 0

        CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick())); // => Next 1
    }
    SECTION("Minimal setup, Processing repeats correctly") {
        Sequence effDemo[] = {
            // State      WV            FS    N  Start         R  Next
            // {Light_Blank, (uint8_t *)0, 0xFF, 2, &color_Black, 0, 1},
            // {Light_Wave, gau8_testWave1, 0xFF, 4, &color_Red, 1, 2},
            // {Light_RevWave, gau8_testWave1, 0xFF, 2, &color_Blue, 2, 1}, // <- Back to 1
            Sequence(2,1, Light_Blank, &color_Black),
            Sequence(4,2, gau8_testWave1, Light_Wave, &color_Red, 1),
            Sequence(2,1, gau8_testWave1, Light_RevWave, &color_Blue, 2),
        };

        static SequenceSM dut(u8_testWaveLen, 1);
        dut.SetEffect(effDemo); // => Start with 0
        CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick())); // => Next 1

        CHECK(CheckColor(Color(1, 0, 0, 0), *dut.Tick())); // <- Switching to next waveform
        CHECK(CheckColor(Color(3, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(5, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(7, 0, 0, 0), *dut.Tick())); // => Repeat
        CHECK(CheckColor(Color(1, 0, 0, 0), *dut.Tick())); // <-
        CHECK(CheckColor(Color(3, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(5, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(7, 0, 0, 0), *dut.Tick())); // => Next 2

        CHECK(CheckColor(Color(0, 0, 8, 0), *dut.Tick())); // <- Switching to next waveform
        CHECK(CheckColor(Color(0, 0, 4, 0), *dut.Tick())); // => Repeat
        CHECK(CheckColor(Color(0, 0, 8, 0), *dut.Tick())); // <-
        CHECK(CheckColor(Color(0, 0, 4, 0), *dut.Tick())); // => Repeat
        CHECK(CheckColor(Color(0, 0, 8, 0), *dut.Tick())); // <-
        CHECK(CheckColor(Color(0, 0, 4, 0), *dut.Tick())); // => Next

        CHECK(CheckColor(Color(1, 0, 0, 0), *dut.Tick())); // <- Switching to next waveform
        CHECK(CheckColor(Color(3, 0, 0, 0), *dut.Tick()));
    }
}

TEST_CASE("Setting All different effects of SequenceSM", "[EffectSquenceStateMachine, Constructor]") {
    Sequence effDemo[] = {
        // State      WV                FS    N   Start         R  Next
        // {Light_Blank, noWave, 0xFF, 2, &color_Red, 0, 1},
        // {Light_Idle, noWave, 0xFF, 4, oldColor, 0, 2},
        // {Light_Blank, noWave, 0xFF, 2, oldColor, 0, 3},
        // {Light_Wave, gau8_testWave1, 0xFF, 4, oldColor, 0, 4},
        // {Light_Freeze, noWave, 0xFF, 4, oldColor, 0, 5},
        // {Light_RevWave, gau8_testWave1, 0xFF, 4, oldColor, 0, 6},
        // {Light_Flicker, noWave, 0x80, 16, oldColor, 0, 0}, // <- Back to 1
        Sequence(2,1, Light_Blank, &color_Red),
        Sequence(4,2, Light_Idle),
        Sequence(2,3, Light_Blank),
        Sequence(4,4, gau8_testWave1, Light_Wave),
        Sequence(4,5, Light_Freeze),
        Sequence(4,6, gau8_testWave1, Light_RevWave),
        Sequence(16,1, Light_Flicker),
   };
    static SequenceSM dut(u8_testWaveLen, 1, 0x80, 0);
    dut.SetEffect(effDemo);

    CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick())); // <- Blank
    CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick())); // => Next 1

    CHECK(CheckColor(Color(0x80, 0, 0, 0), *dut.Tick())); // <- Switch to Idle
    CHECK(CheckColor(Color(0x80, 0, 0, 0), *dut.Tick()));
    CHECK(CheckColor(Color(0x80, 0, 0, 0), *dut.Tick()));
    CHECK(CheckColor(Color(0x80, 0, 0, 0), *dut.Tick())); // => Next 2

    CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick())); // <- Blank
    CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick())); // => Next 3

    CHECK(CheckColor(Color(1, 0, 0, 0), *dut.Tick())); // <- Switching to next waveform
    CHECK(CheckColor(Color(3, 0, 0, 0), *dut.Tick()));
    CHECK(CheckColor(Color(5, 0, 0, 0), *dut.Tick()));
    CHECK(CheckColor(Color(7, 0, 0, 0), *dut.Tick())); // => Next 4

    CHECK(CheckColor(Color(7, 0, 0, 0), *dut.Tick())); // <- Switching to Freeze
    CHECK(CheckColor(Color(7, 0, 0, 0), *dut.Tick()));
    CHECK(CheckColor(Color(7, 0, 0, 0), *dut.Tick()));
    CHECK(CheckColor(Color(7, 0, 0, 0), *dut.Tick())); // => Next 5

    CHECK(CheckColor(Color(8, 0, 0, 0), *dut.Tick())); // <- Switching to next waveform
    CHECK(CheckColor(Color(6, 0, 0, 0), *dut.Tick()));
    CHECK(CheckColor(Color(4, 0, 0, 0), *dut.Tick()));
    CHECK(CheckColor(Color(2, 0, 0, 0), *dut.Tick())); // => Next 6

    // Dynamic range is set to 30 internally // @todo change someday
    for (size_t i = 0; i < 16; i++) {
        uint8_t ch = dut.Tick()->GetColor().red;
        // INFO(printf("  %d = %d", i, ch));
        CHECK(((ch >= 0x80) && (ch <= (0x80 + 30))));
    }
}

TEST_CASE("SequenceSM Plots for manual investigation", "[EffectSquenceStateMachine, Constructor]") {
    Sequence effDemo[] = {
        // State      WV                  FS    N   Start         R  Next
        Sequence( Macro_t{Light_Blank, noWave, 0xFF, 4, &color_Red, 0, 1}),
        Sequence( Macro_t{Light_Wave, gau8_fullPulse, 0xFF, 128, oldColor, 0, 2}),
        Sequence( Macro_t{Light_Blank, noWave, 0xFF, 4, oldColor, 0, 3}),
        Sequence( Macro_t{Light_Wave, gau8_fullSlope, 0xFF, 128, oldColor, 0, 4}),
        Sequence( Macro_t{Light_Blank, noWave, 0xFF, 4, oldColor, 0, 5}),
        Sequence( Macro_t{Light_Wave, gau8_initSlope, 0xFF, 128, oldColor, 0, 6}),
        Sequence( Macro_t{Light_Blank, noWave, 0xFF, 4, oldColor, 0, 7}),
        Sequence( Macro_t{Light_Wave, gau8_offsetPulse, 0xFF, 128, oldColor, 0, 8}),
        Sequence( Macro_t{Light_Blank, noWave, 0xFF, 4, oldColor, 0, 9}),
        Sequence( Macro_t{Light_Wave, gau8_offsetSlope, 0xFF, 128, oldColor, 0, 10}),
        Sequence( Macro_t{Light_Blank, noWave, 0xFF, 4, oldColor, 0, 11}),
        Sequence( Macro_t{Light_Flicker, noWave, 0xFF, 64, oldColor, 0, 0}),
    };
    static SequenceSM dut(cu16_TemplateLength, 1, 0x80, 0);
    dut.SetEffect(effDemo);

    std::ofstream plotFile;
    char          stream[100];
    uint8_t       cStream[4];
    plotFile.open(TSTOUTPUT_DIR "EffectSequenceStepper.csv");
    plotFile << "cycle,value" << std::endl;

    for (int i = 0; i < 728; i++) {
        const Color *c = dut.Tick();
        sprintf(stream, "%3d,%d", i, c->GetColor().red);
        plotFile << stream << std::endl;
    }
    plotFile.close();
    REQUIRE(true);
}