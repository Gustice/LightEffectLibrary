
#include "Color.h"
#include "catch.hpp"

// @todo LHF color compare function needed

TEST_CASE("Constructing emtpy Color Object", "[Color, Constructor]") {
    Color     dut  = Color();
    const int size = sizeof(Color_t);
    uint8_t   stream[size];
    dut.WriteByteStreamTo(stream);

    for (size_t i = 0; i < size; i++) {
        REQUIRE(stream[i] == 0);
    }
}

bool ColorIsEqualToStream(Color &C, const uint8_t s[sizeof(Color_t)]) {
    Color_t c = C.GetColor();

    return ((c.red == s[0]) && (c.green == s[1]) && (c.blue == s[2]) && (c.white == s[3]));
}

void DivideByPowOfN(uint8_t stream[sizeof(Color_t)], int n) {
    for (size_t i = 0; i < sizeof(Color_t); i++) {
        stream[i] = (stream[i] >> n);
    }
}

const int size = sizeof(Color_t);

TEST_CASE("Constructing Color Object by Parameter", "[Color, Constructor]") {
    uint8_t setStream[size] = {0x12, 0x34, 0x56, 0x78};

    Color dut = Color(setStream[0], setStream[1], setStream[2], setStream[3]);
    REQUIRE(ColorIsEqualToStream(dut, setStream));
}

TEST_CASE("Constructing Color Object by structure", "[Color, Constructor]") {
    uint8_t setStream[size] = {0x23, 0x45, 0x67, 0x89};
    Color_t setColor;
    setColor.red   = setStream[0];
    setColor.green = setStream[1];
    setColor.blue  = setStream[2];
    setColor.white = setStream[3];

    Color dut = Color(setColor);
    REQUIRE(ColorIsEqualToStream(dut, setStream));
}

TEST_CASE("White Channel stays zero if white parameter is omitted", "[Color, Constructor]") {
    uint8_t setStream[size]   = {0x9A, 0xBC, 0xDE, 0xF1};
    uint8_t checkStream[size] = {0x9A, 0xBC, 0xDE, 0x00};

    Color dut = Color(setStream[0], setStream[1], setStream[2]);
    REQUIRE(ColorIsEqualToStream(dut, checkStream));
}

TEST_CASE("Conservate format of WriteByteStreamTo", "[Color, Format]") {
    uint8_t setStream[size] = {0x12, 0x34, 0x56, 0x78};
    Color_t setColor;
    setColor.red   = setStream[0];
    setColor.green = setStream[1];
    setColor.blue  = setStream[2];
    setColor.white = setStream[3];

    uint8_t stream[size];
    Color   dut = Color(setColor);
    dut.WriteByteStreamTo(stream);
    REQUIRE(ColorIsEqualToStream(dut, setStream));

    Color_t c = dut.GetColor();
    dut.WriteByteStreamTo(stream);
    REQUIRE(c.red == stream[2]);
    REQUIRE(c.green == stream[1]);
    REQUIRE(c.blue == stream[0]);
    REQUIRE(c.white == stream[3]);
}

TEST_CASE("Setting Color by Parameter or by Structure", "[Color, Getter]") {
    Color   dut = Color();
    uint8_t stream[size];
    dut.WriteByteStreamTo(stream);

    for (size_t i = 0; i < size; i++) {
        REQUIRE(stream[i] == 0);
    }

    SECTION("Setting Color Object by Parameter") {
        uint8_t setStream[size] = {0x12, 0x34, 0x56, 0x78};

        dut.SetColor(setStream[0], setStream[1], setStream[2], setStream[3]);
        REQUIRE(ColorIsEqualToStream(dut, setStream));
    }
    SECTION("Setting Color Object by Structure") {
        uint8_t setStream[size] = {0x23, 0x45, 0x67, 0x89};
        Color_t setColor;
        setColor.red   = setStream[0];
        setColor.green = setStream[1];
        setColor.blue  = setStream[2];
        setColor.white = setStream[3];

        dut.SetColor(setColor);
        REQUIRE(ColorIsEqualToStream(dut, setStream));
    }
}

TEST_CASE("Using Add-Operator", "[Color, Operator]") {
    uint8_t setStream1[size]  = {0x12, 0x34, 0x56, 0x78};
    uint8_t setStream2[size]  = {0x98, 0x76, 0x54, 0x32};
    uint8_t checkStream[size] = {0xAA, 0xAA, 0xAA, 0xAA};

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
    REQUIRE(ColorIsEqualToStream(dut, checkStream));
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
        dut = dut * 0xFF;
        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 50% +epsylon") {
        dut = dut * 0x80;
        DivideByPowOfN(checkStream, 1);
        checkStream[0]++; // Special case without floor-effect
        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 50% -epsylon") {
        dut = dut * 0x7F;
        DivideByPowOfN(checkStream, 1);
        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 25% +epsylon") {
        dut = dut * 0x40;
        DivideByPowOfN(checkStream, 2);
        checkStream[0]++; // Special case without floor-effect

        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 25% -epsylon") {
        dut = dut * 0x3F;
        DivideByPowOfN(checkStream, 2);
        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 0%") {
        dut = dut * 0;

        for (size_t i = 0; i < size; i++) {
            checkStream[i] = 0;
        }
        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
}

TEST_CASE("Using Multiply-Operator with even data", "[Color, Operator]") {
    const int     size            = sizeof(Color_t);
    const uint8_t setStream[size] = {0xFC, 0x3C, 0x0C, 0x4};
    uint8_t       checkStream[size];
    memcpy(checkStream, setStream, sizeof(setStream));

    Color_t setColor;
    setColor.red   = setStream[0];
    setColor.green = setStream[1];
    setColor.blue  = setStream[2];
    setColor.white = setStream[3];

    Color dut = Color(setColor);

    SECTION("Multiplying by fullscale") {
        dut = dut * 0xFF;
        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 50% +epsylon") {
        dut = dut * 0x80;
        DivideByPowOfN(checkStream, 1);
        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 50% -epsylon") {
        dut = dut * 0x7F;

        for (size_t i = 0; i < size; i++) {
            checkStream[i] = (setStream[i] >> 1) - 1;
        }

        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 25% +epsylon") {
        dut = dut * 0x40;
        DivideByPowOfN(checkStream, 2);

        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 25% -epsylon") {
        dut = dut * 0x3F;

        for (size_t i = 0; i < size; i++) {
            checkStream[i] = (setStream[i] >> 2) - 1;
        }
        checkStream[3] = 0; // Special Case

        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 0%") {
        dut = dut * 0;

        for (size_t i = 0; i < size; i++) {
            checkStream[i] = 0;
        }

        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
}

TEST_CASE("Object remains untouched after Add-Operator is used", "[Color, Operator]") {
    const int size              = sizeof(Color_t);
    uint8_t   setStream1[size]  = {0x12, 0x34, 0x56, 0x78};
    uint8_t   setStream2[size]  = {0x98, 0x76, 0x54, 0x32};
    uint8_t   checkStream[size] = {0xAA, 0xAA, 0xAA, 0xAA};

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
    REQUIRE(ColorIsEqualToStream(dut, checkStream));
    REQUIRE(ColorIsEqualToStream(dut1, setStream1));
    REQUIRE(ColorIsEqualToStream(dut2, setStream2));
}

TEST_CASE("Object remains untouched after Multiply-Operator is used", "[Color, Operator]") {
    const uint8_t setStream[size] = {0xFF, 0x3F, 0x0F, 0x3};
    uint8_t       checkStream[size];

    Color_t setColor;
    setColor.red   = setStream[0];
    setColor.green = setStream[1];
    setColor.blue  = setStream[2];
    setColor.white = setStream[3];
    Color dut1     = Color(setColor);

    Color dut = dut1 * 0x7F;
    for (size_t i = 0; i < size; i++) {
        checkStream[i] = setStream[i] >> 1;
    }

    REQUIRE(ColorIsEqualToStream(dut, checkStream));
    REQUIRE(ColorIsEqualToStream(dut1, setStream));
}

TEST_CASE("Testsing usage of predefined Colors", "[Color]") {
    Color   dut = CWhite;
    Color_t c    = dut.GetColor();

    uint8_t check[size] = {0x55, 0x55, 0x55, 0x00}; // this is white
    REQUIRE(ColorIsEqualToStream(dut, check));
}