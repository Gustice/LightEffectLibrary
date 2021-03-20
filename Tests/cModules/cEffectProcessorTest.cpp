
#include "EffectProcessor.h"
#include "EffectWaveforms.h"
#include "MyUtils.h"

static bool CheckColor(Color_t c1, const Color_t c2) {
    if (ColorIsEqual(&c1, c2))
        return true;

    INFO(printf(" rgbw: 0x%02x 0x%02x 0x%02x 0x%02x != 0x%02x 0x%02x 0x%02x 0x%02x\n", c1.red, c1.green, c1.blue,
                c1.white, c2.red, c2.green, c2.blue, c2.white));
    INFO(printf(" rgbw: %d %d %d %d != %d %d %d %d\n", c1.red, c1.green, c1.blue, c1.white, c2.red, c2.green, c2.blue,
                c2.white));
    return false;
}

static const uint8_t gau8_testWave1[]               = {1, 2, 3, 4, 5, 6, 7, 8};
static const uint8_t u8_testWaveLen                 = sizeof(gau8_testWave1);
static const uint8_t gau8_testWave2[u8_testWaveLen] = {11, 21, 31, 41, 51, 61, 71, 81};

static const uint8_t cFullIntensity = 0xFF;

TEST_CASE("Running cEffect Processor with different ChangePoints", "[cEffectProcessor]") {
    EffMacro_t effDemo1[] = {
        // State      WV            FS    N  Start         R  Next
        {Light_Idle, NO_WAVE, 0xFF, 4, &cBlue, 0, 1},
        {Light_Idle, NO_WAVE, 0xFF, 4, USEOLD_COLOR, 0, 0},
    };
    EffMacro_t effDemo2[] = {
        // State      WV            FS    N  Start         R  Next
        {Light_Idle, NO_WAVE, 0xFF, 4, &cGreen, 0, 1},
        {Light_Idle, NO_WAVE, 0xFF, 4, USEOLD_COLOR, 0, 0},
    };
    uint8_t fullIntens = 0xFFu;
    uint8_t halfIntens = 0x80u;

    EffectProcessor_t dut;
    EP_Construct(&dut, u8_testWaveLen, 2);
    EP_SetEffect(&dut, effDemo1, NO_COLOR, &cFullIntensity);

    
    SECTION("Running one step and change first step)") {

        CHECK(CheckColor(Color_t{0, 0, 0x80, 0}, *EP_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *EP_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *EP_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *EP_Tick(&dut)));

        EP_SetEffect(&dut, effDemo2, NO_COLOR, &cFullIntensity);
        CHECK(CheckColor(Color_t{0, 0x80, 0x7F, 0}, *EP_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0xFF, 0, 0}, *EP_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0xFF, 0, 0}, *EP_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0xFF, 0, 0}, *EP_Tick(&dut)));
    }

    SECTION("Running one step and change effect in the middle of second step)") {
        CHECK(CheckColor(Color_t{0, 0, 0x80, 0}, *EP_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *EP_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *EP_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *EP_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *EP_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *EP_Tick(&dut)));
        EP_SetEffect(&dut, effDemo2, NO_COLOR, &cFullIntensity);
        CHECK(CheckColor(Color_t{0, 0x80, 0x7F, 0}, *EP_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0xFF, 0, 0}, *EP_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0xFF, 0, 0}, *EP_Tick(&dut)));
        CHECK(CheckColor(Color_t{0, 0xFF, 0, 0}, *EP_Tick(&dut)));
    }
}

TEST_CASE("Running cEffect Processor with different Setup -> longer CrossFade", "[cEffectProcessor]") {
    EffMacro_t effDemo1[] = {
        // State      WV            FS    N  Start         R  Next
        {Light_Idle, NO_WAVE, 0xFF, 8, &cBlue, 0, 1},
        {Light_Idle, NO_WAVE, 0xFF, 8, USEOLD_COLOR, 0, 0},
    };
    EffMacro_t effDemo2[] = {
        // State      WV            FS    N  Start         R  Next
        {Light_Idle, NO_WAVE, 0xFF, 8, &cGreen, 0, 1},
        {Light_Idle, NO_WAVE, 0xFF, 8, USEOLD_COLOR, 0, 0},
    };
    uint8_t fullIntens = 0xFFu;
    uint8_t halfIntens = 0x80u;

    EffectProcessor_t dut;
    EP_Construct(&dut, u8_testWaveLen, 4);
    EP_SetEffect(&dut, effDemo1, NO_COLOR, &cFullIntensity);

    CHECK(CheckColor(Color_t{0, 0, 0x40, 0}, *EP_Tick(&dut)));
    CHECK(CheckColor(Color_t{0, 0, 0x80, 0}, *EP_Tick(&dut)));
    CHECK(CheckColor(Color_t{0, 0, 0xC0, 0}, *EP_Tick(&dut)));
    CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *EP_Tick(&dut)));
    CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *EP_Tick(&dut)));
    CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *EP_Tick(&dut)));
    CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *EP_Tick(&dut)));
    CHECK(CheckColor(Color_t{0, 0, 0xFF, 0}, *EP_Tick(&dut)));
    EP_SetEffect(&dut, effDemo2, NO_COLOR, &cFullIntensity);
    CHECK(CheckColor(Color_t{0, 0x40, 0xBF, 0}, *EP_Tick(&dut)));
    CHECK(CheckColor(Color_t{0, 0x80, 0x7F, 0}, *EP_Tick(&dut)));
    CHECK(CheckColor(Color_t{0, 0xC0, 0x3F, 0}, *EP_Tick(&dut)));
    CHECK(CheckColor(Color_t{0, 0xFF, 0, 0}, *EP_Tick(&dut)));
}

TEST_CASE("cEffectProcessor plots for manual evaluation", "[cEffectProcessor, Constructor]") {
    EffMacro_t effDemo1[] = {
        // State      WV            FS    N  Start         R  Next
        {Light_Blank, NO_WAVE, 0xFF, 8, &cRed, 0, 1},
        {Light_Wave, gau8_initSlope, 0xFF, 32, USEOLD_COLOR, 0, 2},
        {Light_Wave, gau8_offsetPulse, 0xFF, 64, USEOLD_COLOR, 0, 3},
        {Light_Idle, NO_WAVE, 0xFF, 62, USEOLD_COLOR, 0, 2},
    };
    EffMacro_t effDemo2[] = {
        // State      WV            FS    N  Start         R  Next
        {Light_Blank, NO_WAVE, 0xFF, 4, &cGreen, 0, 1},
        {Light_Wave, gau8_fullPulse, 0xFF, 16, USEOLD_COLOR, 3, 2},
        {Light_Blank, NO_WAVE, 0xFF, 64, &cGreen, 0, 1},
    };
    uint8_t intens = 0x55;

    EffectProcessor_t dut;
    EP_Construct(&dut, cu16_TemplateLength, 16);
    EP_SetEffect(&dut, effDemo1, NO_COLOR, &intens);

    std::ofstream plotFile;
    char          stream[128];
    uint8_t       cStream[4];
    plotFile.open(TSTOUTPUT_DIR "cEffectProcessorRun.csv");
    plotFile << "cycle,R,G,B,W,RGB" << std::endl;

    int i = 0;
    for (; i < 128; i++) {
        Color_t c = *EP_Tick(&dut);
        sprintf(stream, "%3d,%3.2f,%3.2f,%3.2f,%3.2f,%3.2f", i, (float)c.red / (4*0xFF), (float)c.green / (4*0xFF),
                (float)c.blue / (4*0xFF), (float)c.white / (4*0xFF),
                (float)((uint16_t)c.red + c.green + c.blue) / (4*0xFF/3));
        plotFile << stream << std::endl;
    }

    EP_SetEffect(&dut, effDemo2, NO_COLOR, &intens);
    for (; i < (128 + 64); i++) {
        Color_t c = *EP_Tick(&dut);
        sprintf(stream, "%3d,%3.2f,%3.2f,%3.2f,%3.2f,%3.2f", i, (float)c.red / (4*0xFF), (float)c.green / (4*0xFF),
                (float)c.blue / (4*0xFF), (float)c.white / (4*0xFF),
                (float)((uint16_t)c.red + c.green + c.blue) / (4*0xFF/3));
        plotFile << stream << std::endl;
    }

    plotFile.close();
    REQUIRE(true);
}
