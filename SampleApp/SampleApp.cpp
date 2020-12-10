/**
 * @file SampleApp.cpp
 * @author Gustice
 * @brief
 * @version 0.1
 * @date 2019-09-18
 *
 * @copyright Copyright (c) 2019
 * 
 * @note CodeConventions for this Project:
 *  -# Local Clang-Format stale sheet is to be used. Once the style Sheet is updated, all source 
 *      files must be reformated und committed.
 *  -# Comments are placed where the Implementation is. Its justs for that reason because the tools 
 *      that i use tend to jump to implementation and not to the declaration.
 */

// https://www.reddit.com/r/cpp/comments/af74l1/recommendations_for_setting_up_a_modern_c_dev/
// https://codereview.stackexchange.com/questions/189489/cmake-project-template-for-a-general-cross-platform-c14-project

#include "Elements/ColorDemo.h"
#include "Elements/EffectDemo.h"
#include "Elements/MyUtils.h"
#include "Elements/StateMachineDemo.h"

// Ctrl+Shift+O   Search for Symbols in current file (Ctrl+P @)
// Ctrl+T         Search for Symbol in workspace (Ctrl+P #)

void SampleApp(void);

/**
 * @brief   Calls of demo usage of lightning library and library parts
 *
 * @details
 *
 * @param argc  Argument not used
 * @param argv  Argument list not used
 * @return int  Return Value not used
 */
int main(int argc, char const *argv[]) {
    PrintHeader("LightEffectFramework Sample Application");

    PrintChapter("Color Demo");
    ColorDemo();

    PrintChapter("State-Machine Demo");
    StateMachinDemo();
    SequenceMachinDemo();

    PrintChapter("Effect-Processor Demo");
    EffectProcessorDemo();

    PrintChapter("Sample-Application Demo");


    PrintFooter();
    /* code */
    return 0;
}


void SampleApp(void) {

}