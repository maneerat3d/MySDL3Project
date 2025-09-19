# คู่มือการ Build และ Install BGFX สำหรับ Visual Studio

เอกสารนี้สรุปขั้นตอนการคอมไพล์ไลบรารี **BGFX** จาก Source Code โดยใช้ `bgfx.cmake` และ MSVC (Visual Studio) เพื่อสร้าง "คลังไลบรารี" ที่สมบูรณ์และพร้อมใช้งานสำหรับโปรเจกต์บน Windows

## เตรียมความพร้อม (Prerequisites) 🛠️

ก่อนเริ่มต้น ตรวจสอบให้แน่ใจว่าคุณได้ติดตั้งเครื่องมือที่จำเป็นทั้งหมดแล้ว:

1.  **Visual Studio 2022 (หรือใหม่กว่า):**

      * ตรวจสอบว่าได้ติดตั้ง Workload **"Desktop development with C++"** แล้ว
      * เราต้องการ **MSVC Compiler**, **Windows SDK**, และ **CMake** ที่มาพร้อมกับ Visual Studio

2.  **Git:**

      * สำหรับดาวน์โหลด Source Code จาก GitHub ([ดาวน์โหลด Git](https://git-scm.com/downloads/))

-----

## ขั้นตอนที่ 1: เตรียมพื้นที่และดาวน์โหลด Source Code 📂

เราจะ clone โปรเจกต์ `bgfx.cmake` ซึ่งมี submodules ของ `bgfx`, `bimg`, และ `bx` อยู่ภายในเรียบร้อยแล้ว

**สำคัญ:** ทุกคำสั่งในคู่มือนี้ควรจะรันบน **"x64 Native Tools Command Prompt for VS 2022"** ซึ่งสามารถค้นหาได้จาก Start Menu

```bash
# 1. สร้างโฟลเดอร์หลักสำหรับเก็บไลบรารี
mkdir C:\dev\BGFX-Library
cd C:\dev\BGFX-Library

# 2. สร้างโฟลเดอร์เดียวที่จะใช้ "ติดตั้ง" ไลบรารีทั้งหมดหลัง Build เสร็จ
mkdir install

# 3. โคลน Source Code ของ bgfx.cmake
git clone https://github.com/bkaradzic/bgfx.cmake.git

# 4. เข้าไปในโฟลเดอร์ แล้วสั่งให้ Git ดึง Submodules ทั้งหมด
cd bgfx.cmake
git submodule init
git submodule update
```

ณ จุดนี้ โครงสร้างโฟลเดอร์ของคุณควรจะเป็นแบบนี้:

```
C:\dev\BGFX-Library\
├── install\      (ยังว่างอยู่)
└── bgfx.cmake\
    ├── bgfx\     (submodule)
    ├── bimg\     (submodule)
    └── bx\       (submodule)
```

-----

## ขั้นตอนที่ 2: Build และ Install BGFX ⚙️

ขั้นตอนนี้เราจะใช้ CMake เพื่อสร้างโปรเจกต์ไฟล์ของ Visual Studio (`.sln`) และทำการคอมไพล์

```bash
# 1. อยู่ใน Command Prompt ที่เปิดไว้ และอยู่ในโฟลเดอร์ C:\dev\BGFX-Library\bgfx.cmake

# 2. รันคำสั่ง Configure ของ CMake เพื่อสร้างโปรเจกต์สำหรับ Visual Studio
#    -D CMAKE_INSTALL_PREFIX=...: กำหนดปลายทางที่จะติดตั้งไลบรารี
#    -D BGFX_LIBRARY_TYPE=SHARED: สั่งให้ build bgfx เป็น .dll
#    -D BGFX_BUILD_EXAMPLES=OFF: ปิดการ build ตัวอย่าง
#    -D BGFX_BUILD_TOOLS=OFF: ปิดการ build เครื่องมือ
cmake -B build -G "Visual Studio 17 2022" ^
 -D CMAKE_INSTALL_PREFIX=C:/dev/BGFX-Library/install ^
 -D BGFX_LIBRARY_TYPE=SHARED ^
 -D BGFX_BUILD_EXAMPLES=OFF ^
 -D BGFX_BUILD_TOOLS=OFF

# 3. รันคำสั่ง Build (คอมไพล์โค้ดทั้งหมดในโหมด Release)
cmake --build build --config Release

# 4. รันคำสั่ง Install
#    คำสั่งนี้จะคัดลอกไฟล์ที่จำเป็น (.h, .lib, .dll) ไปยังโฟลเดอร์ install
cmake --install build --config Release
```

-----

## เสร็จสิ้น\! ✅

ณ ตอนนี้ โฟลเดอร์ `C:\dev\BGFX-Library\install` ของคุณคือ **"คลังไลบรารี BGFX"** ที่สมบูรณ์สำหรับ Visual Studio ประกอบไปด้วย:

```
C:/dev/BGFX-Library/install/
├── bin/
│   └── bgfx.dll          <-- ไฟล์ DLL ที่ต้องการ
├── include/
│   ├── bgfx/
│   ├── bimg/
│   └── bx/
└── lib/
    ├── bgfx.lib          <-- ไฟล์ Import Library สำหรับ MSVC
    ├── bimg.lib
    ├── bx.lib
    └── cmake/
        └── ... (ไฟล์ config สำหรับให้ find_package หาเจอ)
```

### ตัวอย่างการใช้ในโปรเจกต์ของคุณ (`CMakeLists.txt`)

```cmake
# CMakeLists.txt สำหรับโปรเจกต์ที่ใช้ Visual Studio
cmake_minimum_required(VERSION 3.16)
project(MySDL3Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ระบุที่อยู่ของคลังไลบรารี
set(CMAKE_PREFIX_PATH 
    "${CMAKE_SOURCE_DIR}/lib/sdl3-install"
    "C:/dev/BGFX-Library/install"
)

# ค้นหาแพ็คเกจ
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(bgfx REQUIRED)

# สร้างโปรแกรม
add_executable(MySDL3Project src/main.cpp)

# Link ไลบรารี
target_link_libraries(MySDL3Project PRIVATE 
    SDL3::SDL3 
    SDL3_image::SDL3_image
    bgfx::bgfx
)

# คัดลอก DLLs ไปยังโฟลเดอร์ Output
if(WIN32)
    add_custom_command(
        TARGET MySDL3Project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/lib/sdl3-install/bin/SDL3.dll"
            $<TARGET_FILE_DIR:MySDL3Project>
        COMMENT "Copying SDL3.dll"
    )

    add_custom_command(
        TARGET MySDL3Project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/lib/sdl3-install/bin/SDL3_image.dll"
            $<TARGET_FILE_DIR:MySDL3Project>
        COMMENT "Copying SDL3_image.dll"
    )

    add_custom_command(
        TARGET MySDL3Project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/dev/BGFX-Library/install/bin/bgfx.dll"
            $<TARGET_FILE_DIR:MySDL3Project>
        COMMENT "Copying bgfx.dll"
    )
endif()

# ตั้งค่าให้ .exe อยู่ในโฟลเดอร์เดียวกับ .dll
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:MySDL3Project>)
```