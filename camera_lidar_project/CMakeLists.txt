cmake_minimum_required(VERSION 3.0.2)
project(camera_lidar_project)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  image_transport
  image_geometry
  cv_bridge
  tf2_ros
  tf2_geometry_msgs
  avs_lecture_msgs
)
find_package(OpenCV REQUIRED)
catkin_python_setup()

catkin_package(
  DEPENDS OpenCV
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}
  src/camera_lidar_fusion.cpp
  src/CameraLidarFusion.cpp
)
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
)

catkin_install_python(
  PROGRAMS
    src/${PROJECT_NAME}/pub_camera_info.py
  DESTINATION
    ${CATKIN_PACKAGE_BIN_DESTINATION}
)
