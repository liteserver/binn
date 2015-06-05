#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "binn_json.h"

/*************************************************************************************/

// this is valid only for latin1 character set, in ansi mode (not UTF-8)

void desacentua(char *str) {
  unsigned char *p,*limit;
  int len, c;

  len = strlen(str);
  limit = str+len;

  for (p=str; p<=limit; p++) {
    c = *p;
    if( c>=0xc0 ){
      if (c >= 0xC0 && c <= 0xC5) {        // ÀÁÂÃÄÅ
        c = 0x41;                          // -> A
      } else if (c >= 0xC8 && c <= 0xCB) { // ÈÉÊË
        c = 0x45;                          // -> E
      } else if (c >= 0xCC && c <= 0xCF) { // ÌÍÎÏ
        c = 0x49;                          // -> I
      } else if (c >= 0xD2 && c <= 0xD6) { // ÒÓÔÕÖ
        c = 0x4F;                          // -> O
      } else if (c >= 0xD9 && c <= 0xDC) { // ÙÚÛÜ
        c = 0x55;                          // -> U
      } else if (c >= 0xE0 && c <= 0xE5) { // àáâãäå
        c = 0x61;                          // -> a
      } else if (c >= 0xE8 && c <= 0xEB) { // èéêë
        c = 0x65;                          // -> e
      } else if (c >= 0xEC && c <= 0xEF) { // ìíîï
        c = 0x69;                          // -> i
      } else if (c >= 0xF2 && c <= 0xF6) { // òóôõö
        c = 0x6f;                          // -> o
      } else if (c >= 0xF9 && c <= 0xFC) { // ùúûü
        c = 0x75;                          // -> u
      } else if (c == 0xFD || c == 0xFF) { // ýÿ
        c = 0x79;                          // -> y
      } else if (c == 0xDD) {              // Ý
        c = 0x59;                          // -> Y
      } else if (c == 0xC7) {              // Ç
        c = 0x43;                          // -> C
      } else if (c == 0xE7) {              // ç
        c = 0x63;                          // -> c
      } else if (c == 0xD1) {              // Ñ
        c = 0x4e;                          // -> N
      } else if (c == 0xF1) {              // ñ
        c = 0x6e;                          // -> n
      }
      *p = c;
    }
  }

}

/*************************************************************************************/

BOOL is_accentuated(char *str) {
  unsigned char *p,*limit;

  limit = str+strlen(str);

  for (p=str; p<=limit; p++) {
    if (*p >= 0xc0) return TRUE;
  }

  return FALSE;
}

/*************************************************************************************/
/*
char * to_utf8(char *str) {
  unsigned char *p, *next, buf[8];
  int len, size;
  int32_t codePoint=0;
  strbuffer sbuf;

  if (is_accentuated(str) == FALSE) return str;

  len = strlen(str);
  strbuffer_init(&sbuf, len+10);

  p = str;
  while ((p != 0) && (*p != 0)) {
    next = utf8_iterate(p, &codePoint);
    //if (next == 0) break;
    if (utf8_encode(codePoint, buf, &size) != 0) break;
    strbuffer_append(&sbuf, buf, size);
    p = next;
  }

  // append null terminator - not needed
  //buf[0] = 0;
  //strbuffer_append(&sbuf, buf, 1);

  return strbuffer_pointer(&sbuf);
}
*/
/*************************************************************************************/

binn * json_obj_to_binn(json_t *base) {
  size_t  i, count;
  json_t  *value;
  const char  *key;
  binn  *obj, *list;

  switch (json_typeof(base)) {
  case JSON_OBJECT:
    obj = binn_object();
    json_object_foreach(base, key, value) {
      if (binn_object_set_new(obj, key, json_obj_to_binn(value)) == FALSE) { binn_free(obj); return NULL; }
    }
    return obj;

  case JSON_ARRAY:
    list = binn_list();
    count = json_array_size(base);
    for (i = 0; i < count; i++) {
      value = json_array_get(base, i);
      if (binn_list_add_new(list, json_obj_to_binn(value)) == FALSE) { binn_free(list); return NULL; }
    }
    return list;

  case JSON_STRING:
    return binn_string(json_string_value(base));

  case JSON_INTEGER:
    return binn_int64(json_integer_value(base));

  case JSON_REAL:
    return binn_double(json_real_value(base));

  case JSON_TRUE:
    return binn_bool(TRUE);

  case JSON_FALSE:
    return binn_bool(FALSE);

  case JSON_NULL:
    return binn_null();

  default:
    return NULL;
  }

}

/*************************************************************************************/

json_t * binn_to_json_obj2(binn *base) {
  json_t    *value;
  json_int_t intvalue;
  binn_iter  iter;
  binn   binn_value={0};
  int    id;
  char   key[256], *ptr;
  double floatvalue;

  if (base == NULL) return NULL;

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
    desacentua((char *)base->ptr);
    value = json_string((char *)base->ptr);
    break;

  //case BINN_BLOB:
  //  value = json_string((char *)base->ptr);
  //  break;

  case BINN_INT8:
    intvalue = base->vint8;
    goto loc_integer;
  case BINN_UINT8:
    intvalue = base->vuint8;
    goto loc_integer;
  case BINN_INT16:
    intvalue = base->vint16;
    goto loc_integer;
  case BINN_UINT16:
    intvalue = base->vuint16;
    goto loc_integer;
  case BINN_INT32:
    intvalue = base->vint32;
    goto loc_integer;
  case BINN_UINT32:
    intvalue = base->vuint32;
    goto loc_integer;
  case BINN_INT64:
    intvalue = base->vint64;
    goto loc_integer;
  case BINN_UINT64:
    intvalue = base->vuint64;
loc_integer:
    value = json_integer(intvalue);
    break;

  case BINN_BOOL:
    if (base->vbool)
      value = json_true();
    else
      value = json_false();
    break;

  case BINN_FLOAT:
    value = json_real(base->vfloat);
    break;
  case BINN_DOUBLE:
    value = json_real(base->vdouble);
    break;

  case BINN_CURRENCYSTR:
    ptr = (char *)base->ptr;
    floatvalue = atof(ptr);
    value = json_real(floatvalue);
    break;

  case BINN_OBJECT:
    value = json_object();
    binn_object_foreach(base, key, binn_value) {
      json_object_set_new(value, key, binn_to_json_obj2(&binn_value));
    }
    break;

  case BINN_MAP:
    value = json_object();
    binn_map_foreach(base, id, binn_value) {
      itoa(id, key, 10);
      json_object_set_new(value, key, binn_to_json_obj2(&binn_value));
    }
    break;

  case BINN_LIST:
    value = json_array();
    binn_list_foreach(base, binn_value) {
      json_array_append_new(value, binn_to_json_obj2(&binn_value));
    }
    break;

  default:
    value = NULL;
    break;
  }

  return value;

}

/*************************************************************************************/

json_t * binn_to_json_obj(void *base) {
  binn item;

  if (binn_is_struct(base))
    return binn_to_json_obj((binn*)base);

  binn_load(base, &item);
  return binn_to_json_obj(&item);

}

/*************************************************************************************/

binn * json_to_binn(char *json_str) {
  json_t *base;
  //json_error_t error;
  binn *item;

  base = json_loads(json_str, 0, NULL);
  if (base == NULL) return FALSE;

  item = json_obj_to_binn(base);

  json_decref(base);
  return item;
}

/*************************************************************************************/

char * binn_to_json(void *base) {
  json_t *json; char *ptr;

  json = binn_to_json_obj(base);
  ptr = json_dumps(json, JSON_PRESERVE_ORDER);
  json_decref(json);

  return ptr;
}

/*************************************************************************************/

char * binn_to_javascript(void *base) {
  json_t *json; char *ptr;

  json = binn_to_json_obj(base);
  ptr = json_dumps(json, JSON_PRESERVE_ORDER | JSON_JAVASCRIPT);
  json_decref(json);

  return ptr;
}

/*************************************************************************************/
