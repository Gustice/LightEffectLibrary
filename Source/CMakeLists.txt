project(LightingEffectLibrary)

add_subdirectory(Modules)

# The main program
add_executable(prog SampleApp.cpp)
target_link_libraries(prog PRIVATE LEL_lib)