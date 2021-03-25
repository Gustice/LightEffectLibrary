/**
 * @file GenerateWaveforms.cpp
 * @author Gustice
 * @brief Generates Template-cpp-files with lookup-tables for waveform templates. 
 *  The waveforms are additionally outputted as data to be plotted with the Gnuplot plot-script.
 * @version 0.6
 * @date 2019-10-14
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "Elements/GaussianWaveforms.h"

// @todo: Templates should be generated according to calling arguments => to be implemented
//  - Length of lookup-tables
//  - Full scale intensity
//  - idle intensity
// @todo: This code mixes c and c++ features and must be refactored. There are also other sins in there.


/**
 * @brief Main (obviously)
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char const *argv[]) {
    PrintHeader("LightEffectFramework Sample Application");

    ofstream plotFile;
    char stream[100];
    uint8_t cStream[4];
    plotFile.open("GraphTemplates.cpp");
    PrintChapter("Generate Gaussian Pulses");

    // @todo struct to be initiated with calling arguments
    static const GW_Config_t GaussianConfig {
        0x80,
        UCHAR_MAX / 3,
        0xFF,
    };
    PrintLine("      With template length = %d",GaussianConfig.length);
    PrintLine("      With idle intensity offset = %d",GaussianConfig.offset);
    PrintLine("      With fullscale intensity = %d",GaussianConfig.fullScale);

    plotFile << "#include <stdint.h>" << endl << endl;
    PrintGaussianWaveForms2File(GaussianConfig, plotFile);
    plotFile.close();

    PrintFooter();
    /* code */
    return 0;
}