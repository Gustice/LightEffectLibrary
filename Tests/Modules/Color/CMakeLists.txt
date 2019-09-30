
# All sources that also need to be tested in unit tests go into a static library
add_library(ColorModulTests STATIC ColorTest.cpp)
target_include_directories(ColorModulTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ColorModulTests PUBLIC
                                    LEL_lib 
                                    catch )
