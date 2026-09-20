# ROS 2 Nodes — Turtlesim Demo

## 1. Start the Turtlesim Node

```bash
export DISPLAY=:12
ros2 run turtlesim turtlesim_node
```

**Generic:**

```bash
ros2 run <package_name> <executable_name>
```

---

## 2. Check Running Nodes

```bash
ros2 node list
```

Expected:

```text
/turtlesim
```

---

## 3. Inspect the Turtlesim Node

```bash
ros2 node info /turtlesim
```

**Generic:**

```bash
ros2 node info <node_name>
```

---

## 4. Start the Teleop Node

Open another terminal:

```bash
ros2 run turtlesim turtle_teleop_key
```

**Generic:**

```bash
ros2 run <package_name> <executable_name>
```

---

## 5. Check Running Nodes Again

```bash
ros2 node list
```

Expected:

```text
/teleop_turtle
/turtlesim
```

---

## 6. Inspect the Teleop Node

```bash
ros2 node info /teleop_turtle
```

**Generic:**

```bash
ros2 node info <node_name>
```

---

## 7. Compare the Two Nodes

```bash
ros2 node info /turtlesim
```

```bash
ros2 node info /teleop_turtle
```

---

## 8. Visualize the ROS Graph

```bash
export DISPLAY=:12
rqt_graph
```

---

## 9. Stop the Teleop Node

```text
Ctrl + C
```

Then:

```bash
ros2 node list
```

Expected:

```text
/turtlesim
```

---

## 10. Stop the Turtlesim Node

```text
Ctrl + C
```

Then:

```bash
ros2 node list
```

The Turtlesim nodes should no longer appear.