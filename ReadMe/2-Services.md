# ROS2 Services — Commands

## 1. List Available Services

```bash
ros2 service list
```

**Generic:**

```bash
ros2 service list
```

---

## 2. Start the Service Server Node

```bash
ros2 run demo_nodes_cpp add_two_ints_server
```

**Generic:**

```bash
ros2 run <package_name> <executable_name>
```

---

## 3. Check the Service Type

```bash
ros2 service type /add_two_ints
```

**Generic:**

```bash
ros2 service type <service_name>
```

---

## 4. Inspect the Service

```bash
ros2 service info /add_two_ints
```

**Generic:**

```bash
ros2 service info <service_name>
```

---

## 5. Inspect the Request / Response Interface

```bash
ros2 interface show example_interfaces/srv/AddTwoInts
```

**Generic:**

```bash
ros2 interface show <service_type>
```

---

## 6. Send a Request

```bash
ros2 service call /add_two_ints example_interfaces/srv/AddTwoInts "{a: 3, b: 5}"
```

**Generic:**

```bash
ros2 service call <service_name> <service_type> "<request_data>"
```

---

## 7. Inspect the Server Node

```bash
ros2 node info /add_two_ints_server
```

**Generic:**

```bash
ros2 node info <node_name>
```

---

## 8. Stop the Server

```text
Ctrl + C
```

Then check the available services again:

```bash
ros2 service list
```

`/add_two_ints` should disappear after the server node stops.