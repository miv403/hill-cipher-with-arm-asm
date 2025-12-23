.global neon_hill_cipher
.type neon_hill_cipher, %function

// Function Signature:
// void neon_hill_cipher(uint8_t *input, uint8_t *output, size_t len, uint8_t *matrix);
// x0 = input buffer
// x1 = output buffer
// x2 = length (number of bytes)
// x3 = matrix pointer (4x4 flattened array)

neon_hill_cipher:
    // 1. Load the 4x4 Key Matrix
    // We use 'ldr' with 's' registers to load exactly 4 bytes (32 bits)
    // This zeros the upper bits of the vector register, ensuring safety.
    ldr s0, [x3], #4        // Load Row 0 (4 bytes)
    ldr s1, [x3], #4        // Load Row 1 (4 bytes)
    ldr s2, [x3], #4        // Load Row 2 (4 bytes)
    ldr s3, [x3]            // Load Row 3 (4 bytes)

loop_start:
    // Check if length is 0
    cbz x2, end_func

    // 2. Load 4 bytes of Input (Plaintext vector P)
    ldr s4, [x0], #4        // Load 4 bytes into v4

    // 3. Matrix Multiplication
    // We treat the registers as 8 bytes (.8b) for the vector math.
    // Since bytes 4-7 are zero (thanks to ldr sX), they won't affect the sum.

    // Calculate Row 0 * P
    mul v16.8b, v0.8b, v4.8b   // Multiply bytes
    addv b17, v16.8b           // Sum them up to a scalar
    st1 {v17.b}[0], [x1], #1   // Store result byte 0

    // Calculate Row 1 * P
    mul v16.8b, v1.8b, v4.8b
    addv b17, v16.8b
    st1 {v17.b}[0], [x1], #1   // Store result byte 1

    // Calculate Row 2 * P
    mul v16.8b, v2.8b, v4.8b
    addv b17, v16.8b
    st1 {v17.b}[0], [x1], #1   // Store result byte 2

    // Calculate Row 3 * P
    mul v16.8b, v3.8b, v4.8b
    addv b17, v16.8b
    st1 {v17.b}[0], [x1], #1   // Store result byte 3

    // 4. Decrement loop counter
    sub x2, x2, #4
    b loop_start

end_func:
    ret