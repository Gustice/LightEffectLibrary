
#include "MyUtils.h"
#include "EffectProcessor.h"

using namespace Effect;

static const uint8_t gau8_showWave[] = {0, 255/12, 255*2/12, 255*3/12, 255*4/12, 255*5/12, 
                                        255*6/12, 255*7/12, 255*8/12, 255*9/12, 255*10/12, 255*11/12};
EffMacro_t        eff_ShowDemo[]      = {
    // EffectState  // WaveformRef  FS-Val  Duration   Color           Repeats  Next
    {Light_Blank, (uint8_t *)0, 0xFF, 2, &color_Black, 0, 1},
    {Light_Wave, gau8_showWave, 0xFF, 12, &color_White, 0, 1},
};
static const uint8_t u8_showWaveLen = sizeof(gau8_showWave);

TEST_CASE("Constructing EffectProcessor", "[EffectProcessor, Constructor]") {
    EffectProcessor dut(u8_showWaveLen, 3);
    dut.SetEffect(eff_ShowDemo);
    REQUIRE(true);
}



TEST_CASE("Running Effect Processor with different Setups", "[EffectProcessor]") {
    SECTION("Running with different intensities(applies for idle state)") {
        EffMacro_t effDemo[] = {
            // State      WV            FS    N  Start         R  Next
            {Light_Idle, NO_WAVE, 0xFF, 4, &color_Blue, 0, 1},
            {Light_Idle, NO_WAVE, 0xFF, 4, &color_Green, 0, 2},
        };
        uint8_t fullIntens = 0xFFu;
        uint8_t halfIntens = 0x80u;

        SECTION("Running with full intensity(applies for idle state)") {
            static EffectSM dut(u8_testWaveLen, 0xFF, 0);
            dut.SetEffect(effDemo);

            CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0xFF, 0, 0), *dut.Tick())); // <- Switching to next waveform
        }

        SECTION("Running with half default idle intensity(applies for idle state)") {
            static EffectSM dut(u8_testWaveLen, 0x80, 0);
            dut.SetEffect(effDemo);

            CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0x80, 0, 0), *dut.Tick())); // <- Switching to next waveform
        }
        SECTION("Define other start intensity") {
            static EffectSM dut(u8_testWaveLen, 0xFF, 0);
            dut.SetEffect(effDemo, NO_COLOR, &halfIntens, 0);
            CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
            dut.Tick();
            dut.Tick();
            CHECK(CheckColor(Color(0, 0, 0x80, 0), *dut.Tick()));
            CHECK(CheckColor(Color(0, 0x80, 0, 0), *dut.Tick())); // <- Switching to next waveform
        }
    }
    SECTION("Starting Effect with different colors") {
        EffMacro_t effDemo[] = {
            // State     WV       FS    N  Start         R  Next
            {Light_Idle, NO_WAVE, 0xFF, 4, &color_Blue, 0, 1},
            {Light_Idle, NO_WAVE, 0xFF, 4, &color_Green, 0, 2},
            {Light_Idle, NO_WAVE, 0xFF, 4, &color_Red, 0, 0},
        };

        static EffectSM dut(u8_testWaveLen, 0xFF, 0);
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
        EffMacro_t effDemo[] = {
            // State      WV            FS    N  Start         R  Next
            {Light_Idle, NO_WAVE, 0xFF, 4, &color_Blue, 0, 1},
            {Light_Idle, NO_WAVE, 0xFF, 4, &color_Green, 0, 2},
        };
        static EffectSM dut(u8_testWaveLen, 0xFF, 0);
        dut.SetEffect(effDemo, NO_COLOR, 2);
        CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        dut.Tick();
        dut.Tick();
        CHECK(CheckColor(Color(0, 0, 0xFF, 0), *dut.Tick()));
        CHECK(CheckColor(Color(0, 0xFF, 0, 0), *dut.Tick())); // <- Switching to next waveform
    }
}










TEST_CASE("Running EffectProcessor", "[EffectProcessor, Constructor]") {
    EffectProcessor EP1(u8_showWaveLen, 3);
    EP1.SetEffect(eff_ShowDemo);

    std::ofstream plotFile;
    char stream[100];
    uint8_t cStream[4];
    plotFile.open(TSTOUTPUT_DIR "EffectProcessorRun.csv");
    plotFile << "#cycle,abs,B,G,R,W"  << std::endl;

    for(uint8_t i = 0; i < 20; i++)
    {
        EP1.Tick();
        EP1._pColor.WriteByteStreamTo(cStream);

        sprintf(stream, "%3d,%3.2f,%3.2f,%3.2f,%3.2f",
            i, 
            (float)cStream[0]/(4*0xFF),
            (float)cStream[1]/(4*0xFF),
            (float)cStream[2]/(4*0xFF),
            (float)cStream[3]/(4*0xFF)
            );
     plotFile << stream << std::endl;
    }
    plotFile.close();
    REQUIRE(true);
}

TEST_CASE("Setting EffectProcessor by Parameter or by Structure", "[EffectStateMachine, Constructor]") {

    SECTION("Setting EffectProcessor Object by Parameter") {
    }
    SECTION("Setting EffectProcessor Object by Structure") {
    }
}



