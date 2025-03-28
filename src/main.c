#ifdef _WIN32
    #include "include/windows/windowsonly.h"
    #include "include/windows/raylib.h"
    #else 
    #include "include/linux/raylib.h"
#endif



#include <stdio.h>
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

//  DataJSON type not reusable but it will do for now.
//  Also naming sense is awful
typedef struct {
const cJSON* data;
const cJSON* element;
const cJSON* id ;
const cJSON* chapter;
const cJSON* verse;
const cJSON* english;
const cJSON* arabic;
}DataJSON;


File populateFile(File*);
char *readFile(File*);
cJSON *parseFile(char *buffer);  
unsigned int randomizeNumber (void);


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

    (void)printf("[INFO] Reading File...\n");
    char *buffer = readFile(&file);
    fclose(file.p); // Maybe should check the error

    (void)printf("[INFO] Parsing File...\n");

    cJSON *fileJSON = parseFile(buffer); 
        
    DataJSON dataJSON = {
        .data = NULL,
        .element = NULL,
        .id = NULL,
        .chapter = NULL,
        .verse = NULL,
        .english = NULL,
        .arabic = NULL
    };    
    dataJSON.data = cJSON_GetObjectItemCaseSensitive(fileJSON, "data");

    //TO DO : randomizer for id 
    

    SetTargetFPS(60);
    
    while(!WindowShouldClose()){

        if (dataJSON.element != NULL) {
            cJSON_Delete(dataJSON.element);
        }

        unsigned int randomNumber = randomizeNumber();
        char randomNumberInString[12];
        int writtenCount = snprintf(randomNumberInString,12,"%u",randomNumber);

        if (writtenCount <= 0) {
            fprintf(stderr,"[ERROR] Error when convert random number to string \n");
            exit(EXIT_FAILURE);
        }

        cJSON_ArrayForEach(dataJSON.element,dataJSON.data){
            dataJSON.id = cJSON_GetObjectItemCaseSensitive(dataJSON.element,"id");
            dataJSON.chapter = cJSON_GetObjectItemCaseSensitive(dataJSON.element,"chapter");
            dataJSON.verse = cJSON_GetObjectItemCaseSensitive(dataJSON.element,"verse");
            dataJSON.english = cJSON_GetObjectItemCaseSensitive(dataJSON.element,"english");
            dataJSON.arabic = cJSON_GetObjectItemCaseSensitive(dataJSON.element,"arabic");  
            if (!strcmp(dataJSON.id->valuestring,randomNumberInString))
            {
                printf("id is %s\n",dataJSON.id->valuestring);
                printf("chapter is %s\n",dataJSON.chapter->valuestring);
                printf("arabic is %s\n",dataJSON.arabic->valuestring);
                
            } 
        }

        BeginDrawing();
       
         ClearBackground(RAYWHITE);
       
        EndDrawing();
    }

    cJSON_Delete(fileJSON);
    CloseWindow();
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

    if (feof(file.p)) {
        printf("[INFO] File Empty, When it shouldnt \n");
        
        // TO DO : Download json content over internet and put it into file, 
        // will return error if copy failed
        //  (void)perror("[Error] Failed copying file over internet")
        //  exit(EXIT_FAILURE)
    }

    (void)ungetc (file.status,file.p);

    printf("[INFO] File not empty! \n");
    printf ("[INFO] Lets Start Reading JSON now!\n");

    return file;
}

// https://stackoverflow.com/questions/19111481/reading-and-storing-dynamically-to-char-array
char* readFile(File *pFile){
    File file = *pFile;
    char* buffer = NULL;
    size_t bufferSize = 0;
    
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
    return buffer;
}
    
// https://github.com/DaveGamble/cJSON
cJSON *parseFile (char* buffer){
    cJSON *fileJSON = cJSON_Parse(buffer);

    if (fileJSON == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {     
            (void)fprintf(stderr, 
                "[ERROR] Error parsing before: %s.\n Are you sure your JSON file valid? \n",
                 error_ptr);

            cJSON_Delete(fileJSON);
            free(buffer);        
            exit(EXIT_FAILURE);
        }
    }
    return fileJSON;
}  

unsigned int randomizeNumber (void){
    unsigned int randomNumber;

    #ifdef _WIN32
    randomNumber =  randomizeNumberWindows();
    #else 
    
    unsigned int seed;
    size_t seedCount = 1;
    
    FILE* randomizer = fopen("/dev/urandom","r");
    if (feof(randomizer)){
        perror("[ERROR] How tf urandom even empty to begin with \n");        
        exit(EXIT_FAILURE);
    }
    if (ferror(randomizer)){
        perror("[ERROR] Error when opening urandom \n");
        exit (EXIT_FAILURE);
    }
    
    size_t seedSize = fread(&seed,sizeof(seed),seedCount,randomizer);
    if (seedSize != 1) {
        perror("[ERROR] Read urandom failed \n");
        fclose(randomizer);
        exit(EXIT_FAILURE);
    }

    fclose(randomizer);

    int max = 6348;
    int min = 1;

    randomNumber = seed % (max-min+1)+min;
    printf("[INFO] Random number seed : %u\n", seed);
    printf("[INFO] Random number between %u and %u: %u\n", min, max, randomNumber);


    #endif //_WIN32

    return randomNumber;
}



