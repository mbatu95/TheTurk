from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

import os


def generate_launch_description():

    package_path = get_package_share_directory(
        'the_turk_core'
    )

    urdf_path = os.path.join(
        package_path,
        'urdf',
        'the_turk_pro.urdf'
    )

    rviz_config = os.path.join(
        package_path,
        'rviz',
        'the_turk.rviz'
    )

    with open(urdf_path, 'r') as file:
        robot_description = file.read()

    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[
            {'robot_description': robot_description}
        ]
    )

    rviz = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz',
        arguments=[
            '-d',
            rviz_config
        ]
    )
    
    robot_node = Node(
    package='the_turk_core',
    executable='robot_node',
    name='robot_node',
    output='screen'
)

    control_node = Node(
    package='the_turk_core',
    executable='control_node',
    name='control_node',
    output='screen'
)

    return LaunchDescription([
        robot_state_publisher,
        robot_node,
        control_node,
        rviz
    ])