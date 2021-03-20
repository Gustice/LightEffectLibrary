#pragma once 

class RamClass
{
private:
    const int var1;
    const int var2;
public:
    RamClass(int a1, int a2) :var1(a1), var2(a2) {};
    ~RamClass() {};

    int GetVar1() const {return var1;} 
    int GetVar2() const {return var2;} 
};

class RomClass
{
private:
    const int var1;
    const int var2;
public:
    constexpr RomClass(int a1, int a2) :var1(a1), var2(a2) {};
    ~RomClass() {};

    int GetVar1() const {return var1;} 
    int GetVar2() const {return var2;} 
};