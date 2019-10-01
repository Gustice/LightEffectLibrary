
# All sources that also need to be tested in unit tests go into a static library
add_library(EffectStateMachineTest STATIC ColorTest.cpp)
target_include_directories(EffectStateMachineTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(EffectStateMachineTest PUBLIC
                                    LEL_lib 
                                    catch )
