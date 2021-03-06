#include "ColorDemo.h"

static const Color_t constColorStruct = {0x12, 0x34, 0x56, 0x78};
static const Color   constColorObjByStruct(constColorStruct);
static const Color   constColorObjByParam(0x87, 0x65, 0x43, 0x21);

void ColorDemo(void) {
    PrintSection("Dynamic Color");

    static const Color_t c0 = {0x01, 0x02, 0x03, 0x04};
    static const Color_t c1 = {0x11, 0x12, 0x13, 0x14};
    static Color         colorObj(c0);
    Color_t              cr0 = colorObj.GetColor();
    PrintColor("  Dynamic Color:", cr0);
    colorObj.SetColor(c1);
    PrintColor("  Dynamic Color new:", cr0);

    Color_t cd1 = constColorObjByStruct.GetColor();
    PrintColor("  Static const Color 1:", cd1);
    Color_t cd2 = constColorObjByParam.GetColor();
    PrintColor("  Static const Color 2:", cd2);

    Color mixedColor = constColorObjByParam + constColorObjByStruct;

    PrintColor("  Mixed Color:", mixedColor.GetColor());
    PrintColor("  Static const Color 1:", constColorObjByStruct.GetColor());
    PrintColor("  Static const Color 2:", constColorObjByParam.GetColor());
}

void PrintColor(const char *hint, Color_t const c) { 
	PrintLine("%s   R: %3d   G: %3d   B: %3d   W: %3d", hint, c.red, c.green, c.blue, c.white); 
}
