
#include <iostream>
#include <vector>
#include <string>
#include "ModularMatrix.h"

#define MOD 127
#define DIM 3

int main() {
    // 3x3 Key Matrix
    std::vector<int> keyData = {6, 24, 1, 13, 16, 10, 20, 17, 15};
    ModularMatrix key(DIM, DIM, MOD, keyData);
    ModularMatrix invKey(DIM, DIM, MOD); // Will be calculated

    // Message to encrypt
    std::string message = "HybridProject";
    int len = message.length();
    
    // Padding Calculation
    int remainder = len % DIM;
    int pad = (remainder == 0) ? 0 : (DIM - remainder);
    int totalLen = len + pad;

    // Buffer preparation (integers for processing)
    std::vector<int> buffer(totalLen);
    for(int i=0; i<len; i++) buffer[i] = (unsigned char)message[i];
    for(int i=0; i<pad; i++) buffer[len+i] = 0; // Pad with 0

    // === 1. VISUALIZE PLAINTEXT ===
    std::cout << "=== PHASE 1: PREPARATION ===" << std::endl;
    std::cout << "Original Text: \"" << message << "\"" << std::endl;
    std::cout << "Padding added: " << pad << " bytes" << std::endl;
    
    // Visualize as a matrix
    // We treat totalLen/DIM as rows and DIM as columns for visualization
    ModularMatrix plaintextMatrix(totalLen/DIM, DIM, MOD, buffer);
    plaintextMatrix.print("Plaintext ASCII Values (Column Vectors as Rows for readability)");

    key.print("Encryption Key Matrix");

    // === 2. ENCRYPT ===
    std::cout << "=== PHASE 2: ENCRYPTION ===" << std::endl;
    // Encrypt chunks
    // Cipher = Key * PlainVec
    for(int i=0; i<totalLen; i+=DIM) {
        std::vector<int> chunkData;
        for(int k=0; k<DIM; k++) chunkData.push_back(buffer[i+k]);
        
        ModularMatrix vec(DIM, 1, MOD, chunkData);
        ModularMatrix res = key * vec; // Matrix * Vector (3x3 * 3x1 = 3x1)
        
        for(int k=0; k<DIM; k++) buffer[i+k] = res.get(k, 0);
    }

    // Print Ciphertext Matrix
    ModularMatrix cipherMatrix(totalLen/DIM, DIM, MOD, buffer);
    cipherMatrix.print("Ciphertext Matrix (Encrypted Values)");

    // === 3. GENERATE INVERSE KEY (ASSEMBLY) ===
    std::cout << "=== PHASE 3: KEY INVERSION (Hybrid) ===" << std::endl;
    try {
        invKey = key.getInverse();
        invKey.print("Decryption Key Matrix (Calculated)");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // === 4. DECRYPT ===
    std::cout << "=== PHASE 4: DECRYPTION ===" << std::endl;
    for(int i=0; i<totalLen; i+=DIM) {
        std::vector<int> chunkData;
        for(int k=0; k<DIM; k++) chunkData.push_back(buffer[i+k]);
        
        ModularMatrix vec(DIM, 1, MOD, chunkData);
        ModularMatrix res = invKey * vec;
        
        for(int k=0; k<DIM; k++) buffer[i+k] = res.get(k, 0);
    }
    
    // Print Decrypted Message
    std::cout << "Decrypted Message: ";
    for(int i=0; i<totalLen; i++) {
        // Just print characters, handle null terminator if it stops the string
        if (buffer[i] == 0) break; 
        std::cout << (char)buffer[i];
    }
    std::cout << std::endl;

    return 0;
}
