# ROS 2 Jazzy Installation

At this point, our Ubuntu 24.04 environment is ready.

Now we can install ROS 2 Jazzy.

---

## 1. Update Ubuntu

```bash
apt update
```

```bash
apt upgrade -y
```

---

## 2. Configure Locale

```bash
apt install locales -y
```

```bash
locale-gen en_US en_US.UTF-8
```

```bash
update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
```

```bash
export LANG=en_US.UTF-8
```

Check the locale:

```bash
locale
```

---

## 3. Enable the Universe Repository

```bash
apt install software-properties-common -y
```

```bash
add-apt-repository universe
```

---

## 4. Add the ROS 2 Repository

Install the required tools:

```bash
apt update
```

```bash
apt install curl -y
```

Get the latest ROS APT source package version:

```bash
export ROS_APT_SOURCE_VERSION=$(curl -s https://api.github.com/repos/ros-infrastructure/ros-apt-source/releases/latest | grep -F "tag_name" | awk -F\" '{print $4}')
```

Download the package:

```bash
curl -L -o /tmp/ros2-apt-source.deb "https://github.com/ros-infrastructure/ros-apt-source/releases/download/${ROS_APT_SOURCE_VERSION}/ros2-apt-source_${ROS_APT_SOURCE_VERSION}.$(. /etc/os-release && echo $VERSION_CODENAME)_all.deb"
```

Install the package:

```bash
dpkg -i /tmp/ros2-apt-source.deb
```

---

## 5. Install ROS 2 Jazzy

Update the package list:

```bash
apt update
```

Upgrade the system:

```bash
apt upgrade -y
```

Install ROS 2 Jazzy Desktop:

```bash
apt install ros-jazzy-desktop -y
```

Install the ROS development tools:

```bash
apt install ros-dev-tools -y
```

---

## 6. Set Up the ROS 2 Environment

Source the ROS 2 environment:

```bash
source /opt/ros/jazzy/setup.bash
```

To automatically source ROS 2 whenever a new terminal is opened:

```bash
echo "source /opt/ros/jazzy/setup.bash" >> ~/.bashrc
```

Apply it to the current terminal:

```bash
source ~/.bashrc
```

Check that ROS 2 is available:

```bash
ros2 --help
```

---

## 7. Test ROS 2

### Terminal 1

Run the talker node:

```bash
ros2 run demo_nodes_cpp talker
```

### Terminal 2

Run the listener node:

```bash
ros2 run demo_nodes_cpp listener
```

You should see messages similar to:

```text
I heard: [Hello World: 1]
I heard: [Hello World: 2]
I heard: [Hello World: 3]
```

If the talker is publishing messages and the listener is receiving them, ROS 2 Jazzy is working correctly.