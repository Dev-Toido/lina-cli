#include <iostream>
#include <iomanip>
using namespace std;

class Matrix
{
private:
    int nrow, ncol;
    int **matx;

public:

    // CONSTRUCTOR
    Matrix(int row, int col)
    {
        nrow = row;
        ncol = col;

        matx = new int *[nrow];
        for (int i = 0; i < nrow; i++)
            matx[i] = new int[ncol];
    }
// COPY CONSTRUCTOR (Deep Copy - Silent Fix)
Matrix(const Matrix &m)
{
    nrow = m.nrow;
    ncol = m.ncol;

    matx = new int*[nrow];
    for (int i = 0; i < nrow; i++)
    {
        matx[i] = new int[ncol];
        for (int j = 0; j < ncol; j++)
        {
            matx[i][j] = m.matx[i][j];
        }
    }
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
        cout << "  |\n";

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

    // 🔥 MATRIX MULTIPLICATION
    Matrix operator*(const Matrix &m)
    {
        Matrix result(nrow, m.ncol);

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

    // 🔥 TRANSPOSE
    Matrix transpose()
    {
        Matrix t(ncol, nrow);

        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++)
                t.matx[j][i] = matx[i][j];

        return t;
    }

    // 🔥 ROW ECHELON FORM
    void REF()
    {
        for (int i = 0; i < nrow; i++)
        {
            if (matx[i][i] == 0)
            {
                for (int k = i + 1; k < nrow; k++)
                {
                    if (matx[k][i] != 0)
                    {
                        swap(matx[i], matx[k]);
                        break;
                    }
                }
            }

            for (int k = i + 1; k < nrow; k++)
            {
                float factor = (float)matx[k][i] / matx[i][i];
                for (int j = 0; j < ncol; j++)
                {
                    matx[k][j] = matx[k][j] - factor * matx[i][j];
                }
            }
        }
    }

    // DESTRUCTOR
    ~Matrix()
    {
        for (int i = 0; i < nrow; i++)
            delete[] matx[i];

        delete[] matx;
    }
};

// 🔥 VECTOR CLASS (INHERITANCE)
class Vector : public Matrix
{
public:
    Vector(int n) : Matrix(n, 1) {}
};

int main()
{
    int r1, c1, r2, c2;
    char ch;

    cout << "\033[34m+=======================================+\033[0m\n";
    cout << "\033[34m|        MATRIX OPERATION SYSTEM        |\033[0m\n";
    cout << "\033[34m+=======================================+\033[0m\n";

    // ✅ Separate size input
    cout << "\n Enter size of MATRIX 1 (row,col) : ";
    cin >> r1 >> ch >> c1;

    cout << "\n Enter size of MATRIX 2 (row,col) : ";
    cin >> r2 >> ch >> c2;

    Matrix m1(r1, c1), m2(r2, c2);

    // ✅ Better instructions
    cout << "\n\n\033[36m=========== MATRIX 1 INPUT ===========\033[0m\n";
    cout << "👉 Size: " << r1 << " x " << c1 << endl;
    cout << "👉 Enter elements ROW-WISE\n";
    cout << "👉 Example: 1 2 3 4 ...\n";
    cin >> m1;

    cout << "\n\n\033[36m=========== MATRIX 2 INPUT ===========\033[0m\n";
    cout << "👉 Size: " << r2 << " x " << c2 << endl;
    cout << "👉 Enter elements ROW-WISE\n";
    cout << "👉 Example: 5 6 7 8 ...\n";
    cin >> m2;

    int choice;

    do
    {
        cout << "\n\n========= MENU =========\n";
        cout << "1. Addition\n";
        cout << "2. Subtraction\n";
        cout << "3. Scalar Multiplication\n";
        cout << "4. Matrix Multiplication\n";
        cout << "5. Transpose (m1)\n";
        cout << "6. Row Echelon Form (m1)\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            if (r1 != r2 || c1 != c2)
            {
                cout << "Addition not possible (size mismatch)\n";
                break;
            }
            Matrix sum = m1 + m2;
            cout << "\n*********** ADDITION RESULT ***********";
            cout << sum;
            break;
        }

        case 2:
        {
            if (r1 != r2 || c1 != c2)
            {
                cout << "Subtraction not possible (size mismatch)\n";
                break;
            }
            Matrix diff = m1 - m2;
            cout << "\n********* SUBTRACTION RESULT **********";
            cout << diff;
            break;
        }

        case 3:
        {
            int s;
            cout << "Enter Scalar: ";
            cin >> s;

            Matrix mul1 = m1 * s;
            cout << "\nm1 * Scalar:";
            cout << mul1;

            Matrix mul2 = s * m1;
            cout << "\nScalar * m1:";
            cout << mul2;
            break;
        }

        case 4:
        {
            // ✅ FIXED CONDITION
            if (c1 != r2)
            {
                cout << "Multiplication not possible!\n";
                break;
            }

            Matrix mul = m1 * m2;
            cout << "\n***** MATRIX MULTIPLICATION RESULT *****";
            cout << mul;
            break;
        }

        
        case 5:{
            
    int tchoice;
    cout << "\nTranspose kis matrix ka karna hai?\n";
    cout << "1. Matrix 1\n";
    cout << "2. Matrix 2\n";
    cout << "Enter choice: ";
    cin >> tchoice;

    if (tchoice == 1)
    {
        Matrix t = m1.transpose();
        cout << "\n******** TRANSPOSE OF MATRIX 1 ********";
        cout << t;
    }
    else if (tchoice == 2)
    {
        Matrix t = m2.transpose();
        cout << "\n******** TRANSPOSE OF MATRIX 2 ********";
        cout << t;
    }
    else
    {
        cout << "Invalid choice!\n";
    }

    break;
}

        case 6:
        {
            Matrix temp = m1; // now safe (copy constructor used)
            temp.REF();
            cout << "\n***** ROW ECHELON FORM *****";
            cout << temp;
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