#include "EffectMacro.h"
#include "EffectProcessor.h"
#include "EffectWaveforms.h"
#include "MyUtils.h"
#include "catch.hpp"

struct effectRun_t {
    const char *      Name;
    EffMacro_t *pMacro;
};

const size_t runCount        = 5;
static effectRun_t  tests[runCount] = {
    {"Dark", eff_Dark},         {"StartIdle", eff_StartIdle},       {"Idle", eff_Idle},
    {"StdPulse", eff_StdPulse}, {"NervousPulse", eff_NervousPulse},
};
const size_t maxLength = 96;

TEST_CASE("cEffectProcessor - Plotting of all predefined effect macros", "[cEffectProcessor]") {
    EffectProcessor_t dut[runCount];
    
    std::ofstream plotFile;
    char          stream[128];
    uint8_t       cStream[4];
    plotFile.open(TSTOUTPUT_DIR "cBuildInMacros.csv");
    plotFile << "cycle";

    for (size_t part = 0; part < runCount; part++) {
        plotFile << "," << tests[part].Name;

        EP_Construct(&dut[part], cu16_TemplateLength, 4);
        EP_SetEffect(&dut[part], tests[part].pMacro, &cRed, &gu8_idleIntensity);
    }
    plotFile << std::endl;

    for (size_t step = 0; step < maxLength; step++) {
        plotFile << step;
        for (size_t part = 0; part < runCount; part++) {
            Color_t c = *EP_Tick(&dut[part]);
            plotFile << "," << (int)c.red;
        }
        plotFile << std::endl;
    }
    plotFile.close();
    REQUIRE(true);
}