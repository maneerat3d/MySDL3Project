# กำหนดเวอร์ชันขั้นต่ำของ CMake ที่ต้องการ
cmake_minimum_required(VERSION 3.16)

# ตั้งชื่อโปรเจกต์, เวอร์ชัน, และภาษาที่ใช้
project(MySDL3Project VERSION 1.0 LANGUAGES CXX)

# กำหนดมาตรฐานของ C++ ที่จะใช้ (แนะนำ 17 หรือสูงกว่าสำหรับ Modern C++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- ส่วนที่สำคัญที่สุด: การค้นหา SDL3 ---
# บอก CMake ให้ไปค้นหาไลบรารีในโฟลเดอร์ที่เราเตรียมไว้
# CMAKE_SOURCE_DIR คือ path ของโปรเจกต์ (ที่มีไฟล์ CMakeLists.txt นี้อยู่)
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/lib/sdl3-install")



# ค้นหาแพ็คเกจ SDL3 และกำหนดให้เป็นสิ่งที่ "จำเป็น" (REQUIRED)
# หากหาไม่เจอ CMake จะหยุดทำงานและแจ้งข้อผิดพลาดทันที
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

# สร้าง Executable ของเราจากไฟล์ source code
add_executable(MySDL3Project src/main.cpp)

# --- การ Link ไลบรารีที่ถูกต้องตามหลัก Modern CMake ---
# ทำการ Link โปรเจกต์ของเรากับ SDL3 โดยใช้ "Imported Target" (SDL3::SDL3)
# วิธีนี้จะจัดการเรื่อง include paths, linker flags และ dependencies อื่นๆ ให้อัตโนมัติ
target_link_libraries(MySDL3Project PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)

# --- จัดการ DLL สำหรับ Windows ---
# เพื่อให้โปรแกรมของเราทำงานได้ เราต้อง copy ไฟล์ SDL3.dll ไปไว้ที่เดียวกับไฟล์ .exe
# คำสั่งนี้จะทำงาน "หลัง" จากที่ build โปรเจกต์เสร็จสิ้น (POST_BUILD)
if(WIN32)
    add_custom_command(
        TARGET MySDL3Project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/lib/sdl3-install/bin/SDL3.dll"
        $<TARGET_FILE_DIR:MySDL3Project>
        COMMENT "Copying SDL3.dll to output directory"
    )

    add_custom_command(
        TARGET MySDL3Project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/lib/sdl3-install/bin/SDL3_image.dll"
        $<TARGET_FILE_DIR:MySDL3Project>
        COMMENT "Copying SDL3_image.dll to output directory"
    )

    add_custom_command(
        TARGET MySDL3Project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/assets"  # โฟลเดอร์ต้นทาง
                "$<TARGET_FILE_DIR:MySDL3Project>/assets"  # โฟลเดอร์ปลายทาง
        COMMENT "Copying assets directory to executable directory"
    )
    
endif()

# (ทางเลือก) ตั้งค่า Output Directory ให้ไฟล์ .exe ไปอยู่ในโฟลเดอร์ bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)