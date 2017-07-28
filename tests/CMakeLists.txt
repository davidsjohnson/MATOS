cmake_minimum_required(VERSION 3.6)
project(MATO)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(/usr/local/include)
include_directories(/usr/local/include/libpd)
include_directories(/usr/local/include/rtaudio)
link_directories(/usr/local/lib)

set(SOURCE_FILES timeline_tests.cpp)
add_executable(AgentTests ${SOURCE_FILES})
target_link_libraries(AgentTests MATO_LIB)
