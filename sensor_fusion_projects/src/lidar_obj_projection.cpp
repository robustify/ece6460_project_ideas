#include <ros/ros.h>
#include "LidarObjProjection.hpp"

int main(int argc, char** argv)
{
  ros::init(argc, argv, "camera_lidar_fusion");
  ros::NodeHandle n;
  ros::NodeHandle pn("~");

  sensor_fusion_projects::LidarObjProjection node(n, pn);

  ros::spin();
}
