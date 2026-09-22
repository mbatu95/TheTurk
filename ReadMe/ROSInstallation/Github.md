# ROS 2 Ortamını GitHub'a Bağlama

Bu noktada ROS 2 Jazzy kurulumumuz tamamlandı.

Şimdi geliştirme ortamımızın Git ve GitHub bağlantısını hazırlayacağız.

---

## 1. Git'i Kontrol Et

```bash
git --version
```

Git kurulu değilse:

### Windows / WSL

```bash
sudo apt update
sudo apt install git -y
```

### macOS / Docker

```bash
apt update
apt install git -y
```

---

## 2. Git Bilgilerini Ayarla

Mevcut ayarları kontrol et:

```bash
git config --global --list
```

Yeni Ubuntu ortamında bilgileriniz tanımlı değilse:

```bash
git config --global user.name "GITHUB_USERNAME"
```

```bash
git config --global user.email "GITHUB_EMAIL"
```

Kontrol:

```bash
git config --global user.name
git config --global user.email
```

---

## 3. SSH Key Kontrolü

```bash
ls -la ~/.ssh
```

Mevcut bir SSH key varsa örneğin şunları görebilirsiniz:

```text
id_ed25519
id_ed25519.pub
```

Yeni Ubuntu ortamında SSH key yoksa:

```bash
ssh-keygen -t ed25519 -C "GITHUB_EMAIL"
```

Dosya konumu sorulduğunda varsayılan konumu kullanmak için:

```text
ENTER
```

Public key'i görüntüle:

```bash
cat ~/.ssh/id_ed25519.pub
```

Çıktının tamamını kopyala.

GitHub:

```text
Settings
→ SSH and GPG keys
→ New SSH key
```

Public key'i buraya ekle.

---

## 4. GitHub Bağlantısını Test Et

```bash
ssh -T git@github.com
```

İlk bağlantıda:

```text
Are you sure you want to continue connecting?
```

sorusu gelirse:

```text
yes
```

Başarılı bağlantıda şuna benzer bir mesaj görmelisiniz:

```text
Hi USERNAME! You've successfully authenticated...
```

---

# Mevcut Bir ROS 2 Repository'sini Kullanmak

Repository'nin SSH adresini GitHub'dan kopyala.

Örneğin:

```text
git@github.com:USERNAME/PROJECT.git
```

Repository'yi clone et:

```bash
git clone git@github.com:USERNAME/PROJECT.git
```

Projeye gir:

```bash
cd PROJECT
```

Kontrol:

```bash
git status
```

Remote'u kontrol et:

```bash
git remote -v
```

---

# ROS 2 Workspace'i Build Et

ROS 2 environment:

```bash
source /opt/ros/jazzy/setup.bash
```

Workspace içerisinde:

```bash
colcon build
```

Build tamamlandıktan sonra:

```bash
source install/setup.bash
```

---

# Git Workflow

Değişiklikleri kontrol et:

```bash
git status
```

Değişiklikleri ekle:

```bash
git add .
```

Commit:

```bash
git commit -m "Update project"
```

Push:

```bash
git push
```

---

# ROS 2 .gitignore

ROS 2 workspace repository'sinde en azından aşağıdaki klasörlerin Git'e gönderilmemesi gerekir:

```gitignore
build/
install/
log/
```

Çünkü bunlar:

```bash
colcon build
```

çalıştırıldığında yeniden oluşturulur.

---

# Son Durum

Windows:

```text
Windows
↓
WSL2
↓
Ubuntu 24.04
↓
ROS 2 Jazzy
↓
Git / GitHub
↓
ROS 2 Project
```

macOS:

```text
macOS
↓
Docker
↓
Ubuntu 24.04
↓
ROS 2 Jazzy
↓
Git / GitHub
↓
ROS 2 Project
```