# Intersection Sim Project
![Gazebo Intersection](../img/intersection_sim_gazebo.png)
![Simulated Cameras](../img/intersection_sim.png)

## Quick-Start Guide

To run the simulation, run the following launch command:
```
roslaunch intersection_sim_project sim_launch.launch
```
The vehicle will automatically drive around the loop using the programmed route shown as the green line in Rviz. It will ignore the color of the lights for now...

## Ideas

Here are some ideas for things that can be investigated with this simulation.

### *Use localization to determine where the lights should be in the image*

Real-world autonomous vehicles typically create detailed maps of their operating environments, including the locations of static infrastructure like traffic lights. The vehicle's localization system is then used to know where the lights should be in the camera's image.

This simulation emulates this capability by providing a TF transform from each traffic light to Gazebo's world frame. Using a TF lookup from the traffic light frame (child) to the vehicle's camera frame (parent), the position of the light can be determined relative to the camera, and an image region-of-interest (ROI) can be selected accordingly to focus a state detector.

Consider using the orientation of the traffic light frame with respect to the vehicle to determine which of the four lights is facing the vehicle.

### *Actively detect and classify traffic lights using machine learning*

If your computer has a discrete NVIDIA graphics card, you can run a neural network classifier to detect the traffic lights and extract their ROIs. To do this, you will need to clone this repository: [https://github.com/robustify/yolo_classification.git](https://github.com/robustify/yolo_classification.git). You will also need to follow the instructions in that repository's README for how to install CUDA, and how to compile and configure the neural network library.

After setting up the `yolo_classification` package, run this launch command while the simulation is running to detect the traffic lights:

```
roslaunch yolo_classification yolo_classification.launch camera_name:=/traffic_light_camera run_image_proc:=true image_topic:=image_rect_color
```

The output image with the classifications overlaid is published on `/traffic_light_camera/yolo_image` and can be visualized using `rqt_image_view`.

### *Use color-based processing to determine the current light state*

After extracting the ROI corresponding to a traffic light using one of the above techniques, write an image processing algorithm to determine whether red, yellow, or green is currently lit and output that state on a ROS topic.

### *Control the speed and acceleration of the vehicle to stop if necessary*

Use TF to determine how far the car is from the light at any given moment, and use that distance to regulate the speed of the vehicle to stop at the intersection if the light is red (or yellow... sometimes...)
