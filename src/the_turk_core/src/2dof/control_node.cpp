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
      state_(MotionState::HOME),
      current_joint1_(0.0),
      current_joint2_(0.0),
      state_time_(0.0)
    {
        publisher_ =
            this->create_publisher<std_msgs::msg::Float64MultiArray>(
                "/joint_commands",
                10);

        timer_ =
            this->create_wall_timer(
                50ms,
                std::bind(
                    &ControlNode::update_motion,
                    this));

        RCLCPP_INFO(
            this->get_logger(),
            "Chess motion started.");
    }

private:

    // Robotun yapacağı hareket aşamaları
    enum class MotionState
    {
        HOME,
        APPROACH_PIECE,
        PICK_PIECE,
        LIFT_PIECE,
        MOVE_TO_CLOCK,
        PRESS_CLOCK,
        RETRACT,
        RETURN_HOME
    };


    void update_motion()
    {
        double target_joint1 = 0.0;
        double target_joint2 = 0.0;

        // -----------------------------------------
        // 1. Hangi aşamadaysak hedef açıları belirle
        // -----------------------------------------

        switch (state_)
        {
            case MotionState::HOME:

                target_joint1 = 0.0;
                target_joint2 = 0.0;

                if (state_time_ > 1.0)
                {
                    change_state(
                        MotionState::APPROACH_PIECE,
                        "Approaching chess piece");
                }

                break;


            case MotionState::APPROACH_PIECE:

                target_joint1 = 25.0;
                target_joint2 = -35.0;

                if (
                    reached_target(
                        target_joint1,
                        target_joint2))
                {
                    change_state(
                        MotionState::PICK_PIECE,
                        "Chess piece reached");
                }

                break;


            case MotionState::PICK_PIECE:

                target_joint1 = 32.0;
                target_joint2 = -48.0;

                if (
                    reached_target(
                        target_joint1,
                        target_joint2)
                    &&
                    state_time_ > 1.0)
                {
                    change_state(
                        MotionState::LIFT_PIECE,
                        "Piece picked");
                }

                break;


            case MotionState::LIFT_PIECE:

                target_joint1 = 20.0;
                target_joint2 = -20.0;

                if (
                    reached_target(
                        target_joint1,
                        target_joint2))
                {
                    change_state(
                        MotionState::MOVE_TO_CLOCK,
                        "Moving toward chess clock");
                }

                break;


            case MotionState::MOVE_TO_CLOCK:

                target_joint1 = -30.0;
                target_joint2 = 15.0;

                if (
                    reached_target(
                        target_joint1,
                        target_joint2))
                {
                    change_state(
                        MotionState::PRESS_CLOCK,
                        "Pressing chess clock");
                }

                break;


            case MotionState::PRESS_CLOCK:

                target_joint1 = -40.0;
                target_joint2 = 25.0;

                if (
                    reached_target(
                        target_joint1,
                        target_joint2)
                    &&
                    state_time_ > 0.7)
                {
                    change_state(
                        MotionState::RETRACT,
                        "Retracting from clock");
                }

                break;


            case MotionState::RETRACT:

                target_joint1 = -25.0;
                target_joint2 = 5.0;

                if (
                    reached_target(
                        target_joint1,
                        target_joint2))
                {
                    change_state(
                        MotionState::RETURN_HOME,
                        "Returning home");
                }

                break;


            case MotionState::RETURN_HOME:

                target_joint1 = 0.0;
                target_joint2 = 0.0;

                if (
                    reached_target(
                        target_joint1,
                        target_joint2))
                {
                    change_state(
                        MotionState::HOME,
                        "Motion completed");
                }

                break;
        }


        // -----------------------------------------
        // 2. Hedef açıya yumuşak şekilde yaklaş
        // -----------------------------------------

        double smoothing = 0.08;

        current_joint1_ +=
            (target_joint1 - current_joint1_)
            * smoothing;

        current_joint2_ +=
            (target_joint2 - current_joint2_)
            * smoothing;


        // -----------------------------------------
        // 3. ROS mesajını oluştur
        // -----------------------------------------

        std_msgs::msg::Float64MultiArray message;

        message.data = {
            current_joint1_,
            current_joint2_
        };


        // -----------------------------------------
        // 4. Joint komutlarını yayınla
        // -----------------------------------------

        publisher_->publish(message);


        // Timer her 50 ms çalışıyor
        // 50 ms = 0.05 saniye
        state_time_ += 0.05;
    }


    // -----------------------------------------
    // Hedefe ulaştık mı?
    // -----------------------------------------

    bool reached_target(
        double target_joint1,
        double target_joint2)
    {
        double error_joint1 =
            std::abs(
                target_joint1
                - current_joint1_);

        double error_joint2 =
            std::abs(
                target_joint2
                - current_joint2_);

        double tolerance = 1.0;

        return
            error_joint1 < tolerance
            &&
            error_joint2 < tolerance;
    }


    // -----------------------------------------
    // Yeni hareket aşamasına geç
    // -----------------------------------------

    void change_state(
        MotionState new_state,
        const char * message)
    {
        state_ = new_state;

        state_time_ = 0.0;

        RCLCPP_INFO(
            this->get_logger(),
            "%s",
            message);
    }


    // -----------------------------------------
    // ROS Publisher
    // -----------------------------------------

    rclcpp::Publisher<
        std_msgs::msg::Float64MultiArray
    >::SharedPtr publisher_;


    // -----------------------------------------
    // ROS Timer
    // -----------------------------------------

    rclcpp::TimerBase::SharedPtr timer_;


    // -----------------------------------------
    // Motion State
    // -----------------------------------------

    MotionState state_;


    // -----------------------------------------
    // Robotun mevcut komut açıları
    // -----------------------------------------

    double current_joint1_;
    double current_joint2_;


    // -----------------------------------------
    // Mevcut state'te geçen süre
    // -----------------------------------------

    double state_time_;
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