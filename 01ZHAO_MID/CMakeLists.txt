cmake_minimum_required(VERSION 3.8)

project(OpenGL01)

set(CMAKE_CXX_STANDARD 17)

# 设置宏
add_definitions(-DDEBUG)

file(GLOB ASSETS "./assets")

file(COPY ${ASSETS} DESTINATION "${CMAKE_BINARY_DIR}/assets")

include_directories(
    SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/include
)

link_directories(
    SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

add_compile_options(-Wno-deprecated-declarations)

add_executable(OpenGL01 "./source/main.cpp" "./source/utils/log.cpp" "./glad.c")
#  "glad.c"
include_directories(OpenGL01 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/spdlog)

target_link_libraries(OpenGL01 glfw3.lib app fw wrapper)

add_subdirectory(source/application)
add_subdirectory(source/glframework)
add_subdirectory(source/wrapper)

# uniform 不能改变 相当于const 哟西