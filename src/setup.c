#ifdef _WIN32
#include "include/windows/curl/curl.h"
#include "include/windows/raylib.h"
#include "include/windows/windowsonly.h"
#else
#include "include/linux/curl/curl.h"
#include "include/linux/raylib.h"
#endif

#include "include/JSON.h"
#include "stdlib.h"

File populateFile(File *);
char *readFile(File *pFile);
cJSON *parseFile(char *buffer);
int downloadJSON(File *);

void setupStep(cJSON *fileJSON) {

  File file = {.name = "quran.json", .p = NULL, .mode = "r", .status = EOF};

  TraceLog(LOG_INFO, "Populate File...\n");
  file = populateFile(&file);

  TraceLog(LOG_INFO, "Reading File...\n");
  char *buffer = readFile(&file);
  fclose(file.p); // Maybe should check the error

  TraceLog(LOG_INFO, "Parsing File...\n");
  fileJSON = parseFile(buffer);
  free(buffer);
}


// |||-------------------------------|||
// |||---------Populate File---------|||
// |||-------------------------------|||
File populateFile(File *pFile) {

  File file = *pFile;
  file.p = fopen(file.name, file.mode);

  if (!file.p) {
    TraceLog(LOG_INFO, "File doesnt exist!\n");
    TraceLog(LOG_INFO, "Creating Empty File!\n");
    
    file.mode = "w";
    file.p = fopen(file.name, file.mode);
    if (!file.p) {
      TraceLog(LOG_ERROR, "Failed Creating File \n");
      exit(EXIT_FAILURE);
    }
  }

  TraceLog(LOG_INFO, "File does exist! \n");
  file.status = fgetc(file.p);

  if (ferror(file.p)) {
    TraceLog(LOG_ERROR, "Checking file for any content failed \n");
    exit(EXIT_FAILURE);
  }

  if (feof(file.p)) {
    TraceLog(LOG_INFO, "File Empty, When it shouldnt \n");

    // download JSON over internet
    downloadJSON(&file);
  }

  (void)ungetc(file.status, file.p);


  TraceLog(LOG_INFO, "Setting file to read mode \n");
  // file must open in read mode
  if (strcmp(file.mode, "r")) {
    file.mode = "r";
    file.p = freopen(file.name, file.mode, file.p);
    if (!file.p) {
      TraceLog(LOG_ERROR, "Failed setting file mode to read \n");
      exit(EXIT_FAILURE);
    }
  }

  TraceLog(LOG_INFO, "File not empty! \n");
  TraceLog(LOG_INFO, "Lets Start Reading JSON now!\n");

  return file;
}


// |||-------------------------------|||
// |||---------Read File-------------|||
// |||-------------------------------|||
// https://stackoverflow.com/questions/19111481/reading-and-storing-dynamically-to-char-array
char *readFile(File *pFile) {

  File file = *pFile;
  char *buffer = NULL;
  size_t bufferSize = 0;

  for (;;) {
    const int ch = fgetc(file.p);

    if (ferror(file.p)) {
      TraceLog(LOG_ERROR, "Reading failed \n");
      free(buffer);
      exit(EXIT_FAILURE);
    }

    if (feof(file.p)) {
      TraceLog(LOG_INFO, "Reading finished, reaching end of file \n");      
      break;
    }
    
    char *tmp = realloc(buffer, bufferSize + 1);

    if (tmp == NULL) {
      TraceLog(LOG_ERROR, "Could not allocate memory \n");
      free(buffer);
      exit(EXIT_FAILURE);
    }

    buffer = tmp;
    buffer[bufferSize++] = ch;
  }

  return buffer;
}


// |||-------------------------------|||
// |||---------Parse File------------|||
// |||-------------------------------|||
// https://github.com/DaveGamble/cJSON
cJSON *parseFile(char *buffer) {
  cJSON *fileJSON = cJSON_Parse(buffer);

  if (fileJSON == NULL) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL) {
      TraceLog(LOG_ERROR, "Parsing error \n");
      (void)fprintf(stderr,
                    "[ERROR] Error parsing before: %s.\n Are you sure your "
                    "JSON file valid? \n",
                    error_ptr);

      cJSON_Delete(fileJSON);
      free(buffer);
      exit(EXIT_FAILURE);
    }
  }
  return fileJSON;
}


// |||-------------------------------|||
// |||---------Download JSON---------|||
// |||-------------------------------|||
// https://stackoverflow.com/questions/1636333/download-file-using-libcurl-in-c-c
int downloadJSON(File *file) {
  // file must open in write mode
  TraceLog(LOG_INFO, "Setting file to write mode \n");
  if (strcmp(file->mode, "w")) {
    file->mode = "w";
    file->p = freopen(file->name, file->mode, file->p);
    if (!file->p) {
      TraceLog(LOG_ERROR, "Failed setting file mode to write \n");
      exit(EXIT_FAILURE);
    }
  }

  TraceLog(LOG_INFO, "Begin download JSON over internet \n");
  CURL *curl;
  CURLcode res;
  char *url = "https://raw.githubusercontent.com/nopperabou0/quranReader/refs/"
              "heads/main/quran.json";
  char outfilename[FILENAME_MAX] = file->name;
  
  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file->p);
    res = curl_easy_perform(curl);
    // Check if the download was successful
    if (res != CURLE_OK) {
      TraceLog(LOG_ERROR, "File download failed \n");
      fprintf(stderr, "[Error] curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));   
    } else {
      TraceLog(LOG_INFO, "File downloaded successfully \n");
    }

    curl_easy_cleanup(curl);
  }
  return 0;
}
