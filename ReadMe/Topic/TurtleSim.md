# ROS 2 Topics — Turtlesim Demo

## 1. Start Turtlesim

```bash
export DISPLAY=:12
ros2 run turtlesim turtlesim_node
```

---

## 2. Check Available Topics

```bash
ros2 topic list
```

---

## 3. Start Keyboard Control

Open another terminal:

```bash
ros2 run turtlesim turtle_teleop_key
```

Use the arrow keys to move the turtle.

---

## 4. Inspect the Velocity Command Topic

```bash
ros2 topic info /turtle1/cmd_vel
```

**Generic:**

```bash
ros2 topic info <topic_name>
```

Expected:

```text
Type: geometry_msgs/msg/Twist
Publisher count: 1
Subscription count: 1
```

---

## 5. Check the Message Type

```bash
ros2 topic type /turtle1/cmd_vel
```

**Generic:**

```bash
ros2 topic type <topic_name>
```

Expected:

```text
geometry_msgs/msg/Twist
```

---

## 6. Inspect the Message Interface

```bash
ros2 interface show geometry_msgs/msg/Twist
```

**Generic:**

```bash
ros2 interface show <interface_type>
```

The important fields are:

```text
linear
angular
```

---

## 7. Watch the Commands

```bash
ros2 topic echo /turtle1/cmd_vel
```

Now press the arrow keys in the teleop terminal.

You should see messages such as:

```text
linear:
  x: 2.0
  y: 0.0
  z: 0.0

angular:
  x: 0.0
  y: 0.0
  z: 0.0
```

---

## 8. Publish a Command Manually

Stop using the keyboard controller and publish directly:

```bash
ros2 topic pub --once /turtle1/cmd_vel geometry_msgs/msg/Twist "{linear: {x: 2.0}, angular: {z: 0.0}}"
```

The turtle should move forward.

**Generic:**

```bash
ros2 topic pub --once <topic_name> <message_type> "<message>"
```

---

## 9. Make the Turtle Rotate

```bash
ros2 topic pub --once /turtle1/cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.0}, angular: {z: 2.0}}"
```

---

## 10. Make the Turtle Move in a Circle

```bash
ros2 topic pub --rate 10 /turtle1/cmd_vel geometry_msgs/msg/Twist "{linear: {x: 2.0}, angular: {z: 1.0}}"
```

Stop with:

```text
Ctrl + C
```

---

## 11. Inspect the Turtle Pose Topic

```bash
ros2 topic info /turtle1/pose
```

Then:

```bash
ros2 topic echo /turtle1/pose
```

The turtle continuously publishes its current state:

```text
x
y
theta
linear_velocity
angular_velocity
```

---

## 12. Visualize the Communication

```bash
export DISPLAY=:12
rqt_graph
```