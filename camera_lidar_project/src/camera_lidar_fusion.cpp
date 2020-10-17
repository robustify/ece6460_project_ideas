#include <ros/ros.h>
#include "CameraLidarFusion.hpp"

int main(int argc, char** argv)
{
  ros::init(argc, argv, "camera_lidar_fusion");
  ros::NodeHandle n;
  ros::NodeHandle pn("~");
  
  camera_lidar_project::CameraLidarFusion node(n, pn);
  
  ros::spin();
}
