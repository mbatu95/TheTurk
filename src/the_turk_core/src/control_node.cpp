#include <chrono>
#include <cmath>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"

using namespace std::chrono_literals;

class ControlNode : public rclcpp::Node
{
public:
    ControlNode()
    : Node("control_node"),
      time_(0.0)
    {
        publisher_ =
            this->create_publisher<std_msgs::msg::Float64MultiArray>(
                "/joint_commands",
                10);

        // Her 50 ms'de bir yeni açı gönder
        timer_ =
            this->create_wall_timer(
                50ms,
                std::bind(  
                    &ControlNode::publish_joint_commands,
                    this));
    }

private:

    void publish_joint_commands()
    {
        std_msgs::msg::Float64MultiArray message;

        // Yavaş sinusoidal hareket
        double joint1 =
            60.0 * std::sin(time_);

        double joint2 =
            45.0 * std::sin(time_ * 0.7);

        message.data = {
            joint1,
            joint2
        };

        publisher_->publish(message);

        // RCLCPP_INFO(
        //     this->get_logger(),
        //     "Joint 1 = %.1f deg | Joint 2 = %.1f deg",
        //     joint1,
        //     joint2);

        // // Küçük artırıyoruz -> yavaş hareket
        time_ += 0.1;
    }


    rclcpp::Publisher<
        std_msgs::msg::Float64MultiArray
    >::SharedPtr publisher_;

    rclcpp::TimerBase::SharedPtr timer_;

    double time_;
};


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(
        std::make_shared<ControlNode>()
    );

    rclcpp::shutdown();

    return 0;
}