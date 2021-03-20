
#include "catch.hpp"
#include "Color_t.h"

// @todo LHF color compare function needed

TEST_CASE("Constructing emtpy Color_t structure", "[Color_t, Constructor]") {
    Color_t   dut  = {0, 0, 0, 0};
    const int size = sizeof(Color_t);
    uint8_t   stream[size];
    WriteByteStreamTo(&dut, stream);

    for (size_t i = 0; i < size; i++) {
        REQUIRE(stream[i] == 0);
    }
}

bool ColorIsEqualToStream(Color_t c, const uint8_t s[sizeof(Color_t)]) {
    if (((c.red == s[0]) && (c.green == s[1]) && (c.blue == s[2]) && (c.white == s[3])))
        return true;

    INFO(printf("c.red:   %02x  |  b0: %02x\n", c.red, s[0]));
    INFO(printf("c.green: %02x  |  b1: %02x\n", c.green, s[1]));
    INFO(printf("c.blue:  %02x  |  b2: %02x\n", c.blue, s[2]));
    INFO(printf("c.white: %02x  |  b3: %02x\n", c.white, s[3]));
    return false;
}

static void DivideByPowOfN(uint8_t stream[sizeof(Color_t)], int n) {
    for (size_t i = 0; i < sizeof(Color_t); i++) {
        stream[i] = (stream[i] >> n);
    }
}

const int size = sizeof(Color_t);

TEST_CASE("Checking EqualTest-Function", "[Color_t, Constructor]") {
    uint8_t setStream[size] = {0x12, 0x34, 0x56, 0x78};

    Color_t dut = Color_t{setStream[0], setStream[1], setStream[2], setStream[3]};
    REQUIRE(ColorIsEqualToStream(dut, setStream));
}

TEST_CASE("Conservate Color_t format of WriteByteStreamTo", "[Color_t, Format]") {
    uint8_t setStream[size] = {0x12, 0x34, 0x56, 0x78};
    Color_t setColor;
    Color_t dut;
    SetColorByRgb(&dut, setStream[0], setStream[1], setStream[2], setStream[3]);

    uint8_t stream[size];
    WriteByteStreamTo(&dut, stream);
    REQUIRE(dut.red == stream[2]);
    REQUIRE(dut.green == stream[1]);
    REQUIRE(dut.blue == stream[0]);
    REQUIRE(dut.white == stream[3]);
}

TEST_CASE("Setting Color_t by Parameter or by Structure", "[Color_t, Getter]") {
    SECTION("Setting Color_t structure by Structure") {
        uint8_t setStream[size] = {0x23, 0x45, 0x67, 0x89};
        Color_t setColor;
        setColor.red   = setStream[0];
        setColor.green = setStream[1];
        setColor.blue  = setStream[2];
        setColor.white = setStream[3];

        Color_t dut;
        SetColor(&dut, setColor);
        REQUIRE(ColorIsEqualToStream(dut, setStream));
    }

    SECTION("Setting Color_t structure by Parameter") {
        uint8_t setStream[size] = {0x9A, 0xBC, 0xDE, 0xF1};

        Color_t dut;
        SetColorByRgb(&dut, setStream[0], setStream[1], setStream[2], setStream[3]);
        REQUIRE(ColorIsEqualToStream(dut, setStream));
    }
}

TEST_CASE("Using MixColor", "[Color_t, Operator]") {
    uint8_t setStream1[size]  = {0x12, 0x34, 0x56, 0x78};
    uint8_t setStream2[size]  = {0x98, 0x76, 0x54, 0x32};
    uint8_t checkStream[size] = {0xAA, 0xAA, 0xAA, 0xAA};

    Color_t setColor1 = {
        .red   = setStream1[0],
        .green = setStream1[1],
        .blue  = setStream1[2],
        .white = setStream1[3],
    };
    Color_t setColor2 = {
        .red   = setStream2[0],
        .green = setStream2[1],
        .blue  = setStream2[2],
        .white = setStream2[3],
    };

    Color_t dut1 = Color_t(setColor1);
    Color_t dut2 = Color_t(setColor2);

    MixColor(&dut1, dut2);
    REQUIRE(ColorIsEqualToStream(dut1, checkStream));
}

TEST_CASE("Using MixColorScaled", "[Color_t, Method]") {
    Color_t c1, c2, c3;
    SetColor(&c1, cRed);
    SetColor(&c2, cGreen);
    SetColor(&c3, cBlue);
    uint8_t checkStream1[size] = {0x80, 0x7F, 0x00, 0x00};
    uint8_t checkStream2[size] = {0x60, 0x5F, 0x3F, 0x00};

    MixColorScaled(&c1, c2, 0x7F);
    REQUIRE(ColorIsEqualToStream(c1, checkStream1));

    MixColorScaled(&c1, c3, 0x3F);
    REQUIRE(ColorIsEqualToStream(c1, checkStream2));
}

TEST_CASE("Using ScaleColor with odd data", "[Color_t, Operator]") {
    const int     size            = sizeof(Color_t);
    const uint8_t setStream[size] = {0xFF, 0x3F, 0x0F, 0x3};
    uint8_t       checkStream[size];
    memcpy(checkStream, setStream, sizeof(setStream));

    Color_t dut;
    SetColorByRgb(&dut, setStream[0], setStream[1], setStream[2], setStream[3]);

    SECTION("Multiplying by fullscale") {
        ScaleColor(&dut, 0xFF);
        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 50% +epsylon") {
        ScaleColor(&dut, 0x80);
        DivideByPowOfN(checkStream, 1);
        checkStream[0]++; // Special case without floor-effect
        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 50% -epsylon") {
        ScaleColor(&dut, 0x7F);
        DivideByPowOfN(checkStream, 1);
        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 25% +epsylon") {
        ScaleColor(&dut, 0x40);
        DivideByPowOfN(checkStream, 2);
        checkStream[0]++; // Special case without floor-effect

        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 25% -epsylon") {
        ScaleColor(&dut, 0x3F);
        DivideByPowOfN(checkStream, 2);
        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 0%") {
        ScaleColor(&dut, 0x00);
        for (size_t i = 0; i < size; i++) {
            checkStream[i] = 0;
        }
        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
}

TEST_CASE("Using ScaleColor with even data", "[Color_t, Operator]") {
    const int     size            = sizeof(Color_t);
    const uint8_t setStream[size] = {0xFC, 0x3C, 0x0C, 0x4};
    uint8_t       checkStream[size];
    memcpy(checkStream, setStream, sizeof(setStream));

    Color_t dut;
    SetColorByRgb(&dut, setStream[0], setStream[1], setStream[2], setStream[3]);

    SECTION("Multiplying by fullscale") {
        ScaleColor(&dut, 0xFF);
        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 50% +epsylon") {
        ScaleColor(&dut, 0x80);
        DivideByPowOfN(checkStream, 1);
        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 50% -epsylon") {
        ScaleColor(&dut, 0x7F);
        for (size_t i = 0; i < size; i++) {
            checkStream[i] = (setStream[i] >> 1) - 1;
        }

        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 25% +epsylon") {
        ScaleColor(&dut, 0x40);
        DivideByPowOfN(checkStream, 2);

        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 25% -epsylon") {
        ScaleColor(&dut, 0x3F);
        for (size_t i = 0; i < size; i++) {
            checkStream[i] = (setStream[i] >> 2) - 1;
        }
        checkStream[3] = 0; // Special Case

        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
    SECTION("Multiplying by 0%") {
        ScaleColor(&dut, 0x00);
        for (size_t i = 0; i < size; i++) {
            checkStream[i] = 0;
        }

        REQUIRE(ColorIsEqualToStream(dut, checkStream));
    }
}

TEST_CASE("Comparing Color_t", "[Color_t]") {
    Color_t c1 = {1,2,3,4};
    Color_t c2 = {1,2,3,4};
    CHECK ( (ColorIsEqual(&c1, c2) ));

    Color_t cN1 = {0,2,3,4 };
    CHECK ( ColorNotEqual(&c1, cN1) );
    Color_t cN2 = {1,0,3,4 };
    CHECK ( ColorNotEqual(&c1, cN2) );
    Color_t cN3 = {1,2,0,4 };
    CHECK ( ColorNotEqual(&c1, cN3) );
    Color_t cN4 = {1,2,3,0 };
    CHECK ( ColorNotEqual(&c1, cN4) );

    Color_t c3 = {0 ,0,0,0};
    CHECK ( ColorNotEqual(&c1, c3) );
    SetColorByRgb(&c3, 1,2,3,4);
    CHECK ( (ColorIsEqual(&c1, c3) ));
}
