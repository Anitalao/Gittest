cmake_minimum_required(VERSION 2.8.3)
project(app)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  rviz
)

find_package(Qt5  REQUIRED COMPONENTS  Widgets  )
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES
  src/main.cpp
  src/loginwidget.cpp
  src/qrviz.cpp
)

set(FORMS
  src/loginwidget.ui
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES app
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)

file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/app/*.hpp *.h basewidget/*.hpp *.h)

include_directories(
  include/app

  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_node ${SOURCES} ${FORMS} )

target_link_libraries(${PROJECT_NAME}_node
   ${catkin_LIBRARIES}
)

target_link_libraries(${PROJECT_NAME}_node  Qt5::Widgets )