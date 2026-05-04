/**
 * LINA-CLI: Matrix Operation System (v1.5)
 * Upgrades: Rank, Gauss-Jordan Inverse, Linear Independence, Shear, Projection.
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <limits>

using namespace std;

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

// ==========================================
// TEMPLATE MATRIX CLASS
// ==========================================
template <typename T>
class Matrix
{
protected:
    int nrow, ncol;
    std::vector<std::vector<T>> matx;

public:
    Matrix(int row = 0, int col = 0) : nrow(row), ncol(col)
    {
        if (nrow > 0 && ncol > 0)
            matx.assign(nrow, std::vector<T>(ncol, 0));
    }

    friend istream &operator>>(istream &is, Matrix<T> &m)
    {
        cout << "\n+-----------------------------------+\n";
        cout << "| \033[42m\033[30m ENTER ELEMENTS ROW WISE \033[0m         |\n";
        cout << "+-----------------------------------+\n";
        for (int i = 0; i < m.nrow; i++)
        {
            cout << " Row " << i + 1 << " (" << m.ncol << " elements) : ";
            for (int j = 0; j < m.ncol; j++)
                is >> m.matx[i][j];
        }
        return is;
    }

    friend ostream &operator<<(ostream &os, const Matrix<T> &m)
    {
        if (m.nrow == 0 || m.ncol == 0)
            return os;
        int width = 10;
        int total = (m.ncol * width) + 2;
        if (total < 14)
            total = 14;

        os << "\n+";
        for (int i = 0; i < total; i++)
            os << "-";
        os << "+\n|";
        int leftSpace = (total - 12) / 2;
        int rightSpace = total - 12 - leftSpace;
        for (int i = 0; i < leftSpace; i++)
            os << " ";
        os << "FINAL MATRIX";
        for (int i = 0; i < rightSpace; i++)
            os << " ";
        os << "|\n+";
        for (int i = 0; i < total; i++)
            os << "-";
        os << "+\n";

        for (int i = 0; i < m.nrow; i++)
        {
            os << "| ";
            for (int j = 0; j < m.ncol; j++)
                os << setw(width) << m.matx[i][j];
            int remainingSpace = total - (m.ncol * width) - 1;
            for (int j = 0; j < remainingSpace; j++)
                os << " ";
            os << "|\n";
        }
        os << "+";
        for (int i = 0; i < total; i++)
            os << "-";
        os << "+\n";
        return os;
    }

    Matrix<T> operator+(const Matrix<T> &m) const
    {
        if (nrow != m.nrow || ncol != m.ncol)
            throw std::invalid_argument("Dimension mismatch!");
        Matrix<T> result(nrow, ncol);
        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++)
                result.matx[i][j] = matx[i][j] + m.matx[i][j];
        return result;
    }

    Matrix<T> operator-(const Matrix<T> &m) const
    {
        if (nrow != m.nrow || ncol != m.ncol)
            throw std::invalid_argument("Dimension mismatch!");
        Matrix<T> result(nrow, ncol);
        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++)
                result.matx[i][j] = matx[i][j] - m.matx[i][j];
        return result;
    }

    Matrix<T> operator*(T scalar) const
    {
        Matrix<T> result(nrow, ncol);
        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++)
                result.matx[i][j] = matx[i][j] * scalar;
        return result;
    }

    Matrix<T> operator*(const Matrix<T> &m) const
    {
        if (ncol != m.nrow)
            throw std::invalid_argument("Dimension mismatch for multiplication!");
        Matrix<T> result(nrow, m.ncol);
        for (int i = 0; i < nrow; i++)
        {
            for (int j = 0; j < m.ncol; j++)
            {
                result.matx[i][j] = 0;
                for (int k = 0; k < ncol; k++)
                    result.matx[i][j] += matx[i][k] * m.matx[k][j];
            }
        }
        return result;
    }

    Matrix<T> transpose() const
    {
        Matrix<T> t(ncol, nrow);
        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++)
                t.matx[j][i] = matx[i][j];
        return t;
    }

    void ref()
    {
        int limit = (nrow < ncol) ? nrow : ncol;
        for (int i = 0; i < limit; i++)
        {
            T pivot = std::abs(matx[i][i]);
            if (pivot < 1e-9)
            {
                for (int k = i + 1; k < nrow; k++)
                {
                    if (std::abs(matx[k][i]) > 1e-9)
                    {
                        matx[i].swap(matx[k]);
                        break;
                    }
                }
            }
            if (std::abs(matx[i][i]) > 1e-9)
            {
                for (int k = i + 1; k < nrow; k++)
                {
                    T factor = matx[k][i] / matx[i][i];
                    for (int j = 0; j < ncol; j++)
                    {
                        matx[k][j] = matx[k][j] - factor * matx[i][j];
                        if (std::abs(matx[k][j]) < 1e-9)
                            matx[k][j] = 0;
                    }
                }
            }
        }
    }
    // --- V1.5 RECOVERED FEATURE: DETERMINANT ---
    Matrix<T> getMinor(int excludeRow, int excludeCol) const {
        Matrix<T> minorMat(nrow - 1, ncol - 1);
        int r = 0;
        for (int i = 0; i < nrow; i++) {
            if (i == excludeRow) continue; // Skip the crossed-out row
            int c = 0;
            for (int j = 0; j < ncol; j++) {
                if (j == excludeCol) continue; // Skip the crossed-out column
                minorMat.matx[r][c] = this->matx[i][j];
                c++;
            }
            r++;
        }
        return minorMat;
    }

    double determinant() const {
        if (nrow != ncol) throw std::invalid_argument("Determinant is only defined for square matrices!");
        if (nrow == 1) return (double)matx[0][0];
        if (nrow == 2) return (double)((matx[0][0] * matx[1][1]) - (matx[0][1] * matx[1][0]));

        double det = 0.0;
        int sign = 1;
        
        // Recursively calculate sub-determinants across the top row
        for (int f = 0; f < ncol; f++) {
            Matrix<T> minorMat = getMinor(0, f);
            det += sign * (double)matx[0][f] * minorMat.determinant();
            sign = -sign; // Alternating signs (+ - + -)
        }
        
        // Clean up floating point dust
        if (std::abs(det) < 1e-9) return 0.0;
        return det;
    }

    // --- V1.5 FEATURE: RANK ---
    int rank() const
    {
        Matrix<double> temp(nrow, ncol);
        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++)
                temp.matx[i][j] = (double)matx[i][j];
        temp.ref();

        int r = 0;
        for (int i = 0; i < temp.nrow; i++)
        {
            bool isNonZero = false;
            for (int j = 0; j < temp.ncol; j++)
            {
                if (std::abs(temp.matx[i][j]) > 1e-9)
                {
                    isNonZero = true;
                    break;
                }
            }
            if (isNonZero)
                r++;
        }
        return r;
    }

    // --- V1.5 FEATURE: INVERSE (GAUSS-JORDAN) ---
    Matrix<double> inverse() const
    {
        if (nrow != ncol)
            throw std::invalid_argument("Only square matrices can have an inverse!");
        int n = nrow;

        // Create Augmented Matrix [A | I]
        std::vector<std::vector<double>> aug(n, std::vector<double>(2 * n, 0.0));
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                aug[i][j] = (double)matx[i][j];
            aug[i][n + i] = 1.0;
        }

        // Gauss-Jordan Elimination
        for (int i = 0; i < n; i++)
        {
            if (std::abs(aug[i][i]) < 1e-9)
            {
                for (int k = i + 1; k < n; k++)
                {
                    if (std::abs(aug[k][i]) > 1e-9)
                    {
                        aug[i].swap(aug[k]);
                        break;
                    }
                }
            }

            if (std::abs(aug[i][i]) < 1e-9)
                throw std::runtime_error("Matrix is singular (Determinant is 0). Inverse does not exist!");

            double pivot = aug[i][i];
            for (int j = 0; j < 2 * n; j++)
                aug[i][j] /= pivot;

            // Eliminate ABOVE and BELOW the pivot
            for (int k = 0; k < n; k++)
            {
                if (k != i)
                {
                    double factor = aug[k][i];
                    for (int j = 0; j < 2 * n; j++)
                    {
                        aug[k][j] -= factor * aug[i][j];
                        if (std::abs(aug[k][j]) < 1e-9)
                            aug[k][j] = 0.0;
                    }
                }
            }
        }

        // Extract the Right Half (The Inverse)
        Matrix<double> inv(n, n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                inv.matx[i][j] = aug[i][n + j];

        return inv;
    }

    Matrix<double> solveSystem(const Matrix<T> &B) const
    {
        if (nrow != ncol)
            throw std::invalid_argument("System must be square!");
        if (nrow != B.nrow)
            throw std::invalid_argument("Dimension mismatch!");

        int n = nrow;
        std::vector<std::vector<double>> aug(n, std::vector<double>(n + 1));

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                aug[i][j] = (double)matx[i][j];
            aug[i][n] = (double)B.matx[i][0];
        }

        for (int i = 0; i < n; i++)
        {
            if (std::abs(aug[i][i]) < 1e-9)
            {
                for (int k = i + 1; k < n; k++)
                {
                    if (std::abs(aug[k][i]) > 1e-9)
                    {
                        aug[i].swap(aug[k]);
                        break;
                    }
                }
            }
            if (std::abs(aug[i][i]) < 1e-9)
                throw std::runtime_error("System has no unique solution.");

            for (int k = i + 1; k < n; k++)
            {
                double factor = aug[k][i] / aug[i][i];
                for (int j = i; j <= n; j++)
                    aug[k][j] -= factor * aug[i][j];
            }
        }

        Matrix<double> solution(n, 1);
        for (int i = n - 1; i >= 0; i--)
        {
            solution.matx[i][0] = aug[i][n];
            for (int j = i + 1; j < n; j++)
                solution.matx[i][0] -= aug[i][j] * solution.matx[j][0];
            solution.matx[i][0] /= aug[i][i];
        }
        return solution;
    }

    // STATIC TRANSFORMATION HELPERS
    static Matrix<T> Identity(int size)
    {
        Matrix<T> I(size, size);
        for (int i = 0; i < size; i++)
            I.matx[i][i] = 1;
        return I;
    }
    static Matrix<T> Scale2D(T scaleX, T scaleY)
    {
        Matrix<T> S = Identity(2);
        S.matx[0][0] = scaleX;
        S.matx[1][1] = scaleY;
        return S;
    }
    static Matrix<T> Reflect2D(bool acrossXAxis, bool acrossYAxis)
    {
        Matrix<T> R = Identity(2);
        if (acrossXAxis)
            R.matx[1][1] = -1;
        if (acrossYAxis)
            R.matx[0][0] = -1;
        return R;
    }
    static Matrix<T> Rotate2D(double angleDegrees)
    {
        Matrix<T> R = Identity(2);
        const double PI = 3.14159265358979323846;
        double radians = angleDegrees * PI / 180.0;
        R.matx[0][0] = (T)std::cos(radians);
        R.matx[0][1] = (T)-std::sin(radians);
        R.matx[1][0] = (T)std::sin(radians);
        R.matx[1][1] = (T)std::cos(radians);
        return R;
    }

    // --- V1.5 FEATURE: SHEAR & PROJECTION ---
    static Matrix<T> Shear2D(T kx, T ky)
    {
        Matrix<T> S = Identity(2);
        S.matx[0][1] = kx;
        S.matx[1][0] = ky;
        return S;
    }
    static Matrix<T> Project2D(bool ontoX)
    {
        Matrix<T> P = Identity(2);
        if (ontoX)
            P.matx[1][1] = 0;
        else
            P.matx[0][0] = 0;
        return P;
    }
};

// ==========================================
// TEMPLATE VECTOR CLASS
// ==========================================
template <typename T>
class Vector : public Matrix<T>
{
public:
    // Standard constructor
    Vector(int size = 0) : Matrix<T>(size, 1) {}

    // The Conversion Constructor (Allows Matrix to be cast as a Vector)
    Vector(const Matrix<T> &m) : Matrix<T>(m) {}

    int getrow() const { return this->nrow; }
    T &operator[](int index) { return this->matx[index][0]; }
    T operator[](int index) const { return this->matx[index][0]; }
};
// ==========================================
// TRANSFORMATION PIPELINE
// ==========================================
template <typename T>
class TransformPipeline
{
private:
    Matrix<T> masterTransform;
    int dimensions;

public:
    TransformPipeline(int dims) : dimensions(dims) { masterTransform = Matrix<T>::Identity(dims); }
    void showPipelineMatrix() const { cout << "\n[ Current Master Transformation Matrix ]" << masterTransform; }
    void addScale(T sx, T sy)
    {
        if (dimensions == 2)
            masterTransform = Matrix<T>::Scale2D(sx, sy) * masterTransform;
    }
    void addRotation(double degrees)
    {
        if (dimensions == 2)
            masterTransform = Matrix<T>::Rotate2D(degrees) * masterTransform;
    }
    void addReflection(bool acrossX, bool acrossY)
    {
        if (dimensions == 2)
            masterTransform = Matrix<T>::Reflect2D(acrossX, acrossY) * masterTransform;
    }

    // --- V1.5 PIPELINE ADDITIONS ---
    void addShear(T kx, T ky)
    {
        if (dimensions == 2)
            masterTransform = Matrix<T>::Shear2D(kx, ky) * masterTransform;
    }
    void addProjection(bool ontoX)
    {
        if (dimensions == 2)
            masterTransform = Matrix<T>::Project2D(ontoX) * masterTransform;
    }

    Vector<T> apply(const Vector<T> &targetVector) const
    {
        if (targetVector.getrow() != dimensions)
            throw std::invalid_argument("Vector dimension mismatch!");
        return masterTransform * targetVector;
    }
};

// ==========================================
// FILE HANDLING UTILITIES
// ==========================================
template <typename T>
void autoLogAndPromptSave(const Matrix<T> &m, const string &operationName)
{
    ofstream logFile("lina_history.txt", ios::app);
    if (logFile.is_open())
    {
        logFile << "\n=== " << operationName << " ===\n"
                << m << "\n";
        logFile.close();
        cout << GREEN << "\n[Auto-Saved to lina_history.txt]\n"
             << RESET;
    }
    char choice;
    cout << "Save a copy to a custom path? (Y/N): ";
    cin >> choice;
    if (choice == 'Y' || choice == 'y')
    {
        string path;
        cout << "Enter path: ";
        cin >> path;
        ofstream customFile(path, ios::app);
        if (customFile.is_open())
        {
            customFile << "\n=== " << operationName << " ===\n"
                       << m << "\n";
            customFile.close();
            cout << GREEN << "[Success] Result copied to " << path << "!\n"
                 << RESET;
        }
        else
        {
            cout << RED << "[Error] File access failed.\n"
                 << RESET;
        }
    }
}

void viewLog()
{
    ifstream file("lina_history.txt");
    if (!file.is_open())
    {
        cout << YELLOW << "\n[Notice] No history log found yet.\n"
             << RESET;
        return;
    }
    cout << "\n\033[35m" << BOLD << "=== LINA-CLI OPERATION LOG ===" << RESET << "\n";
    string line;
    while (getline(file, line))
        cout << line << "\n";
    cout << "\033[35m" << BOLD << "==============================" << RESET << "\n";
    file.close();
}

// ==========================================
// MAIN CLI LOOP
// ==========================================
int main()
{
    cin.exceptions(ios_base::failbit);
    int choice = 0;

    cout << "\n\033[34m+=======================================================+\033[0m\n";
    cout << "\033[34m|\033[0m" << BOLD << CYAN << "             LINA-CLI: MATRIX OPERATION SYSTEM         " << RESET << "\033[34m|\033[0m\n";
    cout << "\033[34m+=======================================================+\033[0m\n";

    do
    {
        try
        {
            cout << "\n\n"
                 << BOLD << "========= MAIN MENU =========" << RESET << "\n";
            cout << "1. Matrix Arithmetics (+, -, *)\n";
            cout << "2. Transpose Matrix\n";
            cout << "3. Advanced Properties (Inverse, Rank, Independence,Determinate) " << RESET << "\n";
            cout << "4. Row Echelon Form (REF)\n";
            cout << "5. Solve Linear Systems (Ax = B)\n";
            cout << "6. Matrix Transformation (2D Pipeline) " << RESET << "\n";
            cout << "7. View Operation Log\n";
            cout << RED << "8. Exit\n"
                 << RESET;
            cout << "Enter your preferred operation: ";

            cin >> choice;

            if (choice == 8)
            {
                cout << CYAN << "\nExiting LINA-CLI... Goodbye!\n\n"
                     << RESET;
                break;
            }

            switch (choice)
            {
            case 1: // MATRIX ARITHMETICS
            {
                int arithChoice;
                cout << "\n"
                     << BOLD << "--- Matrix Arithmetics ---" << RESET << "\n";
                cout << "1. Addition\n2. Subtraction\n3. Multiplication\nSelect: ";
                cin >> arithChoice;

                if (arithChoice == 1 || arithChoice == 2)
                {
                    int r1, c1, r2, c2;
                    cout << "Size of Matrix A (rows cols): ";
                    cin >> r1 >> c1;
                    cout << "Size of Matrix B (rows cols): ";
                    cin >> r2 >> c2;
                    if (r1 != r2 || c1 != c2)
                        throw std::invalid_argument("Size mismatch!");

                    Matrix<double> A(r1, c1), B(r2, c2);
                    cout << "\nInput Matrix A:";
                    cin >> A;
                    cout << "\nInput Matrix B:";
                    cin >> B;

                    if (arithChoice == 1)
                    {
                        Matrix<double> sum = A + B;
                        cout << GREEN << "\n*********** ADDITION RESULT ***********" << RESET;
                        cout << sum;
                        autoLogAndPromptSave(sum, "Addition");
                    }
                    else
                    {
                        Matrix<double> diff = A - B;
                        cout << GREEN << "\n********* SUBTRACTION RESULT **********" << RESET;
                        cout << diff;
                        autoLogAndPromptSave(diff, "Subtraction");
                    }
                }
                else if (arithChoice == 3)
                {
                    int multChoice;
                    cout << "\nMultiply by:\n1. Scalar\n2. Another Matrix\nSelect: ";
                    cin >> multChoice;
                    if (multChoice == 1)
                    {
                        int r1, c1;
                        double s;
                        cout << "Size of Matrix A (rows cols): ";
                        cin >> r1 >> c1;
                        Matrix<double> A(r1, c1);
                        cout << "\nInput Matrix A:";
                        cin >> A;
                        cout << "Enter Scalar multiplier: ";
                        cin >> s;
                        Matrix<double> result = A * s;
                        cout << GREEN << "\n*********** SCALAR RESULT ***********" << RESET;
                        cout << result;
                        autoLogAndPromptSave(result, "Scalar Multiplication");
                    }
                    else if (multChoice == 2)
                    {
                        int r1, c1, r2, c2;
                        cout << "Size of Matrix A (rows cols): ";
                        cin >> r1 >> c1;
                        cout << "Size of Matrix B (rows cols): ";
                        cin >> r2 >> c2;
                        if (c1 != r2)
                            throw std::invalid_argument("Matrix A cols must equal Matrix B rows!");
                        Matrix<double> A(r1, c1), B(r2, c2);
                        cout << "\nInput Matrix A:";
                        cin >> A;
                        cout << "\nInput Matrix B:";
                        cin >> B;
                        Matrix<double> product = A * B;
                        cout << GREEN << "\n***** MATRIX MULTIPLICATION RESULT *****" << RESET;
                        cout << product;
                        autoLogAndPromptSave(product, "Matrix Multiplication");
                    }
                }
                break;
            }
            case 2: // TRANSPOSE
            {
                int r1, c1;
                cout << "\n\033[36m=== TRANSPOSE ===\033[0m\nSize of Matrix (rows cols): ";
                cin >> r1 >> c1;
                Matrix<double> A(r1, c1);
                cout << "\nInput Matrix:";
                cin >> A;
                Matrix<double> t = A.transpose();
                cout << GREEN << "\n******** TRANSPOSE RESULT ********" << RESET;
                cout << t;
                autoLogAndPromptSave(t, "Transposed Matrix");
                break;
            }
            case 3: // ADVANCED PROPERTIES (V1.5)
            {
                int advChoice;
                cout << "\n" << BOLD << "--- Advanced Properties ---" << RESET << "\n";
                // UPDATE THIS MENU TEXT
                cout << "1. Calculate Rank\n2. Gauss-Jordan Inverse\n3. Check Linear Independence\n4. Calculate Determinant\nSelect: ";
                cin >> advChoice;

                if (advChoice == 1)
                {
                    int r1, c1;
                    cout << "Size of Matrix (rows cols): ";
                    cin >> r1 >> c1;
                    Matrix<double> A(r1, c1);
                    cout << "\nInput Matrix:";
                    cin >> A;
                    int r = A.rank();
                    cout << GREEN << "\nMatrix Rank: " << r << RESET << "\n";
                }
                else if (advChoice == 2)
                {
                    int r1, c1;
                    cout << "Size of Matrix (rows cols): ";
                    cin >> r1 >> c1;
                    Matrix<double> A(r1, c1);
                    cout << "\nInput Matrix:";
                    cin >> A;
                    Matrix<double> inv = A.inverse();
                    cout << GREEN << "\n********** INVERSE MATRIX **********" << RESET;
                    cout << inv;
                    autoLogAndPromptSave(inv, "Inverse Matrix");
                }
                else if (advChoice == 3)
                {
                    int r1, c1;
                    cout << "Enter number of vectors (rows) and their dimension (cols): ";
                    cin >> r1 >> c1;
                    Matrix<double> A(r1, c1);
                    cout << "\nInput Vectors row by row:";
                    cin >> A;
                    int r = A.rank();
                    cout << CYAN << "\nCalculated Rank: " << r << RESET << "\n";
                    if (r == r1)
                    {
                        cout << GREEN << "Conclusion: Vectors are Linearly INDEPENDENT.\n"
                             << RESET;
                    }
                    else
                    {
                        cout << RED << "Conclusion: Vectors are Linearly DEPENDENT.\n"
                             << RESET;
                    }
                }
                else if (advChoice == 4) {
                    int r1, c1;
                    cout << "Size of Matrix (rows cols): "; cin >> r1 >> c1;
                    Matrix<double> A(r1, c1); 
                    cout << "\nInput Matrix:"; cin >> A;
                    
                    double det = A.determinant();
                    cout << CYAN << "\n********** DETERMINANT **********" << RESET;
                    cout << GREEN << "\n|A| = " << det << RESET << "\n";
                }
                break;
            }
            case 4: // REF
            {
                int r1, c1;
                cout << "\n\033[36m=== ROW ECHELON FORM (REF) ===\033[0m\nSize of Matrix (rows cols): ";
                cin >> r1 >> c1;
                Matrix<double> A(r1, c1);
                cout << "\nInput Matrix:";
                cin >> A;
                A.ref();
                cout << GREEN << "\n********** REF RESULT **********" << RESET;
                cout << A;
                autoLogAndPromptSave(A, "Row Echelon Form");
                break;
            }
            case 5: // SOLVE SYSTEM
            {
                int n;
                cout << "\n\033[35m=== SOLVE LINEAR SYSTEM (A*x = B) ===\033[0m\nEnter variables (n): ";
                cin >> n;
                Matrix<double> A(n, n);
                Vector<double> B(n);
                cout << "\nInput Coefficient Matrix A (" << n << "x" << n << "):";
                cin >> A;
                cout << "\nInput Constants Vector B (" << n << " elements):";
                cin >> B;
                Vector<double> solution = A.solveSystem(B);
                cout << GREEN << "\n********** SOLUTION VECTOR (x) **********" << RESET;
                cout << solution;
                autoLogAndPromptSave(solution, "Linear System Solution");
                break;
            }
            case 6: // TRANSFORMATION PIPELINE
            {
                cout << "\n\033[35m=== 2D TRANSFORMATION PIPELINE ===\033[0m\n";
                Vector<double> point(2);
                cout << "Enter the X coordinate: ";
                cin >> point[0];
                cout << "Enter the Y coordinate: ";
                cin >> point[1];

                TransformPipeline<double> pipe(2);
                int pipeChoice;
                cout << GREEN << "\nTarget Vector created at (" << point[0] << ", " << point[1] << ")" << RESET << "\n";

                do
                {
                    cout << "\n--- Add Transformation ---\n";
                    cout << "1. Scale\n2. Rotate\n3. Reflect\n4. Shear " << YELLOW << "[NEW]" << RESET << "\n5. Projection " << YELLOW << "[NEW]" << RESET << "\n6. APPLY PIPELINE & VIEW RESULT\n";
                    cout << "Enter choice: ";
                    cin >> pipeChoice;

                    if (pipeChoice == 1)
                    {
                        double sx, sy;
                        cout << "Scale X factor: ";
                        cin >> sx;
                        cout << "Scale Y factor: ";
                        cin >> sy;
                        pipe.addScale(sx, sy);
                        cout << YELLOW << "-> Scale added.\n"
                             << RESET;
                    }
                    else if (pipeChoice == 2)
                    {
                        double angle;
                        cout << "Rotation Angle (degrees): ";
                        cin >> angle;
                        pipe.addRotation(angle);
                        cout << YELLOW << "-> Rotation added.\n"
                             << RESET;
                    }
                    else if (pipeChoice == 3)
                    {
                        int rx, ry;
                        cout << "Reflect X-axis? (1/0): ";
                        cin >> rx;
                        cout << "Reflect Y-axis? (1/0): ";
                        cin >> ry;
                        pipe.addReflection(rx == 1, ry == 1);
                        cout << YELLOW << "-> Reflection added.\n"
                             << RESET;
                    }
                    else if (pipeChoice == 4)
                    {
                        double kx, ky;
                        cout << "Shear X factor: ";
                        cin >> kx;
                        cout << "Shear Y factor: ";
                        cin >> ky;
                        pipe.addShear(kx, ky);
                        cout << YELLOW << "-> Shear added.\n"
                             << RESET;
                    }
                    else if (pipeChoice == 5)
                    {
                        int projChoice;
                        cout << "Project onto: 1. X-axis  2. Y-axis: ";
                        cin >> projChoice;
                        pipe.addProjection(projChoice == 1);
                        cout << YELLOW << "-> Projection added.\n"
                             << RESET;
                    }
                } while (pipeChoice != 6);

                pipe.showPipelineMatrix();
                Vector<double> transformedPoint = pipe.apply(point);
                cout << GREEN << "\n********** FINAL TRANSFORMED COORDINATE **********" << RESET;
                cout << transformedPoint;
                autoLogAndPromptSave(transformedPoint, "Pipeline Coordinate");
                break;
            }
            case 7: // LOG
                viewLog();
                break;
            default:
                cout << RED << "\n[Error] Invalid choice!\n"
                     << RESET;
            }
        }
        catch (const std::exception &e)
        {
            if (cin.fail())
            {
                cout << RED << "\n[Input Error] Please enter numbers only.\n"
                     << RESET;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = 0;
            }
            else
            {
                cout << RED << "\n[Fatal Math Error] " << e.what() << RESET << "\n";
            }
            cout << YELLOW << "-> Aborted. Returning to Menu...\n"
                 << RESET;
        }
    } while (choice != 8);

    return 0;
}