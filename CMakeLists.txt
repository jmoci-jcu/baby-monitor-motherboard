cmake_minimum_required(VERSION 3.12)
project(BaMon)
set(CMAKE_CXX_STANDARD 20)
set(COMPILE_DEFINITIONS -O2 -Wall)
find_package(OpenCV REQUIRED)
add_executable(BaMon src/main.cpp)
target_link_libraries(BaMon ${OpenCV_LIBS})
add_subdirectory(lib/bluez_inc)
target_link_libraries(BaMon ${OpenCV_LIBS} Binc)
target_include_directories(BaMon PRIVATE lib/bluez_inc/binc ${GLIB_INCLUDE_DIRS})
target_sources(BaMon 
    PUBLIC
    src/main.cpp
    src/bluetooth_logger/bluetooth_logger.cpp
    src/log_hook/log_hook.cpp
    )

target_sources(PiCapture 
    PUBLIC
    src/main.cpp
    src/log_storer/log_storer.cpp
    )

