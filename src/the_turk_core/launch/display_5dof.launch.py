from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

import os


def generate_launch_description():

    package_path = get_package_share_directory(
        'the_turk_core'
    )

    # 5-DOF URDF
    urdf_path = os.path.join(
        package_path,
        'urdf',
        '5dof',
        'the_turk_5dof.urdf'
    )

    # Aynı RViz config'i şimdilik kullanabiliriz
    rviz_config = os.path.join(
        package_path,
        'rviz',
        'the_turk.rviz'
    )

    with open(urdf_path, 'r') as file:
        robot_description = file.read()


    # -----------------------------------------
    # Robot State Publisher
    # -----------------------------------------

    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        parameters=[
            {'robot_description': robot_description}
        ],
        output='screen'
    )


    # -----------------------------------------
    # 5-DOF Robot Node
    # -----------------------------------------

    robot_node = Node(
        package='the_turk_core',
        executable='robot_node_5dof',
        name='robot_node_5dof',
        output='screen'
    )


    # -----------------------------------------
    # 5-DOF Control Node
    # -----------------------------------------

    control_node = Node(
        package='the_turk_core',
        executable='control_node_5dof',
        name='control_node_5dof',
        output='screen'
    )


    # -----------------------------------------
    # RViz
    # -----------------------------------------

    rviz = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz',
        arguments=[
            '-d',
            rviz_config
        ],
        output='screen'
    )


    return LaunchDescription([
        robot_state_publisher,
        robot_node,
        control_node,
        rviz
    ])