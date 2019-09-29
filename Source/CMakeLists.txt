project(LightingEffectLibrary)

set(CMAKE_C_COMPILER "gcc-8.2")
set(CMAKE_CXX_COMPILER "/usr/bin/g++-8.2")


# All sources that also need to be tested in unit tests go into a static library
add_library(LEL_lib STATIC  Modules/Color.cpp 
                            Modules/Color.h 
                            Modules/EffectMemory.cpp
                            Modules/EffectMemory.h 
                            Modules/Effects.cpp
                            Modules/Effects.h )
target_include_directories(LEL_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Modules)

# The main program
add_executable(prog SampleApp.cpp)
target_link_libraries(prog PRIVATE LEL_lib)