# Makefile for ARM64 Cross Compilation

CC = aarch64-linux-gnu-gcc
AS = aarch64-linux-gnu-as

# Output binary name
TARGET = myapp_arm

# Source files
SRCS = main.c
ASM = hill_neon.S

# Build rules
all: $(TARGET)

$(TARGET): $(SRCS) $(ASM)
	$(CC) $(SRCS) $(ASM) -o $(TARGET) -static -O2

clean:
	rm -f $(TARGET) *_encrypted *_decrypted.bin