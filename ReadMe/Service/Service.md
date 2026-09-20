# ROS 2 Services — Demo

## 1. Check Available Services

```bash
ros2 service list
```

---

## 2. Start the Service Server

```bash
ros2 run demo_nodes_cpp add_two_ints_server
```

**Generic:**

```bash
ros2 run <package_name> <executable_name>
```

---

## 3. Check Services Again

```bash
ros2 service list
```

Find:

```text
/add_two_ints
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

## 5. Check the Service Type

```bash
ros2 service type /add_two_ints
```

**Generic:**

```bash
ros2 service type <service_name>
```

Expected:

```text
example_interfaces/srv/AddTwoInts
```

---

## 6. Inspect the Service Interface

```bash
ros2 interface show example_interfaces/srv/AddTwoInts
```

**Generic:**

```bash
ros2 interface show <interface_type>
```

Expected:

```text
int64 a
int64 b
---
int64 sum
```

`---` separates:

```text
REQUEST
   ↓
---
   ↓
RESPONSE
```

---

## 7. Call the Service

```bash
ros2 service call /add_two_ints example_interfaces/srv/AddTwoInts "{a: 3, b: 5}"
```

**Generic:**

```bash
ros2 service call <service_name> <service_type> "<request>"
```

Expected response:

```text
sum: 8
```

---

## 8. Try Another Request

```bash
ros2 service call /add_two_ints example_interfaces/srv/AddTwoInts "{a: 20, b: 22}"
```

Expected:

```text
sum: 42
```

---

## 9. Inspect the Server Node

```bash
ros2 node info /add_two_ints_server
```

Find:

```text
Service Servers:
  /add_two_ints: example_interfaces/srv/AddTwoInts
```

---

## 10. Stop the Service Server

```text
Ctrl + C
```

Then:

```bash
ros2 service list
```

`/add_two_ints` should disappear.