Binn接口和用法
======================

### 容器

有3种类型的容器：

* 列表
* 映射
* 对象

列表
-----

值按顺序存储：

> [2, 5, 9.1, "value", true]

#### 写入

```c
binn *list;

// 创建一个新列表
list = binn_list();

// 向其中添加值
binn_list_add_int32(list, 123);
binn_list_add_double(list, 2.55);
binn_list_add_str(list, "testing");

// 通过网络发送或保存到文件...
send(sock, binn_ptr(list), binn_size(list));

// 释放缓冲区
binn_free(list);
```

#### 按位置读取

```c
int id;
double rate;
char *name;

id = binn_list_int32(list, 1);
rate = binn_list_double(list, 2);
name = binn_list_str(list, 3);
```

#### 读取相同类型的值

```c
int i, count;
double note;

count = binn_count(list);
for(i=1; i<=count; i++) {
  note = binn_list_double(list, i);
}
```

#### 使用for each读取

```c
binn_iter iter;
binn value;

binn_list_foreach(list, value) {
  do_something(&value);
}
```

映射
----

值与整数键一起存储：

> {2: "test", 5: 2.5, 10: true}

您可以在将使用映射的应用程序之间共享的头文件中定义整数键：

```c
#define USER_ID    11
#define USER_NAME  12
#define USER_VALUE 13
```

#### 写入

```c
binn *map;

// 创建一个新映射
map = binn_map();

// 向其中添加值
binn_map_set_int32(map, USER_ID, 123);
binn_map_set_str(map, USER_NAME, "John");
binn_map_set_double(map, USER_VALUE, 2.55);

// 通过网络发送或保存到文件...
send(sock, binn_ptr(map), binn_size(map));

// 释放缓冲区
binn_free(map);
```

#### 按键读取

```c
int id;
char *name;
double note;

id = binn_map_int32(map, USER_ID);
name = binn_map_str(map, USER_NAME);
note = binn_map_double(map, USER_VALUE);
```

#### 顺序读取

```c
binn_iter iter;
binn value;
int id;

binn_map_foreach(map, id, value) {
  do_something(id, &value);
}
```

对象
-------

值与字符串键一起存储：

> {"name": "John", "grade": 8.5, "active": true}

#### 写入

```c
binn *obj;

// 创建一个新对象
obj = binn_object();

// 向其中添加值
binn_object_set_int32(obj, "id", 123);
binn_object_set_str(obj, "name", "John");
binn_object_set_double(obj, "total", 2.55);

// 通过网络发送或保存到文件...
send(sock, binn_ptr(obj), binn_size(obj));

// 释放缓冲区
binn_free(obj);
```

#### 按键读取

```c
int id;
char *name;
double total;

id = binn_object_int32(obj, "id");
name = binn_object_str(obj, "name");
total = binn_object_double(obj, "total");
```

#### 顺序读取

```c
binn_iter iter;
binn value;
char key[256];

binn_object_foreach(obj, key, value) {
  do_something(key, &value);
}
```

---

## 嵌套结构

我们可以将容器放入其他容器中（对象内的列表，对象列表，映射列表...）

### 示例1：对象内的列表

> {id: 123, name: "John", values: [2.5, 7.35, 9.15]}

#### 写入

```c
binn *obj, *list;

// 创建一个新对象
obj = binn_object();

// 向其中添加值
binn_object_set_int32(obj, "id", 123);
binn_object_set_str(obj, "name", "John");

// 创建一个新列表
list = binn_list();
binn_list_add_double(list, 2.50);
binn_list_add_double(list, 7.35);
binn_list_add_double(list, 9.15);
binn_object_set_list(obj, "values", list);
binn_free(list);

// 通过网络发送或保存到文件...
send(sock, binn_ptr(obj), binn_size(obj));

// 释放内存
binn_free(obj);
```

#### 读取

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

### 示例2：对象列表

> [ {name: "John", email: "john@gmail.com"} , {name: "Eric", email: "eric@gmail.com"} ]

#### 写入

```c
binn *list, *obj;

// 创建一个新列表
list = binn_list();

// 创建第一个对象
obj = binn_object();
binn_object_set_str(obj, "name", "John");
binn_object_set_str(obj, "email", "john@gmail.com");
// 添加到列表并丢弃
binn_list_add_object(list, obj);
binn_free(obj);

// 创建第二个对象
obj = binn_object();
binn_object_set_str(obj, "name", "Eric");
binn_object_set_str(obj, "email", "eric@gmail.com");
// 添加到列表并丢弃
binn_list_add_object(list, obj);
binn_free(obj);

// 通过网络发送或保存到文件...
send(sock, binn_ptr(list), binn_size(list));

// 释放内存
binn_free(list);
```

#### 读取

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

#### 注意

读取时，内部容器作为静态指针返回，不应该被“释放”。

---

### Binn值

有些函数返回一个名为binn value的结构。以下是处理它们的示例：

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