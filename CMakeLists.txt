cmake_minimum_required(VERSION 3.6)
project(MATO)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(/usr/local/include)
include_directories(/usr/local/include/libpd)
include_directories(/usr/local/include/rtaudio)
link_directories(/usr/local/lib)

set(SOURCE_FILES include/Timeline.h src/Audio/Timeline.cpp src/main.cpp include/Agent.h src/Agent.cpp /
        include/PdPatch.h src/Audio/PdPatch.cpp /
        src/Audio/PdObject.cpp include/PdObject.h /
        src/BDI/Interpreter.cpp include/Interpreter.h /
        src/BDI/Belief.cpp include/Belief.h /
        src/BDI/Goal.cpp include/Goal.h /
        src/AgentComm/OscListener.cpp include/OscListener.h /
        include/common.h src/AgentComm/AgentMonitor.cpp include/AgentMonitor.h /
        include/cxxopts.hpp include/OscSender.h include/SensorMonitor.h /
        include/Behavior.h src/Behaviors/TempoBehavior.cpp include/TempoBehavior.h /
        src/Behaviors/PdStateBehavior.cpp include/PdStateBehavior.h)
add_executable(MATO ${SOURCE_FILES})
target_link_libraries(MATO pd pdcpp rtaudio oscpack)

add_library(MATO_LIB SHARED ${SOURCE_FILES})
target_link_libraries(MATO_LIB pd pdcpp rtaudio oscpack)

add_subdirectory(tests)