
#include "include/Classes.h"
#include "include/Structures.h"
#include <assert.h>

RamType_t ramVar = {1, 2};
RomType_t romVar = {11, 12};

RamClass       ramObj(1, 2);
const RomClass romObj(11, 12);

void SomeFunction() {
    int i1 = ramVar.Val1;
    int i2 = romVar.Val1;

    int i3 = ramObj.GetVar1();
    int i4 = romObj.GetVar1();
}

int main(int argc, char const *argv[]) {
    SomeFunction();

    ramVar.Val1++;
    return 0;
}
