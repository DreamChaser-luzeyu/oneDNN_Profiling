cmake_minimum_required(VERSION 3.22)
project(oneDNNDemo)

set(ONEDNN_HOME "${PROJECT_SOURCE_DIR}/oneDNN_install")

include_directories("${ONEDNN_HOME}/include")
include_directories("${PROJECT_SOURCE_DIR}")

add_executable(matmul profiling_matmul.cpp)
add_executable(batchmm profiling_batchmm.cpp)
add_executable(softmax profiling_softmax.cpp)
add_executable(relu profiling_eltwiserelu.cpp)

target_link_libraries(matmul "${ONEDNN_HOME}/lib/libdnnl.so")
target_link_libraries(batchmm "${ONEDNN_HOME}/lib/libdnnl.so")
target_link_libraries(softmax "${ONEDNN_HOME}/lib/libdnnl.so")
target_link_libraries(relu "${ONEDNN_HOME}/lib/libdnnl.so")
