#!/bin/bash
rosservice call /hdl_graph_slam/save_map "{utm: true, resolution: 0.05, destination: 'map.pcd'}"