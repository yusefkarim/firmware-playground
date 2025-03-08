cmake_minimum_required(VERSION 3.8.2)
project(storm_the_castle)

#Toolchain file
include(tm4c123g.cmake)

#Tivaware files
set(TIVAWARE_PATH "$ENV{HOME}/tiva-projects/tivaware")
include_directories(${TIVAWARE_PATH})
include_directories(${TIVAWARE_PATH}/inc)

#Header files
include_directories(inc)
#Source files
file(GLOB SOURCES "src/*.c")
add_executable(${CMAKE_PROJECT_NAME}.axf ${SOURCES})

target_link_libraries(
    ${CMAKE_PROJECT_NAME}.axf
    ${TIVAWARE_PATH}/usblib/gcc/libusb.a
    ${TIVAWARE_PATH}/driverlib/gcc/libdriver.a
)
