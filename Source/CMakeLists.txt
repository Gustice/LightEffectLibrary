# The main program
add_library(LEL_library STATIC 
                    "Color.cpp" 
                    "EffectMacro.cpp" 
                    "EffectProcessor.cpp" 
                    "EffectStateMachine.cpp" 
                    "EffectWaveforms.cpp"
)

target_link_directories(LEL_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(LEL_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
