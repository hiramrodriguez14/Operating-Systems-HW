cmake_minimum_required(VERSION 3.10)

project(inClass)

# Include the current directory for header files 
include_directories(include)

set(FILE_NAME "mutex")
set(CMAKE_C_STANDARD 11)

add_executable(${CMAKE_PROJECT_NAME}
   src/${FILE_NAME}.c)

target_link_libraries(${CMAKE_PROJECT_NAME}
   m)
