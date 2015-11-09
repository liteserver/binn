TARGET = libbinn.so.1.0
LINK1  = libbinn.so.1
LINK2  = libbinn.so
SHORT  = binn

$(TARGET): binn.o
	gcc -shared -Wl,-soname,$(LINK1) -o $@ $^
	strip $(TARGET)

binn.o: src/binn.c src/binn.h
	gcc -Wall -fPIC -c $<

install:
	cp $(TARGET) /usr/lib
	cd /usr/lib && ln -sf $(TARGET) $(LINK1)
	cd /usr/lib && ln -sf $(TARGET) $(LINK2)

clean:
	rm -f *.o $(TARGET)
