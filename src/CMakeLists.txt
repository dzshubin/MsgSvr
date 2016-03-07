
cmake_minimum_required (VERSION 2.8)
# Project
project (LoginSvr)


# policy
cmake_policy(SET CMP0015 OLD)

message (STATUS "compiler is ${CMAKE_CXX_COMPILER_ID}" )
message (STATUS "source dir: ${PROJECT_SOURCE_DIR} ")
message (STATUS "current dir: ${CMAKE_CURRENT_SOURCE_DIR}")

ADD_DEFINITIONS (-std=c++0x -g -Wall )

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
    endif ()
    elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

    else ()
        message(FATAL_ERROR "Your C++ compiler does not support C++11.")
    endif ()


# 设置boost库路径
set (Boost_root_path    ../boost)
set (Boost_head_path    ${Boost_root_path}/include)
set (Boost_lib_path	${Boost_root_path}/lib)


include_directories(${Boost_head_path})
LINK_DIRECTORIES(${Boost_lib_path})


# 设置pb生成的文件路径
set (Pb_gen_root_path	../protobuf_gen)
set (Pb_gen_file_path	${Pb_gen_root_path}/include/)

include_directories(${Pb_gen_file_path})


# 设置protobuf路径
set (Pb_head_path 	../protobuf/protobuf)
set (Pb_lib_path	../protobuf/lib)

include_directories(${Pb_head_path})
LINK_DIRECTORIES(${Pb_lib_path})



# set used boost
set (Boost_system_lib 		    libboost_system.a)
set (Boost_serialization_lib 	libboost_serialization.a)
set (Boost_signals_lib 		    libboost_signals.a)


set (Boost_used_libs ${Boost_system_lib} ${Boost_serialization_lib} ${Boost_signals_lib})


# set used protobuf
set (Pb_lib	libprotobuf.a)

set (Pb_used_libs ${Pb_lib})


# 当前目前下的所有源文件
aux_source_directory (. DIR_SRCS)
aux_source_directory (${Pb_gen_file_path} PB_GEN_SRCS)

file(GLOB_RECURSE HEAD_FILES *h *hpp)
file(GLOB_RECURSE PB_GEN_HEAD_FILES ../protobuf_gen/include/*.h ../protobuf_gen/include/*.hpp)


ADD_EXECUTABLE(${PROJECT_NAME} ${DIR_SRCS} ${PB_GEN_SRCS} ${HEAD_FILES} ${PB_GEN_HEAD_FILES})


#
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Boost_used_libs} ${Pb_used_libs} )
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -lpthread)


