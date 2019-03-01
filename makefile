ifeq ($(OS),Windows_NT)
    TARGET = binn-1.0.dll
else ifeq ($(PLATFORM),iPhoneOS)
    TARGET = libbinn.so
    CFLAGS += -fPIC
else ifeq ($(PLATFORM),iPhoneSimulator)
    TARGET = libbinn.so
    CFLAGS += -fPIC
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        OS = OSX
        TARGET = libbinn.1.dylib
        LINK1  = libbinn.dylib
    else
        TARGET = libbinn.so.1.0
        LINK1  = libbinn.so.1
        LINK2  = libbinn.so
    endif
    CFLAGS += -fPIC
endif

SHORT  = binn

CC     = gcc
STRIP  = strip

.PHONY: test

all: $(TARGET)

libbinn.so.1.0: binn.o
	$(CC) -shared -Wl,-soname,$(LINK1) -o $@ $^
	$(STRIP) $(TARGET)

libbinn.1.dylib: binn.o
	$(CC) -dynamiclib -install_name "$@" -current_version 1.0.0 -compatibility_version 1.0 -o $@ $^
	#$(STRIP) $(TARGET)

libbinn.so: binn.o
	$(CC) -shared -o $@ $^ $(LDFLAGS)
	$(STRIP) -x $(TARGET)

binn-1.0.dll: binn.o dllmain.o
	$(CC) -shared -Wl,--out-implib,binn-1.0.lib -o $@ $^
	$(STRIP) $(TARGET)

binn.o: src/binn.c src/binn.h
	$(CC) -Wall $(CFLAGS) -c $<

dllmain.o: src/win32/dllmain.c
	$(CC) -Wall -c $<

install:
ifeq ($(OS),Windows_NT)
	$(error install not supported on Windows)
else ifeq ($(OS),OSX)
	cp $(TARGET) /usr/local/lib
	cd /usr/local/lib && ln -sf $(TARGET) $(LINK1)
	cp src/binn.h /usr/local/include
else
	cp $(TARGET) /usr/lib
	cd /usr/lib && ln -sf $(TARGET) $(LINK1)
	cd /usr/lib && ln -sf $(TARGET) $(LINK2)
	cp src/binn.h /usr/include
endif

clean:
	rm -f *.o $(TARGET)

uninstall:
	rm -f /usr/lib/$(LINK1) /usr/lib/$(LINK2) /usr/lib/$(TARGET) /usr/include/binn.h

test: test/test_binn.c test/test_binn2.c src/binn.c
	$(CC) -g -Wall -DDEBUG -o test/test_binn $^
	cd test && ./test_binn
