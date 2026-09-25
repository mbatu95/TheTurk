#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <limits>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"

using namespace std::chrono_literals;

class ControlNode : public rclcpp::Node
{
public:
    ControlNode()
    : Node("control_node_5dof")
    {
        publisher_ =
            this->create_publisher<std_msgs::msg::Float64MultiArray>(
                "/joint_commands_5dof",
                10);

        // -------------------------------------------------
        // Initial Cartesian pose
        // -------------------------------------------------

        current_pose_ = {
            0.48,   // x
            0.00,   // y
            0.48,   // z
            TOOL_DOWN,
            0.0
        };

        start_pose_ = current_pose_;
        target_pose_ = current_pose_;

        current_gripper_ = GRIPPER_OPEN;
        start_gripper_ = current_gripper_;
        target_gripper_ = current_gripper_;

        // Initial joint estimate
        current_joint_command_ = {
            0.0,
            -0.60,
            1.20,
            -0.60,
            0.0,
            GRIPPER_OPEN
        };

        timer_ =
            this->create_wall_timer(
                50ms,
                std::bind(
                    &ControlNode::update,
                    this));

        RCLCPP_INFO(
            this->get_logger(),
            "Advanced 5-DOF chess controller started.");

        // First trajectory:
        // move from initial position to safe position
        set_cartesian_target(
            State::MOVE_SAFE,
            SAFE_X,
            SAFE_Y,
            SAFE_Z,
            TOOL_DOWN,
            0.0,
            GRIPPER_OPEN,
            2.0,
            "Moving to safe position");
    }

private:

    // =====================================================
    // CONSTANTS
    // =====================================================

    static constexpr double PI =
        3.14159265358979323846;

    static constexpr double TOOL_DOWN =
        -PI / 2.0;

    static constexpr double DT =
        0.05;

    // -----------------------------------------------------
    // Simplified robot geometry
    // -----------------------------------------------------

    static constexpr double BASE_HEIGHT =
        0.385;

    static constexpr double L1 =
        0.50;

    static constexpr double L2 =
        0.43;

    // -----------------------------------------------------
    // Chess board geometry
    // -----------------------------------------------------

    // Height of top surface of board relative to base frame.
    // Tune this when the board is added to simulation.
    static constexpr double BOARD_Z =
        0.020;

    // Approximate height where gripper center should be
    // while holding the chess piece.
    static constexpr double PIECE_GRASP_HEIGHT =
        0.065;

    static constexpr double GRASP_Z =
        BOARD_Z + PIECE_GRASP_HEIGHT;

    // Safe vertical clearance above pieces.
    static constexpr double TRAVEL_Z =
        0.38;

    static constexpr double APPROACH_Z =
        0.25;

    // -----------------------------------------------------
    // Example chess positions
    // -----------------------------------------------------

    static constexpr double PICK_X =
        0.55;

    static constexpr double PICK_Y =
        0.15;

    static constexpr double PLACE_X =
        0.40;

    static constexpr double PLACE_Y =
        -0.20;

    // -----------------------------------------------------
    // Safe position
    // -----------------------------------------------------

    static constexpr double SAFE_X =
        0.50;

    static constexpr double SAFE_Y =
        0.0;

    static constexpr double SAFE_Z =
        0.50;

    // -----------------------------------------------------
    // Gripper
    // -----------------------------------------------------

    static constexpr double GRIPPER_OPEN =
        0.030;

    static constexpr double GRIPPER_CLOSED =
        0.005;

    // =====================================================
    // DATA TYPES
    // =====================================================

    struct CartesianPose
    {
        double x;
        double y;
        double z;

        double pitch;
        double roll;
    };

    struct JointCommand
    {
        double q1;
        double q2;
        double q3;
        double q4;
        double q5;

        double gripper;
    };

    enum class State
    {
        MOVE_SAFE,

        MOVE_ABOVE_PICK,
        DESCEND_TO_PICK,

        CLOSE_GRIPPER,

        LIFT_PIECE,

        MOVE_ABOVE_PLACE,
        DESCEND_TO_PLACE,

        OPEN_GRIPPER,

        LIFT_AFTER_PLACE,

        RETURN_SAFE,

        RETURN_HOME,

        FINISHED
    };

    // =====================================================
    // JOINT LIMITS
    // =====================================================

    // These should correspond to the URDF limits.

    static constexpr double Q1_MIN =
        -PI;

    static constexpr double Q1_MAX =
        PI;

    static constexpr double Q2_MIN =
        -1.74533;

    static constexpr double Q2_MAX =
        1.57080;

    static constexpr double Q3_MIN =
        -2.35619;

    static constexpr double Q3_MAX =
        2.35619;

    static constexpr double Q4_MIN =
        -2.09440;

    static constexpr double Q4_MAX =
        2.09440;

    static constexpr double Q5_MIN =
        -PI;

    static constexpr double Q5_MAX =
        PI;

    // Maximum joint change per 50 ms command.
    // This prevents sudden large jumps if IK branch changes.

    static constexpr double MAX_JOINT_STEP =
        0.06;

    // =====================================================
    // STATE VARIABLES
    // =====================================================

    State state_ =
        State::MOVE_SAFE;

    CartesianPose current_pose_;
    CartesianPose start_pose_;
    CartesianPose target_pose_;

    double current_gripper_;
    double start_gripper_;
    double target_gripper_;

    double trajectory_time_ =
        0.0;

    double trajectory_duration_ =
        1.0;

    JointCommand current_joint_command_;

    // =====================================================
    // UTILITY
    // =====================================================

    double clamp(
        double value,
        double minimum,
        double maximum)
    {
        return std::max(
            minimum,
            std::min(
                value,
                maximum));
    }

    // -----------------------------------------------------
    // Quintic interpolation
    //
    // s(t) = 10t^3 - 15t^4 + 6t^5
    //
    // velocity and acceleration both start/end smoothly.
    // -----------------------------------------------------

    double quintic(
        double t)
    {
        t =
            clamp(
                t,
                0.0,
                1.0);

        double t2 = t * t;
        double t3 = t2 * t;
        double t4 = t3 * t;
        double t5 = t4 * t;

        return
            10.0 * t3
            -
            15.0 * t4
            +
            6.0 * t5;
    }

    double interpolate(
        double start,
        double target,
        double alpha)
    {
        return
            start
            +
            alpha *
            (target - start);
    }

    // =====================================================
    // JOINT LIMIT CHECK
    // =====================================================

    bool joint_limits_valid(
        const JointCommand & q)
    {
        return
            q.q1 >= Q1_MIN &&
            q.q1 <= Q1_MAX &&

            q.q2 >= Q2_MIN &&
            q.q2 <= Q2_MAX &&

            q.q3 >= Q3_MIN &&
            q.q3 <= Q3_MAX &&

            q.q4 >= Q4_MIN &&
            q.q4 <= Q4_MAX &&

            q.q5 >= Q5_MIN &&
            q.q5 <= Q5_MAX;
    }

    // =====================================================
    // IK SOLUTION COST
    // =====================================================

    double solution_cost(
        const JointCommand & candidate)
    {
        double dq1 =
            candidate.q1 -
            current_joint_command_.q1;

        double dq2 =
            candidate.q2 -
            current_joint_command_.q2;

        double dq3 =
            candidate.q3 -
            current_joint_command_.q3;

        double dq4 =
            candidate.q4 -
            current_joint_command_.q4;

        double dq5 =
            candidate.q5 -
            current_joint_command_.q5;

        return
            dq1 * dq1 +
            dq2 * dq2 +
            dq3 * dq3 +
            dq4 * dq4 +
            dq5 * dq5;
    }

    // =====================================================
    // CALCULATE ONE IK BRANCH
    // =====================================================

    bool calculate_ik_branch(
        double x,
        double y,
        double z,
        double tool_pitch,
        double wrist_roll,
        double elbow_sign,
        JointCommand & solution)
    {
        // -------------------------------------------------
        // J1: base rotation
        // -------------------------------------------------

        double q1 =
            std::atan2(
                y,
                x);

        // -------------------------------------------------
        // Horizontal distance
        // -------------------------------------------------

        double r =
            std::sqrt(
                x * x +
                y * y);

        // Basic keep-out zone around base.
        // Don't allow end-effector too close to base axis.

        if (r < 0.25)
        {
            return false;
        }

        // -------------------------------------------------
        // Relative height
        // -------------------------------------------------

        double z_relative =
            z - BASE_HEIGHT;

        // -------------------------------------------------
        // Cosine law
        // -------------------------------------------------

        double D =
            (
                r * r +
                z_relative * z_relative -
                L1 * L1 -
                L2 * L2
            )
            /
            (
                2.0 *
                L1 *
                L2
            );

        if (D < -1.0 || D > 1.0)
        {
            return false;
        }

        D =
            clamp(
                D,
                -1.0,
                1.0);

        // -------------------------------------------------
        // J3: elbow
        //
        // Two possible solutions:
        // +acos(D)
        // -acos(D)
        // -------------------------------------------------

        double q3 =
            elbow_sign *
            std::acos(D);

        // -------------------------------------------------
        // J2: shoulder
        // -------------------------------------------------

        double q2 =
            std::atan2(
                z_relative,
                r)
            -
            std::atan2(
                L2 * std::sin(q3),
                L1 + L2 * std::cos(q3));

        // -------------------------------------------------
        // J4: wrist pitch
        // -------------------------------------------------

        double q4 =
            tool_pitch -
            q2 -
            q3;

        // -------------------------------------------------
        // J5: wrist roll
        // -------------------------------------------------

        double q5 =
            wrist_roll;

        solution.q1 =
            q1;

        solution.q2 =
            q2;

        solution.q3 =
            q3;

        solution.q4 =
            q4;

        solution.q5 =
            q5;

        solution.gripper =
            current_gripper_;

        return
            joint_limits_valid(
                solution);
    }

    // =====================================================
    // INVERSE KINEMATICS
    // =====================================================

    bool inverse_kinematics(
        const CartesianPose & pose,
        JointCommand & best_solution)
    {
        JointCommand elbow_up;
        JointCommand elbow_down;

        bool valid_up =
            calculate_ik_branch(
                pose.x,
                pose.y,
                pose.z,
                pose.pitch,
                pose.roll,
                +1.0,
                elbow_up);

        bool valid_down =
            calculate_ik_branch(
                pose.x,
                pose.y,
                pose.z,
                pose.pitch,
                pose.roll,
                -1.0,
                elbow_down);

        if (!valid_up && !valid_down)
        {
            RCLCPP_WARN_THROTTLE(
                this->get_logger(),
                *this->get_clock(),
                1000,
                "No valid IK solution for x=%.3f y=%.3f z=%.3f",
                pose.x,
                pose.y,
                pose.z);

            return false;
        }

        if (valid_up && !valid_down)
        {
            best_solution =
                elbow_up;

            return true;
        }

        if (!valid_up && valid_down)
        {
            best_solution =
                elbow_down;

            return true;
        }

        // Both are valid.
        // Pick the one closest to current joint configuration.

        double cost_up =
            solution_cost(
                elbow_up);

        double cost_down =
            solution_cost(
                elbow_down);

        if (cost_up <= cost_down)
        {
            best_solution =
                elbow_up;
        }
        else
        {
            best_solution =
                elbow_down;
        }

        return true;
    }

    // =====================================================
    // JOINT STEP LIMITER
    // =====================================================

    double limit_joint_step(
        double current,
        double target)
    {
        double difference =
            target - current;

        difference =
            clamp(
                difference,
                -MAX_JOINT_STEP,
                MAX_JOINT_STEP);

        return
            current +
            difference;
    }

    // =====================================================
    // PUBLISH COMMAND
    // =====================================================

    void publish_pose(
        const CartesianPose & pose,
        double gripper)
    {
        JointCommand target;

        if (!inverse_kinematics(
                pose,
                target))
        {
            return;
        }

        // -------------------------------------------------
        // Limit sudden joint changes
        // -------------------------------------------------

        target.q1 =
            limit_joint_step(
                current_joint_command_.q1,
                target.q1);

        target.q2 =
            limit_joint_step(
                current_joint_command_.q2,
                target.q2);

        target.q3 =
            limit_joint_step(
                current_joint_command_.q3,
                target.q3);

        target.q4 =
            limit_joint_step(
                current_joint_command_.q4,
                target.q4);

        target.q5 =
            limit_joint_step(
                current_joint_command_.q5,
                target.q5);

        target.gripper =
            gripper;

        current_joint_command_ =
            target;

        std_msgs::msg::Float64MultiArray message;

        message.data = {
            target.q1,
            target.q2,
            target.q3,
            target.q4,
            target.q5,
            target.gripper
        };

        publisher_->publish(
            message);
    }

    // =====================================================
    // CREATE NEW CARTESIAN TRAJECTORY
    // =====================================================

    void set_cartesian_target(
        State next_state,
        double x,
        double y,
        double z,
        double pitch,
        double roll,
        double gripper,
        double duration,
        const std::string & description)
    {
        state_ =
            next_state;

        start_pose_ =
            current_pose_;

        target_pose_ = {
            x,
            y,
            z,
            pitch,
            roll
        };

        start_gripper_ =
            current_gripper_;

        target_gripper_ =
            gripper;

        trajectory_time_ =
            0.0;

        trajectory_duration_ =
            duration;

        RCLCPP_INFO(
            this->get_logger(),
            "%s",
            description.c_str());
    }

    // =====================================================
    // UPDATE CARTESIAN TRAJECTORY
    // =====================================================

    void update()
    {
        if (state_ == State::FINISHED)
        {
            return;
        }

        trajectory_time_ +=
            DT;

        double normalized_time =
            trajectory_time_
            /
            trajectory_duration_;

        normalized_time =
            clamp(
                normalized_time,
                0.0,
                1.0);

        double alpha =
            quintic(
                normalized_time);

        // -------------------------------------------------
        // Interpolate position
        // -------------------------------------------------

        current_pose_.x =
            interpolate(
                start_pose_.x,
                target_pose_.x,
                alpha);

        current_pose_.y =
            interpolate(
                start_pose_.y,
                target_pose_.y,
                alpha);

        current_pose_.z =
            interpolate(
                start_pose_.z,
                target_pose_.z,
                alpha);

        // -------------------------------------------------
        // Interpolate orientation
        // -------------------------------------------------

        current_pose_.pitch =
            interpolate(
                start_pose_.pitch,
                target_pose_.pitch,
                alpha);

        current_pose_.roll =
            interpolate(
                start_pose_.roll,
                target_pose_.roll,
                alpha);

        // -------------------------------------------------
        // Interpolate gripper
        // -------------------------------------------------

        current_gripper_ =
            interpolate(
                start_gripper_,
                target_gripper_,
                alpha);

        // -------------------------------------------------
        // Calculate IK and publish
        // -------------------------------------------------

        publish_pose(
            current_pose_,
            current_gripper_);

        if (normalized_time < 1.0)
        {
            return;
        }

        // Force exact final Cartesian state.

        current_pose_ =
            target_pose_;

        current_gripper_ =
            target_gripper_;

        next_state();
    }

    // =====================================================
    // STATE MACHINE
    // =====================================================

    void next_state()
    {
        switch (state_)
        {
            // =================================================
            // SAFE -> ABOVE PIECE
            // =================================================

            case State::MOVE_SAFE:
            {
                set_cartesian_target(
                    State::MOVE_ABOVE_PICK,

                    PICK_X,
                    PICK_Y,
                    APPROACH_Z,

                    TOOL_DOWN,
                    0.0,

                    GRIPPER_OPEN,

                    2.5,

                    "Moving above chess piece");

                break;
            }

            // =================================================
            // ABOVE PIECE -> VERTICAL DESCENT
            // =================================================

            case State::MOVE_ABOVE_PICK:
            {
                set_cartesian_target(
                    State::DESCEND_TO_PICK,

                    PICK_X,
                    PICK_Y,
                    GRASP_Z,

                    TOOL_DOWN,
                    0.0,

                    GRIPPER_OPEN,

                    2.0,

                    "Descending vertically to chess piece");

                break;
            }

            // =================================================
            // REACHED PIECE -> CLOSE GRIPPER
            // =================================================

            case State::DESCEND_TO_PICK:
            {
                set_cartesian_target(
                    State::CLOSE_GRIPPER,

                    PICK_X,
                    PICK_Y,
                    GRASP_Z,

                    TOOL_DOWN,
                    0.0,

                    GRIPPER_CLOSED,

                    1.0,

                    "Closing gripper");

                break;
            }

            // =================================================
            // PICK -> VERTICAL LIFT
            // =================================================

            case State::CLOSE_GRIPPER:
            {
                set_cartesian_target(
                    State::LIFT_PIECE,

                    PICK_X,
                    PICK_Y,
                    TRAVEL_Z,

                    TOOL_DOWN,
                    0.0,

                    GRIPPER_CLOSED,

                    2.0,

                    "Lifting chess piece vertically");

                break;
            }

            // =================================================
            // HIGH -> TRAVEL ABOVE TARGET
            // =================================================

            case State::LIFT_PIECE:
            {
                set_cartesian_target(
                    State::MOVE_ABOVE_PLACE,

                    PLACE_X,
                    PLACE_Y,
                    TRAVEL_Z,

                    TOOL_DOWN,
                    0.0,

                    GRIPPER_CLOSED,

                    3.0,

                    "Moving above target square");

                break;
            }

            // =================================================
            // ABOVE TARGET -> VERTICAL DESCENT
            // =================================================

            case State::MOVE_ABOVE_PLACE:
            {
                set_cartesian_target(
                    State::DESCEND_TO_PLACE,

                    PLACE_X,
                    PLACE_Y,
                    GRASP_Z,

                    TOOL_DOWN,
                    0.0,

                    GRIPPER_CLOSED,

                    2.0,

                    "Descending vertically to target square");

                break;
            }

            // =================================================
            // PLACE -> OPEN GRIPPER
            // =================================================

            case State::DESCEND_TO_PLACE:
            {
                set_cartesian_target(
                    State::OPEN_GRIPPER,

                    PLACE_X,
                    PLACE_Y,
                    GRASP_Z,

                    TOOL_DOWN,
                    0.0,

                    GRIPPER_OPEN,

                    1.0,

                    "Opening gripper");

                break;
            }

            // =================================================
            // RELEASE -> VERTICAL RETREAT
            // =================================================

            case State::OPEN_GRIPPER:
            {
                set_cartesian_target(
                    State::LIFT_AFTER_PLACE,

                    PLACE_X,
                    PLACE_Y,
                    TRAVEL_Z,

                    TOOL_DOWN,
                    0.0,

                    GRIPPER_OPEN,

                    2.0,

                    "Retreating vertically");

                break;
            }

            // =================================================
            // RETREAT -> CENTRAL SAFE POINT
            // =================================================

            case State::LIFT_AFTER_PLACE:
            {
                set_cartesian_target(
                    State::RETURN_SAFE,

                    SAFE_X,
                    SAFE_Y,
                    SAFE_Z,

                    TOOL_DOWN,
                    0.0,

                    GRIPPER_OPEN,

                    3.0,

                    "Returning through safe waypoint");

                break;
            }

            // =================================================
            // SAFE -> HOME
            // =================================================

            case State::RETURN_SAFE:
            {
                set_cartesian_target(
                    State::RETURN_HOME,

                    0.48,
                    0.0,
                    0.48,

                    TOOL_DOWN,
                    0.0,

                    GRIPPER_OPEN,

                    2.0,

                    "Returning home");

                break;
            }

            // =================================================
            // FINISHED
            // =================================================

            case State::RETURN_HOME:
            {
                state_ =
                    State::FINISHED;

                RCLCPP_INFO(
                    this->get_logger(),
                    "Chess pick-and-place completed.");

                break;
            }

            case State::FINISHED:
            default:
            {
                break;
            }
        }
    }

    // =====================================================
    // ROS
    // =====================================================

    rclcpp::Publisher<
        std_msgs::msg::Float64MultiArray
    >::SharedPtr publisher_;

    rclcpp::TimerBase::SharedPtr timer_;
};


// =========================================================
// MAIN
// =========================================================

int main(
    int argc,
    char * argv[])
{
    rclcpp::init(
        argc,
        argv);

    rclcpp::spin(
        std::make_shared<ControlNode>());

    rclcpp::shutdown();

    return 0;
}