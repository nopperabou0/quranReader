#include <stdio.h>
#ifdef _WIN32
    #include "include/windows/raylib.h"
#else
    #include "include/linux/raylib.h"
#endif

#include <errno.h>


FILE *checkJson (char*);

int main(void){
    
    const int WIDTH_SCREEN = 800;
    const int HEIGHT_SCREEN = 600;
    
    InitWindow(WIDTH_SCREEN,HEIGHT_SCREEN,"Quran Reader");
    
    char *fileName  = "quran.json" ;
    FILE *pFile =  checkJson(fileName); 
    if (!pFile)
    {   
        perror("File doesnt exist");
    }

    SetTargetFPS(60);
    
    while(!WindowShouldClose()){
        BeginDrawing();
       
         ClearBackground(RAYWHITE);
       
        EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}

FILE *checkJson (char *fileName){
    FILE *pFile;
    pFile = fopen(fileName,"r");
    return pFile;
}
