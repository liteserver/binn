Binn
====
[![Build Status](https://github.com/liteserver/binn/actions/workflows/build-and-test.yml/badge.svg)](https://github.com/liteserver/binn/actions)
[![Tests](https://img.shields.io/badge/tests-1815-brightgreen.svg)]()
[![Stable](https://img.shields.io/badge/status-stable-brightgreen.svg)]()

<p align="left"><a href="https://github.com/liteserver/binn/blob/master/README.md">English</a> | <a href="https://github.com/liteserver/binn/blob/master/README-zh.md">中文</a> | <a href="https://github.com/liteserver/binn/blob/master/README-ja.md">日本語</a></p>

Binn - это формат сериализации двоичных данных, разработанный для того, чтобы быть **компактным**, **быстрым** и **простым в использовании**.


Производительность
-------------------

Элементы хранятся с указанием их размеров для увеличения производительности чтения.

Библиотека использует zero-copy при чтении строк, блобов и контейнеров.

Строки завершаются нулевым символом, поэтому при чтении библиотека возвращает указатель на них внутри буфера, избегая выделения памяти и копирования данных.


Типы данных
-----------

Формат Binn поддерживает все это:

Примитивные типы данных:

* null
* boolean (`true` и `false`)
* целые числа (до 64 бит, знаковые или беззнаковые)
* числа с плавающей точкой (IEEE одинарной и двойной точности)
* строка
* блоб (двоичные данные)
* определенные пользователем

Контейнеры:

* список
* карта (ассоциативный массив с числовым ключом)
* объект (ассоциативный массив с текстовым ключом)

Формат
--------
Элементы хранятся следующим образом:
<pre>
boolean, null:
[type]

int, float (хранение: byte, word, dword или qword):
[type][data]

string, blob:
[type][size][data]

list, object, map:
[type][size][count][data]
</pre>

Пример структуры
---------------------
Данные json, такие как {"hello":"world"}, сериализуются в binn следующим образом:

<pre>
  \xE2           // тип = объект (контейнер)
  \x11           // общий размер контейнера
  \x01           // количество пар ключ/значение
  \x05hello      // ключ
  \xA0           // тип = строка
  \x05world\x00  // значение (завершается нулевым символом)
</pre>

Вы можете ознакомиться с [полной спецификацией](spec-ru.md)

Пример использования
-------------

Запись

```c
binn *obj;

// создание нового объекта
obj = binn_object();

// добавление значений к нему
binn_object_set_int32(obj, "id", 123);
binn_object_set_str(obj, "name", "John");
binn_object_set_double(obj, "total", 2.55);

// отправка по сети или сохранение в файл...
send(sock, binn_ptr(obj), binn_size(obj));

// освобождение буфера
binn_free(obj);
```

Чтение

```c
int id;
char *name;
double total;

id = binn_object_int32(obj, "id");
name = binn_object_str(obj, "name");
total = binn_object_double(obj, "total");
```

### Больше примеров

Вы можете найти больше примеров использования [здесь](usage-ru.md) и в [папке с примерами](examples)


## Другие реализации

 * Javascript: [liteserver/binn.js](https://github.com/liteserver/binn.js)
 * PHP: [et-nik/binn-php](https://github.com/et-nik/binn-php)
 * PHP: [JaredClemence/binn](https://github.com/JaredClemence/binn)
 * Python: [meeron/pybinn](https://github.com/meeron/pybinn)
 * Elixir: [thanos/binn](https://github.com/thanos/binn)
 * Erlang: [tonywallace64/erl_binn](https://github.com/tonywallace64/erl_binn) (частичная реализация)
 * F#: [meeron/FSBinn](https://github.com/meeron/FSBinn)
 * Rust: [Binn IR](https://lib.rs/crates/binn-ir)
 * Go: [et-nik/binngo](https://github.com/et-nik/binngo)

Не стесняйтесь делать обертку для вашего предпочтительного языка. Затем сообщите нам, чтобы мы могли указать его здесь.


Как использовать
----------

 1. Включая файл binn.c в ваш проект; или
 2. Включая статическую библиотеку в ваш проект; или
 3. Связывание с библиотекой binn:

### На Linux и MacOSX:
```
gcc myapp.c -lbinn
```

### На Windows:

Включите `binn-3.0.lib` в ваш проект MSVC или используйте MinGW:
```
gcc myapp.c -lbinn-3.0
```


Компиляция библиотеки
---------------------

### На Linux и MacOSX:

```
git clone https://github.com/liteserver/binn
cd binn
make
sudo make install
```

Он создаст файл `libbinn.so.3.0` на Linux и `libbinn.3.dylib` на MacOSX


### На Windows:

Используйте включенный проект Visual Studio в папке src/win32 или скомпилируйте его с помощью MinGW:

```
git clone https://github.com/liteserver/binn
cd binn
make
```

Оба создадут файл `binn-3.0.dll`


### Статическая библиотека

Для создания статической библиотеки:

```
make static
```

Он создаст файл `libbinn.a`


### На Android:

Проверьте наличие предварительно скомпилированных бинарных файлов в проекте [android-binn-native](https://github.com/litereplica/android-binn-native)


Регрессионные тесты
----------------

### На Linux, MacOSX и Windows (MinGW):

```
cd binn
make test
```

### На Windows (Visual Studio):

Используйте включенный проект в папке test/win32


Надежность
-----------

Текущая версия (3.0) стабильна и готова к использованию в производстве

Так как она кросс-платформенная, данные могут передаваться между устройствами с little-endian и big-endian


Лицензия
-------
Apache 2.0


Контакты
-------

Вопросы, предложения, поддержка: contact AT litereplica DOT io
