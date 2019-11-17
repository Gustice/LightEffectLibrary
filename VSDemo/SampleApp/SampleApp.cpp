// SampleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "MyUtils.h"

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


#include "Elements/ColorDemo.h"
#include "Elements/EffectDemo.h"
#include "Elements/MyUtils.h"
#include "Elements/StateMachineDemo.h"


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
    // StateMachinDemo();

    PrintChapter("Effect-Processor Demo");
    // EffectProcessorDemo();

    PrintChapter("Sample-Application Demo");
    SampleApp();

    PrintFooter();
    /* code */
    return 0;
}






void SampleApp(void) {


}