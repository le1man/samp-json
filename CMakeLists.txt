cmake_minimum_required(VERSION 3.19)
project(samp-json)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(open.mp-sdk)

include_directories(
    .
    pawn/source
    pawn/source/linux
)

add_definitions(
	-DHAVE_STDINT_H=1
	-DPAWN_CELL_SIZE=32
)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

if(WIN32)
	add_compile_options(/bigobj)
endif()

set(json_spirit_cpp
    json_spirit/json_spirit_reader.cpp
    json_spirit/json_spirit_writer.cpp
    json_spirit/json_spirit_value.cpp
)

add_library(json_spirit STATIC ${json_spirit_cpp})
target_compile_features(json_spirit PUBLIC cxx_std_11)
target_include_directories(json_spirit PUBLIC ${Boost_INCLUDE_DIRS})
if(WIN32)
	if(CMAKE_BUILD_TYPE MATCHES "Release")
		target_compile_options(json_spirit PRIVATE /Ox)
	endif()
else()
	target_compile_options(json_spirit PRIVATE -O3 -fPIC)
endif()
target_link_libraries(json_spirit PRIVATE ${Boost_LIBRARIES})

add_library(${PROJECT_NAME} SHARED main.cpp)
if(WIN32)
	if(CMAKE_BUILD_TYPE MATCHES "Release")
		target_compile_options(${PROJECT_NAME} PRIVATE /O2)
	endif()
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -O2 -fPIC -fshort-wchar)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE OMP-SDK json_spirit)