#include "include/PointerClass.h"
#include "include/ValueClass.h"
#include "include/ReferenceClass.h"
#include <cassert>
#include <stdio.h>
#include <string.h>

void UsageOfValueTypes(void) {
    ColorV c1;
    ColorV c2(11,12,13);
    c1.SetColor(1, 2, 3);

    ColorV c3 = c1 + c2;
    assert( c3 == ColorV(12,14,16));
}

void UsageOfReferenceTypes(void) {
    ColorR c1;
    ColorR c2(11,12,13);
    c1.SetColor(1, 2, 3);

    ColorR c3 = c1 + c2; // Manipulates also C1
    assert( c3 == ColorR(12,14,16));
}

void UsageOfPointerTypes(void) {
    ColorP c1b;
    ColorP c2b;
    
    ColorP * c1 = &c1b;
    ColorP * c2 = &c2b;
    c1->SetColor(1, 2, 3);
    c2->SetColor(11,12,13);

    ColorP * c3 = (*c1) + c2;

    ColorP v3 = ColorP(12,14,16);
    assert( *c3 == &v3);
}


int main(int argc, char const *argv[]) {
    UsageOfValueTypes();
    UsageOfReferenceTypes();
    UsageOfPointerTypes();
    return 0;
}

static bool CheckColor(ColorV c1, ColorV c2) {
    if (c1 == c2)
        return true;

    Color_t v1 = c1.GetColor();
    Color_t v2 = c2.GetColor();
    printf(" rgb: 0x%02x 0x%02x 0x%02x != 0x%02x 0x%02x 0x%02x \n", v1.red, v1.green, v1.blue, v2.red, v2.green,
           v2.blue);
    return false;
}

