import shutil
import sys
from pathlib import Path

def create_project():
    """
    Main function to create a new project from the SDL3 template.
    This script is designed to be run from within the template directory itself.
    """
    print("🚀 SDL3 Project Creator by AJA (v2.0) 🚀")

    ignore_list = [
        '*.pyc',
        'doc',
        '*.py',
        '__pycache__',
        'build',
        'dist',
        '.vscode',
        '.git',
        '*.egg-info'
    ]
    
    # --- 1. กำหนด Path ที่จำเป็นโดยอัตโนมัติ ---
    # โฟลเดอร์ Template คือโฟลเดอร์ที่สคริปต์นี้อยู่
    template_path = Path(__file__).parent.resolve()
    # โฟลเดอร์ที่จะเก็บโปรเจกต์ใหม่ คือโฟลเดอร์แม่ของ Template
    projects_parent_dir = template_path.parent
    
    template_name = template_path.name


    print(f"Template directory identified as: '{template_path}'")
    print(f"New projects will be created in: '{projects_parent_dir}'")
    
    

    # --- 2. รับชื่อโปรเจกต์ใหม่จากผู้ใช้ ---
    new_project_name = input("\nEnter the name for your new project: ").strip()

    if not new_project_name:
        print("❌ Error: Project name cannot be empty.")
        sys.exit(1)

    new_project_path = projects_parent_dir / new_project_name

    # --- 3. ตรวจสอบว่าโปรเจกต์ชื่อนี้ยังไม่มีอยู่ ---
    if new_project_path.exists():
        print(f"❌ Error: A project named '{new_project_name}' already exists at '{new_project_path}'")
        sys.exit(1)

    try:
        # --- 4. คัดลอกไฟล์ทั้งหมดจาก Template (ยกเว้นไฟล์สคริปต์นี้) ---
        print(f"\n📁 Copying template '{template_name}' to '{new_project_path}'...")
        ignore_patterns = shutil.ignore_patterns(*ignore_list)
        shutil.copytree(template_path, new_project_path, ignore=ignore_patterns)

        # --- 5. แก้ไขไฟล์ CMakeLists.txt ---
        print(f"✏️  Updating project name to '{new_project_name}' in CMakeLists.txt...")
        cmake_file_path = new_project_path / "CMakeLists.txt"
        
        if cmake_file_path.is_file():
            # อ่านเนื้อหาเดิม
            content = cmake_file_path.read_text(encoding="utf-8")

            # แทนที่ชื่อโปรเจกต์, ชื่อ executable, และส่วนที่เกี่ยวข้อง
            content = content.replace(f"project({template_name}", f"project({new_project_name}")
            content = content.replace(f"add_executable({template_name}", f"add_executable({new_project_name}")
            content = content.replace(f"target_link_libraries({template_name}", f"target_link_libraries({new_project_name}")
            content = content.replace(f"TARGET {template_name} POST_BUILD", f"TARGET {new_project_name} POST_BUILD")
            content = content.replace(f"$<TARGET_FILE_DIR:{template_name}>", f"$<TARGET_FILE_DIR:{new_project_name}>")
            
            
            # เขียนเนื้อหาใหม่ทับลงไป
            cmake_file_path.write_text(content, encoding="utf-8")
        else:
            print(f"⚠️ Warning: CMakeLists.txt not found in new project. Skipping rename.")

    except Exception as e:
        print(f"\n❌ An unexpected error occurred: {e}")
        # หากเกิดข้อผิดพลาด ให้ลบโฟลเดอร์ที่สร้างไม่เสร็จทิ้งไป
        if new_project_path.exists():
            shutil.rmtree(new_project_path)
            print(f"🗑️  Cleaned up incomplete project directory.")
        sys.exit(1)

    # --- 6. แจ้งผลลัพธ์ ---
    print("\n✅ Success! Your new SDL3 project has been created.")
    print(f"   Location: {new_project_path}")
    print("\nNext steps:")
    print(f"   1. Go to the new directory: cd {new_project_path}")
    print(f"   2. Configure CMake: cmake -B build")
    print(f"   3. Build the project: cmake --build build\n")

if __name__ == "__main__":
    create_project()