#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../src/binn.c"

static binn* create_list(const unsigned char *buf, unsigned int len);
static binn* create_map(const unsigned char *buf, unsigned int len);
static binn* create_object(const unsigned char *buf, unsigned int len);

static binn* create_list(const unsigned char *buf, unsigned int len) {
  unsigned short value_size;
  char *value;
  binn *item;

  binn *list = binn_list();

  while(len >= 2) {
    unsigned char value_type = *buf; buf++; len--;

    switch(value_type) {
    case 1:
      item = create_list(buf, len);
      binn_list_add_list(list, item);
      binn_free(item);
      break;
    case 2:
      item = create_map(buf, len);
      binn_list_add_map(list, item);
      binn_free(item);
      break;
    case 3:
      item = create_object(buf, len);
      binn_list_add_object(list, item);
      binn_free(item);
      break;
    case 4:
      binn_list_add_null(list);
      break;
    case 5:
      if (len < 1) goto done;
      binn_list_add_bool(list, *buf);
      buf++; len--;
      break;
    case 6:
      if (len < 1) goto done;
      binn_list_add_int8(list, *buf);
      buf++; len--;
      break;
    case 7:
      if (len < 1) goto done;
      binn_list_add_uint8(list, *buf);
      buf++; len--;
      break;
    case 8:
      if (len < 2) goto done;
      binn_list_add_int16(list, *(short*)buf);
      buf += 2; len -= 2;
      break;
    case 9:
      if (len < 2) goto done;
      binn_list_add_uint16(list, *(unsigned short*)buf);
      buf += 2; len -= 2;
      break;
    case 10:
      if (len < 4) goto done;
      binn_list_add_int32(list, *(int*)buf);
      buf += 4; len -= 4;
      break;
    case 11:
      if (len < 4) goto done;
      binn_list_add_uint32(list, *(unsigned int*)buf);
      buf += 4; len -= 4;
      break;
    case 12:
      if (len < 8) goto done;
      binn_list_add_int64(list, *(long long*)buf);
      buf += 8; len -= 8;
      break;
    case 13:
      if (len < 8) goto done;
      binn_list_add_uint64(list, *(unsigned long long*)buf);
      buf += 8; len -= 8;
      break;
    case 14:
      if (len < 4) goto done;
      binn_list_add_float(list, *(float*)buf);
      buf += 4; len -= 4;
      break;
    case 15:
      if (len < 8) goto done;
      binn_list_add_double(list, *(double*)buf);
      buf += 8; len -= 8;
      break;
    case 16: // string
      // the value can be up to 16-bit size
      if (len < 2) goto done;
      value_size = *(unsigned short*)buf;
      value = (char*)malloc(value_size+1); // random content
      value[value_size] = 0; // null terminator
      binn_list_add_str(list, value);
      free(value);
      buf += 2; len -= 2;
      break;
    case 17: // blob
      // the value can be up to 16-bit size
      if (len < 2) goto done;
      value_size = *(unsigned short*)buf;
      value = (char*)malloc(value_size); // random content
      binn_list_add_blob(list, value, value_size);
      free(value);
      buf += 2; len -= 2;
      break;
    //default:
    //  ...
    }
  }

done:
  return list;
}

static binn* create_map(const unsigned char *buf, unsigned int len) {
  unsigned short value_size;
  char *value;
  binn *item;

  binn *map = binn_map();

  while(len >= 5) {
    int key = *(int*)buf; buf += 4; len -= 4;
    unsigned char value_type = *buf; buf++; len--;

    switch(value_type) {
    case 1:
      item = create_list(buf, len);
      binn_map_set_list(map, key, item);
      binn_free(item);
      break;
    case 2:
      item = create_map(buf, len);
      binn_map_set_map(map, key, item);
      binn_free(item);
      break;
    case 3:
      item = create_object(buf, len);
      binn_map_set_object(map, key, item);
      binn_free(item);
      break;
    case 4:
      binn_map_set_null(map, key);
      break;
    case 5:
      if (len < 1) goto done;
      binn_map_set_bool(map, key, *buf);
      buf++; len--;
      break;
    case 6:
      if (len < 1) goto done;
      binn_map_set_int8(map, key, *buf);
      buf++; len--;
      break;
    case 7:
      if (len < 1) goto done;
      binn_map_set_uint8(map, key, *buf);
      buf++; len--;
      break;
    case 8:
      if (len < 2) goto done;
      binn_map_set_int16(map, key, *(short*)buf);
      buf += 2; len -= 2;
      break;
    case 9:
      if (len < 2) goto done;
      binn_map_set_uint16(map, key, *(unsigned short*)buf);
      buf += 2; len -= 2;
      break;
    case 10:
      if (len < 4) goto done;
      binn_map_set_int32(map, key, *(int*)buf);
      buf += 4; len -= 4;
      break;
    case 11:
      if (len < 4) goto done;
      binn_map_set_uint32(map, key, *(unsigned int*)buf);
      buf += 4; len -= 4;
      break;
    case 12:
      if (len < 8) goto done;
      binn_map_set_int64(map, key, *(long long*)buf);
      buf += 8; len -= 8;
      break;
    case 13:
      if (len < 8) goto done;
      binn_map_set_uint64(map, key, *(unsigned long long*)buf);
      buf += 8; len -= 8;
      break;
    case 14:
      if (len < 4) goto done;
      binn_map_set_float(map, key, *(float*)buf);
      buf += 4; len -= 4;
      break;
    case 15:
      if (len < 8) goto done;
      binn_map_set_double(map, key, *(double*)buf);
      buf += 8; len -= 8;
      break;
    case 16: // string
      // the value can be up to 16-bit size
      if (len < 2) goto done;
      value_size = *(unsigned short*)buf;
      value = (char*)malloc(value_size+1); // random content
      value[value_size] = 0; // null terminator
      binn_map_set_str(map, key, value);
      free(value);
      buf += 2; len -= 2;
      break;
    case 17: // blob
      // the value can be up to 16-bit size
      if (len < 2) goto done;
      value_size = *(unsigned short*)buf;
      value = (char*)malloc(value_size); // random content
      binn_map_set_blob(map, key, value, value_size);
      free(value);
      buf += 2; len -= 2;
      break;
    //default:
    //  ...
    }
  }

done:
  return map;
}

static binn* create_object(const unsigned char *buf, unsigned int len) {
  char key[256]; // random content
  unsigned short value_size;
  char *value;
  binn *item;

  binn *obj = binn_object();

  while(len >= 3) {
    unsigned char key_size = *buf; buf++; len--;
    if (len < key_size + 1) goto done;
    memcpy(key, buf, key_size); buf += key_size; len -= key_size;
    key[key_size] = 0;

    unsigned char value_type = *buf; buf++; len--;

    switch(value_type) {
    case 1:
      item = create_list(buf, len);
      binn_object_set_list(obj, key, item);
      binn_free(item);
      break;
    case 2:
      item = create_map(buf, len);
      binn_object_set_map(obj, key, item);
      binn_free(item);
      break;
    case 3:
      item = create_object(buf, len);
      binn_object_set_object(obj, key, item);
      binn_free(item);
      break;
    case 4:
      binn_object_set_null(obj, key);
      break;
    case 5:
      if (len < 1) goto done;
      binn_object_set_bool(obj, key, *buf);
      buf++; len--;
      break;
    case 6:
      if (len < 1) goto done;
      binn_object_set_int8(obj, key, *buf);
      buf++; len--;
      break;
    case 7:
      if (len < 1) goto done;
      binn_object_set_uint8(obj, key, *buf);
      buf++; len--;
      break;
    case 8:
      if (len < 2) goto done;
      binn_object_set_int16(obj, key, *(short*)buf);
      buf += 2; len -= 2;
      break;
    case 9:
      if (len < 2) goto done;
      binn_object_set_uint16(obj, key, *(unsigned short*)buf);
      buf += 2; len -= 2;
      break;
    case 10:
      if (len < 4) goto done;
      binn_object_set_int32(obj, key, *(int*)buf);
      buf += 4; len -= 4;
      break;
    case 11:
      if (len < 4) goto done;
      binn_object_set_uint32(obj, key, *(unsigned int*)buf);
      buf += 4; len -= 4;
      break;
    case 12:
      if (len < 8) goto done;
      binn_object_set_int64(obj, key, *(long long*)buf);
      buf += 8; len -= 8;
      break;
    case 13:
      if (len < 8) goto done;
      binn_object_set_uint64(obj, key, *(unsigned long long*)buf);
      buf += 8; len -= 8;
      break;
    case 14:
      if (len < 4) goto done;
      binn_object_set_float(obj, key, *(float*)buf);
      buf += 4; len -= 4;
      break;
    case 15:
      if (len < 8) goto done;
      binn_object_set_double(obj, key, *(double*)buf);
      buf += 8; len -= 8;
      break;
    case 16: // string
      // the value can be up to 16-bit size
      if (len < 2) goto done;
      value_size = *(unsigned short*)buf;
      value = (char*)malloc(value_size+1); // random content
      value[value_size] = 0; // null terminator
      binn_object_set_str(obj, key, value);
      free(value);
      buf += 2; len -= 2;
      break;
    case 17: // blob
      // the value can be up to 16-bit size
      if (len < 2) goto done;
      value_size = *(unsigned short*)buf;
      value = (char*)malloc(value_size); // random content
      binn_object_set_blob(obj, key, value, value_size);
      free(value);
      buf += 2; len -= 2;
      break;
    //default:
    //  ...
    }
  }

done:
  return obj;
}

static void encode_binn(const unsigned char *buf, unsigned int len){
  binn *obj = NULL;

  if (len < 1) return;
  unsigned char type = *buf; buf++; len--;

  switch(type % 4) {
  case 1:
    obj = create_list(buf, len);
    break;
  case 2:
    obj = create_map(buf, len);
    break;
  case 3:
    obj = create_object(buf, len);
    break;
  }

  if (!obj) return;

  void *ptr = binn_ptr(obj);
  int size = binn_size(obj);

  // now decode it
  binn item;
  binn_load_ex(ptr, size, &item);

  binn_free(obj);
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  encode_binn(data, size);
  return 0;
}
