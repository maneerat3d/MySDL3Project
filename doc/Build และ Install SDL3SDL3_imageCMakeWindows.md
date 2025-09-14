# คู่มือการ Build และ Install SDL3 + SDL3_image ด้วย CMake บน Windows

เอกสารนี้สรุปขั้นตอนการคอมไพล์ไลบรารี SDL3 และ SDL3_image จาก Source Code โดยใช้ CMake สำหรับการพัฒนาโปรแกรมบน Windows ด้วย MSVC (Visual Studio) วิธีการนี้จะสร้าง "คลังไลบรารี" ที่สมบูรณ์และพร้อมใช้งานในที่เดียว เพื่อป้องกันปัญหา Compiler Mismatch และความไม่เข้ากันของไลบรารี

---

##  เตรียมความพร้อม (Prerequisites) 🛠️

ก่อนเริ่มต้น ตรวจสอบให้แน่ใจว่าคุณได้ติดตั้งเครื่องมือที่จำเป็นทั้งหมดแล้ว:

1.  **Visual Studio 2022 (หรือใหม่กว่า):**
    * ตรวจสอบว่าได้ติดตั้ง Workload **"Desktop development with C++"** แล้ว
    * สิ่งที่เราต้องการคือ **MSVC Compiler**, **Windows SDK**, และ **CMake** ที่มาพร้อมกับ Visual Studio

2.  **Git:**
    * สำหรับดาวน์โหลด Source Code จาก GitHub [ดาวน์โหลด Git](https://git-scm.com/downloads/)

3.  **Ninja Build System (แนะนำ):**
    * เป็น Build tool ที่ทำงานได้รวดเร็วกว่า MSBuild ที่เป็นค่าเริ่มต้น
    * ติดตั้งผ่าน Command Prompt (cmd) หรือ PowerShell:
      ```bash
      pip install ninja
      ```
    * หากไม่มี Python สามารถดาวน์โหลด `ninja.exe` ได้โดยตรงจาก [GitHub Releases](https://github.com/ninja-build/ninja/releases) แล้วนำไปวางไว้ในโฟลเดอร์ที่อยู่ใน PATH ของระบบ

---

## ขั้นตอนที่ 1: เตรียมพื้นที่และดาวน์โหลด Source Code 📂

เราจะสร้างโฟลเดอร์ใหม่ทั้งหมดเพื่อจัดเก็บไลบรารีให้เป็นระเบียบและป้องกันการปะปนกับไฟล์เก่า

**สำคัญ:** ทุกคำสั่งในคู่มือนี้ควรจะรันบน **"x64 Native Tools Command Prompt for VS 2022"** ซึ่งสามารถค้นหาได้จาก Start Menu เพื่อให้แน่ใจว่าเรากำลังใช้ MSVC Compiler ที่ถูกต้อง



```bash
# 1. สร้างโฟลเดอร์หลักสำหรับเก็บทุกอย่าง
mkdir C:\dev\SDL3-Libraries
cd C:\dev\SDL3-Libraries

# 2. สร้างโฟลเดอร์เดียวที่จะใช้ "ติดตั้ง" ไลบรารีทั้งหมดหลัง Build เสร็จ
mkdir install

# 3. โคลน Source Code ของ SDL3 และ SDL3_image มาใหม่ทั้งหมด
git clone [https://github.com/libsdl-org/SDL.git](https://github.com/libsdl-org/SDL.git)
git clone [https://github.com/libsdl-org/SDL_image.git](https://github.com/libsdl-org/SDL_image.git)
```

ณ จุดนี้ โครงสร้างโฟลเดอร์ของคุณควรจะเป็นแบบนี้:
```
C:\dev\SDL3-Libraries\
├── install\      (ยังว่างอยู่)
├── SDL\          (Source code ของ SDL3)
└── SDL_image\    (Source code ของ SDL_image)
```

---

## ขั้นตอนที่ 2: Build และ Install SDL3 (Core Library) ⚙️

เราจะ Build SDL3 ก่อน เพราะ SDL_image ต้องใช้ SDL3 เป็นพื้นฐาน (Dependency)

```bash
# 1. เข้าไปที่โฟลเดอร์ Source Code ของ SDL3
cd SDL

# 2. รันคำสั่ง Configure ของ CMake
#    -B build: สร้างโฟลเดอร์ build เพื่อไม่ให้ Source Code สกปรก
#    -G "Ninja": บอกให้ใช้ Ninja เป็นตัว Build
#    -D CMAKE_INSTALL_PREFIX=...: กำหนดปลายทางที่จะติดตั้งไลบรารี
cmake -B build -G "Ninja" -D CMAKE_INSTALL_PREFIX=C:/dev/SDL3-Libraries/install

# 3. รันคำสั่ง Build (คอมไพล์โค้ดทั้งหมด)
#    --config Release: สั่งให้ Build ในโหมด Release เพื่อประสิทธิภาพสูงสุด
cmake --build build --config Release

# 4. รันคำสั่ง Install
#    คำสั่งนี้จะคัดลอกไฟล์ที่จำเป็น (.h, .lib, .dll) ไปยังโฟลเดอร์ install ที่เรากำหนดไว้
cmake --install build
```
เมื่อเสร็จสิ้น โฟลเดอร์ `C:\dev\SDL3-Libraries\install` จะมีไฟล์ของ SDL3 (include, lib, bin) เข้าไปอยู่แล้ว

---

## ขั้นตอนที่ 3: Build และ Install SDL3_image 🖼️

ขั้นตอนต่อไปคือการ Build SDL_image โดยเราต้องบอก CMake ให้รู้ว่า SDL3 ที่ Build เสร็จแล้วนั้นอยู่ที่ไหน

```bash
# 1. กลับออกมาแล้วเข้าไปที่โฟลเดอร์ SDL_image
cd ..
cd SDL_image

# 2. รันคำสั่ง Configure ของ CMake
#    สำคัญ: เราต้องระบุทั้ง CMAKE_INSTALL_PREFIX และ CMAKE_PREFIX_PATH
#    - CMAKE_INSTALL_PREFIX: บอกให้ติดตั้ง SDL_image ไปไว้ที่เดียวกับ SDL3
#    - CMAKE_PREFIX_PATH: บอก CMake ให้ไป "ค้นหา" dependency (SDL3) จากที่นี่
cmake -B build -G "Ninja" -D CMAKE_INSTALL_PREFIX=C:/dev/SDL3-Libraries/install -D CMAKE_PREFIX_PATH=C:/dev/SDL3-Libraries/install

# 3. รันคำสั่ง Build
cmake --build build --config Release

# 4. รันคำสั่ง Install
cmake --install build
```

---

## เสร็จสิ้น! ✅

ณ ตอนนี้ โฟลเดอร์ `C:\dev\SDL3-Libraries\install` ของคุณคือ **"คลังไลบรารี SDL3"** ที่สมบูรณ์และพร้อมใช้งาน ประกอบไปด้วย:
```
C:/dev/SDL3-Libraries/install/
├── bin/
│   ├── SDL3.dll
│   └── SDL3_image.dll
│   └── ... (dependency DLLs อื่นๆ เช่น libpng, libjpeg)
├── include/
│   ├── SDL3/
│   │   └── SDL.h, SDL_main.h, ...
│   └── SDL3_image/
│       └── SDL_image.h
└── lib/
    ├── SDL3.lib
    ├── SDL3_image.lib
    └── cmake/
        └── ... (ไฟล์ config สำหรับให้ find_package หาเจอ)
```
คุณสามารถนำโฟลเดอร์ `install` นี้ไปใช้กับโปรเจกต์ใดๆ ของคุณก็ได้ โดยตั้งค่า `CMAKE_PREFIX_PATH` ในโปรเจกต์ของคุณให้ชี้มาที่นี่

### ตัวอย่างการใช้ในโปรเจกต์ของคุณ (`CMakeLists.txt`)

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(MyGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# บอก CMake ให้หาไลบรารีที่เราเพิ่งสร้างไว้
set(CMAKE_PREFIX_PATH "C:/dev/SDL3-Libraries/install")

# ค้นหาแพ็คเกจ
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

# สร้างโปรแกรม
add_executable(MyGame src/main.cpp)

# ลิงก์ไลบรารี
target_link_libraries(MyGame PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
```