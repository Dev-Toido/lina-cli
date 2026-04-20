#include <iostream>
#include <iomanip>
using namespace std;

class Matrix
{
private:
    int nrow, ncol;
    int **matx;

public:
    Matrix(int row, int col)
    {
        nrow = row;
        ncol = col;

        matx = new int *[nrow];
        for (int i = 0; i < nrow; i++)
            matx[i] = new int[ncol];
    }

    // INPUT
    friend istream &operator>>(istream &is, Matrix &m)
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
    friend ostream &operator<<(ostream &os, const Matrix &m)
    {
        int width = 8;
        int total = m.ncol * width + 3;

        cout << "\n+";
        for (int i = 0; i < total; i++)
            cout << "-";
        cout << "+\n";

        cout << "|";
        int titleSpace = (total - 12) / 2;
        for (int i = 0; i < titleSpace; i++)
            cout << " ";
        cout << "FINAL MATRIX";
        for (int i = 0; i < titleSpace - 1; i++)
            cout << " ";
        cout << "|\n";

        cout << "+";
        for (int i = 0; i < total; i++)
            cout << "-";
        cout << "+\n";

        for (int i = 0; i < m.nrow; i++)
        {
            cout << "|";
            for (int j = 0; j < m.ncol; j++)
                cout << setw(width) << m.matx[i][j];
            cout << "   |\n";
        }

        cout << "+";
        for (int i = 0; i < total; i++)
            cout << "-";
        cout << "+\n";

        return os;
    }

    // ADDITION
    Matrix operator+(const Matrix &m)
    {
        Matrix result(nrow, ncol);

        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++)
                result.matx[i][j] = matx[i][j] + m.matx[i][j];

        return result;
    }

    // SUBTRACTION
    Matrix operator-(const Matrix &m)
    {
        Matrix result(nrow, ncol);

        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++)
                result.matx[i][j] = matx[i][j] - m.matx[i][j];

        return result;
    }

    // m1 * scalar
    Matrix operator*(int scalar)
    {
        Matrix result(nrow, ncol);

        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++)
                result.matx[i][j] = matx[i][j] * scalar;

        return result;
    }

    // scalar * m1
    friend Matrix operator*(int scalar, const Matrix &m)
    {
        Matrix result(m.nrow, m.ncol);

        for (int i = 0; i < m.nrow; i++)
            for (int j = 0; j < m.ncol; j++)
                result.matx[i][j] = scalar * m.matx[i][j];

        return result;
    }

    ~Matrix()
    {
        for (int i = 0; i < nrow; i++)
            delete[] matx[i];

        delete[] matx;
    }
};

int main()
{
    int a, b;
    char ch;

    cout << "\033[34m+=======================================+\033[0m\n";
    cout << "\033[34m|        MATRIX OPERATION SYSTEM        |\033[0m\n";
    cout << "\033[34m+=======================================+\033[0m\n";

    cout << "\n Enter Number of Rows and Columns in (a,b) form : ";
    cin >> a >> ch >> b;

    Matrix m1(a, b), m2(a, b);

    cout << "\n=========== MATRIX 1 INPUT ===========";
    cin >> m1;

    cout << "\n=========== MATRIX 2 INPUT ===========";
    cin >> m2;

    Matrix sum = m1 + m2;
    cout << "\n\n*********** ADDITION RESULT ***********";
    cout << sum;

    Matrix diff = m1 - m2;
    cout << "\n\n********* SUBTRACTION RESULT **********";
    cout << diff;

    int s;
    cout << "\n Enter Scalar Value : ";
    cin >> s;

    Matrix mul1 = m1 * s;
    cout << "\n\n_________ m1 * Scalar __________";
    cout << mul1;

    Matrix mul2 = s * m1;
    cout << "\n___________ Scalar * m1 _____________";
    cout << mul2;

    cout << "\n+=======================================+\n";
    cout << "|         PROGRAM EXECUTED DONE        |\n";
    cout << "+=======================================+\n";

    return 0;
}