#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MOD 127
#define DIM 3

// Declare the external assembly function
extern long modInverse(long a, long m);

// Helper to handle negative C modulo results
int mod(int x) {
    return (x % MOD + MOD) % MOD;
}

// Multiply Matrix (3x3) x Vector (3x1)
// Result = Matrix * Vector
void matVecMultiply(int *matrix, int *vec, int *result) {
    for (int i = 0; i < DIM; i++) {
        result[i] = 0;
        for (int j = 0; j < DIM; j++) {
            // Flattened index: row * DIM + col
            result[i] += matrix[i * DIM + j] * vec[j];
        }
        result[i] = mod(result[i]);
    }
}

// Calculate Determinant for 3x3 Matrix
int getDeterminant(int *m) {
    // Rule of Sarrus/Laplace expansion for row 0
    int det = m[0] * (m[4]*m[8] - m[5]*m[7]) -
              m[1] * (m[3]*m[8] - m[5]*m[6]) +
              m[2] * (m[3]*m[7] - m[4]*m[6]);
    return mod(det);
}

// Get Inverse Key Matrix using Adjugate Matrix
// Inverse = (det^-1) * adj(A)
void getInverseMatrix(int *key, int *invKey) {
    int det = getDeterminant(key);
    printf("[DEBUG] Determinant: %d\n", det);

    // CALL TO ASSEMBLY
    long detInv = modInverse(det, MOD);
    
    if (detInv == -1) {
        printf("Error: Matrix is not invertible modulo %d.\n", MOD);
        exit(1);
    }
    printf("[DEBUG] Modular Inverse (from ASM): %ld\n", detInv);

    // Calculate Adjugate (Transpose of Cofactor) and multiply by detInv
    // Hardcoded for 3x3 for clarity
    invKey[0] = mod((key[4]*key[8] - key[5]*key[7]) * detInv);
    invKey[1] = mod((key[2]*key[7] - key[1]*key[8]) * detInv); // Transposed
    invKey[2] = mod((key[1]*key[5] - key[2]*key[4]) * detInv); // Transposed
    
    invKey[3] = mod((key[5]*key[6] - key[3]*key[8]) * detInv); // Transposed
    invKey[4] = mod((key[0]*key[8] - key[2]*key[6]) * detInv);
    invKey[5] = mod((key[2]*key[3] - key[0]*key[5]) * detInv); // Transposed
    
    invKey[6] = mod((key[3]*key[7] - key[4]*key[6]) * detInv); // Transposed
    invKey[7] = mod((key[1]*key[6] - key[0]*key[7]) * detInv); // Transposed
    invKey[8] = mod((key[0]*key[4] - key[1]*key[3]) * detInv);
}

void processText(char *input, int *keyMatrix) {
    int len = strlen(input);
    
    // Padding Logic
    int remainder = len % DIM;
    int pad = (remainder == 0) ? 0 : (DIM - remainder);
    int totalLen = len + pad;

    char *processed = malloc(totalLen + 1);
    strcpy(processed, input);
    
    // Fill padding with ASCII 0 (Null) or 32 (Space)
    for (int i = 0; i < pad; i++) {
        processed[len + i] = 0; // Using Null padding
    }
    processed[totalLen] = '\0';

    printf("\n--- Processing ---\n");
    int vec[DIM], res[DIM];
    
    printf("Result (ASCII Integers): ");
    for (int i = 0; i < totalLen; i += DIM) {
        // Load vector
        for (int k = 0; k < DIM; k++) vec[k] = processed[i + k];
        
        // Transform
        matVecMultiply(keyMatrix, vec, res);
        
        // Print and store back
        for (int k = 0; k < DIM; k++) {
            printf("%d ", res[k]);
            processed[i+k] = (char)res[k];
        }
    }
    printf("\nDecoded String: %s\n", processed);
    free(processed);
}

int main() {
    // 3x3 Key Matrix (Invertible mod 127)
    // [ 6, 24,  1]
    // [13, 16, 10]
    // [20, 17, 15]
    int key[9] = {6, 24, 1, 13, 16, 10, 20, 17, 15};
    int invKey[9];

    char message[] = "HybridProject";

    printf("Original Message: %s\n", message);

    // 1. Encrypt
    printf("\n[ENCRYPTION]");
    processText(message, key); // Message is modified in place conceptually here just for display
    // For proper simulation, let's pretend 'message' is now ciphertext
    // Since processText prints output but doesn't return the array, 
    // we will rely on the logic flow for the project demo.
    
    // 2. Compute Inverse Key (using Assembly for EEA)
    printf("\n[KEY GENERATION]");
    getInverseMatrix(key, invKey);

    printf("Inverse Key Matrix:\n");
    for(int i=0; i<3; i++) printf("%d %d %d\n", invKey[i*3], invKey[i*3+1], invKey[i*3+2]);

    // 3. Decrypt
    // For demonstration, let's re-encrypt the original to get the ciphertext bytes
    // then decrypt those bytes.
    int len = strlen(message);
    int rem = len % DIM;
    int pad = (rem == 0) ? 0 : (DIM - rem);
    int totalLen = len + pad;
    
    char *buffer = malloc(totalLen + 1);
    strcpy(buffer, message);
    for(int i=0; i<pad; i++) buffer[len+i] = 0;
    
    // Encrypt Buffer
    for(int i=0; i<totalLen; i+=DIM) {
        int v[3] = {buffer[i], buffer[i+1], buffer[i+2]};
        int r[3];
        matVecMultiply(key, v, r);
        buffer[i] = r[0]; buffer[i+1] = r[1]; buffer[i+2] = r[2];
    }
    
    printf("\n[DECRYPTION VERIFICATION]\n");
    printf("Ciphertext values: ");
    for(int i=0; i<totalLen; i++) printf("%d ", buffer[i]);
    printf("\n");

    // Decrypt Buffer using invKey
    for(int i=0; i<totalLen; i+=DIM) {
        int v[3] = {buffer[i], buffer[i+1], buffer[i+2]};
        int r[3];
        matVecMultiply(invKey, v, r);
        buffer[i] = r[0]; buffer[i+1] = r[1]; buffer[i+2] = r[2];
    }
    
    printf("Decrypted Message: %s\n", buffer);
    free(buffer);

    return 0;
}
