#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../src/binn.c"

static void decode_binn(const unsigned char *buf, unsigned int len){
  binn item;

  binn_load(buf, &item);
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  decode_binn(data, size);
  return 0;
}
