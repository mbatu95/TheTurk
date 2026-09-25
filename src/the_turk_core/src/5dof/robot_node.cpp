#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
#include "sensor_msgs/msg/joint_state.hpp"

class RobotNode : public rclcpp::Node
{
public:
    RobotNode()
    : Node("robot_node_5dof")
    {
        // ControlNode'dan gelen joint komutlarını dinle
        subscription_ =
            this->create_subscription<std_msgs::msg::Float64MultiArray>(
                "/joint_commands_5dof",
                10,
                std::bind(
                    &RobotNode::joint_command_callback,
                    this,
                    std::placeholders::_1));

        // Robotun mevcut joint durumunu yayınla
        joint_state_publisher_ =
            this->create_publisher<sensor_msgs::msg::JointState>(
                "/joint_states",
                10);

        RCLCPP_INFO(
            this->get_logger(),
            "5-DOF Robot Node started.");
    }

private:
    void joint_command_callback(
        const std_msgs::msg::Float64MultiArray::SharedPtr message)
    {
        // 5 arm joint + 1 gripper = 6 değer bekliyoruz
        if (message->data.size() < 6)
        {
            RCLCPP_WARN(
                this->get_logger(),
                "Expected 6 joint commands, received %zu.",
                message->data.size());

            return;
        }

        sensor_msgs::msg::JointState joint_state;

        joint_state.header.stamp = this->now();

        joint_state.name = {
            "joint1_base_yaw",
            "joint2_shoulder_pitch",
            "joint3_elbow_pitch",
            "joint4_wrist_pitch",
            "joint5_wrist_roll",
            "gripper_joint"
        };

        // Yeni sistemde joint komutlarını doğrudan
        // SI birimleriyle gönderiyoruz:
        //
        // Revolute joints -> radians
        // Prismatic gripper -> meters

        joint_state.position = {
            message->data[0],
            message->data[1],
            message->data[2],
            message->data[3],
            message->data[4],
            message->data[5]
        };

        joint_state_publisher_->publish(joint_state);
    }

    rclcpp::Subscription<
        std_msgs::msg::Float64MultiArray
    >::SharedPtr subscription_;

    rclcpp::Publisher<
        sensor_msgs::msg::JointState
    >::SharedPtr joint_state_publisher_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(
        std::make_shared<RobotNode>());

    rclcpp::shutdown();

    return 0;
}