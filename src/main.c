#include <stdio.h>
#ifdef _WIN32
    #include "include/windows/raylib.h"
#else
    #include "include/linux/raylib.h"
#endif

#include <errno.h>


FILE *openFile (char*,char*);
int checkContent(char*,FILE*); 

int main(void){
    
    const int WIDTH_SCREEN = 800;
    const int HEIGHT_SCREEN = 600;
    
    InitWindow(WIDTH_SCREEN,HEIGHT_SCREEN,"Quran Reader");
    
    char *fileName  = "quran.json" ;
    FILE *pFile =  openFile(fileName,"r"); 
    if (!pFile)
    {   
        printf("[INFO] File doesnt exist!\n");  
        printf("[INFO] Creating Empty File!\n");
        pFile = fopen(fileName,"w"); 
        if (!pFile)
        {   
            perror("[ERROR] Creating File Failed");
            return 1;    
        }
        pFile = freopen(fileName,"r",pFile);
          
    }
    else {
        printf("[INFO] File does exist!\n"); 
    }

    if (!pFile)
    {   
        perror("[ERROR] Read Failed");
        return 2;    
    }

    int fileStatus = checkContent(fileName,pFile);
    
    // is this how to handle the error?
    // i dont think so, if its EOF, its either file empty or unget failed
    if (fileStatus == EOF) {
        printf("[INFO] File Empty, When it shouldnt \n");
        
        // TO DO : download json content over internet and put it into file,
        // will return error for now.
        // copyJson();

        // return 3;

    }
    fileStatus = ungetc (fileStatus,pFile);
    if (fileStatus == EOF) {
        printf("[INFO] Unget Failed \n");
        // Turn this on when copyJson() func have been made.
        // return 4; 
    }
    printf("[INFO] Unget Success! \n");

    
    
    
    printf ("[INFO] Lets Start Reading JSON now!\n");


    SetTargetFPS(60);
    
    while(!WindowShouldClose()){
        BeginDrawing();
       
         ClearBackground(RAYWHITE);
       
        EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}

FILE *openFile (char *fileName,char* mode){
    FILE *pFile;
    pFile = fopen(fileName,mode);
    return pFile;
}

int checkContent (char *fileName,FILE *pFile){

    int status;

    status = fgetc(pFile);

    if (status != EOF) {
        return status;
    }
    return EOF;   
}

