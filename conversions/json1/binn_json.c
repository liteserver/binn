#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#ifdef WIN32
#include <windows.h>
#endif

#include "binn_json.h"

typedef struct {
  char *data;
  size_t len;
  size_t cap;
} strbuf_t;

BINN_PRIVATE void strbuf_init(strbuf_t *sb) {
  sb->len = 0;
  sb->cap = 256;
  sb->data = malloc(sb->cap);
  if (sb->data) {
    sb->data[0] = '\0';
  }
}

BINN_PRIVATE void strbuf_append(strbuf_t *sb, const char *str) {
  size_t str_len = strlen(str);
  while (sb->len + str_len + 1 > sb->cap) {
    sb->cap *= 2;
    sb->data = realloc(sb->data, sb->cap);
  }
  strcpy(sb->data + sb->len, str);
  sb->len += str_len;
}

static const char hexdigits[] = {
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

BINN_PRIVATE void to_hex(char *source, int size, char *dest){
  char *end = source + size;
  for(; source<end; source++){
    unsigned char c = *source;
    *(dest++) = hexdigits[(c>>4)&0xf];
    *(dest++) = hexdigits[c&0xf];
  }
  *dest = 0;
}

BINN_PRIVATE void binn_to_json_obj(binn *base, strbuf_t *sb) {
  long long int intvalue;
  unsigned long long int uintvalue;
  binn_iter  iter;
  binn   binn_value={0};
  int    id, size, count;
  char   key[256], *ptr;
  double floatvalue;

  if (base == NULL) return;

  switch (base->type) {
  case BINN_STRING:
  case BINN_DATE:
  case BINN_TIME:
  case BINN_DATETIME:
  case BINN_DECIMAL:
  case BINN_XML:
  case BINN_HTML:
  case BINN_CSS:
  case BINN_JSON:
  case BINN_JAVASCRIPT:
    strbuf_append(sb, "\"");
    strbuf_append(sb, (char *)base->ptr);
    strbuf_append(sb, "\"");
    break;

  case BINN_BLOB:
    size = (base->size * 2) + 1;
    ptr = malloc(size);
    if (!ptr) return;
    to_hex(base->ptr, base->size, ptr);
    strbuf_append(sb, "\"");
    strbuf_append(sb, ptr);
    strbuf_append(sb, "\"");
    free(ptr);
    break;

  case BINN_INT8:
    intvalue = base->vint8;
    goto loc_signed_integer;
  case BINN_UINT8:
    uintvalue = base->vuint8;
    goto loc_unsigned_integer;
  case BINN_INT16:
    intvalue = base->vint16;
    goto loc_signed_integer;
  case BINN_UINT16:
    uintvalue = base->vuint16;
    goto loc_unsigned_integer;
  case BINN_INT32:
    intvalue = base->vint32;
    goto loc_signed_integer;
  case BINN_UINT32:
    uintvalue = base->vuint32;
    goto loc_unsigned_integer;
  case BINN_INT64:
    intvalue = base->vint64;
loc_signed_integer:
    snprintf(key, sizeof(key), "%lld", intvalue);
    strbuf_append(sb, key);
    break;
  case BINN_UINT64:
    uintvalue = base->vuint64;
loc_unsigned_integer:
    snprintf(key, sizeof(key), "%llu", uintvalue);
    strbuf_append(sb, key);
    break;

  case BINN_BOOL:
    if (base->vbool)
      strbuf_append(sb, "true");
    else
      strbuf_append(sb, "false");
    break;

  case BINN_FLOAT:
    floatvalue = base->vfloat;
    goto loc_double;
  case BINN_DOUBLE:
    floatvalue = base->vdouble;
loc_double:
    snprintf(key, sizeof(key), "%f", floatvalue);
    strbuf_append(sb, key);
    break;

  case BINN_CURRENCYSTR:
    ptr = (char *)base->ptr;
    floatvalue = atof(ptr);
    goto loc_double;

  case BINN_OBJECT:
    strbuf_append(sb, "{");
    count = 0;
    binn_object_foreach(base, key, binn_value) {
      if (count > 0) strbuf_append(sb, ",");
      strbuf_append(sb, "\"");
      strbuf_append(sb, key);
      strbuf_append(sb, "\":");
      binn_to_json_obj(&binn_value, sb);
      count++;
    }
    strbuf_append(sb, "}");
    break;

  case BINN_MAP:
    strbuf_append(sb, "{");
    count = 0;
    binn_map_foreach(base, id, binn_value) {
#ifdef _MSC_VER
      itoa(id, key, 10);
#else
      snprintf(key, sizeof(key), "%d", id);
#endif
      if (count > 0) strbuf_append(sb, ",");
      strbuf_append(sb, "\"");
      strbuf_append(sb, key);
      strbuf_append(sb, "\":");
      binn_to_json_obj(&binn_value, sb);
      count++;
    }
    strbuf_append(sb, "}");
    break;

  case BINN_LIST:
    strbuf_append(sb, "[");
    count = 0;
    binn_list_foreach(base, binn_value) {
      if (count > 0) strbuf_append(sb, ",");
      binn_to_json_obj(&binn_value, sb);
      count++;
    }
    strbuf_append(sb, "]");
    break;

  case BINN_NULL:
    strbuf_append(sb, "null");
    break;

  default:
    strbuf_append(sb, "unknown");
    break;
  }

}

BINN_PRIVATE char* binn_to_json(void *base) {
  strbuf_t sb;
  strbuf_init(&sb);

  if (binn_is_struct(base)) {
    binn_to_json_obj((binn*)base, &sb);
  } else {
    binn item;
    binn_load(base, &item);
    binn_to_json_obj(&item, &sb);
  }

  return sb.data;
}
