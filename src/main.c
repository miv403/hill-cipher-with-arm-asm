#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

// Declare the external assembly function
extern void neon_hill_cipher(uint8_t *in, uint8_t *out, size_t len, uint8_t *matrix);

// Simple hardcoded 4x4 Key Matrix (Diagonal 3)
// Determinant is non-zero mod 256.
uint8_t KEY_MATRIX[16] = {
    3, 0, 0, 0,
    0, 3, 0, 0,
    0, 0, 3, 0,
    0, 0, 0, 3
};

// Inverse of 3 mod 256 is 171 (since 3 * 171 = 513 = 256*2 + 1)
uint8_t INV_KEY_MATRIX[16] = {
    171, 0, 0, 0,
    0, 171, 0, 0,
    0, 0, 171, 0,
    0, 0, 0, 171
};

double get_time_sec() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

void process_file(const char *in_fname, const char *out_fname, uint8_t *matrix, const char *mode) {
    FILE *fin = fopen(in_fname, "rb");
    if (!fin) {
        printf("Error: Could not open input file %s\n", in_fname);
        return;
    }

    // Get file size
    fseek(fin, 0, SEEK_END);
    size_t fsize = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    // Calculate padded size (must be divisible by 4)
    size_t padded_size = (fsize + 3) & ~3;
    
    // Allocate buffers
    uint8_t *input_buf = (uint8_t*)calloc(padded_size, 1);
    uint8_t *output_buf = (uint8_t*)malloc(padded_size);

    // Read file
    fread(input_buf, 1, fsize, fin);
    fclose(fin);

    // BENCHMARK START
    printf("[%s] Processing %zu bytes... ", mode, padded_size);
    double start = get_time_sec();
    
    neon_hill_cipher(input_buf, output_buf, padded_size, matrix);
    
    double end = get_time_sec();
    // BENCHMARK END

    double time_taken = end - start;
    double throughput = (padded_size / (1024.0 * 1024.0)) / time_taken;
    printf("Done in %.4f sec (%.2f MB/s)\n", time_taken, throughput);

    // Write output
    FILE *fout = fopen(out_fname, "wb");
    // For simplicity, we write the full padded buffer. 
    // In a real app, you would store the original size in a header.
    fwrite(output_buf, 1, padded_size, fout);
    fclose(fout);

    free(input_buf);
    free(output_buf);
}

int main(int argc, char **argv) {
    char filename[256];
    char enc_filename[300];
    char dec_filename[300];

    // Hardcoded input file as requested
    strcpy(filename, "test.bin");

    // 1. Encryption
    // Construct output name: <file>_encrypted
    snprintf(enc_filename, sizeof(enc_filename), "%s_encrypted", filename);
    process_file(filename, enc_filename, KEY_MATRIX, "ENCRYPT");

    // 2. Decryption
    // Construct output name: <file>_decrypted.<ext>
    // We assume the original extension is part of 'filename'
    snprintf(dec_filename, sizeof(dec_filename), "%s_decrypted.bin", filename);
    // Simple logic to keep extension if possible, but strict logic is bloat.
    // Let's just append _decrypted.
    
    process_file(enc_filename, dec_filename, INV_KEY_MATRIX, "DECRYPT");

    printf("\nSuccess! Files generated:\n - %s\n - %s\n", enc_filename, dec_filename);

    return 0;
}