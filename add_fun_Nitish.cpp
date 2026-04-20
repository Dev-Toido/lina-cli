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

    // Input
    friend istream &operator>>(istream &is, Matrix &m)
    {
        cout << "Enter elements row wise:\n";

        for (int i = 0; i < m.nrow; i++)
        {
            cout << "Enter " << i + 1 << " row : ";
            for (int j = 0; j < m.ncol; j++)
            {
                is >> m.matx[i][j];
            }
        }
        return is;
    }

    // Output
    friend ostream &operator<<(ostream &os, const Matrix &m)
    {
        cout << "_______________________" << endl;
        cout << "| Your Final Matrix   |" << endl;
        cout << "***********************" << endl;
        cout << endl;

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

    // 🔥 Addition
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

    //  Subtraction
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

    //  Scalar Multiplication
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

    // Destructor
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

    //  here Addition
    Matrix sum = m1 + m2;
    cout << "\nAddition Result:\n";
    cout << sum;

    //  here Subtraction
    Matrix diff = m1 - m2;
    cout << "\nSubtraction Result:\n";
    cout << diff;

    //  here Scalar Multiplication
    int s;
    cout << "\nEnter scalar value: ";
    cin >> s;

    Matrix scaled = m1 * s;
    cout << "\nScalar Multiplication Result:\n";
    cout << scaled;

    return 0;
}