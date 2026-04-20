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
    friend std::istream &operator>>(std::istream &is, Matrix &m)
    {
        for (int i = 0; i < m.nrow; ++i)
        {
            for (int j = 0; j < m.ncol; ++j)
            {
                cout << "Element (" << i + 1 << " * " << j + 1 << ") : ";
                is >> m.matx[i][j]; // Read into matrix element
            }
        }
        return is; // Return stream for chaining (e.g., cin >> m1 >> m2;)
    }
    friend std::ostream &operator<<(std::ostream &os, const Matrix &m)
    {
        for (int i = 0; i < m.nrow; ++i)
        {
            for (int j = 0; j < m.ncol; ++j)
            {
                os << m.matx[i][j] << "\t"; // Tab separated output
            }
            os << "\n"; // Newline after each row
        }
        return os; // Return stream for chaining (e.g., cout << m1 << m2;)
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
    int a;
    int b;
    cout << "Enter no of Row and Colume for your matrix in(a b)form : ";
    cin >> a >> b;

    Matrix m1(a, b);
    cout << "Enter the elements of the array: " << endl;
    cin >> m1;
    cout << m1;

    return 0;
}