CC = g++
CFLAGS = -O2 -std=c++17 -I . -I ../ -I ../include/
LDFLAGS= -L ../lib/Windows/ -L ../bin/
LDLIBS = -lraylib -limnotgui -pthread -lopengl32 -lgdi32 -lwinmm -static -static-libgcc -static-libstdc++
OBJS = demo.o
TARGET = demo.exe

.PHONY: all test execute clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

test: $(TARGET) execute clean

execute:
	-$(TARGET)

clean:
	-del /q $(OBJS) $(TARGET)