# ROS 2 Nodes — Demo

## 1. Check Running Nodes

```bash
ros2 node list
```

**Generic:**

```bash
ros2 node list
```

---

## 2. Start the Talker Node

```bash
ros2 run demo_nodes_cpp talker
```

**Generic:**

```bash
ros2 run <package_name> <executable_name>
```

---

## 3. Check Running Nodes Again

```bash
ros2 node list
```

**Generic:**

```bash
ros2 node list
```

---

## 4. Inspect the Talker Node

```bash
ros2 node info /talker
```

**Generic:**

```bash
ros2 node info <node_name>
```

---

## 5. Inspect the Chatter Topic

```bash
ros2 topic info /chatter
```

**Generic:**

```bash
ros2 topic info <topic_name>
```

---

## 6. Check the Message Type

```bash
ros2 topic type /chatter
```

**Generic:**

```bash
ros2 topic type <topic_name>
```

---

## 7. Inspect the Message Interface

```bash
ros2 interface show std_msgs/msg/String
```

**Generic:**

```bash
ros2 interface show <interface_type>
```

---

## 8. Read the Published Messages

```bash
ros2 topic echo /chatter
```

**Generic:**

```bash
ros2 topic echo <topic_name>
```

---

## 9. Start the Listener Node

```bash
ros2 run demo_nodes_cpp listener
```

**Generic:**

```bash
ros2 run <package_name> <executable_name>
```

---

## 10. Check Both Nodes

```bash
ros2 node list
```

**Generic:**

```bash
ros2 node list
```

---

## 11. Inspect the Listener Node

```bash
ros2 node info /listener
```

**Generic:**

```bash
ros2 node info <node_name>
```

---

## 12. Inspect the Chatter Topic Again

```bash
ros2 topic info /chatter
```

**Generic:**

```bash
ros2 topic info <topic_name>
```

---

## 13. Visualize the ROS Graph

```bash
rqt_graph
```

**Generic:**

```bash
rqt_graph
```

---

## 14. Stop the Nodes

```text
Ctrl + C
```

Then:

```bash
ros2 node list
```