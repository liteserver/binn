/*

Basic examples for object, map and list without dynamic allocation (heap).

Note1: a binn object can use 80 bytes from stack so take care when using 
       in devices with small stack size (this size may be smaller in embedded
       devices).

Note2: GetWriteConvertedData() and binn_get_str() may use more than 128 bytes
       of stack (there is a char array of 128 bytes). Again, take this info
       into consideration when using in restriced environemnts.

*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "binn.h"

#define MAX_OBJ_SIZE 256

#define ID_KEY       10
#define NAME_KEY     20
#define STATUS_KEY   30
#define BLOB_KEY     40

void print_size(binn *obj)
{
    void *ptr = binn_ptr(obj);
    int size = binn_size(obj);

    printf("\tPTR=%p, SIZE=%d\r\n",ptr,size);
}

// note: the family of integer encoders may use an smaller integer type or 
// even an unsigned version if your data type fits into it
void dump_value(binn *item)
{
    switch(item->type)
    {
        case BINN_INT8:  printf("%d",item->vint8);break;
        case BINN_INT16: printf("%d",item->vint16);break;
        case BINN_INT32: printf("%d",item->vint32);break;
        case BINN_INT64: printf("%lld",item->vint64);break;
        case BINN_UINT8: printf("%d",item->vuint8);break;
        case BINN_UINT16:printf("%d",item->vuint16);break;
        case BINN_UINT32:printf("%d",item->vuint32);break;
        case BINN_UINT64:printf("%lld",item->vuint64);break;
        case BINN_BOOL:  printf("%s",item->vbool ? "true" : "false");break;
        case BINN_STRING:printf("%s",(char *)item->ptr);break;
        case BINN_BLOB:
            for(uint8_t *p = item->ptr, pos = 0 ; pos < item->size ; pos++) 
                printf("%02X ",p[pos]);
            break;
        default:
            printf("type not supported in this example");
            break;
    }
}

void dump(binn *item)
{
    uint8_t *ptr = (uint8_t *)binn_ptr(item);
    int size = binn_size(item);

    for(int pos = 0 ; pos < size ; pos++)
    {
        if(pos && (pos % 32 == 0))
            printf("\r\n");

        if(pos % 32 == 0)
            printf("\t%04u ",pos);
        
        printf("%02X",*ptr++);
    }
    printf("\r\n");

    binn_iter iter;
    binn value;    // take care, 80 bytes on stack
    char key[256]; // take care, 256 bytes on stack 
    int  id;
    
    if(item->type == BINN_OBJECT)
    {
        binn_object_foreach(item, key, value)
        {
            printf("\t%6s -> ",key);
            dump_value(&value);
            printf("\r\n");
        }
    }
    else if(item->type == BINN_MAP)
    {
        binn_map_foreach(item, id, value)
        {
            printf("\t%d -> ",id);
            dump_value(&value);
            printf("\r\n");
        }
    }
    else if(item->type == BINN_LIST)
    {
        binn_list_foreach(item, value)
        {
            printf("\t -> ");
            dump_value(&value);
            printf("\r\n");
        }
    }
}

void obj_test_encode(binn *obj)
{
    uint8_t blob[] = {'0','1','2','3','4'};

    binn_object_set_int32(obj, "ID", 1234567);
    print_size(obj);
    
    binn_object_set_str(obj, "NAME", "Marcelo");
    print_size(obj);

    binn_object_set_bool(obj,"STATUS",true);
    print_size(obj);

    binn_object_set_blob(obj,"BLOB",blob,5);
    print_size(obj);
}

void obj_test_decode(uint8_t *buf, int size)
{
    int bs;
    static binn obj = { 0 };

    binn_load(buf,&obj);

    int id = binn_object_int32(&obj,"ID");
    char *name = binn_object_str(&obj,"NAME");
    bool status = binn_object_bool(&obj,"STATUS");

    uint8_t *blob = (uint8_t *)binn_object_blob(&obj,"BLOB",&bs);

    printf("\t%-6s=%u\r\n","ID",id);
    printf("\t%-6s=%s\r\n","NAME",name);
    printf("\t%-6s=%s\r\n","STATUS",status?"true":"false");
    printf("\t%-6s=[%u] ","BLOB",bs);

    for(int pos = 0 ; pos < bs ; pos++)
        printf("%02X",blob[pos]);

    printf("\r\n");
}

void obj_test(void)
{
    static binn obj = { 0 };
    static uint8_t data[MAX_OBJ_SIZE];
    
    printf("[1] Creating object ...\r\n");
    binn_create(&obj, BINN_OBJECT, MAX_OBJ_SIZE, data);
    print_size(&obj);

    printf("[2] Encoding object ...\r\n");
    obj_test_encode(&obj);
    printf("[3] Dumping object ...\r\n");
    dump(&obj);
    printf("[4] Decoding object ...\r\n");
    obj_test_decode((uint8_t *)binn_ptr(&obj),binn_size(&obj));
}

void map_test_encode(binn *map)
{
    uint8_t blob[] = {'0','1','2','3','4'};

    binn_map_set_int32(map,ID_KEY,1234567);
    print_size(map);
    
    binn_map_set_str(map,NAME_KEY,"Marcelo");
    print_size(map);

    binn_map_set_bool(map,STATUS_KEY,true);
    print_size(map);

    binn_map_set_blob(map,BLOB_KEY,blob,5);
    print_size(map);
}

void map_test_decode(uint8_t *buf, int size)
{
    int bs;
    static binn map = { 0 };

    binn_load(buf,&map);

    int id = binn_map_int32(&map,ID_KEY);
    char *name = binn_map_str(&map,NAME_KEY);
    bool status = binn_map_bool(&map,STATUS_KEY);

    uint8_t *blob = (uint8_t *)binn_map_blob(&map,BLOB_KEY,&bs);

    printf("\t%-6s=%u\r\n","ID",id);
    printf("\t%-6s=%s\r\n","NAME",name);
    printf("\t%-6s=%s\r\n","STATUS",status?"true":"false");
    printf("\t%-6s=[%u] ","BLOB",bs);

    for(int pos = 0 ; pos < bs ; pos++)
        printf("%02X",blob[pos]);

    printf("\r\n");
}

void map_test(void)
{
    static binn map = { 0 };
    static uint8_t data[MAX_OBJ_SIZE];
    
    printf("[1] Creating map ...\r\n");
    binn_create(&map, BINN_MAP, MAX_OBJ_SIZE, data);
    print_size(&map);

    printf("[2] Encoding map ...\r\n");
    map_test_encode(&map);
    printf("[3] Dumping map ...\r\n");
    dump(&map);
    printf("[4] Decoding map ...\r\n");
    map_test_decode((uint8_t *)binn_ptr(&map),binn_size(&map));
}

void lst_test_encode(binn *lst)
{
    uint8_t blob[] = {'0','1','2','3','4'};

    binn_list_add_int32(lst,1234567);
    print_size(lst);
    
    binn_list_add_str(lst,"Marcelo");
    print_size(lst);

    binn_list_add_bool(lst,true);
    print_size(lst);

    binn_list_add_blob(lst,blob,5);
    print_size(lst);
}

void lst_test_decode(uint8_t *buf, int size)
{
    int bs;
    static binn lst = { 0 };

    binn_load(buf,&lst);

    int id = binn_list_int32(&lst,1);
    char *name = binn_list_str(&lst,2);
    bool status = binn_list_bool(&lst,3);

    uint8_t *blob = (uint8_t *)binn_list_blob(&lst,4,&bs);

    printf("\t%-6s=%u\r\n","ID",id);
    printf("\t%-6s=%s\r\n","NAME",name);
    printf("\t%-6s=%s\r\n","STATUS",status?"true":"false");
    printf("\t%-6s=[%u] ","BLOB",bs);

    for(int pos = 0 ; pos < bs ; pos++)
        printf("%02X",blob[pos]);

    printf("\r\n");
}

void lst_test(void)
{
    static binn lst = { 0 };
    static uint8_t data[MAX_OBJ_SIZE];
    
    printf("[1] Creating list ...\r\n");
    binn_create(&lst, BINN_LIST, MAX_OBJ_SIZE, data);
    print_size(&lst);

    printf("[2] Encoding list ...\r\n");
    lst_test_encode(&lst);
    printf("[3] Dumping list ...\r\n");
    dump(&lst);
    printf("[4] Decoding list ...\r\n");
    lst_test_decode((uint8_t *)binn_ptr(&lst),binn_size(&lst));    
}

int main(void)
{
    printf("--== Object test ==--\r\n");
    obj_test();

    printf("--== Map test ==--\r\n");
    map_test();

    printf("--== List test ==--\r\n");
    lst_test();

    return 0;
}
