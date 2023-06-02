Binn
====
[![Build Status](https://github.com/liteserver/binn/actions/workflows/build-and-test.yml/badge.svg)](https://github.com/liteserver/binn/actions)
[![Tests](https://img.shields.io/badge/tests-1815-brightgreen.svg)]()
[![Stable](https://img.shields.io/badge/status-stable-brightgreen.svg)]()

<p align="left"><a href="https://github.com/liteserver/binn/blob/master/README.md">English</a> | <a href="https://github.com/liteserver/binn/blob/master/README-ja.md">日本語</a> | <a href="https://github.com/liteserver/binn/blob/master/README-ru.md">Русский</a></p>

Binn是一种二进制数据序列化格式，设计目的是使其**紧凑**，**快速**且**易于使用**。


性能
-----------

元素存储时附带其大小，以提高读取性能。

在读取字符串、二进制大对象和容器时，库使用零拷贝。

字符串以空字符结尾，因此在读取时，库会返回指向缓冲区内部的指针，避免内存分配和数据复制。


数据类型
----------

Binn格式支持以下所有类型：

基本数据类型：

* 空值
* 布尔值（`true` 和 `false`）
* 整数（最多64位有符号或无符号）
* 浮点数（IEEE单精度和双精度）
* 字符串
* 二进制大对象（二进制数据）
* 用户自定义

容器：

* 列表
* 映射（数字键关联数组）
* 对象（文本键关联数组）

格式
--------
元素以以下方式存储：
<pre>
布尔值，空值：
[类型]

整数，浮点数（存储：字节，字，双字或四字）：
[类型][数据]

字符串，二进制大对象：
[类型][大小][数据]

列表，对象，映射：
[类型][大小][计数][数据]
</pre>

示例结构
---------------------
一个类似于{"hello":"world"}的json数据在binn中的序列化表示为：

<pre>
  \xE2           // 类型 = 对象（容器）
  \x11           // 容器总大小
  \x01           // 键/值对计数
  \x05hello      // 键
  \xA0           // 类型 = 字符串
  \x05world\x00  // 值（空字符结尾）
</pre>

您可以查看[完整规范](spec-zh.md)

使用示例
-------------

写入

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

读取

```c
int id;
char *name;
double total;

id = binn_object_int32(obj, "id");
name = binn_object_str(obj, "name");
total = binn_object_double(obj, "total");
```

### 更多示例

您可以在[这里](usage-zh.md)和[示例文件夹](examples)中找到更多使用示例。


## 其他实现

 * Javascript: [liteserver/binn.js](https://github.com/liteserver/binn.js)
 * PHP: [et-nik/binn-php](https://github.com/et-nik/binn-php)
 * PHP: [JaredClemence/binn](https://github.com/JaredClemence/binn)
 * Python: [meeron/pybinn](https://github.com/meeron/pybinn)
 * Elixir: [thanos/binn](https://github.com/thanos/binn)
 * Erlang: [tonywallace64/erl_binn](https://github.com/tonywallace64/erl_binn) (部分实现)
 * F#: [meeron/FSBinn](https://github.com/meeron/FSBinn)
 * Rust: [Binn IR](https://lib.rs/crates/binn-ir)
 * Go: [et-nik/binngo](https://github.com/et-nik/binngo)

请随时为您喜欢的语言制作一个包装器。然后告诉我们，以便我们在此列出。


如何使用
----------

 1. 将binn.c文件包含在您的项目中；或者
 2. 将静态库包含在您的项目中；或者
 3. 链接到binn库：

### 在Linux和MacOSX上：
```
gcc myapp.c -lbinn
```

### 在Windows上：

将`binn-3.0.lib`包含在您的MSVC项目中或使用MinGW：
```
gcc myapp.c -lbinn-3.0
```


编译库
---------------------

### 在Linux和MacOSX上：

```
git clone https://github.com/liteserver/binn
cd binn
make
sudo make install
```

它将在Linux上创建文件`libbinn.so.3.0`，在MacOSX上创建文件`libbinn.3.dylib`


### 在Windows上：

使用src/win32文件夹中的包含的Visual Studio项目或使用MinGW编译：

```
git clone https://github.com/liteserver/binn
cd binn
make
```

两者都将创建文件`binn-3.0.dll`


### 静态库

要生成静态库：

```
make static
```

它将创建文件`libbinn.a`


### 在Android上：

在[android-binn-native](https://github.com/litereplica/android-binn-native)项目中查找预编译的二进制文件


回归测试
----------------

### 在Linux，MacOSX和Windows（MinGW）上：

```
cd binn
make test
```

### 在Windows（Visual Studio）上：

使用test/win32文件夹中的包含的项目


可靠性
-----------

当前版本（3.0）稳定且可用于生产环境

由于它是跨平台的，因此可以在小端和大端设备之间传输数据


许可证
-------
Apache 2.0


联系方式
-------

问题，建议，支持：contact AT litereplica DOT io
