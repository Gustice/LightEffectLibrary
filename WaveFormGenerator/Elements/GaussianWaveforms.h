
#pragma once

#include "WGUtils.h"
#include <limits.h>

typedef struct GW_Config_def {
    int length;
    int offset;
    int fullScale;
} GW_Config_t;

void PrintGaussianWaveForms2File(GW_Config_t config, ofstream &plotFile);
