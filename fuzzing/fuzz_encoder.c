#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../src/binn.c"

struct buf {
  const unsigned char *ptr;
  unsigned int len;
};

static binn* create_list(struct buf *buf);
static binn* create_map(struct buf *buf);
static binn* create_object(struct buf *buf);

static binn* create_list(struct buf *buf) {
  unsigned short value_size;
  char *value;
  binn *item;

  binn *list = binn_list();

  while(buf->len >= 2) {
    unsigned char value_type = *buf->ptr; buf->ptr++; buf->len--;

    switch(value_type) {
    case 1:
      item = create_list(buf);
      binn_list_add_list(list, item);
      binn_free(item);
      break;
    case 2:
      item = create_map(buf);
      binn_list_add_map(list, item);
      binn_free(item);
      break;
    case 3:
      item = create_object(buf);
      binn_list_add_object(list, item);
      binn_free(item);
      break;
    case 4:
      binn_list_add_null(list);
      break;
    case 5:
      if (buf->len < 1) goto done;
      binn_list_add_bool(list, *buf->ptr);
      buf->ptr++; buf->len--;
      break;
    case 6:
      if (buf->len < 1) goto done;
      binn_list_add_int8(list, *buf->ptr);
      buf->ptr++; buf->len--;
      break;
    case 7:
      if (buf->len < 1) goto done;
      binn_list_add_uint8(list, *buf->ptr);
      buf->ptr++; buf->len--;
      break;
    case 8:
      if (buf->len < 2) goto done;
      binn_list_add_int16(list, *(short*)buf->ptr);
      buf->ptr += 2; buf->len -= 2;
      break;
    case 9:
      if (buf->len < 2) goto done;
      binn_list_add_uint16(list, *(unsigned short*)buf->ptr);
      buf->ptr += 2; buf->len -= 2;
      break;
    case 10:
      if (buf->len < 4) goto done;
      binn_list_add_int32(list, *(int*)buf->ptr);
      buf->ptr += 4; buf->len -= 4;
      break;
    case 11:
      if (buf->len < 4) goto done;
      binn_list_add_uint32(list, *(unsigned int*)buf->ptr);
      buf->ptr += 4; buf->len -= 4;
      break;
    case 12:
      if (buf->len < 8) goto done;
      binn_list_add_int64(list, *(long long*)buf->ptr);
      buf->ptr += 8; buf->len -= 8;
      break;
    case 13:
      if (buf->len < 8) goto done;
      binn_list_add_uint64(list, *(unsigned long long*)buf->ptr);
      buf->ptr += 8; buf->len -= 8;
      break;
    case 14:
      if (buf->len < 4) goto done;
      binn_list_add_float(list, *(float*)buf->ptr);
      buf->ptr += 4; buf->len -= 4;
      break;
    case 15:
      if (buf->len < 8) goto done;
      binn_list_add_double(list, *(double*)buf->ptr);
      buf->ptr += 8; buf->len -= 8;
      break;
    case 16: // string
      // the value can be up to 16-bit size
      if (buf->len < 2) goto done;
      value_size = *(unsigned short*)buf->ptr;
      value = (char*)malloc(value_size+1); // random content
      value[value_size] = 0; // null terminator
      binn_list_add_str(list, value);
      free(value);
      buf->ptr += 2; buf->len -= 2;
      break;
    case 17: // blob
      // the value can be up to 16-bit size
      if (buf->len < 2) goto done;
      value_size = *(unsigned short*)buf->ptr;
      value = (char*)malloc(value_size); // random content
      binn_list_add_blob(list, value, value_size);
      free(value);
      buf->ptr += 2; buf->len -= 2;
      break;
    //default:
    //  ...
    }
  }

done:
  return list;
}

static binn* create_map(struct buf *buf) {
  unsigned short value_size;
  char *value;
  binn *item;

  binn *map = binn_map();

  while(buf->len >= 5) {
    int key = *(int*)buf->ptr; buf->ptr += 4; buf->len -= 4;
    unsigned char value_type = *buf->ptr; buf->ptr++; buf->len--;

    switch(value_type) {
    case 1:
      item = create_list(buf);
      binn_map_set_list(map, key, item);
      binn_free(item);
      break;
    case 2:
      item = create_map(buf);
      binn_map_set_map(map, key, item);
      binn_free(item);
      break;
    case 3:
      item = create_object(buf);
      binn_map_set_object(map, key, item);
      binn_free(item);
      break;
    case 4:
      binn_map_set_null(map, key);
      break;
    case 5:
      if (buf->len < 1) goto done;
      binn_map_set_bool(map, key, *buf->ptr);
      buf->ptr++; buf->len--;
      break;
    case 6:
      if (buf->len < 1) goto done;
      binn_map_set_int8(map, key, *buf->ptr);
      buf->ptr++; buf->len--;
      break;
    case 7:
      if (buf->len < 1) goto done;
      binn_map_set_uint8(map, key, *buf->ptr);
      buf->ptr++; buf->len--;
      break;
    case 8:
      if (buf->len < 2) goto done;
      binn_map_set_int16(map, key, *(short*)buf->ptr);
      buf->ptr += 2; buf->len -= 2;
      break;
    case 9:
      if (buf->len < 2) goto done;
      binn_map_set_uint16(map, key, *(unsigned short*)buf->ptr);
      buf->ptr += 2; buf->len -= 2;
      break;
    case 10:
      if (buf->len < 4) goto done;
      binn_map_set_int32(map, key, *(int*)buf->ptr);
      buf->ptr += 4; buf->len -= 4;
      break;
    case 11:
      if (buf->len < 4) goto done;
      binn_map_set_uint32(map, key, *(unsigned int*)buf->ptr);
      buf->ptr += 4; buf->len -= 4;
      break;
    case 12:
      if (buf->len < 8) goto done;
      binn_map_set_int64(map, key, *(long long*)buf->ptr);
      buf->ptr += 8; buf->len -= 8;
      break;
    case 13:
      if (buf->len < 8) goto done;
      binn_map_set_uint64(map, key, *(unsigned long long*)buf->ptr);
      buf->ptr += 8; buf->len -= 8;
      break;
    case 14:
      if (buf->len < 4) goto done;
      binn_map_set_float(map, key, *(float*)buf->ptr);
      buf->ptr += 4; buf->len -= 4;
      break;
    case 15:
      if (buf->len < 8) goto done;
      binn_map_set_double(map, key, *(double*)buf->ptr);
      buf->ptr += 8; buf->len -= 8;
      break;
    case 16: // string
      // the value can be up to 16-bit size
      if (buf->len < 2) goto done;
      value_size = *(unsigned short*)buf->ptr;
      value = (char*)malloc(value_size+1); // random content
      value[value_size] = 0; // null terminator
      binn_map_set_str(map, key, value);
      free(value);
      buf->ptr += 2; buf->len -= 2;
      break;
    case 17: // blob
      // the value can be up to 16-bit size
      if (buf->len < 2) goto done;
      value_size = *(unsigned short*)buf->ptr;
      value = (char*)malloc(value_size); // random content
      binn_map_set_blob(map, key, value, value_size);
      free(value);
      buf->ptr += 2; buf->len -= 2;
      break;
    //default:
    //  ...
    }
  }

done:
  return map;
}

static binn* create_object(struct buf *buf) {
  char key[256];
  unsigned short value_size;
  char *value;
  binn *item;

  binn *obj = binn_object();

  while(buf->len >= 3) {
    unsigned char key_size = *buf->ptr; buf->ptr++; buf->len--;
    if (buf->len < key_size + 1) goto done;
    memcpy(key, buf->ptr, key_size); buf->ptr += key_size; buf->len -= key_size;
    key[key_size] = 0;

    unsigned char value_type = *buf->ptr; buf->ptr++; buf->len--;

    switch(value_type) {
    case 1:
      item = create_list(buf);
      binn_object_set_list(obj, key, item);
      binn_free(item);
      break;
    case 2:
      item = create_map(buf);
      binn_object_set_map(obj, key, item);
      binn_free(item);
      break;
    case 3:
      item = create_object(buf);
      binn_object_set_object(obj, key, item);
      binn_free(item);
      break;
    case 4:
      binn_object_set_null(obj, key);
      break;
    case 5:
      if (buf->len < 1) goto done;
      binn_object_set_bool(obj, key, *buf->ptr);
      buf->ptr++; buf->len--;
      break;
    case 6:
      if (buf->len < 1) goto done;
      binn_object_set_int8(obj, key, *buf->ptr);
      buf->ptr++; buf->len--;
      break;
    case 7:
      if (buf->len < 1) goto done;
      binn_object_set_uint8(obj, key, *buf->ptr);
      buf->ptr++; buf->len--;
      break;
    case 8:
      if (buf->len < 2) goto done;
      binn_object_set_int16(obj, key, *(short*)buf->ptr);
      buf->ptr += 2; buf->len -= 2;
      break;
    case 9:
      if (buf->len < 2) goto done;
      binn_object_set_uint16(obj, key, *(unsigned short*)buf->ptr);
      buf->ptr += 2; buf->len -= 2;
      break;
    case 10:
      if (buf->len < 4) goto done;
      binn_object_set_int32(obj, key, *(int*)buf->ptr);
      buf->ptr += 4; buf->len -= 4;
      break;
    case 11:
      if (buf->len < 4) goto done;
      binn_object_set_uint32(obj, key, *(unsigned int*)buf->ptr);
      buf->ptr += 4; buf->len -= 4;
      break;
    case 12:
      if (buf->len < 8) goto done;
      binn_object_set_int64(obj, key, *(long long*)buf->ptr);
      buf->ptr += 8; buf->len -= 8;
      break;
    case 13:
      if (buf->len < 8) goto done;
      binn_object_set_uint64(obj, key, *(unsigned long long*)buf->ptr);
      buf->ptr += 8; buf->len -= 8;
      break;
    case 14:
      if (buf->len < 4) goto done;
      binn_object_set_float(obj, key, *(float*)buf->ptr);
      buf->ptr += 4; buf->len -= 4;
      break;
    case 15:
      if (buf->len < 8) goto done;
      binn_object_set_double(obj, key, *(double*)buf->ptr);
      buf->ptr += 8; buf->len -= 8;
      break;
    case 16: // string
      // the value can be up to 16-bit size
      if (buf->len < 2) goto done;
      value_size = *(unsigned short*)buf->ptr;
      value = (char*)malloc(value_size+1); // random content
      value[value_size] = 0; // null terminator
      binn_object_set_str(obj, key, value);
      free(value);
      buf->ptr += 2; buf->len -= 2;
      break;
    case 17: // blob
      // the value can be up to 16-bit size
      if (buf->len < 2) goto done;
      value_size = *(unsigned short*)buf->ptr;
      value = (char*)malloc(value_size); // random content
      binn_object_set_blob(obj, key, value, value_size);
      free(value);
      buf->ptr += 2; buf->len -= 2;
      break;
    //default:
    //  ...
    }
  }

done:
  return obj;
}

static void encode_binn(const unsigned char *data, unsigned int len){
  struct buf buf = {data, len};
  binn *obj = NULL;

  if (len < 1) return;
  unsigned char type = *buf.ptr; buf.ptr++; buf.len--;

  switch(type % 4) {
  case 1:
    obj = create_list(&buf);
    break;
  case 2:
    obj = create_map(&buf);
    break;
  case 3:
    obj = create_object(&buf);
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
