cmake_minimum_required(VERSION 3.23)
project(final)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall")
include_directories(OpenGL)
include_directories(OpenGL/include) # OpenGL/include has to contain the required OpenGL's .h files
include_directories(OpenGL/lib) # OpenGL/lib has to contain the required OpenGL's .lib files


file(GLOB CPP_SOURCE
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
	)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_executable(main main.cpp ${CPP_SOURCE})
