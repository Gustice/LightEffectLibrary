# The main program
add_library(LEL_library STATIC 
                    "Color.cpp" 
                    "Macro.cpp" 
                    "Processor.cpp" 
                    "StateMachine.cpp" 
                    "SequenceStepper.cpp"
                    "Sequecer.cpp"
                    "Waveforms.cpp"
)

target_link_directories(LEL_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(LEL_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
