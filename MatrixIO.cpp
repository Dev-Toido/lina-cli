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

        if (nrow > 0 && ncol > 0)
        {
            matx = new T *[nrow];
            for (int i = 0; i < nrow; i++)
            {
                matx[i] = new T[ncol]{0}; // Initialize to 0
            }
        }
        else
        {
            matx = nullptr;
        }
    }

    // COPY CONSTRUCTOR (Deep Copy)
    Matrix(const Matrix<T> &m)
    {
        nrow = m.nrow;
        ncol = m.ncol;

        if (nrow > 0 && ncol > 0)
        {
            matx = new T *[nrow];
            for (int i = 0; i < nrow; i++)
            {
                matx[i] = new T[ncol];
                for (int j = 0; j < ncol; j++)
                {
                    matx[i][j] = m.matx[i][j];
                }
            }
        }
        else
        {
            matx = nullptr;
        }
    }

    // COPY ASSIGNMENT OPERATOR (Crucial for avoiding crashes with temp matrices)
    Matrix<T> &operator=(const Matrix<T> &m)
    {
        if (this == &m)
            return *this; // Protect against self-assignment

        // 1. Delete old memory
        if (matx != nullptr)
        {
            for (int i = 0; i < nrow; i++)
                delete[] matx[i];
            delete[] matx;
        }

        // 2. Copy new data
        nrow = m.nrow;
        ncol = m.ncol;
        if (nrow > 0 && ncol > 0)
        {
            matx = new T *[nrow];
            for (int i = 0; i < nrow; i++)
            {
                matx[i] = new T[ncol];
                for (int j = 0; j < ncol; j++)
                {
                    matx[i][j] = m.matx[i][j];
                }
            }
        }
        else
        {
            matx = nullptr;
        }
        return *this;
    }

    // DESTRUCTOR
    ~Matrix()
    {
        if (matx != nullptr)
        {
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
        if (m.nrow == 0 || m.ncol == 0)
            return os; // Safeguard

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
                        T *tempRow = matx[i];
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
            if (i == excludeRow)
                continue;
            int c = 0;
            for (int j = 0; j < ncol; j++)
            {
                if (j == excludeCol)
                    continue;
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
        if (nrow != ncol)
        {
            cout << "Error: Determinant is only defined for square matrices!\n";
            return 0;
        }
        if (nrow == 1)
            return matx[0][0];
        if (nrow == 2)
            return (matx[0][0] * matx[1][1]) - (matx[0][1] * matx[1][0]);

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
    // GAUSSIAN ELIMINATION SOLVER (Ax = B)
    // Takes a Vector B of constants, returns a Vector x of solutions.
    Vector<double> solveSystem(const Vector<T> &B) const
    {
        if (nrow != ncol)
        {
            cout << RED << "Error: System must be square (n equations with n variables)!\n"
                 << RESET;
            return Vector<double>(0);
        }
        if (nrow != B.nrow)
        {
            cout << RED << "Error: Dimension mismatch between Matrix A and Vector B!\n"
                 << RESET;
            return Vector<double>(0);
        }

        // 1. Create an Augmented Matrix [A | B] using doubles to prevent division truncation
        int n = nrow;
        double **aug = new double *[n];
        for (int i = 0; i < n; i++)
        {
            aug[i] = new double[n + 1]; // +1 column for the B vector
            for (int j = 0; j < n; j++)
            {
                aug[i][j] = (double)matx[i][j];
            }
            aug[i][n] = (double)B[i]; // Append B to the end
        }

        // 2. Forward Elimination (Your REF logic adapted for the augmented matrix)
        for (int i = 0; i < n; i++)
        {
            // Find pivot
            if (std::abs(aug[i][i]) < 1e-9)
            {
                for (int k = i + 1; k < n; k++)
                {
                    if (std::abs(aug[k][i]) > 1e-9)
                    {
                        double *temp = aug[i];
                        aug[i] = aug[k];
                        aug[k] = temp;
                        break;
                    }
                }
            }

            // Check if singular (no unique solution)
            if (std::abs(aug[i][i]) < 1e-9)
            {
                cout << YELLOW << "Warning: Matrix is singular. System has no unique solution.\n"
                     << RESET;
                for (int r = 0; r < n; r++)
                    delete[] aug[r];
                delete[] aug;
                return Vector<double>(0);
            }

            // Eliminate
            for (int k = i + 1; k < n; k++)
            {
                double factor = aug[k][i] / aug[i][i];
                for (int j = i; j <= n; j++)
                { // Notice j <= n to include the augmented column
                    aug[k][j] -= factor * aug[i][j];
                }
            }
        }

        // 3. Back Substitution
        Vector<double> solution(n);
        for (int i = n - 1; i >= 0; i--)
        {
            solution[i] = aug[i][n]; // Start with the augmented column value
            for (int j = i + 1; j < n; j++)
            {
                solution[i] -= aug[i][j] * solution[j]; // Subtract the known variables
            }
            solution[i] /= aug[i][i]; // Divide by the pivot
        }

        // 4. Cleanup Memory
        for (int i = 0; i < n; i++)
            delete[] aug[i];
        delete[] aug;

        return solution;
    }
    // ==========================================
    // TRANSFORMATION MATRIX GENERATORS (2D Space)
    // ==========================================

    // Generates an Identity Matrix (1s on the diagonal, 0s elsewhere)
    static Matrix<T> Identity(int size)
    {
        Matrix<T> I(size, size);
        for (int i = 0; i < size; i++)
        {
            I.matx[i][i] = 1;
        }
        return I;
    }

    // SCALING MATRIX
    static Matrix<T> Scale2D(T scaleX, T scaleY)
    {
        Matrix<T> S = Identity(2);
        S.matx[0][0] = scaleX;
        S.matx[1][1] = scaleY;
        return S;
    }

    // REFLECTION MATRIX
    static Matrix<T> Reflect2D(bool acrossXAxis, bool acrossYAxis)
    {
        Matrix<T> R = Identity(2);
        if (acrossXAxis)
            R.matx[1][1] = -1; // Invert Y coordinates
        if (acrossYAxis)
            R.matx[0][0] = -1; // Invert X coordinates
        return R;
    }

    // ROTATION MATRIX (Counter-Clockwise)
    static Matrix<T> Rotate2D(double angleDegrees)
    {
        Matrix<T> R = Identity(2);
        const double PI = 3.14159265358979323846;
        double radians = angleDegrees * PI / 180.0;

        // 2D Rotation Formula:
        // [ cos(θ)  -sin(θ) ]
        // [ sin(θ)   cos(θ) ]
        R.matx[0][0] = (T)std::cos(radians);
        R.matx[0][1] = (T)-std::sin(radians);
        R.matx[1][0] = (T)std::sin(radians);
        R.matx[1][1] = (T)std::cos(radians);
        return R;
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
        if (this->ncol != 1 && this->nrow > 0)
        {
            cout << YELLOW << "Warning: Converting an N x M matrix into a vector!\n"
                 << RESET;
        }
    }
    int getrow() { return nrow; }
    T &operator[](int index) { return this->matx[index][0]; }
    T operator[](int index) const { return this->matx[index][0]; }

    // MAGNITUDE (Works in N-Dimensional Space)
    double magnitude() const
    {
        double sum = 0;
        for (int i = 0; i < this->nrow; ++i)
        {
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
            cout << RED << "\n[Math Error] Dot product failed: Vectors must be the exact same dimension!\n"
                 << RESET;
            return 0;
        }

        // 2. Compute N-Dimensional Dot Product
        T result = 0;
        for (int i = 0; i < this->nrow; ++i)
        {
            result += this->matx[i][0] * v.matx[i][0];
        }
        return result;
    }

    // CROSS PRODUCT (Strictly adapted for 2D and 3D Spaces)
    Vector<T> cross(const Vector<T> &v) const
    {
        if (this->nrow != v.nrow)
        {
            cout << RED << "\n[Math Error] Cross product failed: Vectors must be the same dimension!\n"
                 << RESET;
            return Vector<T>(0);
        }

        // --- 2D SPACE CROSS PRODUCT ---
        // Returns a 1D Vector (which acts as a single scalar value representing the Z-axis magnitude)
        if (this->nrow == 2)
        {
            Vector<T> result(1);
            result[0] = (this->matx[0][0] * v.matx[1][0]) - (this->matx[1][0] * v.matx[0][0]); // (x1*y2 - y1*x2)

            cout << CYAN << "\n[Note] 2D Cross Product computed. Returning 1D scalar vector (Z-magnitude).\n"
                 << RESET;
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
            cout << RED << "\n[Math Error] Cross product is mathematically defined only for 2D and 3D vectors!\n"
                 << RESET;
            return Vector<T>(0); // Return empty vector
        }
    }
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
    // Initialize pipeline with an Identity Matrix
    TransformPipeline(int dims) : dimensions(dims)
    {
        masterTransform = Matrix<T>::Identity(dims);
    }

    // Reset the pipeline
    void reset()
    {
        masterTransform = Matrix<T>::Identity(dimensions);
    }

    // View the current combined matrix
    void showPipelineMatrix() const
    {
        cout << "\n[ Current Master Transformation Matrix ]";
        cout << masterTransform;
    }

    // --- Add Transformations ---
    // Note: newTransform is placed on the LEFT of the multiplication!

    void addScale(T sx, T sy)
    {
        if (dimensions == 2)
        {
            masterTransform = Matrix<T>::Scale2D(sx, sy) * masterTransform;
        }
    }

    void addRotation(double degrees)
    {
        if (dimensions == 2)
        {
            masterTransform = Matrix<T>::Rotate2D(degrees) * masterTransform;
        }
    }

    void addReflection(bool acrossX, bool acrossY)
    {
        if (dimensions == 2)
        {
            masterTransform = Matrix<T>::Reflect2D(acrossX, acrossY) * masterTransform;
        }
    }

    // --- Execution ---
    // Applies the master matrix to a specific vector
    Vector<T> apply(const Vector<T> &targetVector) const
    {
        if (targetVector.nrow != dimensions)
        {
            cout << "\nError: Vector dimension does not match pipeline dimension!\n";
            return Vector<T>(0);
        }

        // Because Vector has a conversion constructor from Matrix,
        // this Matrix * Vector multiplication safely returns a Vector!
        return masterTransform * targetVector;
    }
};
int main()
{
    int r1, c1, r2, c2;
    char ch;

    cout << "\n\033[34m+=======================================================+\033[0m\n";
    cout << "\033[34m|\033[0m" << BOLD << CYAN << "             LINA-CLI: MATRIX OPERATION SYSTEM         " << RESET << "\033[34m|\033[0m\n";
    cout << "\033[34m+=======================================================+\033[0m\n";

    cout << "\n Enter size of MATRIX 1 (row,col) : ";
    cin >> r1 >> ch >> c1;

    cout << " Enter size of MATRIX 2 (row,col) : ";
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
        cout << "\n\n"
             << BOLD << "========= MAIN MENU =========" << RESET << "\n";
        cout << "1. Addition\n";
        cout << "2. Subtraction\n";
        cout << "3. Scalar Multiplication\n";
        cout << "4. Matrix Multiplication\n";
        cout << "5. Transpose (m1/m2)\n";
        cout << "6. Row Echelon Form (m1/m2)\n";
        cout << "7. Solve Linear Systems (Ax = B)\n";
        cout << "8. Matrix Transformation (2D Pipeline)\n";
        cout << RED << "9. Exit\n"
             << RESET;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            if (r1 != r2 || c1 != c2)
            {
                cout << RED << "Addition not possible (size mismatch)\n"
                     << RESET;
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
                cout << RED << "Subtraction not possible (size mismatch)\n"
                     << RESET;
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
                cout << RED << "Multiplication not possible! (m1 columns must equal m2 rows)\n"
                     << RESET;
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
            cout << "\nTranspose which matrix?\n";
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
            else
                cout << RED << "Invalid choice!\n"
                     << RESET;
            break;
        }

        case 6:
        {
            int rchoice;
            cout << "\nFind REF for which matrix?\n";
            cout << "1. Matrix 1\n";
            cout << "2. Matrix 2\n";
            cout << "Enter choice: ";
            cin >> rchoice;

            if (rchoice == 1)
            {
                Matrix<double> temp = m1;
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
            else
                cout << RED << "Invalid choice!\n"
                     << RESET;
            break;
        }

        case 7:
        {
            cout << "\n\033[35m=== SOLVE LINEAR SYSTEM (A*x = B) ===\033[0m\n";
            cout << "Matrix 1 (m1) will be used as the coefficient matrix 'A'.\n";

            Vector<double> B(r1);
            cout << "Enter the constants for Vector B (" << r1 << " elements):\n";
            cin >> B;

            Vector<double> solution = m1.solveSystem(B);

            if (solution.getrow() > 0)
            {
                cout << GREEN << "\n********** SOLUTION VECTOR (x) **********" << RESET;
                cout << solution;
            }
            break;
        }

        case 8:
        {
            cout << "\n\033[35m=== 2D TRANSFORMATION PIPELINE ===\033[0m\n";

            Vector<double> point(2);
            cout << "Enter the X coordinate: ";
            cin >> point[0];
            cout << "Enter the Y coordinate: ";
            cin >> point[1];

            TransformPipeline<double> pipe(2);
            int pipeChoice;

            cout << "\n"
                 << GREEN << "Target Vector created at (" << point[0] << ", " << point[1] << ")" << RESET << "\n";

            // Mini-menu for building the pipeline
            do
            {
                cout << "\n--- Add Transformation ---\n";
                cout << "1. Scale\n";
                cout << "2. Rotate\n";
                cout << "3. Reflect\n";
                cout << "4. APPLY PIPELINE & VIEW RESULT\n";
                cout << "Enter choice: ";
                cin >> pipeChoice;

                if (pipeChoice == 1)
                {
                    double sx, sy;
                    cout << "Enter Scale X factor: ";
                    cin >> sx;
                    cout << "Enter Scale Y factor: ";
                    cin >> sy;
                    pipe.addScale(sx, sy);
                    cout << YELLOW << "-> Scale added to pipeline.\n"
                         << RESET;
                }
                else if (pipeChoice == 2)
                {
                    double angle;
                    cout << "Enter Rotation Angle (degrees): ";
                    cin >> angle;
                    pipe.addRotation(angle);
                    cout << YELLOW << "-> Rotation added to pipeline.\n"
                         << RESET;
                }
                else if (pipeChoice == 3)
                {
                    int rx, ry;
                    cout << "Reflect across X-axis? (1 for Yes, 0 for No): ";
                    cin >> rx;
                    cout << "Reflect across Y-axis? (1 for Yes, 0 for No): ";
                    cin >> ry;
                    pipe.addReflection(rx == 1, ry == 1);
                    cout << YELLOW << "-> Reflection added to pipeline.\n"
                         << RESET;
                }
            } while (pipeChoice != 4);

            pipe.showPipelineMatrix();

            Vector<double> transformedPoint = pipe.apply(point);

            cout << GREEN << "\n********** FINAL TRANSFORMED COORDINATE **********" << RESET;
            cout << transformedPoint;
            break;
        }

        case 9:
            cout << CYAN << "\nExiting LINA-CLI... Goodbye!\n\n"
                 << RESET;
            break;

        default:
            cout << RED << "Invalid choice! Please select from the menu.\n"
                 << RESET;
        }

    } while (choice != 9);

    return 0;
}