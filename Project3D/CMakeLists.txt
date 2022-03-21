cmake_minimum_required(VERSION 3.19)
project(2DGameTest)

set(CMAKE_CXX_STANDARD 17)

add_executable(2DGameTest main.cpp map.cpp headers/map.h character.cpp headers/character.h enemy.cpp headers/enemy.h shot.cpp headers/shot.h platform.cpp headers/platform.h tinyxml2.cpp tinyxml2.h headers/utils.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
set(COMPILE_FLAGS "-Wdeprecated-declarations")

target_link_libraries(2DGameTest ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )