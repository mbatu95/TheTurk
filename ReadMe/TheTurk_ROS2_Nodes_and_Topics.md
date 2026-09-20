# TheTurk

A robotics project built step by step while learning ROS 2 from first
principles.

## ROS 2 --- Nodes & Topics

### 1. List Running Nodes

``` bash
ros2 node list
```

Shows all currently discovered ROS 2 nodes.

### 2. Run the Demo Talker

``` bash
ros2 run demo_nodes_cpp talker
```

Runs the `talker` executable from the `demo_nodes_cpp` package.

``` text
ros2 run <package> <executable>
```

The talker creates a ROS 2 node called `/talker`, which publishes
messages to `/chatter`.

### 3. Inspect the Talker Node

``` bash
ros2 node info /talker
```

Shows information about the `/talker` node, including its publishers,
subscribers, services, and actions.

### 4. List Available Topics

``` bash
ros2 topic list
```

Shows the topics currently discovered in the ROS graph. After starting
the talker, `/chatter` should appear.

### 5. List Topics With Message Types

``` bash
ros2 topic list -t
```

Example:

``` text
/chatter [std_msgs/msg/String]
```

### 6. Inspect a Topic

``` bash
ros2 topic info /chatter
```

Example:

``` text
Type: std_msgs/msg/String
Publisher count: 1
Subscription count: 0
```

At this point:

``` text
/talker
   │
   │ Publisher
   ▼
/chatter
```

There is a publisher, but no subscriber yet.

### 7. Listen to a Topic

``` bash
ros2 topic echo /chatter
```

Creates a temporary subscriber from the ROS 2 CLI and prints every
received message.

``` text
/talker
   │
   │ Publisher
   ▼
/chatter
   │
   │ Subscriber
   ▼
ros2 topic echo
```

### 8. Inspect the Message Type

``` bash
ros2 interface show std_msgs/msg/String
```

Shows the structure of the message used by `/chatter`.

``` text
string data
```

A message can therefore look like:

``` text
data: "Hello World: 1"
```

### 9. Run the Demo Listener

``` bash
ros2 run demo_nodes_cpp listener
```

Runs the `listener` executable from the same demo package. The listener
creates a subscriber to `/chatter`.

``` text
/talker [Publisher] ---> /chatter ---> [Subscriber] /listener
```

## Mental Model

``` text
NODE
 │
 └── Publisher
        │
        │ Message
        ▼
      TOPIC
        │
        │ Message
        ▼
     Subscriber
        │
        ▼
       NODE
```

**Node** --- A running ROS 2 software component.

**Publisher** --- Publishes messages.

**Topic** --- A named communication channel.

**Message** --- The structured data being transferred.

**Subscriber** --- Receives messages from a topic.
