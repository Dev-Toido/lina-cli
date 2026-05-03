#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

class Matrix
{
private:
    int nrow, ncol;
    int **matx;

public:
    // 1. UPDATED CONSTRUCTOR: Handles normal matrices and the (0, 0) empty matrix
    Matrix(int row = 0, int col = 0)
    {
        if (row <= 0 || col <= 0)
        {
            nrow = 0;
            ncol = 0;
            matx = nullptr; // Crucial: point to nothing safely
        }
        else
        {
            nrow = row;
            ncol = col;
            matx = new int *[row];
            for (int i = 0; i < row; i++)
            {
                matx[i] = new int[col]{0}; // Initialize with 0s to be safe
            }
        }
    }

    // 2. NEW COPY CONSTRUCTOR: Prevents double-free crashes
    Matrix(const Matrix &other)
    {
        nrow = other.nrow;
        ncol = other.ncol;
        if (nrow > 0 && ncol > 0)
        {
            matx = new int *[nrow];
            for (int i = 0; i < nrow; ++i)
            {
                matx[i] = new int[ncol];
                for (int j = 0; j < ncol; ++j)
                {
                    matx[i][j] = other.matx[i][j];
                }
            }
        }
        else
        {
            matx = nullptr;
        }
    }

    // 3. NEW COPY ASSIGNMENT OPERATOR: Prevents memory leaks on assignment
    Matrix& operator=(const Matrix &other)
    {
        if (this == &other) return *this; // Protect against self-assignment

        // Free existing memory first
        if (matx != nullptr)
        {
            for (int i = 0; i < nrow; ++i)
                delete[] matx[i];
            delete[] matx;
        }

        // Copy new data
        nrow = other.nrow;
        ncol = other.ncol;
        if (nrow > 0 && ncol > 0)
        {
            matx = new int *[nrow];
            for (int i = 0; i < nrow; ++i)
            {
                matx[i] = new int[ncol];
                for (int j = 0; j < ncol; ++j)
                {
                    matx[i][j] = other.matx[i][j];
                }
            }
        }
        else
        {
            matx = nullptr;
        }
        return *this;
    }

    // INPUT
    friend istream &operator>>(istream &is, Matrix &m)
    {
        cout << "\n+-----------------------------------+\n";
        cout << "|   ENTER ELEMENTS ROW WISE         |\n";
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
    friend ostream &operator<<(ostream &os, const Matrix &m)
    {
        // If it's an empty matrix (e.g., failed multiplication), print a warning
        if (m.nrow == 0 || m.ncol == 0) {
            os << "[Empty Matrix / Invalid Operation]\n";
            return os;
        }

        int width = 8;
        int total = m.ncol * width + 3;

        os << "+--" << setfill(' ') << setw(total - 4) << "" << "--+" << endl << setfill(' ');

        for (int i = 0; i < m.nrow; i++)
        {
            os << "|";
            for (int j = 0; j < m.ncol; j++)
            {
                os << setw(width) << m.matx[i][j];
            }
            os << "   |" << endl;
        }

        os << "+--" << setfill(' ') << setw(total - 4) << "" << "--+" << endl << setfill(' ');
        return os;
    }

    // Addition
    Matrix operator+(const Matrix &m) const
    {
        if (nrow != m.nrow || ncol != m.ncol)
        {
            cout << "Matrix addition not possible!\n";
            return Matrix(0, 0); // Return empty matrix on failure
        }
        Matrix result(nrow, ncol);
        for (int i = 0; i < nrow; i++)
        {
            for (int j = 0; j < ncol; j++)
            {
                result.matx[i][j] = matx[i][j] + m.matx[i][j];
            }
        }
        return result;
    }

    // Subtraction
    Matrix operator-(const Matrix &m) const
    {
        if (nrow != m.nrow || ncol != m.ncol)
        {
            cout << "Matrix subtraction not possible!\n";
            return Matrix(0, 0); // Return empty matrix on failure
        }
        Matrix result(nrow, ncol);
        for (int i = 0; i < nrow; i++)
        {
            for (int j = 0; j < ncol; j++)
            {
                result.matx[i][j] = matx[i][j] - m.matx[i][j];
            }
        }
        return result;
    }

    // Scalar Multiplication (m1 * s)
    Matrix operator*(int scalar) const
    {
        Matrix result(nrow, ncol);
        for (int i = 0; i < nrow; i++)
        {
            for (int j = 0; j < ncol; j++)
            {
                result.matx[i][j] = matx[i][j] * scalar;
            }
        }
        return result;
    }

    // Scalar Multiplication (s * m1)
    friend Matrix operator*(int scalar, const Matrix &m)
    {
        Matrix result(m.nrow, m.ncol);
        for (int i = 0; i < m.nrow; i++)
        {
            for (int j = 0; j < m.ncol; j++)
            {
                result.matx[i][j] = m.matx[i][j] * scalar;
            }
        }
        return result;
    }

    // Multiplication of two matrices
    Matrix operator*(const Matrix &mat) const
    {
        if (this->ncol == mat.nrow)
        {
            int n = this->nrow, m = this->ncol, p = mat.ncol;
            Matrix result(n, p);
            
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < p; j++)
                {
                    int ele = 0;
                    for (int k = 0; k < m; k++)
                    {
                        ele += this->matx[i][k] * mat.matx[k][j];
                    }
                    result.matx[i][j] = ele;
                }
            }
            return result;
        }
        else
        {
            return Matrix(0, 0); 
        }
    }

    // 4. UPDATED DESTRUCTOR: Safely checks for nullptrs
    ~Matrix()
    {
        if (matx != nullptr) 
        {
            for (int i = 0; i < nrow; ++i)
            {
                delete[] matx[i];
            }
            delete[] matx;
        }
    }
};
int main()
{
    int a, b;
    char buf;

    cout << "Enter no of Row and Column for your first matrix in(a,b) form: ";
    cin >> a >> buf >> b;

    Matrix m1(a, b);

    cout << "Enter no of Row and Column for your second matrix in(a,b) form: ";
    cin >> a >> buf >> b;

    Matrix m2(a, b);

    cout << "\nMatrix 1:\n";
    cin >> m1;

    cout << "\nMatrix 2:\n";
    cin >> m2;

    // Addition
    Matrix sum = m1 + m2;
    cout << "\nAddition Result:\n";
    cout << sum;

    // Subtraction
    Matrix diff = m1 - m2;
    cout << "\nSubtraction Result:\n";
    cout << diff;

    // Scalar Multiplication
    int s;
    cout << "\nEnter scalar value: ";
    cin >> s;

    Matrix scaled1 = m1 * s;
    cout << "\nResult (m1 * s):\n";
    cout << scaled1;

    Matrix scaled2 = s * m1;
    cout << "\nResult (s * m1):\n";
    cout << scaled2;

    Matrix product = m1 * m2;
    cout << "The product of two matrix: " << endl;
    cout << product << endl;
    return 0;
}