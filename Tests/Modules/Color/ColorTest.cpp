
#include "Color.h"
#include "catch.hpp"

TEST_CASE("Constructing emtpy Color Object", "[Color, Constructing]") {
    Color     dut  = Color();
    const int size = sizeof(Color_t);
    uint8_t   stream[size];
    dut.WriteByteStreamTo(stream);

    for (size_t i = 0; i < size; i++) {
        REQUIRE(stream[i] == 0);
    }
}

TEST_CASE("Constructing Color Object by Parameter", "[Color, Constructing]") {
    const int size            = sizeof(Color_t);
    uint8_t   setStream[size] = {0x12, 0x34, 0x56, 0x78};

    Color   dut = Color(setStream[0], setStream[1], setStream[2], setStream[3]);
    Color_t c   = dut.GetColor();

    REQUIRE(c.red == setStream[0]);
    REQUIRE(c.green == setStream[1]);
    REQUIRE(c.blue == setStream[2]);
    REQUIRE(c.white == setStream[3]);
}

TEST_CASE("Constructing Color Object by structure", "[Color, Constructing]") {
    const int size            = sizeof(Color_t);
    uint8_t   setStream[size] = {0x23, 0x45, 0x67, 0x89};
    Color_t   setColor;
    setColor.red   = setStream[0];
    setColor.green = setStream[1];
    setColor.blue  = setStream[2];
    setColor.white = setStream[3];

    Color   dut = Color(setColor);
    Color_t c   = dut.GetColor();

    REQUIRE(c.red == setStream[0]);
    REQUIRE(c.green == setStream[1]);
    REQUIRE(c.blue == setStream[2]);
    REQUIRE(c.white == setStream[3]);
}

TEST_CASE("White Channel stays zero if white parameter is omitted", "[Color, Constructing]") {
    const int size            = sizeof(Color_t);
    uint8_t   setStream[size] = {0x9A, 0xBC, 0xDE, 0xF1};

    Color   dut = Color(setStream[0], setStream[1], setStream[2]);
    Color_t c   = dut.GetColor();

    REQUIRE(c.red == setStream[0]);
    REQUIRE(c.green == setStream[1]);
    REQUIRE(c.blue == setStream[2]);
    REQUIRE(c.white == 0);
}

TEST_CASE("Conservate format of WriteByteStreamTo", "[Color, Format]") {
    const int size            = sizeof(Color_t);
    uint8_t   setStream[size] = {0x12, 0x34, 0x56, 0x78};

    Color_t setColor;
    setColor.red   = setStream[0];
    setColor.green = setStream[1];
    setColor.blue  = setStream[2];
    setColor.white = setStream[3];

    uint8_t stream[size];
    Color   dut = Color(setColor);
    dut.WriteByteStreamTo(stream);

    Color_t c = dut.GetColor();
    REQUIRE(c.red == setStream[0]);
    REQUIRE(c.green == setStream[1]);
    REQUIRE(c.blue == setStream[2]);
    REQUIRE(c.white == setStream[3]);

    dut.WriteByteStreamTo(stream);
    REQUIRE(c.red == stream[2]);
    REQUIRE(c.green == stream[1]);
    REQUIRE(c.blue == stream[0]);
    REQUIRE(c.white == stream[3]);
}

TEST_CASE("Setting Color by Parameter or by Structure", "[Color, Manipulating]") {
    Color     dut  = Color();
    const int size = sizeof(Color_t);
    uint8_t   stream[size];
    dut.WriteByteStreamTo(stream);

    for (size_t i = 0; i < size; i++) {
        REQUIRE(stream[i] == 0);
    }

    SECTION("Setting Color Object by Parameter") {
        const int size            = sizeof(Color_t);
        uint8_t   setStream[size] = {0x12, 0x34, 0x56, 0x78};

        dut.SetColor(setStream[0], setStream[1], setStream[2], setStream[3]);
        Color_t c = dut.GetColor();

        REQUIRE(c.red == setStream[0]);
        REQUIRE(c.green == setStream[1]);
        REQUIRE(c.blue == setStream[2]);
        REQUIRE(c.white == setStream[3]);
    }
    SECTION("Setting Color Object by Structure") {
        const int size            = sizeof(Color_t);
        uint8_t   setStream[size] = {0x23, 0x45, 0x67, 0x89};
        Color_t   setColor;
        setColor.red   = setStream[0];
        setColor.green = setStream[1];
        setColor.blue  = setStream[2];
        setColor.white = setStream[3];

        dut.SetColor(setColor);
        Color_t c = dut.GetColor();

        REQUIRE(c.red == setStream[0]);
        REQUIRE(c.green == setStream[1]);
        REQUIRE(c.blue == setStream[2]);
        REQUIRE(c.white == setStream[3]);
    }
}

TEST_CASE("Using Add-Operator", "[Color, Operator]") {
    const int size             = sizeof(Color_t);
    uint8_t   setStream1[size] = {0x12, 0x34, 0x56, 0x78};
    uint8_t   setStream2[size] = {0x98, 0x76, 0x54, 0x32};

    Color_t setColor1;
    setColor1.red   = setStream1[0];
    setColor1.green = setStream1[1];
    setColor1.blue  = setStream1[2];
    setColor1.white = setStream1[3];

    Color_t setColor2;
    setColor2.red   = setStream2[0];
    setColor2.green = setStream2[1];
    setColor2.blue  = setStream2[2];
    setColor2.white = setStream2[3];

    Color dut1 = Color(setColor1);
    Color dut2 = Color(setColor2);

    Color dut = dut1 + dut2;

    Color_t c = dut.GetColor();
    REQUIRE(c.red == 0xAA);
    REQUIRE(c.green == 0xAA);
    REQUIRE(c.blue == 0xAA);
    REQUIRE(c.white == 0xAA);
}

TEST_CASE("Using Multiply-Operator with odd data", "[Color, Operator]") {
    const int     size            = sizeof(Color_t);
    const uint8_t setStream[size] = {0xFF, 0x3F, 0x0F, 0x3};
    uint8_t       checkStream[size];
    memcpy(checkStream, setStream, sizeof(setStream));

    Color_t setColor;
    setColor.red   = setStream[0];
    setColor.green = setStream[1];
    setColor.blue  = setStream[2];
    setColor.white = setStream[3];

    Color dut = Color(setColor);

    SECTION("Multiplying by fullscale") {
        dut       = dut * 0xFF;
        Color_t c = dut.GetColor();
        REQUIRE(c.red == checkStream[0]);
        REQUIRE(c.green == checkStream[1]);
        REQUIRE(c.blue == checkStream[2]);
        REQUIRE(c.white == checkStream[3]);
    }
    SECTION("Multiplying by 50% +epsylon") {
        dut = dut * 0x80;

        for (size_t i = 0; i < size; i++) {
            checkStream[i] = (setStream[i] >> 1);
        }
        checkStream[0]++; // Special case without floor-effect

        Color_t c = dut.GetColor();
        REQUIRE(c.red == checkStream[0]);
        REQUIRE(c.green == checkStream[1]);
        REQUIRE(c.blue == checkStream[2]);
        REQUIRE(c.white == checkStream[3]);
    }
    SECTION("Multiplying by 50% -epsylon") {
        dut = dut * 0x7F;

        for (size_t i = 0; i < size; i++) {
            checkStream[i] = setStream[i] >> 1;
        }

        Color_t c = dut.GetColor();
        REQUIRE(c.red == checkStream[0]);
        REQUIRE(c.green == checkStream[1]);
        REQUIRE(c.blue == checkStream[2]);
        REQUIRE(c.white == checkStream[3]);
    }
    SECTION("Multiplying by 25% +epsylon") {
        dut = dut * 0x40;

        for (size_t i = 0; i < size; i++) {
            checkStream[i] = (setStream[i] >> 2);
        }
        checkStream[0]++; // Special case without floor-effect

        Color_t c = dut.GetColor();
        REQUIRE(c.red == checkStream[0]);
        REQUIRE(c.green == checkStream[1]);
        REQUIRE(c.blue == checkStream[2]);
        REQUIRE(c.white == checkStream[3]);
    }
    SECTION("Multiplying by 25% -epsylon") {
        dut = dut * 0x3F;

        for (size_t i = 0; i < size; i++) {
            checkStream[i] = (setStream[i] >> 2);
        }

        Color_t c = dut.GetColor();
        REQUIRE(c.red == checkStream[0]);
        REQUIRE(c.green == checkStream[1]);
        REQUIRE(c.blue == checkStream[2]);
        REQUIRE(c.white == checkStream[3]);
    }
    SECTION("Multiplying by 0%") {
        dut = dut * 0;

        for (size_t i = 0; i < size; i++) {
            checkStream[i] = 0;
        }

        Color_t c = dut.GetColor();
        REQUIRE(c.red == checkStream[0]);
        REQUIRE(c.green == checkStream[1]);
        REQUIRE(c.blue == checkStream[2]);
        REQUIRE(c.white == checkStream[3]);
    }
}


TEST_CASE("Using Multiply-Operator with even data", "[Color, Operator]") {
    const int     size            = sizeof(Color_t);
    const uint8_t setStream[size] = {0xFE, 0x3E, 0x0E, 0x4};
    uint8_t       checkStream[size];
    memcpy(checkStream, setStream, sizeof(setStream));

    Color_t setColor;
    setColor.red   = setStream[0];
    setColor.green = setStream[1];
    setColor.blue  = setStream[2];
    setColor.white = setStream[3];

    Color dut = Color(setColor);

    SECTION("Multiplying by fullscale") {
        dut       = dut * 0xFF;
        Color_t c = dut.GetColor();
        REQUIRE(c.red == checkStream[0]);
        REQUIRE(c.green == checkStream[1]);
        REQUIRE(c.blue == checkStream[2]);
        REQUIRE(c.white == checkStream[3]);
    }
    SECTION("Multiplying by 50% +epsylon") {
        dut = dut * 0x80;

        for (size_t i = 0; i < size; i++) {
            checkStream[i] = (setStream[i] >> 1);
        }

        Color_t c = dut.GetColor();
        REQUIRE(c.red == checkStream[0]);
        REQUIRE(c.green == checkStream[1]);
        REQUIRE(c.blue == checkStream[2]);
        REQUIRE(c.white == checkStream[3]);
    }
    SECTION("Multiplying by 50% -epsylon") {
        dut = dut * 0x7F;

        for (size_t i = 0; i < size; i++) {
            checkStream[i] = (setStream[i] >> 1) -1;
        }

        Color_t c = dut.GetColor();
        REQUIRE(c.red == checkStream[0]);
        REQUIRE(c.green == checkStream[1]);
        REQUIRE(c.blue == checkStream[2]);
        REQUIRE(c.white == checkStream[3]);
    }
    SECTION("Multiplying by 25% +epsylon") {
        dut = dut * 0x40;

        for (size_t i = 0; i < size; i++) {
            checkStream[i] = (setStream[i] >> 2);
        }

        Color_t c = dut.GetColor();
        REQUIRE(c.red == checkStream[0]);
        REQUIRE(c.green == checkStream[1]);
        REQUIRE(c.blue == checkStream[2]);
        REQUIRE(c.white == checkStream[3]);
    }
    SECTION("Multiplying by 25% -epsylon") {
        dut = dut * 0x3F;

        for (size_t i = 0; i < size; i++) {
            checkStream[i] = (setStream[i] >> 2);
        }
        checkStream[0]--; // Special Case
        checkStream[3] = 0; // Special Case

        Color_t c = dut.GetColor();
        REQUIRE(c.red == checkStream[0]);
        REQUIRE(c.green == checkStream[1]);
        REQUIRE(c.blue == checkStream[2]);
        REQUIRE(c.white == checkStream[3]);
    }
    SECTION("Multiplying by 0%") {
        dut = dut * 0;

        for (size_t i = 0; i < size; i++) {
            checkStream[i] = 0;
        }

        Color_t c = dut.GetColor();
        REQUIRE(c.red == checkStream[0]);
        REQUIRE(c.green == checkStream[1]);
        REQUIRE(c.blue == checkStream[2]);
        REQUIRE(c.white == checkStream[3]);
    }
}
