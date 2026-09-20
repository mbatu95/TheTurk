# ROS2 Actions — Commands

## 1. List Available Actions

```bash id="ad7a01"
ros2 action list
```

**Generic:**

```bash id="ad7a02"
ros2 action list
```

---

## 2. Start the Action Server Node

```bash id="ad7a03"
ros2 run action_tutorials_cpp fibonacci_action_server
```

**Generic:**

```bash id="ad7a04"
ros2 run <package_name> <executable_name>
```

---

## 3. Check the Action Type

```bash id="ad7a05"
ros2 action type /fibonacci
```

**Generic:**

```bash id="ad7a06"
ros2 action type <action_name>
```

---

## 4. Inspect the Action

```bash id="ad7a07"
ros2 action info /fibonacci
```

**Generic:**

```bash id="ad7a08"
ros2 action info <action_name>
```

---

## 5. Inspect the Goal / Result / Feedback Interface

```bash id="ad7a09"
ros2 interface show action_tutorials_interfaces/action/Fibonacci
```

**Generic:**

```bash id="ad7a10"
ros2 interface show <action_type>
```

The interface contains three sections:

```text id="ad7a11"
GOAL
---
RESULT
---
FEEDBACK
```

---

## 6. Send a Goal

```bash id="ad7a12"
ros2 action send_goal /fibonacci action_tutorials_interfaces/action/Fibonacci "{order: 10}"
```

**Generic:**

```bash id="ad7a13"
ros2 action send_goal <action_name> <action_type> "<goal_data>"
```

This sends a goal to the Action Server and waits for the final result.

---

## 7. Send a Goal and Watch Feedback

```bash id="ad7a14"
ros2 action send_goal /fibonacci action_tutorials_interfaces/action/Fibonacci "{order: 10}" --feedback
```

**Generic:**

```bash id="ad7a15"
ros2 action send_goal <action_name> <action_type> "<goal_data>" --feedback
```

`--feedback` lets us see intermediate feedback while the Action is executing.

---

## 8. Inspect the Action Server Node

```bash id="ad7a16"
ros2 node info /fibonacci_action_server
```

**Generic:**

```bash id="ad7a17"
ros2 node info <node_name>
```

---

## 9. Stop the Action Server

```text id="ad7a18"
Ctrl + C
```

Then check the available Actions again:

```bash id="ad7a19"
ros2 action list
```

`/fibonacci` should disappear after the Action Server node stops.