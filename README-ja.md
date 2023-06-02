Binn
====
[![Build Status](https://github.com/liteserver/binn/actions/workflows/build-and-test.yml/badge.svg)](https://github.com/liteserver/binn/actions)
[![Tests](https://img.shields.io/badge/tests-1815-brightgreen.svg)]()
[![Stable](https://img.shields.io/badge/status-stable-brightgreen.svg)]()

<p align="left"><a href="https://github.com/liteserver/binn/blob/master/README.md">English</a> | <a href="https://github.com/liteserver/binn/blob/master/README-zh.md">中文</a> | <a href="https://github.com/liteserver/binn/blob/master/README-ru.md">Русский</a></p>

Binnは、**コンパクト**で**高速**で**使いやすい**バイナリデータシリアル化フォーマットです。

パフォーマンス
-----------

要素は、読み取りパフォーマンスを向上させるために、サイズとともに格納されます。

ライブラリは、文字列、BLOB、およびコンテナを読み取るときにゼロコピーを使用します。

文字列はヌル終端されているため、ライブラリが読み取るときにバッファ内のポインタが返され、メモリ割り当てとデータコピーが回避されます。

データタイプ
----------

Binnフォーマットは、以下のすべてをサポートしています。

プリミティブデータタイプ:

* null
* ブーリアン（`true`および`false`）
* 整数（最大64ビット、符号付きまたは符号なし）
* 浮動小数点数（IEEE単精度および倍精度）
* 文字列
* BLOB（バイナリデータ）
* ユーザー定義

コンテナ:

* リスト
* マップ（数値キー連想配列）
* オブジェクト（テキストキー連想配列）

フォーマット
--------
要素は次のように格納されます。
<pre>
ブール値、null:
[type]

int、float（ストレージ：バイト、ワード、DWORD、またはQWORD）：
[type][data]

文字列、BLOB：
[type][size][data]

リスト、オブジェクト、マップ：
[type][size][count][data]
</pre>

例の構造
---------------------
JSONデータ（例：{"hello":"world"}）は、Binnで次のようにシリアル化されます。

<pre>
  \xE2           // type = object (container)
  \x11           // container total size
  \x01           // key/value pairs count
  \x05hello      // key
  \xA0           // type = string
  \x05world\x00  // value (null terminated)
</pre>

[完全な仕様](spec-ja.md)を確認できます。

使用例
-------------

書き込み

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

読み取り

```c
int id;
char *name;
double total;

id = binn_object_int32(obj, "id");
name = binn_object_str(obj, "name");
total = binn_object_double(obj, "total");
```

### その他の例

[こちら](usage-ja.md)や[examplesフォルダ](examples)で、より多くの使用例を見つけることができます。

## その他の実装

 * Javascript: [liteserver/binn.js](https://github.com/liteserver/binn.js)
 * PHP: [et-nik/binn-php](https://github.com/et-nik/binn-php)
 * PHP: [JaredClemence/binn](https://github.com/JaredClemence/binn)
 * Python: [meeron/pybinn](https://github.com/meeron/pybinn)
 * Elixir: [thanos/binn](https://github.com/thanos/binn)
 * Erlang: [tonywallace64/erl_binn](https://github.com/tonywallace64/erl_binn) (部分的な実装)
 * F#: [meeron/FSBinn](https://github.com/meeron/FSBinn)
 * Rust: [Binn IR](https://lib.rs/crates/binn-ir)
 * Go: [et-nik/binngo](https://github.com/et-nik/binngo)

お気に入りの言語でラッパーを作成してください。その後、ここにリストアップするためにお知らせください。

使い方
----------

 1. プロジェクトにbinn.cファイルを含めるか、または
 2. プロジェクトに静的ライブラリを含めるか、または
 3. binnライブラリにリンクする:

### LinuxおよびMacOSXでの場合:
```
gcc myapp.c -lbinn
```

### Windowsでの場合:

MSVCプロジェクトに`binn-3.0.lib`を含めるか、MinGWを使用してください。
```
gcc myapp.c -lbinn-3.0
```


ライブラリのコンパイル
---------------------

### LinuxおよびMacOSXでの場合:

```
git clone https://github.com/liteserver/binn
cd binn
make
sudo make install
```

Linuxでは`libbinn.so.3.0`ファイル、MacOSXでは`libbinn.3.dylib`ファイルが作成されます。

### Windowsでの場合:

src/win32フォルダにあるVisual Studioプロジェクトを使用するか、MinGWを使用してコンパイルしてください。

```
git clone https://github.com/liteserver/binn
cd binn
make
```

どちらも`binn-3.0.dll`ファイルが作成されます。

### 静的ライブラリ

静的ライブラリを生成するには：

```
make static
```

`libbinn.a`ファイルが作成されます。

### Androidでの場合:

[android-binn-native](https://github.com/litereplica/android-binn-native)プロジェクトで事前にコンパイルされたバイナリを確認してください。

回帰テスト
----------------

### Linux、MacOSX、およびWindows（MinGW）での場合:

```
cd binn
make test
```

### Windows（Visual Studio）での場合:

test/win32フォルダにあるプロジェクトを使用してください。

信頼性
-----------

現在のバージョン（3.0）は安定しており、本番環境で使用する準備が整っています。

クロスプラットフォームであるため、リトルエンディアンとビッグエンディアンのデバイス間でデータを転送できます。

ライセンス
-------
Apache 2.0

連絡先
-------

質問、提案、サポート：contact AT litereplica DOT io
