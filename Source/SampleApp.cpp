/**
 * @file SampleApp.cpp
 * @author Gustice
 * @brief 
 * @version 0.1
 * @date 2019-09-18
 * 
 * @copyright Copyright (c) 2019
 */

#include "SampleApp.h"
#include <string.h>
#include <stdio.h>
#include <iostream>

using std::string;
using std::cout;

/// Demo Effect Macro
EffMacro_type eff_TestStdPulse[] = {
	{Light_Idle,	    (uint8_t*)0,		0,		32,         &color_ColdWhite,	0,          1},
	{Light_Wave,	    gau8_offsetPulse,	0xFF,	32,         USEOLD_COLOR,		0,          2},
	{Light_Idle,	    (uint8_t*)0,		0,		32,         USEOLD_COLOR,		0,          0},
};

void PrintLine(string output) {cout<<output<<std::endl;}
void PrintColor(string hint, Color_t const c) {
cout<< hint
    << " R:" << (int)c.red 
    << "  G: " << (int)c.green
    << "  B: " << (int)c.blue 
    << "  W: " << (int)c.white << std::endl;
}


static const Color_t constColorStruct = {0x12, 0x34, 0x56, 0x78};
static const Color constColorObjByStruct(constColorStruct);
static const Color constColorObjByParam(0x87, 0x65, 0x43, 0x21);

void ColorDemo(void)
{
    PrintLine("## Color Demo");
    
    static const Color_t c0 = {0x01, 0x02, 0x03, 0x04};
    static const Color_t c1 = {0x11, 0x12, 0x13, 0x14};
    Color colorObj(c0);
    Color_t cr0 = colorObj.GetColor();
    PrintColor("  Dynamic Color:" ,cr0);
    colorObj.SetColor(c1);
    PrintColor("  Dynamic Color new:" ,cr0);

    Color_t cd1 = constColorObjByStruct.GetColor();
    PrintColor("  Static const Color 1:" ,cd1);
    Color_t cd2 = constColorObjByParam.GetColor();
    PrintColor("  Static const Color 2:" ,cd2);

/// @todo Check memory usage and integrity of summands 
    Color mixedColor = constColorObjByParam + constColorObjByStruct;
    
    PrintColor("  Mixed Color:" ,mixedColor.GetColor());
    PrintColor("  Static const Color 1:" ,constColorObjByStruct.GetColor());
    PrintColor("  Static const Color 2:" ,constColorObjByParam.GetColor());
}



static const uint8_t gau8_testWave[] = {0,1,2,3,4,5,6,7,8,9,10,11};
EffMacro_type eff_Demo[] = {
    // EffectState  // WafeformRef  FS-Val  Duration   Color           Repeats  Next
    {Light_Blank,   (uint8_t*)0,    0xFF,   4,         &color_Black,    0,      1},
    {Light_Wave,    gau8_testWave,  0xFF,   6,         &color_Black,    0,      1},
};
static const uint8_t u8_testWaveLen = sizeof(gau8_testWave);

void StateMachinDemo(void)
{
    PrintLine("## StateMachine Demo");

    EffectSM Eff1(u8_testWaveLen,0,3);
    EffectSM Eff2(u8_testWaveLen,0,4);
    Eff1.SetEffect(eff_Demo);
    Eff2.SetEffect(eff_Demo, NO_COLOR, 3);
    
    PrintLine("  No  Effect1            Effect2 (delayed)");
    PrintLine("  No  tick index fade    tick index fade    ");
    char stream[100];    

    for(uint8_t i = 0; i < 16; i++)
    {
        uint8_t tick1 = Eff1.Tick();
        uint8_t tick2 = Eff2.Tick();
        sprintf(stream, "  %2d  %4d  %4d  %3d      %4d  %4d  %3d    " ,i,
            tick1, Eff1.GetWaveformIdx(), Eff1.GetDissolveRatio(),
            tick2, Eff2.GetWaveformIdx(), Eff2.GetDissolveRatio());
        PrintLine(stream);
    }
}


/**
 * @brief   Demo usage of Lightning Library
 * 
 * @details
 * 
 * @param argc  Argument not used
 * @param argv  Argument list not used
 * @return int  Return Value not used
 */
int main(int argc, char const *argv[])
{
    PrintLine("Sample application running");
    PrintLine("");

    ColorDemo();

    StateMachinDemo();

    /* code */
    return 0;
}





/**
 * @file Cpp_Gnu_Template.cpp
 * @author Gustice
 * @brief
 * @date 2019-06-06
 *
 * @copyright Copyright (c) 2019
 *
 * @note Certain preconditions needs to be met:
 *      - Install MinGW form http://www.mingw.org/
 *          Don't forget to create environment variable
 *          You can check the correct installation by typing g++ --version in to command line
 *          See https://code.visualstudio.com/docs/cpp/config-mingw for further documentation
 * 
 *      - c_cpp_properties.json can be generated by:
 *          - Ctrl+Shift+P 
 *          - C/C++: Edit configurations
 *          - Generate c_cpp_properties.json from Cpp_Gnu_Template
 *          - get sure "intelliSenseMode" is set to "clang-x64",
 *            This sets IntelliSens to appropriate Compiler include files
 * 
 *      - tasks.json can be generated by:
 *          - Tasks: Configure Tasks
 *          - Create from templates
 *          - Select: Other
 *      - Project can be built by Command
 *          - Tasks: Run Task
 *          - Select appropriate tasks
 *          - Hit Shortcut Ctrl-Shift-B alternatively
 * 
 *      - launch.json can be generated by:
 *          - Open debugger view
 *          - Click on gear symbol for setup
 *          - Select C++ GDB/LLDB
 *          - Select default configuration
 *      - Debugger can be started 
 *          - Debug: Select and Start Debugging
 *          - Select appropriate tasks
 *          - Hit Shortcut F5 alternatively
 *      - GDB-debug-commands can be used by prepending '-exec'
 *          - Example -exec info breakpoints
 * 
 *      - .gitignore-file can be generated by command prompt in directory
 *          - type following command:
 *              nul>.gitignore
 * 
 *      - .clang-format-file can be generated by command prompt in directory
 *          - type following command:
 *              clang-format -style=llvm -dump-config >.clang-format
 **************************************************************************************************/

/* Additional Hints */
// Ctrl+Shift+O   Search for Symbols in current file (Ctrl+P @)
// Ctrl+T         Search for Symbol in workspace (Ctrl+P #)