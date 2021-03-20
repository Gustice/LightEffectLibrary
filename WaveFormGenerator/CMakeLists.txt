
add_executable(WaveFormGen GenerateWaveforms.cpp Elements/GaussianWaveforms.cpp Elements/WGUtils.cpp)
target_include_directories(WaveFormGen PUBLIC Elements)