cmake_minimum_required(VERSION 3.9)
project(cmake_test)

# Enables testing from this directory downwards.
enable_testing(true)

# Prepare "Catch" library for other executables.
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Make main executable that will be linked with other tests.
set(TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests Catch)

add_executable(TestTuple TestTuple.cpp ${TEST_SOURCES})
target_link_libraries(TestTuple Catch raytracer)

add_executable(TestColour TestColour.cpp ${TEST_SOURCES})
target_link_libraries(TestColour Catch raytracer)

add_executable(TestCanvas TestCanvas.cpp ${TEST_SOURCES})
target_link_libraries(TestCanvas Catch raytracer)

add_executable(TestMatrix TestMatrix.cpp ${TEST_SOURCES})
target_link_libraries(TestMatrix Catch raytracer)

add_executable(TestTransform TestTransform.cpp ${TEST_SOURCES})
target_link_libraries(TestTransform Catch raytracer)

add_executable(TestMath TestMath.cpp ${TEST_SOURCES})
target_link_libraries(TestMath Catch raytracer)