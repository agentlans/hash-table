.PHONY: all clean

all: src/hashtable.c src/vector.c
	$(CC) -fPIC -shared src/hashtable.c src/vector.c -Iinclude/ -o libhasht.so

#check:
#	$(CXX) Test.cpp ./libhasht.so -o TestApp
#	./TestApp

clean:
	rm libhasht.so #TestApp


