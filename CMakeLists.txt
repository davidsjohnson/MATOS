cmake_minimum_required(VERSION 3.6)
project(MATO)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
include_directories(/usr/local/include)
include_directories(/usr/local/include/libpd)
include_directories(/usr/local/include/rtaudio)
link_directories(/usr/local/lib)

set(SOURCE_FILES src/main.cpp include/Agent.h src/Agent.cpp /
        include/PdPatch.h src/Output/PdPatch.cpp /
        src/Output/PdObject.cpp include/PdObject.h /
        src/BDI/Interpreter.cpp include/Interpreter.h /
        src/BDI/Belief.cpp include/Belief.h /
        src/BDI/Goal.cpp include/Goal.h
        /
        src/AgentComm/OscListener.cpp include/OscListener.h /
        include/Monitor.h include/common.h src/AgentComm/OscMonitor.cpp include/OscMonitor.h /
        include/cxxopts.hpp include/OscSender.h src/Input/ProximityMonitor.cpp include/ProximityMonitor.h include/Behavior.h src/Behaviors/TempoBehavior.cpp include/TempoBehavior.h)
add_executable(MATO ${SOURCE_FILES})
target_link_libraries(MATO pd pdcpp rtaudio oscpack)