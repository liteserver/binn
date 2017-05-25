ifeq ($(OS),Windows_NT)
	TARGET = binn-1.0.dll
else
	TARGET = libbinn.so.1.0
endif

LINK1  = libbinn.so.1
LINK2  = libbinn.so
SHORT  = binn

.PHONY: test

all: $(TARGET)

libbinn.so.1.0: binn.o
	gcc -shared -Wl,-soname,$(LINK1) -o $@ $^
	strip $(TARGET)

binn-1.0.dll: binn.o dllmain.o
	gcc -shared -Wl,--out-implib,binn-1.0.lib -o $@ $^
	strip $(TARGET)

binn.o: src/binn.c src/binn.h
ifeq ($(OS),Windows_NT)
	gcc -Wall -c $<
else
	gcc -Wall -fPIC -c $<
endif

dllmain.o: src/win32/dllmain.c
	gcc -Wall -c $<

install:
ifeq ($(OS),Windows_NT)
	$(error install not supported on Windows)
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
	gcc -g -Wall -DDEBUG -o test/test_binn $^
	cd test && ./test_binn
