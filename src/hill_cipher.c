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

// === NEW: Visualization Helper ===
void printMatrix(const char* label, int *matrix, int rows, int cols) {
    printf("%s:\n", label);
    for (int i = 0; i < rows; i++) {
        printf("  [ ");
        for (int j = 0; j < cols; j++) {
            // Adjust formatting %3d to align numbers neatly
            printf("%3d ", matrix[i * cols + j]);
        }
        printf("]\n");
    }
    printf("\n");
}

// Multiply Matrix (3x3) x Vector (3x1)
void matVecMultiply(int *matrix, int *vec, int *result) {
    for (int i = 0; i < DIM; i++) {
        result[i] = 0;
        for (int j = 0; j < DIM; j++) {
            result[i] += matrix[i * DIM + j] * vec[j];
        }
        result[i] = mod(result[i]);
    }
}

// Calculate Determinant for 3x3 Matrix
int getDeterminant(int *m) {
    int det = m[0] * (m[4]*m[8] - m[5]*m[7]) -
              m[1] * (m[3]*m[8] - m[5]*m[6]) +
              m[2] * (m[3]*m[7] - m[4]*m[6]);
    return mod(det);
}

// Get Inverse Key Matrix
void getInverseMatrix(int *key, int *invKey) {
    int det = getDeterminant(key);
    printf(">> Determinant Calculation: %d\n", det);

    // CALL TO ASSEMBLY
    long detInv = modInverse(det, MOD);
    
    if (detInv == -1) {
        printf("Error: Matrix is not invertible modulo %d.\n", MOD);
        exit(1);
    }
    printf(">> Assembly returned Modular Inverse: %ld\n\n", detInv);

    // Calculate Adjugate (Transpose of Cofactor) and multiply by detInv
    invKey[0] = mod((key[4]*key[8] - key[5]*key[7]) * detInv);
    invKey[1] = mod((key[2]*key[7] - key[1]*key[8]) * detInv);
    invKey[2] = mod((key[1]*key[5] - key[2]*key[4]) * detInv);
    
    invKey[3] = mod((key[5]*key[6] - key[3]*key[8]) * detInv);
    invKey[4] = mod((key[0]*key[8] - key[2]*key[6]) * detInv);
    invKey[5] = mod((key[2]*key[3] - key[0]*key[5]) * detInv);
    
    invKey[6] = mod((key[3]*key[7] - key[4]*key[6]) * detInv);
    invKey[7] = mod((key[1]*key[6] - key[0]*key[7]) * detInv);
    invKey[8] = mod((key[0]*key[4] - key[1]*key[3]) * detInv);
}

int main() {
    // 3x3 Key Matrix
    int key[9] = {6, 24, 1, 13, 16, 10, 20, 17, 15};
    int invKey[9];

    // Message to encrypt
    char message[] = "HybridProject";
    int len = strlen(message);
    
    // Padding Calculation
    int remainder = len % DIM;
    int pad = (remainder == 0) ? 0 : (DIM - remainder);
    int totalLen = len + pad;

    // Buffer preparation
    char *buffer = malloc(totalLen + 1);
    strcpy(buffer, message);
    for(int i=0; i<pad; i++) buffer[len+i] = 0; // Pad with Null (0)

    // === 1. VISUALIZE PLAINTEXT ===
    printf("=== PHASE 1: PREPARATION ===\n");
    printf("Original Text: \"%s\"\n", message);
    printf("Padding added: %d bytes\n", pad);
    
    // Create a temporary integer array just to print the plaintext as a matrix
    int *plaintextMatrix = malloc(totalLen * sizeof(int));
    for(int i=0; i<totalLen; i++) plaintextMatrix[i] = (unsigned char)buffer[i];
    
    // Print Plaintext as a Matrix (Rows = chunks of 3)
    // We treat totalLen/3 as rows and 3 as columns for visualization
    printMatrix("Plaintext ASCII Values (Column Vectors as Rows for readability)", 
                plaintextMatrix, totalLen/DIM, DIM);
    free(plaintextMatrix);

    printMatrix("Encryption Key Matrix", key, 3, 3);

    // === 2. ENCRYPT ===
    printf("=== PHASE 2: ENCRYPTION ===\n");
    // Encrypt Buffer in place
    for(int i=0; i<totalLen; i+=DIM) {
        int v[3] = {buffer[i], buffer[i+1], buffer[i+2]};
        int r[3];
        matVecMultiply(key, v, r);
        buffer[i] = r[0]; buffer[i+1] = r[1]; buffer[i+2] = r[2];
    }

    // Print Ciphertext Matrix
    int *cipherMatrix = malloc(totalLen * sizeof(int));
    for(int i=0; i<totalLen; i++) cipherMatrix[i] = (unsigned char)buffer[i];
    printMatrix("Ciphertext Matrix (Encrypted Values)", cipherMatrix, totalLen/DIM, DIM);
    free(cipherMatrix);

    // === 3. GENERATE INVERSE KEY (ASSEMBLY) ===
    printf("=== PHASE 3: KEY INVERSION (Hybrid) ===\n");
    getInverseMatrix(key, invKey);
    printMatrix("Decryption Key Matrix (Calculated)", invKey, 3, 3);

    // === 4. DECRYPT ===
    printf("=== PHASE 4: DECRYPTION ===\n");
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