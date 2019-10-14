
#include "Elements/GaussianWaveforms.h"

int main(int argc, char const *argv[]) {
    PrintHeader("LightEffectFramework Sample Application");

    ofstream plotFile;
    char stream[100];
    uint8_t cStream[4];
    plotFile.open("GraphTemplates.cpp");
    PrintChapter("Generate Gaussian Pulses");
    static const GW_Config_t GaussianConfig {
        128,
        UCHAR_MAX / 3,
    };
    PrintLine("      With length = %d",GaussianConfig.templateLength);
    PrintLine("      With offset = %d",GaussianConfig.offset);

    plotFile << "#include <stdint.h>" << endl << endl;

    PrintGaussianWaveForms2File(GaussianConfig, plotFile);
    plotFile.close();


    PrintFooter();
    /* code */
    return 0;
}