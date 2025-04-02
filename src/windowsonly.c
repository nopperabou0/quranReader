#include <windows.h>
#include <bcrypt.h>

#include "include/windows/windowsonly.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct {
  BCRYPT_ALG_HANDLE algorithm;
  PUCHAR buffer;
  size_t bufferSize;
  ULONG flags;
} Randomizer;

unsigned int randomizeNumberWindows(void) {

  ULONG randomValue;

  Randomizer r = {
      .algorithm = NULL,                          // Use the default RNG provider.
      .buffer = (PUCHAR)&randomValue,             // Buffer to receive random bytes.
      .bufferSize = sizeof(randomValue),          // Size of the buffer.
      .flags = BCRYPT_USE_SYSTEM_PREFERRED_RNG    // Use the system-preferred
                                                  // random number generator.
  };

  NTSTATUS status =
      BCryptGenRandom(r.algorithm, r.buffer, r.bufferSize, r.flags);

  if (!BCRYPT_SUCCESS(status)) {
    fprintf(stderr, "[ERROR] error generating random number: 0x%x\n",
            (unsigned int)status);
    exit(EXIT_FAILURE);
  }

  unsigned int randomInt = (unsigned int)randomValue;

  int min = 1;
  int max = 6348;

  unsigned int randomInRange = (randomInt % max - min + 1) + min;

  printf("[INFO] Random number between %u and %u: %u\n", min, max,
         randomInRange);

  return randomInRange;
}
