# Compiler settings
CC = g++
ASM = nasm
CFLAGS = -no-pie -g -Wall
ASMFLAGS = -f elf64 -g

# Target executable name
TARGET = hill_cipher

# Object files
OBJS = hill_cipher.o ModularMatrix.o mod_inverse.o

all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile C++
hill_cipher.o: hill_cipher.cpp ModularMatrix.h
	$(CC) $(CFLAGS) -c hill_cipher.cpp

ModularMatrix.o: ModularMatrix.cpp ModularMatrix.h
	$(CC) $(CFLAGS) -c ModularMatrix.cpp

# Compile Assembly
mod_inverse.o: mod_inverse.asm
	$(ASM) $(ASMFLAGS) mod_inverse.asm

# Clean
clean:
	rm -f *.o $(TARGET)

run: $(TARGET)
	./$(TARGET)
