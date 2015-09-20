Binn
====

Binn is a binary data serialization format designed to be **compact**, **fast** and **easy to use**.

Performance
---------------
The elements are stored with theirs sizes to increase the read performance.

The library uses zero-copy when reading strings, blobs and containers.

The strings are null terminated so when read the library returns a pointer to them inside the buffer, avoiding memory allocation and data copying.

Data Types
------------
The Binn format supports all these:

Primitive data types:

* null
* boolean (<code>true</code> and <code>false</code>)
* integer (up to 64 bits signed or unsigned)
* floating point numbers (IEEE single and double precision)
* string
* blob (binary data)
* user defined

Containers:

* list
* map (numeric key associative array)
* object (text key associative array)

Format
--------
The elements are stored in this way:
<pre>
boolean, null:
[type]

int, float (storage: byte, word, dword or qword):
[type][data]

string, blob:
[type][size][data]

list, object, map:
[type][size][count][data]
</pre>

Example Structure
---------------------
A json data such as {"hello":"world"} is serialized in binn as:
<pre>
  \xE2           // type = object (container)
  \x11           // container total size
  \x01           // key/value pairs count
  \x05hello      // key
  \xA0           // type = string
  \x05world\x00  // value (null terminated)
</pre>

Example Codes
------------------

#### C
Writing
```
binn *obj;

// create a new object
obj = binn_object();

// add values to it
binn_object_set_int32(obj, "id", 123);
binn_object_set_str(obj, "name", "John");
binn_object_set_double(obj, "total", 2.55);

// send over the network or save to a file...
send(sock, binn_ptr(obj), binn_size(obj));

// release the buffer
binn_free(obj);
```
Reading
```
int id;
char *name;
double total;

id = binn_object_int32(obj, "id");
name = binn_object_str(obj, "name");
total = binn_object_double(obj, "total");
```

#### Javascript
Header
```
var binn = require('binn')()
  , to_binn   = binn.encode
  , from_binn = binn.decode
```
Writing
```
var obj = {hello: 'world', number: 123};
var data = to_binn(obj);
```
Reading
```
var obj = from_binn(data);
```

Licence
-------
Apache 2.0
