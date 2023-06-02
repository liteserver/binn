Binnインターフェースと使用法
======================

### コンテナ

3種類のコンテナがあります。

* リスト
* マップ
* オブジェクト

リスト
-----

値は順序立てて格納されます。

> [2, 5, 9.1, "value", true]

#### 書き込み

```c
binn *list;

// 新しいリストを作成
list = binn_list();

// 値を追加
binn_list_add_int32(list, 123);
binn_list_add_double(list, 2.55);
binn_list_add_str(list, "testing");

// ネットワーク経由で送信するか、ファイルに保存する...
send(sock, binn_ptr(list), binn_size(list));

// バッファを解放
binn_free(list);
```

#### 位置での読み取り

```c
int id;
double rate;
char *name;

id = binn_list_int32(list, 1);
rate = binn_list_double(list, 2);
name = binn_list_str(list, 3);
```

#### 同じタイプの値を読み取る

```c
int i, count;
double note;

count = binn_count(list);
for(i=1; i<=count; i++) {
  note = binn_list_double(list, i);
}
```

#### for eachを使った読み取り

```c
binn_iter iter;
binn value;

binn_list_foreach(list, value) {
  do_something(&value);
}
```

マップ
----

値は整数キーで格納されます。

> {2: "test", 5: 2.5, 10: true}

マップを使用するアプリケーション間で共有されるヘッダーファイルで整数キーを定義できます。

```c
#define USER_ID    11
#define USER_NAME  12
#define USER_VALUE 13
```

#### 書き込み

```c
binn *map;

// 新しいマップを作成
map = binn_map();

// 値を追加
binn_map_set_int32(map, USER_ID, 123);
binn_map_set_str(map, USER_NAME, "John");
binn_map_set_double(map, USER_VALUE, 2.55);

// ネットワーク経由で送信するか、ファイルに保存する...
send(sock, binn_ptr(map), binn_size(map));

// バッファを解放
binn_free(map);
```

#### キーでの読み取り

```c
int id;
char *name;
double note;

id = binn_map_int32(map, USER_ID);
name = binn_map_str(map, USER_NAME);
note = binn_map_double(map, USER_VALUE);
```

#### 連続して読み取る

```c
binn_iter iter;
binn value;
int id;

binn_map_foreach(map, id, value) {
  do_something(id, &value);
}
```

オブジェクト
-------

値は文字列キーで格納されます。

> {"name": "John", "grade": 8.5, "active": true}

#### 書き込み

```c
binn *obj;

// 新しいオブジェクトを作成
obj = binn_object();

// 値を追加
binn_object_set_int32(obj, "id", 123);
binn_object_set_str(obj, "name", "John");
binn_object_set_double(obj, "total", 2.55);

// ネットワーク経由で送信するか、ファイルに保存する...
send(sock, binn_ptr(obj), binn_size(obj));

// バッファを解放
binn_free(obj);
```

#### キーでの読み取り

```c
int id;
char *name;
double total;

id = binn_object_int32(obj, "id");
name = binn_object_str(obj, "name");
total = binn_object_double(obj, "total");
```

#### 連続して読み取る

```c
binn_iter iter;
binn value;
char key[256];

binn_object_foreach(obj, key, value) {
  do_something(key, &value);
}
```

---

## 入れ子構造

他のコンテナの中にコンテナを入れることができます（オブジェクトの中のリスト、オブジェクトのリスト、マップのリスト...）

### 例1：オブジェクト内のリスト

> {id: 123, name: "John", values: [2.5, 7.35, 9.15]}

#### 書き込み

```c
binn *obj, *list;

// 新しいオブジェクトを作成
obj = binn_object();

// 値を追加
binn_object_set_int32(obj, "id", 123);
binn_object_set_str(obj, "name", "John");

// 新しいリストを作成
list = binn_list();
binn_list_add_double(list, 2.50);
binn_list_add_double(list, 7.35);
binn_list_add_double(list, 9.15);
binn_object_set_list(obj, "values", list);
binn_free(list);

// ネットワーク経由で送信するか、ファイルに保存する...
send(sock, binn_ptr(obj), binn_size(obj));

// メモリを解放
binn_free(obj);
```

#### 読み取り

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

### 例2：オブジェクトのリスト

> [ {name: "John", email: "john@gmail.com"} , {name: "Eric", email: "eric@gmail.com"} ]

#### 書き込み

```c
binn *list, *obj;

// 新しいリストを作成
list = binn_list();

// 最初のオブジェクトを作成
obj = binn_object();
binn_object_set_str(obj, "name", "John");
binn_object_set_str(obj, "email", "john@gmail.com");
// リストに追加して破棄
binn_list_add_object(list, obj);
binn_free(obj);

// 2番目のオブジェクトを作成
obj = binn_object();
binn_object_set_str(obj, "name", "Eric");
binn_object_set_str(obj, "email", "eric@gmail.com");
// リストに追加して破棄
binn_list_add_object(list, obj);
binn_free(obj);

// ネットワーク経由で送信するか、ファイルに保存する...
send(sock, binn_ptr(list), binn_size(list));

// メモリを解放
binn_free(list);
```

#### 読み取り

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

読み取り時に、内部コンテナは「解放」されない静的ポインタとして返されます。

---

### Binn値

いくつかの関数は、binn値と呼ばれる構造を返します。以下は、それらを扱う例です。

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