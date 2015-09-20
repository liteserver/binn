##Binn Interface & Usage
###Containers
There are 3 types of containers:

* Lists
* Maps
* Objects

-
###Lists
The values are stored in a sequential order:

> [2, 5, 9.1, "value", true]

####Writing
	binn *list;
	
	// create a new list
	list = binn_list();
	
	// add values to it
	binn_list_add_int32(list, 123);
	binn_list_add_double(list, 2.55);
	binn_list_add_str(list, "testing");
	
	// send over the network or save to a file...
	send(sock, binn_ptr(list), binn_size(list));
	
	// release the buffer
	binn_free(list);

####Reading by position
	int id;
	double rate;
	char *name;

	id = binn_list_int32(list, 1);
	rate = binn_list_double(list, 2);
	name = binn_list_str(list, 3);

####Reading values of the same type
	int i, count;
	double note;
	
	count = binn_count(list);
	for(i=1; i<=count; i++) {
	  note = binn_list_double(list, i);
	}

####Reading using for each
	binn_iter iter;
	binn value;
	
	binn_list_foreach(list, value) {
	  do_something(&value);
	}

-
###Maps
The values are stored with integer keys:
> {2: "first", 5: 2.5, 10: true}

You can define the integer keys in a header file shared between the applications that will use the map:

	#define USER_ID    11
	#define USER_NAME  12
	#define USER_VALUE 13

####Writing
	binn *map;
	
	// create a new map
	map = binn_map();
	
	// add values to it
	binn_map_set_int32(map, USER_ID, 123);
	binn_map_set_str(map, USER_NAME, "John");
	binn_map_set_double(map, USER_VALUE, 2.55);
	
	// send over the network or save to a file...
	send(sock, binn_ptr(map), binn_size(map));
	
	// release the buffer
	binn_free(map);

####Reading by key
	int id;
	char *name;
	double note;

	id = binn_map_int32(map, USER_ID);
	name = binn_map_str(map, USER_NAME);
	note = binn_map_double(map, USER_VALUE);

####Reading sequentially
	binn_iter iter;
	binn value;
	int id;
	
	binn_map_foreach(map, id, value) {
	  do_something(id, &value);
	}

-
###Objects
They store values linked to string keys
The values are stored with string keys:
> {"name": "John", "rate": 2.5, "active": true}

####Writing
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

####Reading by key
	int id;
	char *name;
	double total;

	id = binn_object_int32(obj, "id");
	name = binn_object_str(obj, "name");
	total = binn_object_double(obj, "total");

####Reading sequentially
	binn_iter iter;
	binn value;
	char key[256];
	
	binn_object_foreach(obj, key, value) {
	  do_something(key, &value);
	}

-
###Binn Values

Some functions return a structure called binn value. Here is an example of dealing with them:

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

-
###Containers inside containers

We can put containers inside others. Example: a list inside of an object, a list of objects, a list of maps...

####Writing
	binn *obj, *list;
	
	// create a new object
	obj = binn_object();
	
	// add values to it
	binn_object_set_int32(obj, "id", 123);
	binn_object_set_str(obj, "name", "John");
	
	// create a new list
	list = binn_list();
	binn_list_add_double(list, 2.50);
	binn_list_add_double(list, 7.35);
	binn_list_add_double(list, 9.15);
	binn_object_set_list(obj, "values", list);
	binn_free(list);
	
	// send over the network or save to a file...
	send(sock, binn_ptr(obj), binn_size(obj));

	// release the memory
	binn_free(obj);

####Reading
	int id, i, count;
	char *name;
	void *list;
	double note;

	id = binn_object_int32(obj, "id");
	name = binn_object_str(obj, "name");
	list = binn_object_list(obj, "values");

	count = binn_count(list);
	for(i=1; i<=count; i++) {
	  note = binn_list_double(list, i);
	}

When read, the internal containers are returned as pointers that should not be "freed".
