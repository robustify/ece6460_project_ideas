#!/usr/bin/env python

import rospy
import sys
import yaml
from sensor_msgs.msg import CameraInfo, Image

class PubCameraInfo:
    def __init__(self):
        rospy.init_node('pub_camera_info')
        yaml_file = rospy.get_param('~yaml_file')

        # Load data from file
        with open(yaml_file, "r") as file_handle:
            calib_data = yaml.load(file_handle)
        
        self.camera_info_msg = CameraInfo()
        try:
            self.camera_info_msg.width = calib_data["image_width"]
            self.camera_info_msg.height = calib_data["image_height"]
            self.camera_info_msg.K = calib_data["camera_matrix"]["data"]
            self.camera_info_msg.D = calib_data["distortion_coefficients"]["data"]
            self.camera_info_msg.R = calib_data["rectification_matrix"]["data"]
            self.camera_info_msg.P = calib_data["projection_matrix"]["data"]
            self.camera_info_msg.distortion_model = calib_data["distortion_model"]
        except TypeError as e:
            rospy.logwarn('Camera info not found... shutting down')
            sys.exit(0)

        self.sub_image = rospy.Subscriber('image_raw', Image, self.recv_image)
        self.pub_info = rospy.Publisher('camera_info', CameraInfo, latch=True, queue_size=1)
    
    def recv_image(self, msg):
        self.camera_info_msg.header = msg.header
        self.pub_info.publish(self.camera_info_msg)

if __name__ == '__main__':
    node_instance = PubCameraInfo()
    rospy.spin()
