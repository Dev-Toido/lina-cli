#include <iostream>
#include <iomanip>
#include <cmath> 
using namespace std;

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// TEMPLATE MATRIX CLASS
template <typename T>
class Matrix
{
protected:
    int nrow, ncol;
    T **matx;

public:
    // CONSTRUCTOR
    Matrix(int row = 0, int col = 0)
    {
        nrow = row;
        ncol = col;

        if (nrow > 0 && ncol > 0) {
            matx = new T *[nrow];
            for (int i = 0; i < nrow; i++) {
                matx[i] = new T[ncol]{0}; // Initialize to 0
            }
        } else {
            matx = nullptr;
        }
    }

    // COPY CONSTRUCTOR (Deep Copy)
    Matrix(const Matrix<T> &m)
    {
        nrow = m.nrow;
        ncol = m.ncol;

        if (nrow > 0 && ncol > 0) {
            matx = new T *[nrow];
            for (int i = 0; i < nrow; i++)
            {
                matx[i] = new T[ncol];
                for (int j = 0; j < ncol; j++)
                {
                    matx[i][j] = m.matx[i][j];
                }
            }
        } else {
            matx = nullptr;
        }
    }

    // COPY ASSIGNMENT OPERATOR (Crucial for avoiding crashes with temp matrices)
    Matrix<T>& operator=(const Matrix<T> &m)
    {
        if (this == &m) return *this; // Protect against self-assignment

        // 1. Delete old memory
        if (matx != nullptr) {
            for (int i = 0; i < nrow; i++) delete[] matx[i];
            delete[] matx;
        }

        // 2. Copy new data
        nrow = m.nrow;
        ncol = m.ncol;
        if (nrow > 0 && ncol > 0) {
            matx = new T *[nrow];
            for (int i = 0; i < nrow; i++) {
                matx[i] = new T[ncol];
                for (int j = 0; j < ncol; j++) {
                    matx[i][j] = m.matx[i][j];
                }
            }
        } else {
            matx = nullptr;
        }
        return *this;
    }

    // DESTRUCTOR
    ~Matrix()
    {
        if (matx != nullptr) {
            for (int i = 0; i < nrow; i++)
                delete[] matx[i];
            delete[] matx;
        }
    }

    // INPUT
    friend istream &operator>>(istream &is, Matrix<T> &m)
    {
        cout << "\n+-----------------------------------+\n";
        cout << "| \033[42m\033[30m ENTER ELEMENTS ROW WISE \033[0m         |\n";
        cout << "+-----------------------------------+\n";

        for (int i = 0; i < m.nrow; i++)
        {
            cout << " Row " << i + 1 << " : ";
            for (int j = 0; j < m.ncol; j++)
                is >> m.matx[i][j];
        }
        return is;
    }

    // OUTPUT
    friend ostream &operator<<(ostream &os, const Matrix<T> &m)
    {
        if (m.nrow == 0 || m.ncol == 0) return os; // Safeguard

        int width = 8;
        int total = m.ncol * width + 3;

        cout << "\n+";
        for (int i = 0; i < total; i++) cout << "-";
        cout << "+\n";

        cout << "|";
        int titleSpace = (total - 12) / 2;
        for (int i = 0; i < titleSpace; i++) cout << " ";
        cout << "FINAL MATRIX";
        for (int i = 0; i < titleSpace - 1; i++) cout << " ";
        cout << "  |\n";

        cout << "+";
        for (int i = 0; i < total; i++) cout << "-";
        cout << "+\n";

        for (int i = 0; i < m.nrow; i++)
        {
            cout << "|";
            for (int j = 0; j < m.ncol; j++)
                cout << setw(width) << m.matx[i][j];
            cout << "   |\n";
        }

        cout << "+";
        for (int i = 0; i < total; i++) cout << "-";
        cout << "+\n";

        return os;
    }

    // ADDITION
    Matrix<T> operator+(const Matrix<T> &m) const
    {
        Matrix<T> result(nrow, ncol);
        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++)
                result.matx[i][j] = matx[i][j] + m.matx[i][j];
        return result;
    }

    // SUBTRACTION
    Matrix<T> operator-(const Matrix<T> &m) const
    {
        Matrix<T> result(nrow, ncol);
        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++)
                result.matx[i][j] = matx[i][j] - m.matx[i][j];
        return result;
    }

    // m1 * scalar
    Matrix<T> operator*(T scalar) const
    {
        Matrix<T> result(nrow, ncol);
        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++)
                result.matx[i][j] = matx[i][j] * scalar;
        return result;
    }

    // scalar * m1
    friend Matrix<T> operator*(T scalar, const Matrix<T> &m)
    {
        Matrix<T> result(m.nrow, m.ncol);
        for (int i = 0; i < m.nrow; i++)
            for (int j = 0; j < m.ncol; j++)
                result.matx[i][j] = scalar * m.matx[i][j];
        return result;
    }

    // MATRIX MULTIPLICATION
    Matrix<T> operator*(const Matrix<T> &m) const
    {
        Matrix<T> result(nrow, m.ncol);
        for (int i = 0; i < nrow; i++)
        {
            for (int j = 0; j < m.ncol; j++)
            {
                result.matx[i][j] = 0;
                for (int k = 0; k < ncol; k++)
                {
                    result.matx[i][j] += matx[i][k] * m.matx[k][j];
                }
            }
        }
        return result;
    }

    // TRANSPOSE
    Matrix<T> transpose() const
    {
        Matrix<T> t(ncol, nrow);
        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++)
                t.matx[j][i] = matx[i][j];
        return t;
    }

    // ROW ECHELON FORM (No algorithm library used)
    void ref()
    {
        // Custom min logic
        int limit = (nrow < ncol) ? nrow : ncol; 

        for (int i = 0; i < limit; i++)
        {
            // Custom absolute value logic for pivot check to handle floats
            T pivot = (matx[i][i] < 0) ? -matx[i][i] : matx[i][i];

            if (pivot < 1e-9)
            {
                for (int k = i + 1; k < nrow; k++)
                {
                    T check = (matx[k][i] < 0) ? -matx[k][i] : matx[k][i];
                    if (check > 1e-9)
                    {
                        // Custom pointer swap (much faster than std::swap)
                        T* tempRow = matx[i];
                        matx[i] = matx[k];
                        matx[k] = tempRow;
                        break;
                    }
                }
            }

            pivot = (matx[i][i] < 0) ? -matx[i][i] : matx[i][i];
            if (pivot > 1e-9)
            {
                for (int k = i + 1; k < nrow; k++)
                {
                    T factor = matx[k][i] / matx[i][i];
                    for (int j = 0; j < ncol; j++)
                    {
                        matx[k][j] = matx[k][j] - factor * matx[i][j];
                    }
                }
            }
        }
    }

    // HELPER: Minor Matrix
    Matrix<T> getMinor(int excludeRow, int excludeCol) const
    {
        Matrix<T> minorMat(nrow - 1, ncol - 1);
        int r = 0; 
        for (int i = 0; i < nrow; i++)
        {
            if (i == excludeRow) continue;
            int c = 0; 
            for (int j = 0; j < ncol; j++)
            {
                if (j == excludeCol) continue;
                minorMat.matx[r][c] = this->matx[i][j];
                c++;
            }
            r++;
        }
        return minorMat;
    }

    // RECURSIVE DETERMINANT
    T determinant() const
    {
        if (nrow != ncol) {
            cout << "Error: Determinant is only defined for square matrices!\n";
            return 0; 
        }
        if (nrow == 1) return matx[0][0];
        if (nrow == 2) return (matx[0][0] * matx[1][1]) - (matx[0][1] * matx[1][0]);

        T det = 0;
        int sign = 1;

        for (int f = 0; f < ncol; f++)
        {
            Matrix<T> minorMat = getMinor(0, f);
            det += sign * matx[0][f] * minorMat.determinant();
            sign = -sign; 
        }
        return det;
    }
};

// TEMPLATE VECTOR CLASS (INHERITANCE)
template <typename T>
class Vector : public Matrix<T>
{
public:
    Vector(int size = 0) : Matrix<T>(size, 1) {}

    Vector(const Matrix<T> &m) : Matrix<T>(m)
    {
        if (this->ncol != 1 && this->nrow > 0) {
            cout << YELLOW << "Warning: Converting an N x M matrix into a vector!\n" << RESET;
        }
    }

    T& operator[](int index) { return this->matx[index][0]; }
    T operator[](int index) const { return this->matx[index][0]; }

    // MAGNITUDE (Works in N-Dimensional Space)
    double magnitude() const
    {
        double sum = 0;
        for (int i = 0; i < this->nrow; ++i) {
            sum += this->matx[i][0] * this->matx[i][0];
        }
        return std::sqrt(sum);
    }

    // DOT PRODUCT (Works in N-Dimensional Space: 1D, 2D, 3D, 4D...)
    T dot(const Vector<T> &v) const
    {
        // 1. Validate that both vectors exist in the same dimensional space
        if (this->nrow != v.nrow) 
        {
            cout << RED << "\n[Math Error] Dot product failed: Vectors must be the exact same dimension!\n" << RESET;
            return 0; 
        }
        
        // 2. Compute N-Dimensional Dot Product
        T result = 0;
        for (int i = 0; i < this->nrow; ++i) {
            result += this->matx[i][0] * v.matx[i][0];
        }
        return result;
    }

    // CROSS PRODUCT (Strictly adapted for 2D and 3D Spaces)
    Vector<T> cross(const Vector<T> &v) const
    {
        if (this->nrow != v.nrow) 
        {
            cout << RED << "\n[Math Error] Cross product failed: Vectors must be the same dimension!\n" << RESET;
            return Vector<T>(0); 
        }

        // --- 2D SPACE CROSS PRODUCT ---
        // Returns a 1D Vector (which acts as a single scalar value representing the Z-axis magnitude)
        if (this->nrow == 2)
        {
            Vector<T> result(1);
            result[0] = (this->matx[0][0] * v.matx[1][0]) - (this->matx[1][0] * v.matx[0][0]); // (x1*y2 - y1*x2)
            
            cout << CYAN << "\n[Note] 2D Cross Product computed. Returning 1D scalar vector (Z-magnitude).\n" << RESET;
            return result;
        }
        
        // --- 3D SPACE CROSS PRODUCT ---
        // Returns a standard orthogonal 3D Vector
        else if (this->nrow == 3)
        {
            Vector<T> result(3);
            result[0] = this->matx[1][0] * v.matx[2][0] - this->matx[2][0] * v.matx[1][0]; // yz - zy
            result[1] = this->matx[2][0] * v.matx[0][0] - this->matx[0][0] * v.matx[2][0]; // zx - xz
            result[2] = this->matx[0][0] * v.matx[1][0] - this->matx[1][0] * v.matx[0][0]; // xy - yx
            return result;
        }
        
        // --- INVALID SPACES (1D, 4D, 5D, etc.) ---
        else
        {
            cout << RED << "\n[Math Error] Cross product is mathematically defined only for 2D and 3D vectors!\n" << RESET;
            return Vector<T>(0); // Return empty vector
        }
    }
};
int main()
{
    int r1, c1, r2, c2;
    char ch;

    cout << "\033[34m+=======================================+\033[0m\n";
    cout << "\033[34m|        MATRIX OPERATION SYSTEM        |\033[0m\n";
    cout << "\033[34m+=======================================+\033[0m\n";

    cout << "\n Enter size of MATRIX 1 (row,col) : ";
    cin >> r1 >> ch >> c1;

    cout << "\n Enter size of MATRIX 2 (row,col) : ";
    cin >> r2 >> ch >> c2;

    // Using 'double' automatically fixes your REF division truncation!
    Matrix<double> m1(r1, c1), m2(r2, c2);

    cout << "\n\n\033[36m=========== MATRIX 1 INPUT ===========\033[0m\n";
    cout << " Size: " << r1 << " x " << c1 << endl;
    cout << " Enter elements ROW-WISE\n";
    cout << " Example: 1 2 3 4 ...\n";
    cin >> m1;

    cout << "\n\n\033[36m=========== MATRIX 2 INPUT ===========\033[0m\n";
    cout << " Size: " << r2 << " x " << c2 << endl;
    cout << " Enter elements ROW-WISE\n";
    cout << " Example: 5 6 7 8 ...\n";
    cin >> m2;

    int choice;

    do
    {
        cout << "\n\n========= MENU =========\n";
        cout << "1. Addition\n";
        cout << "2. Subtraction\n";
        cout << "3. Scalar Multiplication\n";
        cout << "4. Matrix Multiplication\n";
        cout << "5. Transpose (m1/m2)\n";
        cout << "6. Row Echelon Form (m1/m2)\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            if (r1 != r2 || c1 != c2)
            {
                cout << RED << "Addition not possible (size mismatch)\n" << RESET;
                break;
            }
            Matrix<double> sum = m1 + m2;
            cout << "\n*********** ADDITION RESULT ***********";
            cout << sum;
            break;
        }

        case 2:
        {
            if (r1 != r2 || c1 != c2)
            {
                cout << RED << "Subtraction not possible (size mismatch)\n" << RESET;
                break;
            }
            Matrix<double> diff = m1 - m2;
            cout << "\n********* SUBTRACTION RESULT **********";
            cout << diff;
            break;
        }

        case 3:
        {
            double s;
            cout << "Enter Scalar: ";
            cin >> s;

            Matrix<double> mul1 = m1 * s;
            cout << "\nm1 * Scalar:";
            cout << mul1;

            Matrix<double> mul2 = s * m1;
            cout << "\nScalar * m1:";
            cout << mul2;
            break;
        }

        case 4:
        {
            if (c1 != r2)
            {
                cout << RED << "Multiplication not possible!\n" << RESET;
                break;
            }

            Matrix<double> mul = m1 * m2;
            cout << "\n***** MATRIX MULTIPLICATION RESULT *****";
            cout << mul;
            break;
        }

        case 5:
        {
            int tchoice;
            cout << "\nTranspose kis matrix ka karna hai?\n";
            cout << "1. Matrix 1\n";
            cout << "2. Matrix 2\n";
            cout << "Enter choice: ";
            cin >> tchoice;

            if (tchoice == 1)
            {
                Matrix<double> t = m1.transpose();
                cout << "\n******** TRANSPOSE OF MATRIX 1 ********";
                cout << t;
            }
            else if (tchoice == 2)
            {
                Matrix<double> t = m2.transpose();
                cout << "\n******** TRANSPOSE OF MATRIX 2 ********";
                cout << t;
            }
            else cout << "Invalid choice!\n";
            break;
        }

        case 6:
        {
            int rchoice;
            cout << "\nREF kis matrix ka nikalna hai?\n";
            cout << "1. Matrix 1\n";
            cout << "2. Matrix 2\n";
            cout << "Enter choice: ";
            cin >> rchoice;

            if (rchoice == 1)
            {
                Matrix<double> temp = m1; // Deep copy thanks to the new Assignment Operator!
                temp.ref();
                cout << "\n***** REF OF MATRIX 1 *****";
                cout << temp;
            }
            else if (rchoice == 2)
            {
                Matrix<double> temp = m2;
                temp.ref();
                cout << "\n***** REF OF MATRIX 2 *****";
                cout << temp;
            }
            else cout << "Invalid choice!\n";
            break;
        }

        case 7:
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 7);

    return 0;
}