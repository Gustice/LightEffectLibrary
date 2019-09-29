project(hello_tests)

# The test program
add_executable(tests IntegrationTest.cpp)
target_link_libraries(tests PRIVATE LEL_lib)


# Header only library, therefore INTERFACE
add_library(catch INTERFACE)
# INTERFACE targets only have INTERFACE properties
target_include_directories(catch INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

