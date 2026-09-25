#include <cmath>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
#include "sensor_msgs/msg/joint_state.hpp"

class RobotNode : public rclcpp::Node
{
public:
    RobotNode()
    : Node("robot_node")
    {
        // Gelen joint komutlarını dinle
        subscription_ =
            this->create_subscription<std_msgs::msg::Float64MultiArray>(
                "/joint_commands",
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
    }

private:

    void joint_command_callback(
        const std_msgs::msg::Float64MultiArray::SharedPtr message)
    {
        if (message->data.size() < 2)
        {
            RCLCPP_WARN(
                this->get_logger(),
                "Expected two joint commands.");

            return;
        }

        double joint1_degrees = message->data[0];
        double joint2_degrees = message->data[1];

        // Degree -> radian
        double joint1_radians =
            joint1_degrees * M_PI / 180.0;

        double joint2_radians =
            joint2_degrees * M_PI / 180.0;


        sensor_msgs::msg::JointState joint_state;

        joint_state.header.stamp = this->now();

        joint_state.name = {
            "joint1",
            "joint2"
        };

        joint_state.position = {
            joint1_radians,
            joint2_radians
        };


        joint_state_publisher_->publish(joint_state);


        // RCLCPP_INFO(
        //     this->get_logger(),
        //     "Joint 1 = %.1f deg | Joint 2 = %.1f deg",
        //     joint1_degrees,
        //     joint2_degrees);
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
        std::make_shared<RobotNode>()
    );

    rclcpp::shutdown();

    return 0;
}