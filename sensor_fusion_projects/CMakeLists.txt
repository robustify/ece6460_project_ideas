cmake_minimum_required(VERSION 3.0.2)
project(sensor_fusion_projects)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  image_transport
  image_geometry
  cv_bridge
  tf2_ros
  tf2_geometry_msgs
  avs_lecture_msgs
)
find_package(OpenCV REQUIRED)

catkin_package(
  DEPENDS OpenCV
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(lidar_obj_projection
  src/lidar_obj_projection.cpp
  src/LidarObjProjection.cpp
)
add_dependencies(lidar_obj_projection
  ${catkin_EXPORTED_TARGETS}
)
target_link_libraries(lidar_obj_projection
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
)
