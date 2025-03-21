#include <stdio.h>
#ifdef _WIN32
    #include "include/windows/raylib.h"
    #else
    #include "include/linux/raylib.h"
    
#endif
#include "include/cJSON/cJSON.h"
#include <errno.h> 
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* name;
    FILE* p ;
    char* mode;
    int status; 
}File;

typedef struct {
const cJSON* data;
const cJSON* element;
const cJSON* id ;
const cJSON* chapter;
const cJSON* verse;
const cJSON* english;
const cJSON* arabic;
}DataJson;

File populateFile(File*);


int main(void){
    const int WIDTH_SCREEN = 800;
    const int HEIGHT_SCREEN = 600;

    (void)InitWindow(WIDTH_SCREEN,HEIGHT_SCREEN,"Quran Reader");

    File file = {
        .name = "quran.json",
        .p  = NULL,
        .mode = "r",
        .status = EOF
    };
  
    (void)printf("[INFO] Populate File...\n");


    file = populateFile(&file);
    
    char *buffer = NULL;
    size_t bufferSize = 0;
    

    // https://stackoverflow.com/questions/19111481/reading-and-storing-dynamically-to-char-array
    for (;;)
    {
        const int ch = fgetc(file.p);

        if (ferror(file.p)) {
            (void)perror("[ERROR] Reading failed \n");
            free(buffer);
            exit(EXIT_FAILURE);
        }
        if (feof(file.p)){
            (void)printf("[INFO] Reading finished, reaching end of file \n");
            break;
        }
        char *tmp = realloc(buffer, bufferSize + 1);
        if (tmp == NULL){
            (void)perror("[ERROR] Could not allocate memory \n");
            free(buffer);
            exit(EXIT_FAILURE); 
        }
        buffer = tmp;
        buffer[bufferSize++] = ch;
    }
    fclose(file.p); 

    // https://github.com/DaveGamble/cJSON
    cJSON *fileJson = cJSON_Parse(buffer);

    if (fileJson == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {     
            (void)fprintf(stderr, 
                "[ERROR] Error parsing before: %s.\n Are you sure your JSON file valid? \n",
                 error_ptr);

            cJSON_Delete(fileJson);
            free(buffer);        
            exit(EXIT_FAILURE);
        }
        
    }
    
    DataJson dataJson = {
        .data = NULL,
        .element = NULL,
        .id = NULL,
        .chapter = NULL,
        .verse = NULL,
        .english = NULL,
        .arabic = NULL
    };

    dataJson.data = cJSON_GetObjectItemCaseSensitive(fileJson, "data");
    
    //TO DO : randomizer for id 
    
    cJSON_ArrayForEach(dataJson.element,dataJson.data){
        dataJson.id = cJSON_GetObjectItemCaseSensitive(dataJson.element,"id");
        dataJson.chapter = cJSON_GetObjectItemCaseSensitive(dataJson.element,"chapter");
        dataJson.verse = cJSON_GetObjectItemCaseSensitive(dataJson.element,"verse");
        dataJson.english = cJSON_GetObjectItemCaseSensitive(dataJson.element,"english");
        dataJson.arabic = cJSON_GetObjectItemCaseSensitive(dataJson.element,"arabic");  
        if (!strcmp(dataJson.id->valuestring,"4"))
        {
            printf("id is %s\n",dataJson.id->valuestring);
            printf("chapter is %s\n",dataJson.chapter->valuestring);
        } 
    }
    
    SetTargetFPS(60);
    
    while(!WindowShouldClose()){
        BeginDrawing();
       
         ClearBackground(RAYWHITE);
       
        EndDrawing();
    }
    
    CloseWindow();
    // not sure which one to be freed 

    cJSON_Delete(fileJson);
    free(buffer);        
    return EXIT_SUCCESS;
}


File populateFile(File *pFile){
    File file = *pFile;
    file.p = fopen(file.name,file.mode);

    
    if (!file.p)
    {   
        (void)printf("[INFO] File doesnt exist!\n");  
        (void)printf("[INFO] Creating Empty File!\n");
        file.mode = "w";
        file.p = fopen(file.name,file.mode);
        if (!file.p) {
            (void)perror("[Error] Failed Creating File \n"); 
            exit(EXIT_FAILURE);
        } 
    }

    // file must open in read mode   
    if (strcmp(file.mode, "r")) {
        file.mode = "r";
        file.p = freopen(file.name,file.mode,file.p);         
        if (!file.p) {
            (void)perror("[Error] Failed setting file mode to read \n"); 
            exit(EXIT_FAILURE);
        } 
        
    }

    printf("[INFO] File does exist! \n");
    

    file.status = fgetc(file.p);

    if (ferror(file.p)){
        perror("[ERROR] Checking file for any content failed \n");
        exit(EXIT_FAILURE);
    }


    // Assume if this triggered then file empty not error
    if (feof(file.p)) {
        printf("[INFO] File Empty, When it shouldnt \n");
        
        // TO DO : download json content over internet and put it into file,
        // will return error if copy failed
        //(void)perror("[Error] Failed copying file over internet")
    }

    (void)ungetc (file.status,file.p);

    printf("[INFO] File not empty! \n");
    printf ("[INFO] Lets Start Reading JSON now!\n");

    return file;
}
