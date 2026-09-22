# Windows'ta ROS 2 Jazzy Kurulumu

Bu kurulumda aşağıdaki yapıyı kullanacağız:

```text
Windows
  ↓
WSL2
  ↓
Ubuntu 24.04
  ↓
ROS 2 Jazzy
```

---

## 1. WSL2 Kurulumu

PowerShell'i **Administrator** olarak aç.

```powershell
wsl --install
```

Kurulum tamamlandıktan sonra bilgisayarı yeniden başlat.

WSL durumunu kontrol etmek için:

```powershell
wsl --status
```

Kurulu Linux dağıtımlarını görmek için:

```powershell
wsl --list --verbose
```

Kurulabilecek Linux dağıtımlarını görmek için:

```powershell
wsl --list --online
```

Ubuntu 24.04 kurulu değilse:

```powershell
wsl --install -d Ubuntu-24.04
```

Ubuntu'yu başlat:

```powershell
wsl -d Ubuntu-24.04
```

İlk açılışta Ubuntu senden bir Linux kullanıcı adı ve parola oluşturmanı isteyebilir.

---

## 2. Ubuntu Sürümünü Kontrol Et

Bu noktadan sonraki komutları **Ubuntu terminalinde** çalıştıracağız.

Ubuntu sürümünü kontrol et:

```bash
cat /etc/os-release
```

Sistemi güncelle:

```bash
sudo apt update
sudo apt upgrade -y
```

---

# ROS 2 Jazzy Kurulumu

## 3. Locale Ayarları

```bash
sudo apt install locales -y
```

```bash
sudo locale-gen en_US en_US.UTF-8
```

```bash
sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
```

```bash
export LANG=en_US.UTF-8
```

Kontrol:

```bash
locale
```

---

## 4. Universe Repository

```bash
sudo apt install software-properties-common -y
```

```bash
sudo add-apt-repository universe
```

---

## 5. ROS 2 Repository

Önce gerekli araçları kur:

```bash
sudo apt update
```

```bash
sudo apt install curl -y
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
sudo dpkg -i /tmp/ros2-apt-source.deb
```

---

## 6. ROS 2 Jazzy Kurulumu

Package listesini güncelle:

```bash
sudo apt update
```

Sistemi güncelle:

```bash
sudo apt upgrade -y
```

ROS 2 Jazzy Desktop sürümünü kur:

```bash
sudo apt install ros-jazzy-desktop -y
```

Development araçlarını kur:

```bash
sudo apt install ros-dev-tools -y
```

---

## 7. ROS 2 Environment

ROS 2 environment'ını aktif et:

```bash
source /opt/ros/jazzy/setup.bash
```

Her yeni terminal açıldığında otomatik olarak aktif olması için:

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

# ROS 2 Test

## Terminal 1

```bash
ros2 run demo_nodes_cpp talker
```

Terminalde mesajların yayınlandığını görmelisin.

---

# Kurulum Tamamlandı

Artık sistemimiz:

```text
Windows
  ↓
WSL2
  ↓
Ubuntu 24.04
  ↓
ROS 2 Jazzy
```