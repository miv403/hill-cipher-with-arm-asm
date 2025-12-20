
#include "ModularMatrix.h"

// Private helper for modulo arithmetic
int ModularMatrix::mod(long x) const {
    return (int)((x % modulus + modulus) % modulus);
}

// Constructors
ModularMatrix::ModularMatrix(int r, int c, long m) 
    : rows(r), cols(c), modulus(m), data(r * c, 0) {}

ModularMatrix::ModularMatrix(int n, long m) 
    : rows(n), cols(n), modulus(m), data(n * n, 0) {}

ModularMatrix::ModularMatrix(int r, int c, long m, const std::vector<int>& inputData) 
    : rows(r), cols(c), modulus(m) {
    if (inputData.size() != r * c) {
        throw std::invalid_argument("Input data size does not match matrix dimensions.");
    }
    for (int val : inputData) {
        data.push_back(mod(val));
    }
}

// Accessors
int ModularMatrix::get(int r, int c) const {
    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        throw std::out_of_range("Matrix indices out of range.");
    }
    return data[r * cols + c];
}

void ModularMatrix::set(int r, int c, int val) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        throw std::out_of_range("Matrix indices out of range.");
    }
    data[r * cols + c] = mod(val);
}

int ModularMatrix::getRows() const { return rows; }
int ModularMatrix::getCols() const { return cols; }

// Utility: get minor matrix (exclude row r, col c)
ModularMatrix ModularMatrix::getMinor(int r, int c) const {
    if (rows != cols) {
        throw std::logic_error("Minor undefined for non-square matrices.");
    }
    ModularMatrix minorInternal(rows - 1, cols - 1, modulus);
    int mi = 0;
    for (int i = 0; i < rows; i++) {
        if (i == r) continue;
        int mj = 0;
        for (int j = 0; j < cols; j++) {
            if (j == c) continue;
            minorInternal.set(mi, mj, get(i, j));
            mj++;
        }
        mi++;
    }
    return minorInternal;
}

// Determinant Calculation (Recursive Cofactor Expansion)
int ModularMatrix::getDeterminant() const {
    if (rows != cols) {
        throw std::logic_error("Determinant undefined for non-square matrices.");
    }
    
    // Base case 1x1
    if (rows == 1) {
        return get(0, 0);
    }
    
    // Base case 2x2
    if (rows == 2) {
        return mod((long)get(0, 0) * get(1, 1) - (long)get(0, 1) * get(1, 0));
    }

    // Recursive case
    long det = 0;
    // Expand along the first row
    for (int j = 0; j < cols; j++) {
        // Sign is (-1)^(0+j)
        long sign = (j % 2 == 0) ? 1 : -1;
        long term = sign * get(0, j) * getMinor(0, j).getDeterminant();
        det = mod(det + term);
    }
    return (int)det;
}

// Adjugate Matrix: Transpose of Cofactor Matrix
ModularMatrix ModularMatrix::getAdjugate() const {
    if (rows != cols) {
        throw std::logic_error("Adjugate undefined for non-square matrices.");
    }

    ModularMatrix adj(rows, cols, modulus);
    
    // For 1x1, adjugate is Identity(1) = [1] strictly speaking for inverse, but technically adj(A) s.t. A*adj(A) = det(A)*I.
    // simpler: adj([a]) = [1] because a*1 = det([a]).
    if (rows == 1) {
        adj.set(0, 0, 1);
        return adj;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Cofactor C_ij = (-1)^(i+j) * det(Minor(i, j))
            long sign = ((i + j) % 2 == 0) ? 1 : -1;
            int minorDet = getMinor(i, j).getDeterminant();
            long cofactor = sign * minorDet;
            
            // Transpose: Set adj[j][i] to Cofactor[i][j]
            adj.set(j, i, mod(cofactor));
        }
    }
    return adj;
}

// Scalar Product
ModularMatrix ModularMatrix::operator*(int scalar) const {
    ModularMatrix res(rows, cols, modulus);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            long val = (long)get(i, j) * scalar;
            res.set(i, j, mod(val));
        }
    }
    return res;
}

// Matrix Multiplication
ModularMatrix ModularMatrix::operator*(const ModularMatrix& other) const {
    if (cols != other.rows) {
        throw std::invalid_argument("Matrix dimensions incompatible for multiplication.");
    }
    if (modulus != other.modulus) {
        throw std::invalid_argument("Matrix moduli must match.");
    }

    ModularMatrix res(rows, other.cols, modulus);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < other.cols; j++) {
            long sum = 0;
            for (int k = 0; k < cols; k++) {
                sum += (long)get(i, k) * other.get(k, j);
            }
            res.set(i, j, mod(sum));
        }
    }
    return res;
}

// Inverse Matrix
ModularMatrix ModularMatrix::getInverse() const {
    int det = getDeterminant();
    std::cout << ">> Determinant Calculation: " << det << std::endl;

    long detInv = modInverse(det, modulus);
    
    if (detInv == -1) {
        throw std::runtime_error("Matrix is not invertible (determinant has no modular inverse).");
    }
    std::cout << ">> Assembly returned Modular Inverse: " << detInv << std::endl << std::endl;

    // Inverse = (1/det) * Adjugate
    // In modular arithmetic: detInv * Adjugate
    ModularMatrix adj = getAdjugate();
    return adj * (int)detInv;
}

// Print
void ModularMatrix::print(const char* label) const {
    std::cout << label << ":" << std::endl;
    for (int i = 0; i < rows; i++) {
        std::cout << "  [ ";
        for (int j = 0; j < cols; j++) {
            std::cout << std::setw(3) << get(i, j) << " ";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << std::endl;
}

bool ModularMatrix::operator==(const ModularMatrix& other) const {
    if (rows != other.rows || cols != other.cols || modulus != other.modulus) return false;
    return data == other.data;
}
