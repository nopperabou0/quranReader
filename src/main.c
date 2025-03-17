#include <stdio.h>
#ifdef _WIN32
    #include "include/windows/raylib.h"
#else
    #include "include/linux/raylib.h"
#endif

#include <errno.h>

typedef struct {
    char* name;
    FILE* p ;
    char* mode;
    int status; 
}File;

File populateFile();


int main(void){
    const int WIDTH_SCREEN = 800;
    const int HEIGHT_SCREEN = 600;

    (void)InitWindow(WIDTH_SCREEN,HEIGHT_SCREEN,"Quran Reader");

  
    (void)printf("[INFO] Populate File...\n");
    File file = populateFile();

    

    SetTargetFPS(60);
    
    while(!WindowShouldClose()){
        BeginDrawing();
       
         ClearBackground(RAYWHITE);
       
        EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}


File populateFile(){
    
    File file = {
        .name = "quran.json",
        .p  = NULL,
        .mode = "r",
        .status = EOF
    };

    file.p = fopen(file.name,file.mode);

    
    if (!file.p)
    {   
        (void)printf("[INFO] File doesnt exist!\n");  
        (void)printf("[INFO] Creating Empty File!\n");
        file.mode = "w";
        file.p = fopen(file.name,file.mode);
        if (!file.p) {
            (void)perror("[Error] Failed Creating File"); 
        } 
    }

    // file must open in read mode   
    if (file.mode != "r") {
        file.mode = "r";
        file.p = freopen(file.name,file.mode,file.p);         
        if (!file.p) {
            (void)perror("[Error] Failed setting file mode to read"); 
        } 
        
    }

    printf("[INFO] File does exist!\n");
    

    file.status = fgetc(file.p);

    // Assume if this triggered then file empty not error
    if (file.status == EOF) {
        printf("[INFO] File Empty, When it shouldnt \n");
        
        // TO DO : download json content over internet and put it into file,
        // will return error if copy failed
        //(void)perror("[Error] Failed copying file over internet")
    }

    file.status = ungetc (file.status,file.p);
    
    // Assume if this triggered then file empty not error
    if (file.status == EOF) {
        printf("[Error] File still empty somehow \n");
        // Turn this on when download JSON func have been made.
        // return 4; 
    }

    printf("[INFO] File not empty! \n");
    printf ("[INFO] Lets Start Reading JSON now!\n");

    return file;
}