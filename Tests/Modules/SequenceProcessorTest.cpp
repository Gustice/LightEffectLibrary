
#include "Sequencer.h"
#include "Waveforms.h"
#include "MyUtils.h"

using namespace Effect;

TEST_CASE("Constructing EffectSequencer", "[EffectSequencer, Constructor]") {
    EffectSequencer dut(12, 1, 3);
    REQUIRE(true);
}

static bool CheckColor(Color c1, Color c2) {
    if (c1 == c2)
        return true;

    Color::color_t v1 = c1.GetColor();
    Color::color_t v2 = c2.GetColor();
    INFO(printf(" rgbw: 0x%02x 0x%02x 0x%02x 0x%02x != 0x%02x 0x%02x 0x%02x 0x%02x\n", v1.red, v1.green, v1.blue,
                v1.white, v2.red, v2.green, v2.blue, v2.white));
    INFO(printf(" rgbw: %d %d %d %d != %d %d %d %d\n", v1.red, v1.green, v1.blue, v1.white, v2.red, v2.green, v2.blue,
                v2.white));
    return false;
}

static const uint8_t gau8_testWave1[]               = {1, 2, 3, 4, 5, 6, 7, 8};
static const uint8_t u8_testWaveLen                 = sizeof(gau8_testWave1);
static const uint8_t gau8_testWave2[u8_testWaveLen] = {11, 21, 31, 41, 51, 61, 71, 81};

TEST_CASE("Running Effect Sequencer with different ChangePoints", "[EffectSequencer]") {
    Sequence effDemo1[] = {
        // State      WV            FS    N  Start         R  Next
        Sequence(Macro_t{Light_Idle, noWave, 0xFF, 4, &color_Blue, 0, 1}),
        Sequence(Macro_t{Light_Idle, noWave, 0xFF, 4, oldColor, 0, 0}),
    };
    Sequence effDemo2[] = {
        // State      WV            FS    N  Start         R  Next
        Sequence(Macro_t{Light_Idle, noWave, 0xFF, 4, &color_Green, 0, 1}),
        Sequence(Macro_t{Light_Idle, noWave, 0xFF, 4, oldColor, 0, 0}),
    };
    uint8_t fullIntens = 0xFFu;
    uint8_t halfIntens = 0x80u;

    EffectSequencer dut(u8_testWaveLen, 1, 2);
    dut.SetEffect(effDemo1, noColor, 0xFF);

    SECTION("Running one step and change first step)") {

        CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        dut.SetEffect(effDemo2, noColor, 0xFF);
        CHECK(CheckColor(Color(0, 0x80, 0x7F, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0xFF, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0xFF, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0xFF, 0, 0), *dut.Tick()));
    }

    SECTION("Running one step and change effect in the middle of second step)") {
        CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        dut.SetEffect(effDemo2, noColor, 0xFF);
        CHECK(CheckColor(Color(0, 0x80, 0x7F, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0xFF, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0xFF, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0xFF, 0, 0), *dut.Tick()));
    }
}

TEST_CASE("Running Effect Sequencer with different Setup -> longer CrossFade", "[EffectSequencer]") {
    Sequence effDemo1[] = {
        // State      WV            FS    N  Start         R  Next
        Sequence( Macro_t{Light_Idle, noWave, 0xFF, 8, &color_Blue, 0, 1}),
        Sequence( Macro_t{Light_Idle, noWave, 0xFF, 8, oldColor, 0, 0}),
    };
    Sequence effDemo2[] = {
        // State      WV            FS    N  Start         R  Next
        Sequence( Macro_t{Light_Idle, noWave, 0xFF, 8, &color_Green, 0, 1}),
        Sequence( Macro_t{Light_Idle, noWave, 0xFF, 8, oldColor, 0, 0}),
    };
    uint8_t fullIntens = 0xFFu;
    uint8_t halfIntens = 0x80u;

    EffectSequencer dut(u8_testWaveLen, 1, 4);
    dut.SetEffect(effDemo1, noColor, 0xFF);

    CHECK(CheckColor(Color(0, 0, 0x40, 0), *dut.Tick()));
    CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
    CHECK(CheckColor(Color(0, 0, 0xC0, 0), *dut.Tick()));
    CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
    dut.SetEffect(effDemo2, noColor, 0xFF);
    CHECK(CheckColor(Color(0, 0x40, 0xBF, 0), *dut.Tick()));
    CHECK(CheckColor(Color(0, 0x80, 0x7F, 0), *dut.Tick()));
    CHECK(CheckColor(Color(0, 0xC0, 0x3F, 0), *dut.Tick()));
    CHECK(CheckColor(Color(0, 0xFF, 0, 0), *dut.Tick()));
}


TEST_CASE("Running Effect Sequencer with custom Effects", "[EffectSequencer]") {
    Sequence effDemo1[] = {
        // State      WV            FS    N  Start         R  Next
        Sequence( 2, 1, Light_Idle, &color_Red),
        Sequence( 4, 2, LightSwipeSequence, gau8_testWave1),
        Sequence( 2, 1, Light_Freeze),
    };
    uint8_t fullIntens = 0xFFu;
    uint8_t halfIntens = 0x80u;

    EffectSequencer dut(u8_testWaveLen, 2, 0);
    dut.SetEffect(effDemo1, noColor, 0xFF);

    const Color * c;
    c = dut.Tick();
    CHECK(CheckColor(Color(0xFF, 0, 0, 0), c[0] ));
    CHECK(CheckColor(Color(0xFF, 0, 0, 0), c[1] ));
    dut.Tick();
    
    c = dut.Tick();
    CHECK(CheckColor(Color(1, 0, 0, 0), c[0]));
    CHECK(CheckColor(Color(8, 0, 0, 0), c[1]));
    c = dut.Tick();
    CHECK(CheckColor(Color(3, 0, 0, 0), c[0]));
    CHECK(CheckColor(Color(6, 0, 0, 0), c[1]));
    c = dut.Tick();
    CHECK(CheckColor(Color(5, 0, 0, 0), c[0]));
    CHECK(CheckColor(Color(4, 0, 0, 0), c[1]));
    c = dut.Tick();
    CHECK(CheckColor(Color(7, 0, 0, 0), c[0]));
    CHECK(CheckColor(Color(2, 0, 0, 0), c[1]));

    c = dut.Tick();
    CHECK(CheckColor(Color(7, 0, 0, 0), c[0]));
    CHECK(CheckColor(Color(2, 0, 0, 0), c[1]));
}




TEST_CASE("EffectSequencer plots for manual evaluation", "[EffectSequencer, Constructor]") {
    Sequence effDemo1[] = {
        // State      WV            FS    N  Start         R  Next
        Sequence( Macro_t{Light_Blank, noWave, 0xFF, 8, &color_Red, 0, 1}),
        Sequence( Macro_t{Light_Wave, gau8_initSlope, 0xFF, 32, oldColor, 0, 2}),
        Sequence( Macro_t{Light_Wave, gau8_offsetPulse, 0xFF, 64, oldColor, 0, 3}),
        Sequence( Macro_t{Light_Idle, noWave, 0xFF, 62, oldColor, 0, 2}),
    };
    Sequence effDemo2[] = {
        // State      WV            FS    N  Start         R  Next
        Sequence( Macro_t{Light_Blank, noWave, 0xFF, 4, &color_Green, 0, 1}),
        Sequence( Macro_t{Light_Wave, gau8_fullPulse, 0xFF, 16, oldColor, 3, 2}),
        Sequence( Macro_t{Light_Blank, noWave, 0xFF, 64, &color_Green, 0, 1}),
    };

    EffectSequencer dut(cu16_TemplateLength, 1, 16);
    dut.SetEffect(effDemo1, noColor, 0x55);

    std::ofstream plotFile;
    char          stream[128];
    uint8_t       cStream[4];
    plotFile.open(TSTOUTPUT_DIR "EffectSequencerRun.csv");
    plotFile << "cycle,R,G,B,W,RGB" << std::endl;

    int i = 0;
    for (; i < 128; i++) {
        Color::color_t c = dut.Tick()->GetColor();
        sprintf(stream, "%3d,%3.2f,%3.2f,%3.2f,%3.2f,%3.2f", i, (float)c.red / (4 * 0xFF), (float)c.green / (4 * 0xFF),
                (float)c.blue / (4 * 0xFF), (float)c.white / (4 * 0xFF),
                (float)((uint16_t)c.red + c.green + c.blue) / (4 * 0xFF / 3));
        plotFile << stream << std::endl;
    }

    dut.SetEffect(effDemo2, noColor, 0x55);
    for (; i < (128 + 64); i++) {
        Color::color_t c = dut.Tick()->GetColor();
        sprintf(stream, "%3d,%3.2f,%3.2f,%3.2f,%3.2f,%3.2f", i, (float)c.red / (4 * 0xFF), (float)c.green / (4 * 0xFF),
                (float)c.blue / (4 * 0xFF), (float)c.white / (4 * 0xFF),
                (float)((uint16_t)c.red + c.green + c.blue) / (4 * 0xFF / 3));
        plotFile << stream << std::endl;
    }

    plotFile.close();
    REQUIRE(true);
}
