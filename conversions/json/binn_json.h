#include "../../src/binn.h"
#include "../../../jansson-2.3/jansson.h"

char * binn_to_javascript(void *binn);

char * binn_to_json(void *binn);
binn * json_to_binn(char *json);

json_t * binn_to_json_obj(void *binn);
binn   * json_obj_to_binn(json_t *base);
