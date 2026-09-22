# macOS'ta ROS 2 Jazzy Kurulumu

macOS üzerinde ROS 2 Jazzy çalıştırmak için Docker kullanacağız.

Kuracağımız yapı:

```text
macOS
  ↓
Docker Desktop
  ↓
Ubuntu 24.04 Container
  ↓
ROS 2 Jazzy
```

---

## 1. Docker Desktop Kurulumu

Docker Desktop'ı macOS'a kur ve çalıştır.

Kurulumdan sonra Terminal'i aç.

Docker'ın kurulu olduğunu kontrol et:

```bash
docker --version
```

Docker'ın çalıştığını kontrol et:

```bash
docker info
```

---

## 2. Ubuntu 24.04 Image'ını İndir

```bash
docker pull ubuntu:24.04
```

İndirilen Docker image'larını görmek için:

```bash
docker images
```

Listede `ubuntu` ve `24.04` görünmeli.

---

## 3. Ubuntu Container Oluştur

Yeni bir Ubuntu container oluştur:

```bash
docker run -it \
  --name ros2_jazzy \
  ubuntu:24.04
```

Bu komuttan sonra terminal artık Ubuntu container'ın içerisinde olacaktır.

Ubuntu sürümünü kontrol et:

```bash
cat /etc/os-release
```

Ubuntu 24.04 kullandığımızı doğrula.

---

# ROS 2 Jazzy Kurulumu

## 4. Ubuntu'yu Güncelle

Container içerisinde varsayılan olarak `root` kullanıcısında olduğumuz için `sudo` kullanmamıza gerek yok.

```bash
apt update
```

```bash
apt upgrade -y
```

---

## 5. Locale Ayarları

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

Kontrol:

```bash
locale
```

---

## 6. Universe Repository

```bash
apt install software-properties-common -y
```

```bash
add-apt-repository universe
```

---

## 7. ROS 2 Repository

Gerekli araçları kur:

```bash
apt update
```

```bash
apt install curl -y
```

ROS APT source paketinin güncel sürümünü al:

```bash
export ROS_APT_SOURCE_VERSION=$(curl -s https://api.github.com/repos/ros-infrastructure/ros-apt-source/releases/latest | grep -F "tag_name" | awk -F\" '{print $4}')
```

Paketi indir:

```bash
curl -L -o /tmp/ros2-apt-source.deb "https://github.com/ros-infrastructure/ros-apt-source/releases/download/${ROS_APT_SOURCE_VERSION}/ros2-apt-source_${ROS_APT_SOURCE_VERSION}.$(. /etc/os-release && echo $VERSION_CODENAME)_all.deb"
```

Paketi kur:

```bash
dpkg -i /tmp/ros2-apt-source.deb
```

---

## 8. ROS 2 Jazzy Kurulumu

Package listesini güncelle:

```bash
apt update
```

Sistemi güncelle:

```bash
apt upgrade -y
```

ROS 2 Jazzy Desktop sürümünü kur:

```bash
apt install ros-jazzy-desktop -y
```

Development araçlarını kur:

```bash
apt install ros-dev-tools -y
```

---

## 9. ROS 2 Environment

ROS 2 environment'ını aktif et:

```bash
source /opt/ros/jazzy/setup.bash
```

Her yeni terminalde otomatik olarak aktif olması için:

```bash
echo "source /opt/ros/jazzy/setup.bash" >> ~/.bashrc
```

Değişikliği mevcut terminale uygula:

```bash
source ~/.bashrc
```

ROS 2'nin çalıştığını kontrol et:

```bash
ros2 --help
```

---

# Docker Container Kullanımı

## 10. Container'dan Çıkmak

```bash
exit
```

Container'dan çıktığında container durabilir.

Mevcut container'ları görmek için macOS terminalinde:

```bash
docker ps -a
```

---

## 11. Container'ı Tekrar Başlatmak

```bash
docker start ros2_jazzy
```

Container'ın içerisine tekrar girmek için:

```bash
docker exec -it ros2_jazzy bash
```

ROS 2 environment `.bashrc` içerisinde olduğu için otomatik olarak yüklenmelidir.

Kontrol:

```bash
ros2 --help
```

---
