# LIDAR Localization
![LIDAR Mapping](../img/lidar_slam.png)

## Setup
The following four repositories must be cloned into your ROS workspace: hdl_graph_slam, hdl_localization, ndt_omp, fast_gicp
```
git clone https://github.com/robustify/hdl_graph_slam.git
git clone https://github.com/robustify/hdl_localization.git
git clone https://github.com/koide3/ndt_omp.git
git clone https://github.com/SMRT-AIST/fast_gicp.git --recursive
```
After cloning the repositories, install binary packages they depend on by running `deps.bash` from the root folder of the workspace. Finally, compile your ROS workspace with `release.bash`.

Download `campus_drive_083019.bag` to your computer.

## Build a Map
Start up the map building software:
```
roslaunch lidar_slam_project build_map.launch
```
Play the bag file to start building the map. Depending on the capability of your CPU, you might have to play the bag at a slower rate.
```
rosbag play --clock campus_drive_083019.bag -r 0.75
```

## Save Map
Call the map save service to save the map to a `.pcd` point cloud file:
```
rosservice call /hdl_graph_slam/save_map
```

## Localize on the Map
Stop the map building software and bag playback. Move the saved point cloud file and its corresponding `.utm` file into the `data` folder inside the `hdl_localization` package. Then, start the localization software:
```
roslaunch lidar_slam_project lidar_localization.launch
```
Play the same `campus_drive_083019.bag` file again. In Rviz, click the "2D Pose Estimate" button, then click where the vehicle starts on the map, drag in the direction of the way the vehicle is facing, and let go. This initializes the localization estimate.

## Fuse LIDAR Localization with Odometry

The LIDAR localization system only outputs new position and orientation estimates at the same rate as the LIDAR, which is 10 Hz. This is usually not sufficient for control applications that require a localization estimate. Therefore, adding vehicle speed and yaw rate with the LIDAR localization and implementing a Kalman filter to provide position and orientation estimates at a higher rate could be very helpful...