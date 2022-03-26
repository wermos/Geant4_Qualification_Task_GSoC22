cmake_minimum_required(VERSION 3.10)

# set the project name
project(GSoC)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)


file(GLOB SOURCES
	 CONFIGURE_DEPENDS
	 *.cpp
) 

# add the executable
add_executable(solver ${SOURCES})

