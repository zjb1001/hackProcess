CC=gcc
CFLAGS=-Wall -Wextra -g
LDFLAGS=

SRCS=main.c process_utils.c
OBJS=$(SRCS:.c=.o)
TARGET=process_hacker

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean