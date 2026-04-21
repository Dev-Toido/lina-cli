#include <iostream>
using namespace std;

class Matrix
{
private:
    int nrow, ncol;
    int **matx;

public:
    Matrix(int row, int col)
    {
        nrow = row, ncol = col;
        matx = new int *[row];
        for (int i = 0; i < row; i++)
        {
            matx[i] = new int[col];
        }
    }
    friend istream &operator>>(istream &is, Matrix &m)
    {
        cout << "Enter elements row wise:\n";

        for (int i = 0; i < m.nrow; i++)
        {
            cout << "Enter " << i + 1 << "st row : ";
            for (int j = 0; j < m.ncol; j++)
            {
                is >> m.matx[i][j];
            }
        }
        return is;
    }
    friend ostream &operator<<(ostream &os, const Matrix &m)
    {
        cout << "_______________________" << endl;
        cout << "| Your Final Matrix   |" << endl;
        cout << "***********************" << endl;
        cout << "" << endl;

        for (int i = 0; i < m.nrow; i++)
        {
            os << "  | ";
            for (int j = 0; j < m.ncol; j++)
            {
                os << m.matx[i][j] << " ";
            }
            os << "|\n";
        }

        return os;
    }
    // Addition
    Matrix operator+(const Matrix &m)
    {
        if (nrow != m.nrow || ncol != m.ncol)
        {
            cout << "Matrix addition not possible!\n";
            exit(0);
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
    Matrix operator-(const Matrix &m)
    {
        if (nrow != m.nrow || ncol != m.ncol)
        {
            cout << "Matrix subtraction not possible!\n";
            exit(0);
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
    Matrix operator*(int scalar)
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
    ~Matrix()
    {
        for (int i = 0; i < nrow; ++i)
        {
            delete[] matx[i];
        }
        delete[] matx;
    }
};

int main()
{
    int a, b;
    char buf;

    cout << "Enter no of Row and Column for your matrix in(a,b) form: ";
    cin >> a >> buf >> b;

    Matrix m1(a, b);
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

    return 0;
}