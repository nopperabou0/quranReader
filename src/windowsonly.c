#include <windows.h>
#include <bcrypt.h>
#include <stdio.h>
#include <stdint.h>


#include "include/windows/windowsonly.h"

int randomizeNumber (void) {
  ULONG randomValue;
  NTSTATUS status = BCryptGenRandom(
      NULL,                              // Use the default RNG provider.
      (PUCHAR)&randomValue,              // Buffer to receive random bytes.
      sizeof(randomValue),               // Size of the buffer.
      BCRYPT_USE_SYSTEM_PREFERRED_RNG    // Use the system-preferred random number generator.
  );

  if (!BCRYPT_SUCCESS(status)) {
      fprintf(stderr, "Error generating random number: 0x%x\n",(unsigned int) status);
  }

  // Convert to uint32_t.
  unsigned int randomInt = (unsigned int)randomValue;
  printf ("Random int : %i \n",randomInt);
  // Define desired range [min, max]
  int min = 1;
  int max = 6348;
  int range = max - min + 1;

  // Scale the random value to the desired range using modulo arithmetic.
  // Note: For cryptographic applications, modulo bias may be an issue.
  // For a range as small as this relative to UINT32_MAX, the bias is negligible.
  int randomInRange = (randomInt % range) + min;

  printf("Random number between %u and %u: %u\n", min, max, randomInRange);

  return randomInRange;
} 
