/*This source code copyrighted by Lazy Foo' Productions 2004-2024
and may not be redistributed without written permission.*/

//Using SDL
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_events.h>

#include <numbers>

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = nullptr;
	
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

// Texture ที่จะแสดง
SDL_Texture* gImageTexture = nullptr;

// ตัวแปรสำหรับสถานะการหมุน
double gRotationAngle = 0.0;
bool gIsDragging = false;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( !SDL_Init( SDL_INIT_VIDEO ) )
	{
		SDL_Log( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		// สร้าง Window (ถูกต้อง)
		gWindow = SDL_CreateWindow("My Game", SCREEN_WIDTH, SCREEN_WIDTH, SDL_WINDOW_RESIZABLE);

		// ไม่ต้องเรียก GetWindowSurface()
		// ให้ข้ามมาสร้าง Renderer ได้เลย
		gRenderer = SDL_CreateRenderer(gWindow, nullptr);

		// ตรวจสอบว่าสร้าง Renderer สำเร็จหรือไม่
		if (gRenderer == nullptr) {
			SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
			// จัดการ Error ต่อไป...
		}
		
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;
	
	//Load splash image
	gHelloWorld = SDL_LoadBMP( "assets/hello_world.bmp" );
	if( gHelloWorld == NULL )
	{
		SDL_Log( "Unable to load image %s! SDL Error: %s\n", "getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError() );
		success = false;
	}

	return success;
}

bool SetImageTexture(){

    // สร้าง Texture จาก Surface
    gImageTexture = SDL_CreateTextureFromSurface(gRenderer, gHelloWorld);
    if (!gImageTexture) {
        SDL_Log("Unable to create texture from surface! SDL_Error: %s", SDL_GetError());
		return false;
    }

	return true;
}

void close()
{
	//Deallocate surface
	SDL_DestroySurface( gHelloWorld );
	gHelloWorld = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

    SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	
	SDL_DestroySurface(gScreenSurface);
	gScreenSurface = NULL;

	SDL_DestroySurface(gHelloWorld);
	gHelloWorld = NULL;

	SDL_DestroyTexture(gImageTexture);
	gImageTexture = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

/**
 * @brief คำนวณมุมการหมุนของรูปภาพโดยอิงจากตำแหน่งเมาส์
 *
 * ฟังก์ชันจะคำนวณมุมระหว่างจุดศูนย์กลางของรูปภาพกับตำแหน่งปัจจุบันของเมาส์
 *
 * @param mouseX ตำแหน่งแกน X ของเมาส์
 * @param mouseY ตำแหน่งแกน Y ของเมาส์
 * @param imageCenterX ตำแหน่งแกน X ของจุดศูนย์กลางรูปภาพ
 * @param imageCenterY ตำแหน่งแกน Y ของจุดศูนย์กลางรูปภาพ
 * @return double มุมการหมุนในหน่วยองศา (Degrees)
 */
double calculateRotationAngle(int mouseX, int mouseY, int imageCenterX, int imageCenterY)
{
    // คำนวณ delta (ผลต่าง) ระหว่างตำแหน่งเมาส์กับจุดศูนย์กลางของรูปภาพ
    double deltaY = mouseY - imageCenterY;
    double deltaX = mouseX - imageCenterX;

    // ใช้ atan2 เพื่อคำนวณมุมเป็นเรเดียน (Radian)
    // atan2 ให้ผลลัพธ์ที่ถูกต้องในทุก Quadrant
    double angleInRadians = atan2(deltaY, deltaX);

	// Calculate pi at compile time
	constexpr double PI = std::numbers::pi;
    // แปลงจากเรเดียนเป็นองศา (SDL_RenderTextureRotated ใช้หน่วยองศา)
    // และคืนค่ากลับไป
    return angleInRadians * (180.0 / PI);
}
/**
 * @brief ดำเนินการอัปเดตและวาดภาพสำหรับหนึ่งเฟรมของเกม
 * @param deltaTime คือเวลาที่ผ่านไปตั้งแต่เฟรมที่แล้ว (หน่วยเป็นวินาที)
 */
void tick(float deltaTime)
{
	// === ส่วนอัปเดตสถานะ (Update Logic) ===
    if (gIsDragging)
    {
        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // ตำแหน่งศูนย์กลางของจอ (และรูปภาพ)
        int imageCenterX = SCREEN_WIDTH / 2;
        int imageCenterY = SCREEN_HEIGHT / 2;
        
        // เรียกใช้ฟังก์ชันเพื่อคำนวณและอัปเดตมุมการหมุน
        gRotationAngle = calculateRotationAngle(mouseX, mouseY, imageCenterX, imageCenterY);
    }

    // === ส่วนวาดภาพ (Render) ===
    // 1. ล้างหน้าจอให้เป็นสีเทา
    SDL_SetRenderDrawColor(gRenderer, 0xAA, 0xAA, 0xAA, 0xFF);
    SDL_RenderClear(gRenderer);

    // 2. เตรียมวาด Texture
    if (gImageTexture)
    {
        float texW, texH;
        SDL_GetTextureSize(gImageTexture, &texW, &texH);
        
        SDL_FRect dstRect = {
            (SCREEN_WIDTH - texW) / 2.0f,
            (SCREEN_HEIGHT - texH) / 2.0f,
            (float)texW,
            (float)texH
        };

        // 3. วาด Texture พร้อมการหมุน!
        SDL_RenderTextureRotated(gRenderer, gImageTexture, nullptr, &dstRect, gRotationAngle, nullptr, SDL_FLIP_NONE);
    }

    // 4. แสดงผลขึ้นหน้าจอ
    SDL_RenderPresent(gRenderer);
}

int main( int argc, char* args[] )
{
	if (!init() || !loadMedia()) {
        SDL_Log("Failed to setup SDL.");
        close();
        return 1;
    }

	SetImageTexture();

    bool quit = false;
    SDL_Event e;
    Uint64 last_time = SDL_GetPerformanceCounter();
    const double freq = (double)SDL_GetPerformanceFrequency();

    // === GAME LOOP ===
    while (!quit)
    {
        // 1. จัดการ Events
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            } else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    gIsDragging = true;
                }
            } else if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    gIsDragging = false;
                }
            }
        }

        // คำนวณ Delta Time
        const Uint64 current_time = SDL_GetPerformanceCounter();
        const float delta_time = (float)(current_time - last_time) / freq;
        last_time = current_time;

        // 2. เรียกใช้ tick
        tick(delta_time);
    }

    close();
    return 0;
}
