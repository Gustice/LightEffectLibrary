
#include "Color.h"
#include "catch.hpp"

TEST_CASE("Constructing emtpy Color Object", "[Color, Constructing]") {
  Color dut = Color();
  const int size = sizeof(Color_t);
  uint8_t stream[size];
  dut.WriteByteStreamTo(stream);

  for (size_t i = 0; i < size; i++) {
    REQUIRE(stream[i] == 0);
  }
}

TEST_CASE("Constructing Color Object by Parameter", "[Color, Constructing]") {
  const int size = sizeof(Color_t);
  uint8_t setStream[size] = {0x12, 0x34, 0x56, 0x78};

  Color dut = Color(setStream[0], setStream[1], setStream[2], setStream[3]);
  Color_t c = dut.GetColor();

  REQUIRE(c.red == setStream[0]);
  REQUIRE(c.green == setStream[1]);
  REQUIRE(c.blue == setStream[2]);
  REQUIRE(c.white == setStream[3]);
}

TEST_CASE("Constructing Color Object by structure", "[Color, Constructing]") {
  const int size = sizeof(Color_t);
  uint8_t setStream[size] = {0x23, 0x45, 0x67, 0x89};
  Color_t setColor;
  setColor.red = setStream[0];
  setColor.green = setStream[1];
  setColor.blue = setStream[2];
  setColor.white = setStream[3];

  Color dut = Color(setColor);
  Color_t c = dut.GetColor();

  REQUIRE(c.red == setStream[0]);
  REQUIRE(c.green == setStream[1]);
  REQUIRE(c.blue == setStream[2]);
  REQUIRE(c.white == setStream[3]);
}

TEST_CASE("White Channel stays zero if white parameter is omitted", "[Color, Constructing]") {
  const int size = sizeof(Color_t);
  uint8_t setStream[size] = {0x9A, 0xBC, 0xDE, 0xF1};

  Color dut = Color(setStream[0], setStream[1], setStream[2]);
  Color_t c = dut.GetColor();

  REQUIRE(c.red == setStream[0]);
  REQUIRE(c.green == setStream[1]);
  REQUIRE(c.blue == setStream[2]);
  REQUIRE(c.white == 0);
}

TEST_CASE("Conservate format of WriteByteStreamTo", "[Color, Format]") {
  const int size = sizeof(Color_t);
  uint8_t setStream[size] = {0x12, 0x34, 0x56, 0x78};

  Color_t setColor;
  setColor.red = setStream[0];
  setColor.green = setStream[1];
  setColor.blue = setStream[2];
  setColor.white = setStream[3];

  uint8_t stream[size];
  Color dut = Color(setColor);
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
  Color dut = Color();
  const int size = sizeof(Color_t);
  uint8_t stream[size];
  dut.WriteByteStreamTo(stream);

  for (size_t i = 0; i < size; i++) {
    REQUIRE(stream[i] == 0);
  }

  SECTION("Setting Color Object by Parameter") {
    const int size = sizeof(Color_t);
    uint8_t setStream[size] = {0x12, 0x34, 0x56, 0x78};

    dut.SetColor(setStream[0], setStream[1], setStream[2], setStream[3]);
    Color_t c = dut.GetColor();

    REQUIRE(c.red == setStream[0]);
    REQUIRE(c.green == setStream[1]);
    REQUIRE(c.blue == setStream[2]);
    REQUIRE(c.white == setStream[3]);
  }
  SECTION("Setting Color Object by Structure") {
    const int size = sizeof(Color_t);
    uint8_t setStream[size] = {0x23, 0x45, 0x67, 0x89};
    Color_t setColor;
    setColor.red = setStream[0];
    setColor.green = setStream[1];
    setColor.blue = setStream[2];
    setColor.white = setStream[3];

    dut.SetColor(setColor);
    Color_t c = dut.GetColor();

    REQUIRE(c.red == setStream[0]);
    REQUIRE(c.green == setStream[1]);
    REQUIRE(c.blue == setStream[2]);
    REQUIRE(c.white == setStream[3]);
  }
}

// @todo Testing operators