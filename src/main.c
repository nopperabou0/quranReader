#include <stdio.h>
#ifdef _WIN32
    #include "include/windows/raylib.h"
#else
    #include "include/linux/raylib.h"
#endif

int main(void){
    
    const int WIDTH_SCREEN = 800;
    const int HEIGHT_SCREEN = 600;
    
    InitWindow(WIDTH_SCREEN,HEIGHT_SCREEN,"Quran Reader");
    
    SetTargetFPS(60);
    
    while(!WindowShouldClose()){
        BeginDrawing();
       
         ClearBackground(RAYWHITE);
       
        EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}
