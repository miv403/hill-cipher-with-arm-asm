
#ifndef MODULAR_MATRIX_H
#define MODULAR_MATRIX_H

#include <vector>
#include <iostream>
#include <stdexcept>
#include <iomanip>

extern "C" {
    long modInverse(long a, long m);
}

class ModularMatrix {
private:
    int rows;
    int cols;
    long modulus; // using long to match modInverse signature potentially
    std::vector<int> data;

    int mod(long x) const;

public:
    // Constructors
    ModularMatrix(int r, int c, long m);
    ModularMatrix(int n, long m); // Square matrix
    ModularMatrix(int r, int c, long m, const std::vector<int>& inputData);

    // Accessors
    int get(int r, int c) const;
    void set(int r, int c, int val);
    int getRows() const;
    int getCols() const;

    // Operations
    // Return determinant of this matrix (must be square)
    int getDeterminant() const;

    // Return adjugate of this matrix (must be square)
    ModularMatrix getAdjugate() const;

    // Return the inverse of this matrix (modular arithmetic)
    ModularMatrix getInverse() const;

    // Operator Overloading
    // Matrix Multiplication (Matrix * Matrix)
    ModularMatrix operator*(const ModularMatrix& other) const;

    // Scalar Multiplication (Matrix * Scalar)
    ModularMatrix operator*(int scalar) const;

    // Equality check
    bool operator==(const ModularMatrix& other) const;

    // Utility
    void print(const char* label = "Matrix") const;
    
    // Helpers
    static ModularMatrix createIdentity(int n, long m);
    
    // Helper to get minor matrix
    ModularMatrix getMinor(int r, int c) const;
};

#endif // MODULAR_MATRIX_H
