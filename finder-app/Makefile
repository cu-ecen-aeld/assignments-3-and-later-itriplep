# Default values
CC = gcc
CROSS_COMPILE = 

# Compiler and flags
CC := $(CROSS_COMPILE)$(CC)
CFLAGS = -Wall -O2

# Source files and target executable
SRCS = writer.c
OBJS = $(SRCS:.c=.o)
TARGET = writer

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean target
clean:
	rm -f $(TARGET) $(OBJS)
