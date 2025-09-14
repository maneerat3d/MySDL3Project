# วิธีการ Build และ Install SDL3 ด้วย CMake และ CMAKE\_INSTALL\_PREFIX

เอกสารนี้จะแนะนำขั้นตอนการ Build ไลบรารี SDL3 จาก Source Code โดยใช้ CMake และทำการติดตั้ง (Install) ไปยัง Directory ที่เรากำหนดเองด้วย `CMAKE_INSTALL_PREFIX` ซึ่งเป็นวิธีที่แนะนำสำหรับการจัดการไลบรารีในแต่ละโปรเจกต์

## 🎯 ทำไมต้องใช้ `CMAKE_INSTALL_PREFIX`

โดยปกติ CMake จะติดตั้งไลบรารีไปยังตำแหน่งมาตรฐานของระบบ (เช่น `C:\Program Files` บน Windows หรือ `/usr/local` บน Linux) การใช้ **`CMAKE_INSTALL_PREFIX`** ทำให้เราสามารถ:

  * **แยกไลบรารีสำหรับแต่ละโปรเจกต์**: ป้องกันปัญหาเวอร์ชันของไลบรารีตีกัน
  * **ไม่ต้องใช้สิทธิ์ Admin**: สามารถติดตั้งลงในโฟลเดอร์ของ User ได้โดยตรง
  * **ง่ายต่อการย้ายโปรเจกต์**: สามารถย้ายโปรเจกต์ไปทำเครื่องอื่นได้ง่ายๆ เพียงแค่ copy โฟลเดอร์ทั้งหมดไป

-----

## 🛠️ สิ่งที่ต้องมี (Prerequisites)

1.  **Git**: สำหรับดาวน์โหลด Source Code
2.  **CMake**: เวอร์ชั่น 3.16 หรือสูงกว่า
3.  **C++ Compiler**:
      * **Windows**: Visual Studio (แนะนำ Community Edition) พร้อม C++ workload
      * **macOS**: Xcode Command Line Tools
      * **Linux**: `build-essential` (มี GCC/G++)

-----

## 📝 ขั้นตอนการ Build และ Install

### ขั้นตอนที่ 1: ดาวน์โหลด SDL3 Source Code

เปิด Terminal หรือ Command Prompt แล้วรันคำสั่ง `git` เพื่อ clone repository ล่าสุดของ SDL3

```bash
# Clone repository ของ SDL3
git clone https://github.com/libsdl-org/SDL.git

# เข้าไปยังโฟลเดอร์ของ SDL3 ที่เพิ่ง clone มา
cd SDL
```

### ขั้นตอนที่ 2: Configure โปรเจกต์ด้วย CMake

นี่คือขั้นตอนที่สำคัญที่สุด เราจะรันคำสั่ง `cmake` เพื่อเตรียมไฟล์สำหรับ Build และกำหนด **`CMAKE_INSTALL_PREFIX`** ให้เป็น Path ที่เราต้องการ

```bash
# รูปแบบคำสั่ง
# cmake -B <build_directory> -DCMAKE_INSTALL_PREFIX="<path_to_install_directory>"

# --- ตัวอย่างบน Windows ---
# ติดตั้งไปยังโฟลเดอร์ C:/dev/sdl3-install
cmake -B build -DCMAKE_INSTALL_PREFIX="C:/dev/sdl3-install"

# --- ตัวอย่างบน macOS หรือ Linux ---
# ติดตั้งไปยังโฟลเดอร์ ~/dev/sdl3-install
cmake -B build -DCMAKE_INSTALL_PREFIX="$HOME/dev/sdl3-install"
```

**คำอธิบาย:**

  * **`-B build`**: บอกให้ CMake สร้างโฟลเดอร์ชื่อ `build` ขึ้นมาเพื่อเก็บไฟล์ที่ใช้ในกระบวนการ Build ทั้งหมด วิธีนี้จะทำให้ Source Code ของเราไม่ปะปนกับไฟล์ที่ถูก generate ขึ้นมา
  * **`-DCMAKE_INSTALL_PREFIX="..."`**: กำหนดค่าตัวแปร `CMAKE_INSTALL_PREFIX` ให้เป็น Path ปลายทางที่เราต้องการติดตั้งไลบรารี

### ขั้นตอนที่ 3: Build ไลบรารี

หลังจาก Configure เสร็จสิ้น ให้รันคำสั่ง Build ผ่าน CMake คำสั่งนี้จะทำการ Compile Source Code ทั้งหมด

```bash
# สั่ง Build โดยกำหนด Configuration เป็น Release เพื่อประสิทธิภาพสูงสุด
cmake --build build --config Release
```

### ขั้นตอนที่ 4: Install ไลบรารี

เมื่อ Build สำเร็จ ให้รันคำสั่ง Install เพื่อให้ CMake คัดลอกไฟล์ที่จำเป็น (เช่น `.dll`, `.lib`, `include`) ไปยังตำแหน่งที่เรากำหนดไว้ใน `CMAKE_INSTALL_PREFIX`

```bash
# สั่ง Install โดยใช้ Configuration เดียวกันกับตอน Build
cmake --install build --config Release
```

-----

## ✅ ตรวจสอบผลลัพธ์

เมื่อทุกขั้นตอนเสร็จสิ้น ให้ลองเข้าไปดูในโฟลเดอร์ที่คุณกำหนดไว้ใน `CMAKE_INSTALL_PREFIX` (เช่น `C:/dev/sdl3-install`) คุณจะพบโครงสร้างไฟล์ดังนี้ ซึ่งพร้อมสำหรับนำไปใช้ในโปรเจกต์ของคุณแล้ว

```
sdl3-install/
├── bin/
│   └── SDL3.dll
├── include/
│   └── SDL3/
│       ├── ... (Header files ทั้งหมด)
├── lib/
│   ├── SDL3.lib
│   └── cmake/
│       └── SDL3/
│           ├── SDL3Config.cmake
│           └── ... (ไฟล์อื่นๆ สำหรับ find_package)
└── ...
```