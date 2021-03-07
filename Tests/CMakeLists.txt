project(LlibraryTests)

add_subdirectory(Include)

# The test program
add_executable(tests TestMain.cpp 
                    Modules/ColorTest.cpp
                    Modules/EffectProcessorTest.cpp
                    Modules/EffectStateMachineTest.cpp
                    IntegrationTest.cpp 
                )

target_link_libraries(tests LEL_library 
                            catch 
)
add_compile_definitions(TSTOUTPUT_DIR="Tests/Output/")
