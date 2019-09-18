/***********************************************************************************************//**
 * @file SampleApp.cpp
 * @author Gustice
 * @brief 
 * @version 0.1
 * @date 2019-09-18
 * 
 * @copyright Copyright (c) 2019
 **************************************************************************************************/

#include "SampleApp.h"


/// Demo Effekt Macro
EffMacro_type eff_TestStdPulse[] = {
	{Light_Idle,	    (uint8_t*)0,		0,		32,         &color_ColdWhite,	0,          1},
	{Light_Wave,	    gau8_offsetPulse,	0xFF,	32,         USEOLD_COLOR,		0,          2},
	{Light_Idle,	    (uint8_t*)0,		0,		32,         USEOLD_COLOR,		0,          0},
};


/***********************************************************************************************//**
 * @brief   Demo usage of Lightning Library
 * 
 * @details
 * 
 * @param argc  Argument not used
 * @param argv  Argument list not used
 * @return int  Return Value not used
 **************************************************************************************************/
int main(int argc, char const *argv[])
{
    Color r1;
    Color g1;
    Color b1;
    Color all1;

    r1.SetColor(0xFF, 0xFF, 0,0,0);
    g1.SetColor(0xFF, 0, 0xFF, 0,0);
    b1.SetColor(0xFF, 0, 0, 0xFF, 0);

    all1 = all1 + r1;
    all1 = all1 + g1;
    all1 = all1 + b1;


    /// Testing State machine
    EffectMemory Eff1;
    Eff1.SetEffect(eff_TestStdPulse);
    for(uint8_t i = 0; i < 96; i++)
    {
        Eff1.Tick();
    }

    EffectMemory Eff2;
    Eff2.SetEffect(eff_StdPulse);
    for(uint8_t i = 0; i < 96; i++)
    {
        Eff2.Tick();
    }


    /* code */
    return 0;
}
