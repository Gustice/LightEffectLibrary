project(hello_tests)

add_subdirectory(Modules)
add_subdirectory(Include)

# The test program
add_executable(tests TestMain.cpp 
                     IntegrationTest.cpp 
                     Modules/Color/ColorTest.cpp 
                     Modules/EffectStateMachine/EffectStateMachineTest.cpp 
                )

target_link_libraries(tests LEL_lib 
                            catch )

                            
add_library(integrationTests STATIC IntegrationTest.cpp )
target_include_directories(integrationTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(integrationTests PUBLIC
                                    LEL_lib 
                                    catch )
