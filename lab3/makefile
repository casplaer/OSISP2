CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = reverse_filter
SRCS = main.c utils.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	@echo "Testing with sample input:"
	@echo "Hello\nWorld" | ./$(TARGET)
	@echo "Expected output:"
	@echo "olleH\ndlroW"