
#include "GaussianWaveforms.h"
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>

vector<int> pulse;
vector<int> fullPulse;
vector<int> slope;
vector<int> initslope;
vector<int> fullSlope;

const int maxValue = UCHAR_MAX;

void GenerateGausianPulse(int length, double peakForm, int offset);
void PrintValues(string varName, vector<int> values, ofstream &outputFile, bool doubleLen = false);

void PrintGaussianWaveForms2File(GW_Config_t config, ofstream &outputFile) {
    int  length = config.templateLength;
    int  offset = config.offset;
    char sStream[200];

    vector<int> p(length);

    pulse.resize(length);
    slope.resize(length);
    initslope.resize(length);
    fullPulse.resize(length);
    fullSlope.resize(length);

	PrintSection("Generate Pulse Byte-Streams");
    GenerateGausianPulse(length, 2.3, offset);

    sprintf(sStream, "#define  PULSE_TEMPLATE_LEN    %d", length);
    outputFile << sStream << endl;
    sprintf(sStream, "const uint8_t gu8_idleIntensity = 0x%02X;", offset);
    outputFile << sStream << endl;

	PrintSection("Safe offset Pulse");
	outputFile << endl;
	outputFile << "// Full Pulse: Gaussian pulse from background illumination with full scale intensity" << endl;
    PrintValues("gau8_offsetPulse", pulse, outputFile, true);

	PrintSection("Safe init Pulse");
	outputFile << endl;
	outputFile << "// Init Slope: Gaussian slope from dark to background brightness" << endl;
    PrintValues("gau8_initSlope", initslope, outputFile);
	
	PrintSection("Safe offset Slope");
	outputFile << endl;
	outputFile << "// Full Slope: Gaussian slope from background illumination to full scale brightness" << endl;
    PrintValues("gau8_offsetSlope", slope, outputFile);

	PrintSection("Safe full Slope");
	outputFile << endl;
	outputFile << "// Offset Pulse:Gaussian slope from dark to full scale brightness" << endl;
    PrintValues("gau8_fullSlope", fullSlope, outputFile);

	PrintSection("Safe full Pulse");
	outputFile << endl;
	outputFile << "// Offset Pulse: Gaussian pulse from dark to full scale intensity" << endl;
    PrintValues("gau8_fullPulse", fullPulse, outputFile);
}

void PrintValues(string varName, vector<int> values, ofstream &outputFile, bool doubleLen) {
    int  idx = 0;
    char sStream[200];

    if (doubleLen)
        outputFile << "const uint8_t " << varName << "[PULSE_TEMPLATE_LEN *2] = {" << endl;
    else
        outputFile << "const uint8_t " << varName << "[PULSE_TEMPLATE_LEN] = {" << endl;

    while (idx < values.size()) {
        sprintf(sStream, "0x%02X, ", values[idx++]);
        outputFile << sStream;
        if ((idx % 8) == 0)
            outputFile << endl;
    }
    if (doubleLen) {
        idx = 0;
        outputFile << endl;
        while (idx < values.size()) {
            sprintf(sStream, "0x%02X, ", values[idx++]);
            outputFile << sStream;
            if ((idx % 8) == 0)
                outputFile << endl;
        }
    }

    outputFile << "};" << endl;

    vector<int> xSeries(values.size());
    for (int i = 0; i < values.size(); i++)
        xSeries[i] = i;

    int  n = varName.length();
    char varNameArray[n + 1];
    strcpy(varNameArray, varName.c_str());

    sprintf(sStream, "%s_Plot.txt", varNameArray);

    ofstream plotFile;
    plotFile.open(sStream);
    plotFile << "#Cycl value" << std::endl;

    for (int i = 0; i < values.size(); i++) {
        plotFile << i << values[i] << std::endl;
    }

    plotFile.close();
}

void GenerateGausianPulse(int length, double peakForm, int offset) {
    int temporaryMax = 0;
    int idx          = 0;
    int midLen       = pulse.size() / 2;

    // Generates Full-Pulse around middle position
    for (int i = -midLen; i < midLen; i++) {
        fullPulse[idx] = (int)(exp(-pow(peakForm * i / midLen, 2)) * maxValue);
        idx++;
    }
    idx = 0;

    // Generates pulse around middle position
    for (int i = -midLen; i < midLen; i++) {
        pulse[idx]   = (int)(exp(-pow(peakForm * i / midLen, 2)) * maxValue);
        temporaryMax = std::max(temporaryMax, pulse[idx]);
        idx++;
    }

    for (int i = 1; i < length; i++) {
        slope[i]     = slope[i - 1] + pulse[i];
        fullSlope[i] = fullSlope[i - 1] + fullPulse[i];
    }

    // Normalize Pulse to maximum value by considering demanded offset value
    for (int i = 0; i < pulse.size(); i++) {
        pulse[i]     = pulse[i] * (maxValue - offset) / temporaryMax + offset;
        slope[i]     = slope[i] * (maxValue - offset) / slope[slope.size() - 1] + offset;
        fullSlope[i] = fullSlope[i] * (maxValue) / fullSlope[slope.size() - 1];
    }
    for (int i = 0; i < pulse.size(); i++)
        initslope[i] = ((slope[i] - offset) * offset) / (slope[slope.size() - 1] - offset);
}