# ROS 2 Services — Turtlesim Demo

## 1. Start Turtlesim

```bash
export DISPLAY=:12
ros2 run turtlesim turtlesim_node
```

---

## 2. Check Available Services

```bash
ros2 service list
```

Find services such as:

```text
/clear
/kill
/reset
/spawn
/turtle1/teleport_absolute
/turtle1/teleport_relative
```

---

## 3. Inspect the Spawn Service

```bash
ros2 service info /spawn
```

**Generic:**

```bash
ros2 service info <service_name>
```

---

## 4. Check the Service Type

```bash
ros2 service type /spawn
```

**Generic:**

```bash
ros2 service type <service_name>
```

Expected:

```text
turtlesim/srv/Spawn
```

---

## 5. Inspect the Service Interface

```bash
ros2 interface show turtlesim/srv/Spawn
```

**Generic:**

```bash
ros2 interface show <interface_type>
```

Expected:

```text
float32 x
float32 y
float32 theta
string name
---
string name
```

Request:

```text
x
y
theta
name
```

Response:

```text
name
```

---

## 6. Call the Spawn Service

```bash
ros2 service call /spawn turtlesim/srv/Spawn "{x: 5.0, y: 5.0, theta: 0.0, name: 'turtle2'}"
```

**Generic:**

```bash
ros2 service call <service_name> <service_type> "<request>"
```

A second turtle should appear.

Response:

```text
name: turtle2
```

---

## 7. Inspect the Kill Service

```bash
ros2 service type /kill
```

Then:

```bash
ros2 interface show turtlesim/srv/Kill
```

Expected:

```text
string name
---
```

---

## 8. Call the Kill Service

```bash
ros2 service call /kill turtlesim/srv/Kill "{name: 'turtle2'}"
```

`turtle2` should disappear.

---

## 9. Inspect the Clear Service

```bash
ros2 service type /clear
```

Then:

```bash
ros2 interface show std_srvs/srv/Empty
```

Expected:

```text
---
```

No request data.

No response data.

---

## 10. Call the Clear Service

First move the turtle around to draw lines.

Then:

```bash
ros2 service call /clear std_srvs/srv/Empty "{}"
```

The drawn lines should disappear.

---

## 11. Inspect the Turtlesim Node

```bash
ros2 node info /turtlesim
```

Look under:

```text
Service Servers:
```

You should see services such as:

```text
/clear
/kill
/reset
/spawn
```