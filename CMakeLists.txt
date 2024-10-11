cmake_minimum_required(VERSION 3.22)
project(oneDNNDemo)

#set(ONEDNN_HOME "${PROJECT_SOURCE_DIR}/../oneDNN-3.5.3-O3NOSVE")
set(ONEDNN_HOME "${PROJECT_SOURCE_DIR}/../oneDNN-3.5.3-O3SVE")

include_directories("${ONEDNN_HOME}/include")
include_directories("${PROJECT_SOURCE_DIR}")

add_executable(demo profiling_matmul.cpp)
#add_executable(demo profiling_matmul.cpp)
#add_executable(demo profiling_conv.cpp)

target_link_libraries(demo "${ONEDNN_HOME}/lib/libdnnl.so")