# === Cross Compiler Settings ===
# Use the explicit x86_64 cross-compiler
CC = g++
ASM = nasm

# Compile flags: 
# -static: Required to run easily in QEMU user mode
# -no-pie: Disables Position Independent Executable (simplifies assembly linking)
CFLAGS = -no-pie -g -Wall -static
ASMFLAGS = -f elf64 -g

# Target executable name
TARGET = hill_cipher

# Object files
OBJS = hill_cipher.o ModularMatrix.o mod_inverse.o

all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile C++ (Main)
hill_cipher.o: hill_cipher.cpp ModularMatrix.h
	$(CC) $(CFLAGS) -c hill_cipher.cpp

# Compile C++ (Class)
ModularMatrix.o: ModularMatrix.cpp ModularMatrix.h
	$(CC) $(CFLAGS) -c ModularMatrix.cpp

# Compile Assembly
mod_inverse.o: mod_inverse.asm
	$(ASM) $(ASMFLAGS) mod_inverse.asm

# Clean
clean:
	rm -f *.o $(TARGET)

# Run in QEMU (Emulator)
run: $(TARGET)
	qemu-x86_64 ./$(TARGET)