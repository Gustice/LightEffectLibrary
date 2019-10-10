
#include "ColorDemo.h"

void ColorDemo(void) {
    PrintLine("## Color Demo");

    static const Color_t c0 = {0x01, 0x02, 0x03, 0x04};
    static const Color_t c1 = {0x11, 0x12, 0x13, 0x14};
    static Color                colorObj(c0);
    Color_t              cr0 = colorObj.GetColor();
    PrintColor("  Dynamic Color:", cr0);
    colorObj.SetColor(c1);
    PrintColor("  Dynamic Color new:", cr0);

    Color_t cd1 = constColorObjByStruct.GetColor();
    PrintColor("  Static const Color 1:", cd1);
    Color_t cd2 = constColorObjByParam.GetColor();
    PrintColor("  Static const Color 2:", cd2);

    /// @todo Check memory usage and integrity of summands
    Color mixedColor = constColorObjByParam + constColorObjByStruct;

    PrintColor("  Mixed Color:", mixedColor.GetColor());
    PrintColor("  Static const Color 1:", constColorObjByStruct.GetColor());
    PrintColor("  Static const Color 2:", constColorObjByParam.GetColor());
}