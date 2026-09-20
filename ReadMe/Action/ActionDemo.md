# ROS 2 Actions — Demo

## 1. Check Available Actions

```bash
ros2 action list
```

---

## 2. Start the Action Server

```bash
ros2 run action_tutorials_cpp fibonacci_action_server
```

**Generic:**

```bash
ros2 run <package_name> <executable_name>
```

---

## 3. Check Actions Again

```bash
ros2 action list
```

Expected:

```text
/fibonacci
```

---

## 4. Inspect the Action

```bash
ros2 action info /fibonacci
```

**Generic:**

```bash
ros2 action info <action_name>
```

---

## 5. Check the Action Type

```bash
ros2 action type /fibonacci
```

**Generic:**

```bash
ros2 action type <action_name>
```

Expected:

```text
action_tutorials_interfaces/action/Fibonacci
```

---

## 6. Inspect the Action Interface

```bash
ros2 interface show action_tutorials_interfaces/action/Fibonacci
```

**Generic:**

```bash
ros2 interface show <interface_type>
```

Expected structure:

```text
int32 order
---
int32[] sequence
---
int32[] partial_sequence
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

## 7. Send a Goal

```bash
ros2 action send_goal /fibonacci action_tutorials_interfaces/action/Fibonacci "{order: 10}"
```

**Generic:**

```bash
ros2 action send_goal <action_name> <action_type> "<goal>"
```

You should eventually receive the result.

---

## 8. Send a Goal and Watch Feedback

```bash
ros2 action send_goal /fibonacci action_tutorials_interfaces/action/Fibonacci "{order: 10}" --feedback
```

**Generic:**

```bash
ros2 action send_goal <action_name> <action_type> "<goal>" --feedback
```

Now you should see:

```text
Goal accepted
```

followed by feedback while the action is running:

```text
Feedback:
partial_sequence: [...]
```

and finally:

```text
Result:
sequence: [...]
```

---

## 9. Inspect the Action Server Node

```bash
ros2 node info /fibonacci_action_server
```

Look under:

```text
Action Servers:
```

You should see:

```text
/fibonacci: action_tutorials_interfaces/action/Fibonacci
```

---

## 10. Stop the Action Server

```text
Ctrl + C
```

Then:

```bash
ros2 action list
```

`/fibonacci` should disappear.