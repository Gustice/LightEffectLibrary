# The main program
add_library(cLEL_library STATIC 
                    "Color_t.c" 
                    "EffectMacro.c" 
                    "EffectProcessor.c" 
                    "EffectStateMachine.c" 
                    "EffectWaveforms.c"
)

target_link_directories(cLEL_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(cLEL_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
