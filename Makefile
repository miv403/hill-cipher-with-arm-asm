# Makefile for ARM64 Cross Compilation

CC = aarch64-linux-gnu-gcc
AS = aarch64-linux-gnu-as

# Output binary name
TARGET = hill_cipher
ONLYCTARGET = hill_cipher_only_c
SRC = src/

# Source files
SRCS = $(SRC)main.c
ASM = $(SRC)hill_neon.S

# Build rules
all: $(TARGET)

$(TARGET): $(SRCS) $(ASM)
	$(CC) $(SRCS) $(ASM) -o $(TARGET) -static -O2

# New target for pure C compilation
onlyc: $(SRC)hill_cipher_only.c
	$(CC) $(SRC)hill_cipher_only.c -o $(ONLYCTARGET)  -static -O2

clean:
	rm -f $(TARGET) $(ONLYCTARGET) *_encrypted *_decrypted.bin
run:
	qemu-aarch64 $(TARGET)
test:
	./$(SRC)test.sh