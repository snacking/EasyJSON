# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(EasyJSON VERSION 1.0)

# Enable C++11 support
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the source files to the project
add_executable(EasyJSON main.cpp)

# Include directories for header files
target_include_directories(EasyJSON PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Link any required libraries (if any)
# target_link_libraries(EasyJSON <library_name>)
