cmake_minimum_required(VERSION 3.0.2)
project(lidar_slam_project)

### [Mandatory] List all ROS packages this package is dependent on.
#               This list should match the packages with <depend> tags in package.xml
find_package(catkin REQUIRED COMPONENTS
#   roscpp
)

### Use C++ 17 compiler
# add_compile_options(-std=c++17)

### [Mandatory] Identify this CMake project as a catkin package
catkin_package()

### [Mandatory] Include exported include directories from other packages
#               this package is dependent on.
# include_directories(
#   ${catkin_INCLUDE_DIRS}
# )

### Instruct catkin_make to compile a node executable program
#   First argument is the name of the resulting executable
#    After the executable name, list all .cpp files required
#        to compile the node. Keep in mind that the list of 
#        source files must consist of relative paths to the
#        files from the root of the package folder
# add_executable(template_node
#   src/template_node.cpp
#   src/Template.cpp
# )
# add_dependencies(template_node
#   ${catkin_EXPORTED_TARGETS}
# )
# target_link_libraries(template_node
#   ${catkin_LIBRARIES}
# )
