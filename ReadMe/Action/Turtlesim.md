# ROS 2 Actions — Turtlesim Demo

## 1. Start Turtlesim

```bash
export DISPLAY=:12
ros2 run turtlesim turtlesim_node
```

---

## 2. Check Available Actions

```bash
ros2 action list
```

Expected:

```text
/turtle1/rotate_absolute
```

---

## 3. Inspect the Action

```bash
ros2 action info /turtle1/rotate_absolute
```

**Generic:**

```bash
ros2 action info <action_name>
```

---

## 4. Check the Action Type

```bash
ros2 action type /turtle1/rotate_absolute
```

**Generic:**

```bash
ros2 action type <action_name>
```

Expected:

```text
turtlesim/action/RotateAbsolute
```

---

## 5. Inspect the Action Interface

```bash
ros2 interface show turtlesim/action/RotateAbsolute
```

**Generic:**

```bash
ros2 interface show <interface_type>
```

Expected structure:

```text
float32 theta
---
float32 delta
---
float32 remaining
```

The three sections represent:

```text
GOAL
---
RESULT
---
FEEDBACK
```

---

## 6. Send a Rotation Goal

```bash
ros2 action send_goal /turtle1/rotate_absolute turtlesim/action/RotateAbsolute "{theta: 1.57}"
```

**Generic:**

```bash
ros2 action send_goal <action_name> <action_type> "<goal>"
```

The turtle should rotate toward:

```text
theta = 1.57 rad ≈ 90°
```

---

## 7. Send a Goal and Watch Feedback

```bash
ros2 action send_goal /turtle1/rotate_absolute turtlesim/action/RotateAbsolute "{theta: 3.14}" --feedback
```

**Generic:**

```bash
ros2 action send_goal <action_name> <action_type> "<goal>" --feedback
```

While the turtle rotates, you should see feedback similar to:

```text
Feedback:
  remaining: ...
```

When the rotation finishes:

```text
Result:
  delta: ...
```

---

## 8. Inspect the Turtlesim Node

```bash
ros2 node info /turtlesim
```

Look under:

```text
Action Servers:
```

You should see:

```text
/turtle1/rotate_absolute: turtlesim/action/RotateAbsolute
```

---

## 9. Send Another Goal

```bash
ros2 action send_goal /turtle1/rotate_absolute turtlesim/action/RotateAbsolute "{theta: 0.0}" --feedback
```

The turtle should rotate back toward:

```text
theta = 0 rad
```

---

## 10. Stop Turtlesim

```text
Ctrl + C
```

Then:

```bash
ros2 action list
```

`/turtle1/rotate_absolute` should disappear.