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
    cin >> m1;
    cout << m1;

    return 0;
}