Binn Интерфейс и использование
======================

### Контейнеры

Существует 3 типа контейнеров:

* Списки
* Карты
* Объекты

Списки
-----

Значения хранятся в последовательном порядке:

> [2, 5, 9.1, "value", true]

#### Запись

```c
binn *list;

// создать новый список
list = binn_list();

// добавить значения
binn_list_add_int32(list, 123);
binn_list_add_double(list, 2.55);
binn_list_add_str(list, "testing");

// отправить по сети или сохранить в файл...
send(sock, binn_ptr(list), binn_size(list));

// освободить буфер
binn_free(list);
```

#### Чтение по позиции

```c
int id;
double rate;
char *name;

id = binn_list_int32(list, 1);
rate = binn_list_double(list, 2);
name = binn_list_str(list, 3);
```

#### Чтение значений одного типа

```c
int i, count;
double note;

count = binn_count(list);
for(i=1; i<=count; i++) {
  note = binn_list_double(list, i);
}
```

#### Чтение с использованием for each

```c
binn_iter iter;
binn value;

binn_list_foreach(list, value) {
  do_something(&value);
}
```

Карты
----

Значения хранятся с целочисленными ключами:

> {2: "test", 5: 2.5, 10: true}

Вы можете определить целочисленные ключи в заголовочном файле, общем для приложений, которые будут использовать карту:

```c
#define USER_ID    11
#define USER_NAME  12
#define USER_VALUE 13
```

#### Запись

```c
binn *map;

// создать новую карту
map = binn_map();

// добавить значения
binn_map_set_int32(map, USER_ID, 123);
binn_map_set_str(map, USER_NAME, "John");
binn_map_set_double(map, USER_VALUE, 2.55);

// отправить по сети или сохранить в файл...
send(sock, binn_ptr(map), binn_size(map));

// освободить буфер
binn_free(map);
```

#### Чтение по ключу

```c
int id;
char *name;
double note;

id = binn_map_int32(map, USER_ID);
name = binn_map_str(map, USER_NAME);
note = binn_map_double(map, USER_VALUE);
```

#### Последовательное чтение

```c
binn_iter iter;
binn value;
int id;

binn_map_foreach(map, id, value) {
  do_something(id, &value);
}
```

Объекты
-------

Значения хранятся с ключами-строками:

> {"name": "John", "grade": 8.5, "active": true}

#### Запись

```c
binn *obj;

// создать новый объект
obj = binn_object();

// добавить значения
binn_object_set_int32(obj, "id", 123);
binn_object_set_str(obj, "name", "John");
binn_object_set_double(obj, "total", 2.55);

// отправить по сети или сохранить в файл...
send(sock, binn_ptr(obj), binn_size(obj));

// освободить буфер
binn_free(obj);
```

#### Чтение по ключу

```c
int id;
char *name;
double total;

id = binn_object_int32(obj, "id");
name = binn_object_str(obj, "name");
total = binn_object_double(obj, "total");
```

#### Последовательное чтение

```c
binn_iter iter;
binn value;
char key[256];

binn_object_foreach(obj, key, value) {
  do_something(key, &value);
}
```

---

## Вложенные структуры

Мы можем помещать контейнеры внутрь других (список внутри объекта, список объектов, список карт...)

### Пример 1: Список внутри объекта

> {id: 123, name: "John", values: [2.5, 7.35, 9.15]}

#### Запись

```c
binn *obj, *list;

// создать новый объект
obj = binn_object();

// добавить значения
binn_object_set_int32(obj, "id", 123);
binn_object_set_str(obj, "name", "John");

// создать новый список
list = binn_list();
binn_list_add_double(list, 2.50);
binn_list_add_double(list, 7.35);
binn_list_add_double(list, 9.15);
binn_object_set_list(obj, "values", list);
binn_free(list);

// отправить по сети или сохранить в файл...
send(sock, binn_ptr(obj), binn_size(obj));

// освободить память
binn_free(obj);
```

#### Чтение

```c
int id, i, count;
char *name;
void *list;
double grade;

id = binn_object_int32(obj, "id");
name = binn_object_str(obj, "name");
list = binn_object_list(obj, "values");

count = binn_count(list);
for(i=1; i<=count; i++) {
  grade = binn_list_double(list, i);
}
```

### Пример 2: Список объектов

> [ {name: "John", email: "john@gmail.com"} , {name: "Eric", email: "eric@gmail.com"} ]

#### Запись

```c
binn *list, *obj;

// создать новый список
list = binn_list();

// создать первый объект
obj = binn_object();
binn_object_set_str(obj, "name", "John");
binn_object_set_str(obj, "email", "john@gmail.com");
// добавить в список и удалить
binn_list_add_object(list, obj);
binn_free(obj);

// создать второй объект
obj = binn_object();
binn_object_set_str(obj, "name", "Eric");
binn_object_set_str(obj, "email", "eric@gmail.com");
// добавить в список и удалить
binn_list_add_object(list, obj);
binn_free(obj);

// отправить по сети или сохранить в файл...
send(sock, binn_ptr(list), binn_size(list));

// освободить память
binn_free(list);
```

#### Чтение

```c
int i, count;
void *obj;
char *name, *email;

count = binn_count(list);
for(i=1; i<=count; i++) {
  obj = binn_list_object(list, i);
  name  = binn_object_str(obj, "name");
  email = binn_object_str(obj, "email");
}
```

#### Примечание

При чтении внутренние контейнеры возвращаются в виде статических указателей, которые не следует "освобождать".

---

### Значения Binn

Некоторые функции возвращают структуру, называемую значением binn. Вот пример работы с ними:

```c
void print_value(binn *value) {
  switch (value->type) {
  case BINN_INT32:
    printf("integer: %d\n", value->vint32);
    break;
  case BINN_STRING:
    printf("string: %s\n", value->ptr);
    break;
  ...
  }
}
```