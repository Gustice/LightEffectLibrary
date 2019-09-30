# All sources that also need to be tested in unit tests go into a static library
add_library(LEL_lib STATIC  Color.cpp 
                            Color.h 
                            EffectMemory.cpp
                            EffectMemory.h 
                            Effects.cpp
                            Effects.h )
target_include_directories(LEL_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})