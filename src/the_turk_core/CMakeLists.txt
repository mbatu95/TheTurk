cmake_minimum_required(VERSION 3.8)
project(the_turk_core)

# Compiler warnings
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

# -------------------------
# Control Node
# -------------------------

add_executable(control_node
  src/control_node.cpp
)

ament_target_dependencies(
  control_node
  rclcpp
  std_msgs
)

# -------------------------
# Robot Node
# -------------------------

add_executable(robot_node
  src/robot_node.cpp
)

ament_target_dependencies(
  robot_node
  rclcpp
  std_msgs
  sensor_msgs
)

# -------------------------
# Install executables
# -------------------------

install(
  TARGETS
    control_node
    robot_node
  DESTINATION lib/${PROJECT_NAME}
)

# -------------------------
# Install resources
# -------------------------

install(
  DIRECTORY
    launch
    urdf
    rviz
    meshes
  DESTINATION share/${PROJECT_NAME}
)

# -------------------------
# Testing
# -------------------------

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)

  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)

  ament_lint_auto_find_test_dependencies()
endif()

ament_package()