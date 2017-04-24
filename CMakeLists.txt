cmake_minimum_required(VERSION 3.6)
project(MATO)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
include_directories(/usr/local/include)
include_directories(/usr/local/include/libpd)
include_directories(/usr/local/include/rtaudio)
link_directories(/usr/local/lib)

set(SOURCE_FILES src/main.cpp src/PdAudio/PdObject.cpp include/PdObject.h /
        src/BDI/Interpreter.cpp include/Interpreter.h src/PdAudio/PdPatch.cpp /
        include/PdPatch.h src/Agent.cpp include/Agent.h src/BDI/Belief.cpp /
        include/Belief.h src/BDI/Goal.cpp include/Goal.h src/BDI/ActionGenerator.cpp /
        include/ActionGenerator.h src/Input/OscListener.cpp include/OscListener.h /
        include/Monitor.h include/common.h src/Monitor/OscMonitor.cpp include/OscMonitor.h /
        include/cxxopts.hpp src/BDI/Action.cpp include/Action.h include/OscSender.h)
add_executable(MATO ${SOURCE_FILES})
target_link_libraries(MATO pd pdcpp rtaudio oscpack)