TARGET = libbinn.so.1.0
LINK1  = libbinn.so.1
LINK2  = libbinn.so
SHORT  = binn

.PHONY: test

$(TARGET): binn.o
	gcc -shared -Wl,-soname,$(LINK1) -o $@ $^
	strip $(TARGET)

binn.o: src/binn.c src/binn.h
	gcc -Wall -fPIC -c $<

install:
	cp $(TARGET) /usr/lib
	cd /usr/lib && ln -sf $(TARGET) $(LINK1)
	cd /usr/lib && ln -sf $(TARGET) $(LINK2)
	cp src/binn.h /usr/include

clean:
	rm -f *.o $(TARGET)

uninstall:
	rm -f /usr/lib/$(LINK1) /usr/lib/$(LINK2) /usr/lib/$(TARGET) /usr/include/binn.h

test: test/test_binn.c test/test_binn2.c src/binn.c
	gcc -g -Wall -DDEBUG -o test/test_binn $^
	cd test && ./test_binn
