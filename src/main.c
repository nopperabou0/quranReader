#include "setup.c"

int main(void) {
  const int WIDTH_SCREEN = 800;
  const int HEIGHT_SCREEN = 600;  
  (void)InitWindow(WIDTH_SCREEN, HEIGHT_SCREEN, "Quran Reader");
  
  DataJSON dataJSON = {
    .data = NULL,
    .element = NULL,
    .id = NULL,
    .chapter = NULL,
    .verse = NULL,
    .english = NULL,
    .arabic = NULL
  };

  setupStep(&dataJSON);
  SetTargetFPS(60);

  while (!WindowShouldClose()) {

    if (dataJSON.element != NULL) {
      cJSON_Delete(dataJSON.element);
    }

    unsigned int randomNumber = randomizeNumber();
    char randomNumberInString[12];
    int writtenCount = snprintf(randomNumberInString, 12, "%u", randomNumber);

    if (writtenCount <= 0) {
      fprintf(stderr, "[ERROR] Error when convert random number to string \n");
      exit(EXIT_FAILURE);
    }

    cJSON_ArrayForEach(dataJSON.element, dataJSON.data) {
      dataJSON.id = cJSON_GetObjectItemCaseSensitive(dataJSON.element, "id");
      dataJSON.chapter =
          cJSON_GetObjectItemCaseSensitive(dataJSON.element, "chapter");
      dataJSON.verse =
          cJSON_GetObjectItemCaseSensitive(dataJSON.element, "verse");
      dataJSON.english =
          cJSON_GetObjectItemCaseSensitive(dataJSON.element, "english");
      dataJSON.arabic =
          cJSON_GetObjectItemCaseSensitive(dataJSON.element, "arabic");
      if (!strcmp(dataJSON.id->valuestring, randomNumberInString)) {
        printf("id is %s\n", dataJSON.id->valuestring);
        printf("chapter is %s\n", dataJSON.chapter->valuestring);
        printf("arabic is %s\n", dataJSON.arabic->valuestring);
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

unsigned int randomizeNumber(void) {
  unsigned int randomNumber;

#ifdef _WIN32
  randomNumber = randomizeNumberWindows();
#else

  // https://www.geeksforgeeks.org/generating-random-number-range-c/

  unsigned int seed;
  size_t seedCount = sizeof(seed) / sizeof(unsigned int);

  FILE *randomizer = fopen("/dev/urandom", "r");
  if (feof(randomizer)) {
    perror("[ERROR] How tf urandom even empty to begin with \n");
    exit(EXIT_FAILURE);
  }
  if (ferror(randomizer)) {
    perror("[ERROR] Error when opening urandom \n");
    exit(EXIT_FAILURE);
  }

  size_t seedSize = fread(&seed, sizeof(seed), seedCount, randomizer);
  if (seedSize != seedCount) {
    perror("[ERROR] Read urandom failed \n");
    fclose(randomizer);
    exit(EXIT_FAILURE);
  }

  fclose(randomizer);

  int max = 6348;
  int min = 1;

  randomNumber = seed % (max - min + 1) + min;
  printf("[INFO] Random number between %u and %u: %u\n", min, max,
         randomNumber);

#endif //_WIN32

  return randomNumber;
}

#ifdef _WIN32
#else
// https://stackoverflow.com/questions/1636333/download-file-using-libcurl-in-c-c
int downloadJSON(File *file) {

}
#endif
