# Compiler and flags
CC = gcc
CFLAGS = -Wall -O3

# Source files
SOURCES = main.c PSO.c OF.c
OBJECTS = main.o PSO.o OF.o

# Target executable
TARGET = pso

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) -lm

# Compile source files into object files
main.o: main.c utility.h OF_lib.h
	$(CC) $(CFLAGS) -c main.c

PSO.o: PSO.c utility.h
	$(CC) $(CFLAGS) -c PSO.c

OF.o: OF.c OF_lib.h
	$(CC) $(CFLAGS) -c OF.c

# Clean up object files and the executable
clean:
	rm -f $(OBJECTS) $(TARGET)
