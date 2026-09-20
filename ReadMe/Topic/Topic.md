# ROS 2 Topics — Demo

## 1. Check Available Topics

```bash
ros2 topic list
```

---

## 2. Start the Talker

```bash
ros2 run demo_nodes_cpp talker
```

**Generic:**

```bash
ros2 run <package_name> <executable_name>
```

---

## 3. Check Topics Again

```bash
ros2 topic list
```

Expected:

```text
/chatter
/parameter_events
/rosout
```

---

## 4. Inspect the Chatter Topic

```bash
ros2 topic info /chatter
```

**Generic:**

```bash
ros2 topic info <topic_name>
```

Expected:

```text
Type: std_msgs/msg/String
Publisher count: 1
Subscription count: 0
```

---

## 5. Check the Topic Type

```bash
ros2 topic type /chatter
```

**Generic:**

```bash
ros2 topic type <topic_name>
```

Expected:

```text
std_msgs/msg/String
```

---

## 6. Inspect the Message Interface

```bash
ros2 interface show std_msgs/msg/String
```

**Generic:**

```bash
ros2 interface show <interface_type>
```

Expected:

```text
string data
```

---

## 7. Listen to the Topic

```bash
ros2 topic echo /chatter
```

**Generic:**

```bash
ros2 topic echo <topic_name>
```

---

## 8. Inspect the Topic Again

While `ros2 topic echo /chatter` is running:

```bash
ros2 topic info /chatter
```

Expected:

```text
Type: std_msgs/msg/String
Publisher count: 1
Subscription count: 1
```

---

## 9. Start the Listener

Stop `ros2 topic echo` with:

```text
Ctrl + C
```

Then:

```bash
ros2 run demo_nodes_cpp listener
```

**Generic:**

```bash
ros2 run <package_name> <executable_name>
```

---

## 10. Inspect the Topic

```bash
ros2 topic info /chatter
```

Expected:

```text
Type: std_msgs/msg/String
Publisher count: 1
Subscription count: 1
```

---

## 11. Publish a Message Manually

Stop the Talker first with:

```text
Ctrl + C
```

Then:

```bash
ros2 topic pub --once /chatter std_msgs/msg/String "{data: 'Hello from TheTurk'}"
```

**Generic:**

```bash
ros2 topic pub --once <topic_name> <message_type> "<message>"
```

The Listener should receive:

```text
Hello from TheTurk
```

---

## 12. Publish Continuously

```bash
ros2 topic pub --rate 1 /chatter std_msgs/msg/String "{data: 'Hello from TheTurk'}"
```

**Generic:**

```bash
ros2 topic pub --rate <frequency> <topic_name> <message_type> "<message>"
```

---

## 13. Check the Publishing Frequency

Start the Talker again:

```bash
ros2 run demo_nodes_cpp talker
```

Then:

```bash
ros2 topic hz /chatter
```

**Generic:**

```bash
ros2 topic hz <topic_name>
```

---

## 14. Visualize the Communication

```bash
export DISPLAY=:12
rqt_graph
```