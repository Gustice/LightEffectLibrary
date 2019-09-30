
#include "catch.hpp"
#include "Color.h"

TEST_CASE( "Constructing emtpy Object", "[Color]" ) {
    Color dut = Color();
    uint8_t stream[5];
    
    dut.GetByteStream(stream);
    for (size_t i = 0; i < 5; i++)
    {
        REQUIRE( stream[i] == 0);
    }
}

TEST_CASE( "Constructing emtpy Object 2", "[Color]" ) {
    Color dut = Color();
    uint8_t stream[5];
     
    dut.GetByteStream(stream);
    for (size_t i = 0; i < 5; i++)
    {
        REQUIRE( stream[i] == 0);
    }
}