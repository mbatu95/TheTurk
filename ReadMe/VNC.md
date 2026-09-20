# ROS2 `rqt_graph` — Docker + Xvfb + VNC Setup

Bu not, Docker container içinde çalışan ROS2 sisteminin `rqt_graph` arayüzünü Mac üzerinde görüntülemek için gereken adımları içerir.

---

## 1. Container'a Gir

Mac terminalinden:

```bash
docker exec -it ros2_ubuntu_vnc bash
```

ROS2'nin aktif olduğunu kontrol et:

```bash
echo $ROS_DISTRO
```

Beklenen:

```text
jazzy
```

---

## 2. Talker Node'unu Başlat

**Terminal 1:**

```bash
ros2 run demo_nodes_cpp talker
```

Şu anda sistem:

```text
/talker
   │
   ▼
/chatter
```

Talker `/chatter` topic'ine mesaj publish ediyor.

---

## 3. Listener Node'unu Başlat

Yeni bir terminal aç ve container'a gir:

```bash
docker exec -it ros2_ubuntu_vnc bash
```

Ardından:

```bash
ros2 run demo_nodes_cpp listener
```

Şimdi sistem:

```text
/talker
   │
   │ /chatter
   ▼
/listener
```

Kontrol:

```bash
ros2 topic info /chatter
```

Beklenen:

```text
Type: std_msgs/msg/String
Publisher count: 1
Subscription count: 1
```

ROS2 tarafı artık hazır.

---

# GUI Setup

Docker container'ın fiziksel bir ekranı olmadığı için `rqt_graph` doğrudan görüntülenemez.

Bunun için:

```text
rqt_graph
    ↓
Xvfb
    ↓
x11vnc
    ↓
Docker Port 5900
    ↓
Mac
```

zincirini kullanıyoruz.

---

## 4. Virtual Display Oluştur

Yeni bir container terminalinde:

```bash
Xvfb :12 -screen 0 1280x720x24 &
```

Bu komut `:12` isimli sanal bir ekran oluşturur.

Ardından:

```bash
export DISPLAY=:12
```

Kontrol:

```bash
echo $DISPLAY
```

Beklenen:

```text
:12
```

---

## 5. Virtual Display'i VNC Üzerinden Paylaş

Aynı container içinde:

```bash
x11vnc -display :12 -rfbport 5900 -forever -shared -nopw
```

Bu terminali açık bırak.

Parametreler:

```text
-display :12
    Xvfb ekranını kullan

-rfbport 5900
    VNC server portu

-forever
    Client bağlantısı kopunca server kapanmasın

-shared
    Birden fazla bağlantıya izin ver

-nopw
    Password kullanma
```

> `-nopw` yalnızca lokal development ortamı için kullanılmalıdır.

---

## 6. Mac'ten VNC'ye Bağlan

Mac üzerinde:

```text
Finder
  ↓
Cmd + K
  ↓
Connect to Server
```

Adres:

```text
vnc://localhost:5900
```

Bağlan.

Artık Docker container içindeki `:12` sanal ekranını Mac üzerinde görebilirsin.

---

## 7. `rqt_graph` Başlat

Yeni bir container terminalinde:

```bash
export DISPLAY=:12
```

Ardından:

```bash
rqt_graph
```

VNC ekranında `rqt_graph` penceresi açılacaktır.

Talker ve listener çalışıyorsa ROS graph yaklaşık olarak:

```text
┌─────────┐       /chatter       ┌───────────┐
│ /talker │ ───────────────────► │ /listener │
└─────────┘                      └───────────┘
```

şeklinde görünür.

---

# Quick Start

Her şey daha önce kurulmuşsa günlük kullanım için:

### Terminal 1 — Talker

```bash
docker exec -it ros2_ubuntu_vnc bash
ros2 run demo_nodes_cpp talker
```

### Terminal 2 — Listener

```bash
docker exec -it ros2_ubuntu_vnc bash
ros2 run demo_nodes_cpp listener
```

### Terminal 3 — GUI

```bash
docker exec -it ros2_ubuntu_vnc bash

Xvfb :12 -screen 0 1280x720x24 &
export DISPLAY=:12

x11vnc -display :12 -rfbport 5900 -forever -shared -nopw
```

Mac:

```text
Cmd + K
→ vnc://localhost:5900
```

### Terminal 4 — rqt_graph

```bash
docker exec -it ros2_ubuntu_vnc bash

export DISPLAY=:12
rqt_graph
```

---

# Important — Xvfb Zaten Çalışıyorsa

Container kapatılmadıysa `Xvfb` hâlâ çalışıyor olabilir.

Kontrol:

```bash
ps aux | grep '[X]vfb'
```

Eğer:

```text
Xvfb :12 -screen 0 1280x720x24
```

görüyorsan tekrar Xvfb başlatma.

Tekrar başlatmaya çalışırsan:

```text
Fatal server error:
Server is already active for display 12
```

hatası alırsın.

Bu durumda doğrudan:

```bash
export DISPLAY=:12
```

ile devam edebilirsin.

---

# Complete Flow

```text
ROS2

/talker
   │
   │ publish
   ▼
/chatter
   │
   │ subscribe
   ▼
/listener


GUI

rqt_graph
    │
    ▼
DISPLAY=:12
    │
    ▼
Xvfb
    │
    ▼
x11vnc
    │
    ▼
Port 5900
    │
    ▼
Docker Port Mapping
    │
    ▼
Mac VNC
```

`rqt_graph`, ROS2 graph içerisindeki node'ları ve aralarındaki communication bağlantılarını görsel olarak incelemek için kullanılır.