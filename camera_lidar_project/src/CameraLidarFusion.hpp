#pragma once

#include <ros/ros.h>
#include <avs_lecture_msgs/TrackedObjectArray.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/static_transform_broadcaster.h>
#include <image_geometry/pinhole_camera_model.h>
#include <dynamic_reconfigure/server.h>
#include <camera_lidar_project/CameraLidarFusionConfig.h>

namespace camera_lidar_project
{

class CameraLidarFusion
{
  public:
    CameraLidarFusion(ros::NodeHandle n, ros::NodeHandle pn);

  private:
    void reconfig(CameraLidarFusionConfig& config, uint32_t level);
    void recvImage(const sensor_msgs::ImageConstPtr& msg);
    void recvCameraInfo(const sensor_msgs::CameraInfoConstPtr& msg);
    void recvLidarObjects(const avs_lecture_msgs::TrackedObjectArrayConstPtr& msg);
    cv::Rect2d getCamBbox(const avs_lecture_msgs::TrackedObject& object, const tf2::Transform& transform, const image_geometry::PinholeCameraModel& model);

    geometry_msgs::Point projectPoint(const image_geometry::PinholeCameraModel& model, const cv::Point2d& p);

    tf2_ros::TransformListener listener_;
    tf2_ros::Buffer buffer_;
    tf2_ros::StaticTransformBroadcaster broadcaster_;

    ros::Subscriber sub_image_;
    ros::Subscriber sub_cam_info_;
    ros::Subscriber sub_lidar_objects_;
    ros::Publisher pub_markers_;

    std::shared_ptr<dynamic_reconfigure::Server<CameraLidarFusionConfig> > srv_;

    sensor_msgs::CameraInfo camera_info_;
    geometry_msgs::TransformStamped camera_transform_; // Coordinate transformation from footprint to camera
    bool looked_up_camera_transform_;
    std::vector<cv::Rect2d> cam_bboxes_;
};

}
