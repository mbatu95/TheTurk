cmake_minimum_required(VERSION 3.8)
project(the_turk_core)

# Compiler warnings
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# -------------------------
# Dependencies
# -------------------------

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)


# =========================================================
# 2 DOF
# =========================================================

# -------------------------
# 2 DOF Control Node
# -------------------------

add_executable(
  control_node
  src/2dof/control_node.cpp
)

ament_target_dependencies(
  control_node
  rclcpp
  std_msgs
)


# -------------------------
# 2 DOF Robot Node
# -------------------------

add_executable(
  robot_node
  src/2dof/robot_node.cpp
)

ament_target_dependencies(
  robot_node
  rclcpp
  std_msgs
  sensor_msgs
)


# =========================================================
# 5 DOF
# =========================================================

# -------------------------
# 5 DOF Control Node
# -------------------------

add_executable(
  control_node_5dof
  src/5dof/control_node.cpp
)

ament_target_dependencies(
  control_node_5dof
  rclcpp
  std_msgs
)


# -------------------------
# 5 DOF Robot Node
# -------------------------

add_executable(
  robot_node_5dof
  src/5dof/robot_node.cpp
)

ament_target_dependencies(
  robot_node_5dof
  rclcpp
  std_msgs
  sensor_msgs
)


# =========================================================
# Install executables
# =========================================================

install(
  TARGETS
    control_node
    robot_node
    control_node_5dof
    robot_node_5dof
  DESTINATION lib/${PROJECT_NAME}
)


# =========================================================
# Install resources
# =========================================================

install(
  DIRECTORY
    launch
    urdf
    rviz
    meshes
  DESTINATION share/${PROJECT_NAME}
)


# =========================================================
# Testing
# =========================================================

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)

  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)

  ament_lint_auto_find_test_dependencies()
endif()


ament_package()