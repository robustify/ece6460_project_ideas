#include "CameraLidarFusion.hpp"

using namespace cv;

namespace camera_lidar_project
{

CameraLidarFusion::CameraLidarFusion(ros::NodeHandle n, ros::NodeHandle pn) :
  listener_(buffer_)
{
  std::string cam_ns;
  pn.param("cam_ns", cam_ns, std::string("camera"));
  ros::NodeHandle cam_nh(cam_ns);
  sub_cam_info_ = cam_nh.subscribe("camera_info", 1, &CameraLidarFusion::recvCameraInfo, this);
  sub_image_ = cam_nh.subscribe("image_rect", 1, &CameraLidarFusion::recvImage, this);
  sub_lidar_objects_ = n.subscribe("object_tracks", 1, &CameraLidarFusion::recvLidarObjects, this);
  pub_output_image_ = n.advertise<sensor_msgs::Image>("lidar_projection_image", 1);
  looked_up_camera_transform_ = false;
}

// This function is called whenever a new image is received from either
// the live running camera driver, a bag file recording, or the simulated
// image coming from Gazebo
void CameraLidarFusion::recvImage(const sensor_msgs::ImageConstPtr& msg)
{
  if (!looked_up_camera_transform_) {
    try {
      camera_transform_ = buffer_.lookupTransform("base_footprint", msg->header.frame_id, ros::Time(0));
      looked_up_camera_transform_ = true; // Once the lookup is successful, there is no need to keep doing the lookup
                                          // because the transform is constant
    } catch (tf2::TransformException& ex) {
      ROS_WARN_THROTTLE(1.0, "%s", ex.what());
    }
    return;
  }

  // Convert ROS image message into an OpenCV Mat
  cv::Mat raw_img = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image;

  for (auto& bbox : cam_bboxes_) {
    // Draw a red rectangle according to bbox geometry. Make the edges 3 pixels.
    cv::rectangle(raw_img, bbox, Scalar(0, 0, 255), 3);
  }

  sensor_msgs::ImagePtr output_img_msg = cv_bridge::CvImage(msg->header, "bgr8", raw_img).toImageMsg();
  pub_output_image_.publish(output_img_msg);
}

void CameraLidarFusion::recvLidarObjects(const avs_lecture_msgs::TrackedObjectArrayConstPtr& msg)
{
  if (!looked_up_camera_transform_) {
    return;
  }

  // Create pinhole camera model instance and load
  // its parameters from the camera info
  // generated using the checkerboard calibration program
  image_geometry::PinholeCameraModel model;
  model.fromCameraInfo(camera_info_);

  cam_bboxes_.clear();
  for (auto& obj : msg->objects) {
    tf2::Transform transform;
    tf2::convert(camera_transform_.transform, transform);
    cam_bboxes_.push_back(getCamBbox(obj, transform, model));
  }
}

cv::Rect2d CameraLidarFusion::getCamBbox(const avs_lecture_msgs::TrackedObject& object, const tf2::Transform& transform, const image_geometry::PinholeCameraModel& model)
{
  std::vector<double> xvals(2);
  std::vector<double> yvals(2);
  std::vector<double> zvals(2);
  xvals[0] = -0.5 * object.bounding_box_scale.x;
  xvals[1] = 0.5 * object.bounding_box_scale.x;
  yvals[0] = -0.5 * object.bounding_box_scale.y;
  yvals[1] = 0.5 * object.bounding_box_scale.y;
  zvals[0] = -0.5 * object.bounding_box_scale.z;
  zvals[1] = 0.5 * object.bounding_box_scale.z;

  int min_x = 99999;
  int max_x = 0;
  int min_y = 99999;
  int max_y = 0;
  for (size_t i = 0; i < xvals.size(); i++) {
    for (size_t j = 0; j < yvals.size(); j++) {
      for (size_t k = 0; k < zvals.size(); k++) {
        tf2::Vector3 cam_vect = transform.inverse() * tf2::Vector3(object.pose.position.x + xvals[i],
                                                                   object.pose.position.y + yvals[j],
                                                                   object.pose.position.z + zvals[k]);
        cv::Point2d p = model.project3dToPixel(cv::Point3d(cam_vect.x(), cam_vect.y(), cam_vect.z()));
        if (p.x < min_x) {
          min_x = p.x;
        }
        if (p.y < min_y) {
          min_y = p.y;
        }
        if (p.x > max_x) {
          max_x = p.x;
        }
        if (p.y > max_y) {
          max_y = p.y;
        }
      }
    }
  }

  cv::Rect2d cam_bbox(min_x, min_y, max_x - min_x, max_y - min_y);
  return cam_bbox;
}

void CameraLidarFusion::recvCameraInfo(const sensor_msgs::CameraInfoConstPtr& msg)
{
  camera_info_ = *msg;
}

}
