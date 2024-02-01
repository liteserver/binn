#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../src/binn.c"

static void decode_binn(const unsigned char *buf, unsigned int len){
  binn item;
  binn value;
  binn_iter iter;
  char key[256], *pkey;
  int id, pos;

  // this will check the integrity of the buffer
  if (binn_load_ex(buf, len, &item) == FALSE) return;

  // buffers smaller than 4 bytes cause error
  if (len < 4) return;

  // try to read it as a list
  binn_list_get_value(buf, 1, &value);
  binn_list_get_value(buf, 2, &value);
  binn_list_get_value(buf, 3, &value);

  binn_list_foreach(buf, value) {
    // do nothing
  }

  // try to read it as a map
  binn_map_get_value(buf,    1, &value);
  binn_map_get_value(buf,   10, &value);
  binn_map_get_value(buf, 1024, &value);

  for (pos=1; pos<=binn_count(buf); pos++) {
    binn_map_get_pair(buf, pos, &id, &value);
  }

  binn_map_foreach(buf, id, value) {
    binn_map_get_value(buf, id, &value);
  }

  // try to read it as an object
  binn_object_get_value(buf, "name", &value);
  binn_object_get_value(buf, "age",  &value);
  binn_object_get_value(buf, "city", &value);

  for (pos=1; pos<=binn_count(buf); pos++) {
    binn_object_get_pair(buf, pos, key, &value);
  }

  binn_object_foreach(buf, key, value) {
    binn_object_get_value(buf, key, &value);
  }

}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  decode_binn(data, size);
  return 0;
}
