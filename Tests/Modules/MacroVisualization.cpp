#include "Macro.h"
#include "Processor.h"
#include "Waveforms.h"
#include "catch.hpp"
#include "MyUtils.h"

using namespace Effect;

struct effectRun_t {
    const char *      Name;
    Macro_t *pMacro;
};

const size_t runCount        = 5;
static effectRun_t  tests[runCount] = {
    {"Dark", macDark},         {"StartIdle", macStartIdle},       {"Idle", macIdle},
    {"StdPulse", macStdPulse}, {"NervousPulse", macNervousPulse},
};
const size_t maxLength = 96;

TEST_CASE("EffectProcessor - Plotting of all predefined effect macros", "[cEffectProcessor]") {
    EffectProcessor * dut[runCount];
    
    std::ofstream plotFile;
    char          stream[128];
    uint8_t       cStream[4];
    plotFile.open(TSTOUTPUT_DIR "BuildInMacros.csv");
    plotFile << "cycle";

    for (size_t part = 0; part < runCount; part++) {
        plotFile << "," << tests[part].Name;

        dut[part] = new EffectProcessor(cu16_TemplateLength, 4);
        dut[part]->SetEffect(tests[part].pMacro, &color_Red, gu8_idleIntensity);
    }
    plotFile << std::endl;

    for (size_t step = 0; step < maxLength; step++) {
        plotFile << step;
        for (size_t part = 0; part < runCount; part++) {
            Color::color_t c = dut[part]->Tick()->GetColor();
            plotFile << "," << (int)c.red;
        }
        plotFile << std::endl;
    }
    plotFile.close();
    REQUIRE(true);
}