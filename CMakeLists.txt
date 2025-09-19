# กำหนดเวอร์ชันขั้นต่ำของ CMake ที่ต้องการ
cmake_minimum_required(VERSION 3.16)

# ตั้งชื่อโปรเจกต์, เวอร์ชัน, และภาษาที่ใช้
project(MySDL3Project VERSION 1.0 LANGUAGES CXX)

# กำหนดมาตรฐานของ C++ ที่จะใช้
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- ส่วนที่สำคัญที่สุด: การค้นหาไลบรารี ---
# บอก CMake ให้ไปค้นหาไลบรารีในโฟลเดอร์ที่เราเตรียมไว้
set(CMAKE_PREFIX_PATH 
    "${CMAKE_SOURCE_DIR}/lib/sdl3-install"
    "${CMAKE_SOURCE_DIR}/lib/bgfx-install"
)

# ค้นหาแพ็คเกจที่จำเป็นทั้งหมด
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(bgfx REQUIRED)

# สร้าง Executable ของเราจากไฟล์ source code
add_executable(MySDL3Project src/main.cpp)

# --- การ Link ไลบรารีที่ถูกต้องตามหลัก Modern CMake ---
# ทำการ Link โปรเจกต์ของเรากับไลบรารีทั้งหมด
target_link_libraries(MySDL3Project PRIVATE 
    SDL3::SDL3 
    SDL3_image::SDL3_image
    bgfx::bgfx
)

# --- จัดการ DLL สำหรับ Windows ---
# คำสั่งนี้จะทำงาน "หลัง" จากที่ build โปรเจกต์เสร็จสิ้น (POST_BUILD)
if(WIN32)
    # Copy DLLs ของ SDL
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

    # Copy DLL ของ BGFX (ใช้ชื่อไฟล์ที่ถูกต้องจาก MSVC build)
    add_custom_command(
        TARGET MySDL3Project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/lib/bgfx-install/bin/bgfx.dll"
            $<TARGET_FILE_DIR:MySDL3Project>
        COMMENT "Copying bgfx.dll"
    )
	
    # Copy assets (ถ้ามี)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
        add_custom_command(
            TARGET MySDL3Project POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/assets"
                "$<TARGET_FILE_DIR:MySDL3Project>/assets"
            COMMENT "Copying assets directory"
        )
    endif()
endif()

# (แนะนำ) ตั้งค่าให้ไฟล์ .exe ถูกสร้างในโฟลเดอร์เดียวกับ .dll
# เช่น build/Release หรือ build/Debug เพื่อให้หา DLL เจอตอนรัน
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:MySDL3Project>)