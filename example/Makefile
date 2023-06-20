CC = g++
CFLAGS = -O2 -std=c++17 -I . -I ../ -I ../include/
LDFLAGS= -L ../lib/Windows/ -L ../bin/
LDLIBS = -lraylib -limnotgui -pthread -lopengl32 -lgdi32 -lwinmm -static -static-libgcc -static-libstdc++
OBJS = demo.o
TARGETS = demo.exe korean.exe

.PHONY: all test execute clean

all: $(TARGETS)

%.exe: %.o
	$(CC) -o $@ $< $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

test: $(TARGETS) execute clean

execute:
	-./korean.exe

clean:
	-del /q $(OBJS) $(TARGET)