CC=gcc
CFLAGS=-Wall -Wextra -g
TARGET=hello

all: $(TARGET)

$(TARGET): hello.c
	$(CC) $(CFLAGS) hello.c -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean