# ROS2 Topics — Commands

## 1. List Available Topics

```bash id="tp01"
ros2 topic list
```

**Generic:**

```bash id="tp02"
ros2 topic list
```

---

## 2. Start the Publisher Node

```bash id="tp03"
ros2 run demo_nodes_cpp talker
```

**Generic:**

```bash id="tp04"
ros2 run <package_name> <executable_name>
```

---

## 3. Check the Topic Type

```bash id="tp05"
ros2 topic type /chatter
```

**Generic:**

```bash id="tp06"
ros2 topic type <topic_name>
```

---

## 4. Inspect the Topic

```bash id="tp07"
ros2 topic info /chatter
```

**Generic:**

```bash id="tp08"
ros2 topic info <topic_name>
```

---

## 5. Inspect the Message Interface

```bash id="tp09"
ros2 interface show std_msgs/msg/String
```

**Generic:**

```bash id="tp10"
ros2 interface show <message_type>
```

---

## 6. Read Messages From the Topic

```bash id="tp11"
ros2 topic echo /chatter
```

**Generic:**

```bash id="tp12"
ros2 topic echo <topic_name>
```

The CLI temporarily acts as a subscriber and displays incoming messages.

---

## 7. Start the Subscriber Node

```bash id="tp13"
ros2 run demo_nodes_cpp listener
```

**Generic:**

```bash id="tp14"
ros2 run <package_name> <executable_name>
```

---

## 8. Publish a Message From the CLI

```bash id="tp15"
ros2 topic pub --once /chatter std_msgs/msg/String "{data: 'Hello from TheTurk'}"
```

**Generic:**

```bash id="tp16"
ros2 topic pub --once <topic_name> <message_type> "<message_data>"
```

The CLI temporarily acts as a publisher and sends one message.

---

## 9. Check the Publishing Frequency

```bash id="tp17"
ros2 topic hz /chatter
```

**Generic:**

```bash id="tp18"
ros2 topic hz <topic_name>
```

Shows approximately how many messages per second are being published.

---

## 10. Inspect the Publisher Node

```bash id="tp19"
ros2 node info /talker
```

**Generic:**

```bash id="tp20"
ros2 node info <node_name>
```

---

## 11. Visualize the ROS Graph

```bash id="tp21"
rqt_graph
```

Shows the active nodes and their topic connections visually.

---

## 12. Stop the Nodes

```text id="tp22"
Ctrl + C
```

After stopping the publisher and subscriber, check the available topics again:

```bash id="tp23"
ros2 topic list
```

`/chatter` should disappear when no active publisher or subscriber uses it.