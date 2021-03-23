project(LlibraryTests)

add_subdirectory(Include)

# The test program
add_executable(tests TestMain.cpp 
                    Modules/ColorTest.cpp
                    Modules/ProcessorTest.cpp
                    Modules/StateMachineTest.cpp
                    
                    Modules/SequenceStepperTest.cpp
                    Modules/SequenceProcessorTest.cpp

                    cModules/cColorTest.cpp
                    cModules/cEffectStateMachineTest.cpp
                    cModules/cEffectProcessorTest.cpp
                    cModules/cMacroVisualization.cpp
                    
                    IntegrationTest.cpp 
                )

target_link_libraries(tests LEL_library cLEL_library 
                            catch 
)
add_compile_definitions(TSTOUTPUT_DIR="Tests/Output/")
