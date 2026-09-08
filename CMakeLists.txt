cmake_minimum_required(VERSION 3.15)

project(ScreenJumper)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(raylib REQUIRED)
find_package(OpenCV REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GIO REQUIRED gio-2.0)

add_executable(ScreenJumper
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/platforms.cpp
)

target_link_libraries(ScreenJumper PRIVATE
  raylib
  ${OpenCV_LIBS}
  ${GIO_LIBRARIES}
)

target_include_directories(ScreenJumper PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${OpenCV_INCLUDE_DIRS}
  ${GIO_INCLUDE_DIRS}
)

target_compile_definitions(ScreenJumper PRIVATE GLFW_WAYLAND=ON)
