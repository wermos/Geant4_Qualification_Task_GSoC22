cmake_minimum_required(VERSION 3.10)

# set the project name
project(GSoC)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)


file(GLOB SOURCES
	 CONFIGURE_DEPENDS
	 *.cpp
) 

find_package(Python3 COMPONENTS Development)

# add the executable
add_executable(solver ${SOURCES})

target_include_directories(solver PUBLIC external/boost/include)
target_include_directories(solver PUBLIC ${Python3_INCLUDE_DIRS})
target_include_directories(solver PUBLIC external/matplotlib-cpp)

target_link_libraries(solver PUBLIC ${Python3_LIBRARIES})
