#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char* argv[])
{
    // 1. เริ่มต้นการทำงานของ SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // 2. สร้างหน้าต่าง (Window)
    SDL_Window* window = SDL_CreateWindow(
        "AJA SDL3 Expert Template (Corrected)", // ชื่อหน้าต่าง
        800,                                   // ความกว้าง
        600,                                   // ความสูง
        SDL_WINDOW_RESIZABLE                   // ทำให้ปรับขนาดหน้าต่างได้
    );

    if (!window) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // 3. สร้าง Renderer สำหรับวาดภาพ
    // CHANGED: SDL3's SDL_CreateRenderer no longer takes flags like SDL_RENDERER_ACCELERATED.
    // It will automatically try to use the best available backend.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Could not create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // (Optional) If you want to enable VSync, you can do it like this:
    // SDL_SetRenderVSync(renderer, 1);

    // 4. Main Loop ของโปรแกรม
    // CHANGED: SDL3 uses the standard C++ bool type now.
    bool app_running = true; 
    while (app_running) {
        SDL_Event event;
        // จัดการ Event ทั้งหมดที่เกิดขึ้น
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                // CHANGED: Use the standard C++ false.
                app_running = false; 
            }
        }

        // --- ส่วนของการวาดภาพ ---
        // 5. ล้างหน้าจอด้วยสีที่กำหนด (สีฟ้า)
        SDL_SetRenderDrawColor(renderer, 30, 144, 255, 255);
        SDL_RenderClear(renderer);

        // (สามารถวาด Object อื่นๆ ได้ที่นี่)

        // 6. แสดงผลสิ่งที่วาดทั้งหมดออกทางหน้าจอ
        SDL_RenderPresent(renderer);
    }

    // 7. ทำความสะอาดและคืนหน่วยความจำ
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}