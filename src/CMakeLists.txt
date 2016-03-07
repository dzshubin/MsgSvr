
cmake_minimum_required (VERSION 2.8)
# Project 
project (MsgSvr)


# policy
cmake_policy(SET CMP0015 OLD)

message (STATUS "compiler is ${CMAKE_CXX_COMPILER_ID}" )
message (STATUS "source dir: ${PROJECT_SOURCE_DIR} ")
message (STATUS "current dir: ${CMAKE_CURRENT_SOURCE_DIR}")

# 判断是否支持C++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(FATAL_ERROR  "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


# 设置boost库路径
set (Boost_root_path    ../boost)
set (Boost_head_path    ${Boost_root_path}/include)
set (Boost_lib_path	    ${Boost_root_path}/lib)


include_directories(${Boost_head_path})
LINK_DIRECTORIES(${Boost_lib_path})


set (Boost_system_lib 		    libboost_system.a)
set (Boost_serialization_lib 	libboost_serialization.a)
set (Boost_signals_lib 		    libboost_signals.a)


set (Used_libs ${Boost_system_lib} ${Boost_serialization_lib} ${Boost_signals_lib})

# set used protobuf
set (Pb_lib	libprotobuf.a)
set (Pb_used_libs ${Pb_lib})

# 设置pb生成的文件路径
set (Pb_root_path	../pb)
set (Pb_file_path	${Pb_root_path}/include/)

include_directories(${Pb_file_path})


# 当前目前下的所有源文件
file(GLOB_RECURSE HEAD_FILES *h *hpp)

# pb 头文件
file(GLOB_RECURSE PB_HEAD_FILES "../pb/include/*.h")


aux_source_directory (. DIR_SRCS)
aux_source_directory (${Pb_file_path} DIR_SRCS)
ADD_EXECUTABLE(${PROJECT_NAME} ${DIR_SRCS} ${HEAD_FILES} ${PB_HEAD_FILES})


#
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Used_libs} ${Pb_used_libs})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -lpthread)


