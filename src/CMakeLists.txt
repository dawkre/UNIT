set(PYLON_DIR_PATH /etc/opt/pylon5)
set(DARKNET_DIR_PATH /home/ninja/Pulpit/d2/darknet)

cmake_minimum_required(VERSION 3.1.0)

project(main)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

include_directories(
	${PYLON_DIR_PATH}/include
	${DARKNET_DIR_PATH}/include
)
link_directories(
	${PYLON_DIR_PATH}/lib64
	${DARKNET_DIR_PATH}
)

add_executable(
	main 
	calculations.cpp 
	cameras.cpp 
	detection.cpp 
	main.cpp 
	PTZF_control.c
	calibration.cpp
	kalman.cpp
)
target_include_directories(main PUBLIC ${DARKNET_DIR_PATH}/src)
target_link_libraries(main ${OpenCV_LIBS} pylonutility pylonbase GCBase_gcc_v3_0_Basler_pylon_v5_0 darknet)
target_compile_definitions(main PRIVATE OPENCV=1)