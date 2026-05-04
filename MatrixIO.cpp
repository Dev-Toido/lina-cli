#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <stdexcept> 
#include <vector> // <--- NEW: Standard Template Library Vector
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

// ==========================================
// TEMPLATE MATRIX CLASS (Powered by std::vector)
// ==========================================
template <typename T>
class Matrix
{
protected:
    int nrow, ncol;
    // The 2D grid is now managed entirely by the Standard Library
    std::vector<std::vector<T>> matx; 

public:
    // CONSTRUCTOR (Initializes the vector grid with 0s automatically)
    Matrix(int row = 0, int col = 0) : nrow(row), ncol(col)
    {
        if (nrow > 0 && ncol > 0) {
            matx.assign(nrow, std::vector<T>(ncol, 0));
        }
    }

    // Notice what is missing here! 
    // We deleted the Destructor, Copy Constructor, and Copy Assignment Operator. 
    // std::vector handles all of that deep-copying and memory deletion automatically!

    friend istream &operator>>(istream &is, Matrix<T> &m)
    {
        cout << "\n+-----------------------------------+\n";
        cout << "| \033[42m\033[30m ENTER ELEMENTS ROW WISE \033[0m         |\n";
        cout << "+-----------------------------------+\n";
        for (int i = 0; i < m.nrow; i++) {
            cout << " Row " << i + 1 << " : ";
            for (int j = 0; j < m.ncol; j++) is >> m.matx[i][j];
        }
        return is;
    }

    friend ostream &operator<<(ostream &os, const Matrix<T> &m)
    {
        if (m.nrow == 0 || m.ncol == 0) return os;
        int width = 8;
        int total = (m.ncol * width) + 2;
        if (total < 14) total = 14; 

        os << "\n+";
        for (int i = 0; i < total; i++) os << "-";
        os << "+\n|";
        
        int leftSpace = (total - 12) / 2;
        int rightSpace = total - 12 - leftSpace; 
        for (int i = 0; i < leftSpace; i++) os << " ";
        os << "FINAL MATRIX";
        for (int i = 0; i < rightSpace; i++) os << " ";
        os << "|\n+";
        
        for (int i = 0; i < total; i++) os << "-";
        os << "+\n";

        for (int i = 0; i < m.nrow; i++) {
            os << "| "; 
            for (int j = 0; j < m.ncol; j++) os << setw(width) << m.matx[i][j];
            int remainingSpace = total - (m.ncol * width) - 1;
            for (int j = 0; j < remainingSpace; j++) os << " ";
            os << "|\n";
        }
        os << "+";
        for (int i = 0; i < total; i++) os << "-";
        os << "+\n";
        return os;
    }

    Matrix<T> operator+(const Matrix<T> &m) const
    {
        if (nrow != m.nrow || ncol != m.ncol) 
            throw std::invalid_argument("Addition requires matrices of the exact same size!");
            
        Matrix<T> result(nrow, ncol);
        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++) result.matx[i][j] = matx[i][j] + m.matx[i][j];
        return result;
    }

    Matrix<T> operator-(const Matrix<T> &m) const
    {
        if (nrow != m.nrow || ncol != m.ncol) 
            throw std::invalid_argument("Subtraction requires matrices of the exact same size!");

        Matrix<T> result(nrow, ncol);
        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++) result.matx[i][j] = matx[i][j] - m.matx[i][j];
        return result;
    }

    Matrix<T> operator*(T scalar) const
    {
        Matrix<T> result(nrow, ncol);
        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++) result.matx[i][j] = matx[i][j] * scalar;
        return result;
    }

    friend Matrix<T> operator*(T scalar, const Matrix<T> &m)
    {
        Matrix<T> result(m.nrow, m.ncol);
        for (int i = 0; i < m.nrow; i++)
            for (int j = 0; j < m.ncol; j++) result.matx[i][j] = scalar * m.matx[i][j];
        return result;
    }

    Matrix<T> operator*(const Matrix<T> &m) const
    {
        if (ncol != m.nrow) 
            throw std::invalid_argument("Matrix Multiplication failed: Matrix A columns must equal Matrix B rows!");

        Matrix<T> result(nrow, m.ncol);
        for (int i = 0; i < nrow; i++) {
            for (int j = 0; j < m.ncol; j++) {
                result.matx[i][j] = 0;
                for (int k = 0; k < ncol; k++) result.matx[i][j] += matx[i][k] * m.matx[k][j];
            }
        }
        return result;
    }

    Matrix<T> transpose() const
    {
        Matrix<T> t(ncol, nrow);
        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++) t.matx[j][i] = matx[i][j];
        return t;
    }

    void ref()
    {
        int limit = (nrow < ncol) ? nrow : ncol;
        for (int i = 0; i < limit; i++) {
            T pivot = (matx[i][i] < 0) ? -matx[i][i] : matx[i][i];
            if (pivot < 1e-9) {
                for (int k = i + 1; k < nrow; k++) {
                    T check = (matx[k][i] < 0) ? -matx[k][i] : matx[k][i];
                    if (check > 1e-9) {
                        matx[i].swap(matx[k]); // Built-in std::vector high-speed swap!
                        break;
                    }
                }
            }
            pivot = (matx[i][i] < 0) ? -matx[i][i] : matx[i][i];
            if (pivot > 1e-9) {
                for (int k = i + 1; k < nrow; k++) {
                    T factor = matx[k][i] / matx[i][i];
                    for (int j = 0; j < ncol; j++) matx[k][j] = matx[k][j] - factor * matx[i][j];
                }
            }
        }
    }

    Matrix<T> getMinor(int excludeRow, int excludeCol) const
    {
        Matrix<T> minorMat(nrow - 1, ncol - 1);
        int r = 0;
        for (int i = 0; i < nrow; i++) {
            if (i == excludeRow) continue;
            int c = 0;
            for (int j = 0; j < ncol; j++) {
                if (j == excludeCol) continue;
                minorMat.matx[r][c] = this->matx[i][j];
                c++;
            }
            r++;
        }
        return minorMat;
    }

    T determinant() const
    {
        if (nrow != ncol) throw std::invalid_argument("Determinant is only defined for square matrices!");
        if (nrow == 1) return matx[0][0];
        if (nrow == 2) return (matx[0][0] * matx[1][1]) - (matx[0][1] * matx[1][0]);

        T det = 0;
        int sign = 1;
        for (int f = 0; f < ncol; f++) {
            Matrix<T> minorMat = getMinor(0, f);
            det += sign * matx[0][f] * minorMat.determinant();
            sign = -sign;
        }
        return det;
    }

    Matrix<double> solveSystem(const Matrix<T> &B) const
    {
        if (nrow != ncol) throw std::invalid_argument("System must be square (n equations with n variables)!");
        if (nrow != B.nrow) throw std::invalid_argument("Dimension mismatch between Coefficient Matrix A and Constants Vector B!");

        int n = nrow;
        // Using a 2D Vector for the Augmented Matrix
        std::vector<std::vector<double>> aug(n, std::vector<double>(n + 1));
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) aug[i][j] = (double)matx[i][j];
            aug[i][n] = (double)B.matx[i][0]; 
        }

        for (int i = 0; i < n; i++) {
            if (std::abs(aug[i][i]) < 1e-9) {
                for (int k = i + 1; k < n; k++) {
                    if (std::abs(aug[k][i]) > 1e-9) {
                        aug[i].swap(aug[k]); // Vector swap
                        break;
                    }
                }
            }

            if (std::abs(aug[i][i]) < 1e-9) {
                throw std::runtime_error("Matrix is singular! System has no unique solution (Infinite or No Solutions).");
            }

            for (int k = i + 1; k < n; k++) {
                double factor = aug[k][i] / aug[i][i];
                for (int j = i; j <= n; j++) aug[k][j] -= factor * aug[i][j];
            }
        }

        Matrix<double> solution(n, 1);
        for (int i = n - 1; i >= 0; i--) {
            solution.matx[i][0] = aug[i][n]; 
            for (int j = i + 1; j < n; j++) solution.matx[i][0] -= aug[i][j] * solution.matx[j][0];
            solution.matx[i][0] /= aug[i][i];
        }

        return solution;
    }

    static Matrix<T> Identity(int size)
    {
        Matrix<T> I(size, size);
        for (int i = 0; i < size; i++) I.matx[i][i] = 1;
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
        if (acrossXAxis) R.matx[1][1] = -1; 
        if (acrossYAxis) R.matx[0][0] = -1; 
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
};

// ==========================================
// TEMPLATE VECTOR CLASS
// ==========================================
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

    int getrow() const { return this->nrow; } 
    T &operator[](int index) { return this->matx[index][0]; }
    T operator[](int index) const { return this->matx[index][0]; }

    double magnitude() const
    {
        double sum = 0;
        for (int i = 0; i < this->nrow; ++i) sum += this->matx[i][0] * this->matx[i][0];
        return std::sqrt(sum);
    }

    T dot(const Vector<T> &v) const
    {
        if (this->nrow != v.nrow) throw std::invalid_argument("Dot product failed: Vectors must be the exact same dimension!");
        T result = 0;
        for (int i = 0; i < this->nrow; ++i) result += this->matx[i][0] * v.matx[i][0];
        return result;
    }

    Vector<T> cross(const Vector<T> &v) const
    {
        if (this->nrow != v.nrow) throw std::invalid_argument("Cross product failed: Vectors must be the same dimension!");
        if (this->nrow == 2) {
            Vector<T> result(1);
            result[0] = (this->matx[0][0] * v.matx[1][0]) - (this->matx[1][0] * v.matx[0][0]);
            return result;
        }
        else if (this->nrow == 3) {
            Vector<T> result(3);
            result[0] = this->matx[1][0] * v.matx[2][0] - this->matx[2][0] * v.matx[1][0];
            result[1] = this->matx[2][0] * v.matx[0][0] - this->matx[0][0] * v.matx[2][0];
            result[2] = this->matx[0][0] * v.matx[1][0] - this->matx[1][0] * v.matx[0][0];
            return result;
        }
        else {
            throw std::invalid_argument("Cross product is mathematically defined only for 2D and 3D vectors!");
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
    TransformPipeline(int dims) : dimensions(dims) { masterTransform = Matrix<T>::Identity(dims); }
    void reset() { masterTransform = Matrix<T>::Identity(dimensions); }
    void showPipelineMatrix() const
    {
        cout << "\n[ Current Master Transformation Matrix ]";
        cout << masterTransform;
    }

    void addScale(T sx, T sy) { if (dimensions == 2) masterTransform = Matrix<T>::Scale2D(sx, sy) * masterTransform; }
    void addRotation(double degrees) { if (dimensions == 2) masterTransform = Matrix<T>::Rotate2D(degrees) * masterTransform; }
    void addReflection(bool acrossX, bool acrossY) { if (dimensions == 2) masterTransform = Matrix<T>::Reflect2D(acrossX, acrossY) * masterTransform; }

    Vector<T> apply(const Vector<T> &targetVector) const
    {
        if (targetVector.getrow() != dimensions) throw std::invalid_argument("Vector dimension does not match pipeline dimension!");
        return masterTransform * targetVector;
    }
};

// ==========================================
// FILE HANDLING UTILITY
// ==========================================
template <typename T>
void promptSave(const Matrix<T>& m, const string& operationName)
{
    char choice;
    cout << "\nDo you want to save this result to a file? (Y/N): ";
    cin >> choice;

    if (choice == 'Y' || choice == 'y') {
        string filename;
        cout << "Enter filename (e.g., output.txt) or type 'log' to use default history: ";
        cin >> filename;
        if (filename == "log") filename = "lina_history.txt";

        ofstream outFile(filename, ios::app); 
        if (outFile.is_open()) {
            outFile << "\n=== " << operationName << " ===\n";
            outFile << m; 
            outFile << "\n";
            outFile.close();
            cout << GREEN << "[Success] Result successfully appended to " << filename << "!\n" << RESET;
        } else {
            cout << RED << "[Error] Could not open or create the file.\n" << RESET;
        }
    }
}

// ==========================================
// MAIN FUNCTION (Production CLI Flow)
// ==========================================
int main()
{
    char ch; 
    int choice;

    cout << "\n\033[34m+=======================================================+\033[0m\n";
    cout << "\033[34m|\033[0m" << BOLD << CYAN << "             LINA-CLI: MATRIX OPERATION SYSTEM         " << RESET << "\033[34m|\033[0m\n";
    cout << "\033[34m+=======================================================+\033[0m\n";

    do
    {
        cout << "\n\n" << BOLD << "========= MAIN MENU =========" << RESET << "\n";
        cout << "1. Addition\n";
        cout << "2. Subtraction\n";
        cout << "3. Scalar Multiplication\n";
        cout << "4. Matrix Multiplication\n";
        cout << "5. Transpose\n";
        cout << "6. Row Echelon Form (REF)\n";
        cout << "7. Solve Linear Systems (Ax = B)\n";
        cout << "8. Matrix Transformation (2D Pipeline)\n";
        cout << RED << "9. Exit\n" << RESET;
        cout << "Enter your preferred operation: ";
        cin >> choice;

        if (choice == 9) {
            cout << CYAN << "\nExiting LINA-CLI... Goodbye!\n\n" << RESET;
            break;
        }

        try 
        {
            switch (choice)
            {
            case 1: // ADDITION
            {
                int r1, c1, r2, c2;
                cout << "\n\033[36m=== ADDITION ===\033[0m\n";
                cout << "Size of Matrix A (row,col): "; cin >> r1 >> ch >> c1;
                cout << "Size of Matrix B (row,col): "; cin >> r2 >> ch >> c2;

                Matrix<double> A(r1, c1), B(r2, c2);
                cout << "\nInput Matrix A:"; cin >> A;
                cout << "\nInput Matrix B:"; cin >> B;
                
                Matrix<double> sum = A + B; 
                cout << GREEN << "\n*********** ADDITION RESULT ***********" << RESET;
                cout << sum;
                promptSave(sum, "Addition Result");
                break;
            }

            case 2: // SUBTRACTION
            {
                int r1, c1, r2, c2;
                cout << "\n\033[36m=== SUBTRACTION ===\033[0m\n";
                cout << "Size of Matrix A (row,col): "; cin >> r1 >> ch >> c1;
                cout << "Size of Matrix B (row,col): "; cin >> r2 >> ch >> c2;

                Matrix<double> A(r1, c1), B(r2, c2);
                cout << "\nInput Matrix A:"; cin >> A;
                cout << "\nInput Matrix B:"; cin >> B;
                
                Matrix<double> diff = A - B; 
                cout << GREEN << "\n********* SUBTRACTION RESULT **********" << RESET;
                cout << diff;
                promptSave(diff, "Subtraction Result");
                break;
            }

            case 3: // SCALAR MULTIPLICATION
            {
                int r1, c1;
                double s;
                cout << "\n\033[36m=== SCALAR MULTIPLICATION ===\033[0m\n";
                cout << "Size of Matrix A (row,col): "; cin >> r1 >> ch >> c1;
                
                Matrix<double> A(r1, c1);
                cout << "\nInput Matrix A:"; cin >> A;
                cout << "\nEnter Scalar multiplier: "; cin >> s;

                Matrix<double> result = A * s;
                cout << GREEN << "\n*********** SCALAR RESULT ***********" << RESET;
                cout << result;
                promptSave(result, "Scalar Multiplication Result");
                break;
            }

            case 4: // MATRIX MULTIPLICATION
            {
                int r1, c1, r2, c2;
                cout << "\n\033[36m=== MATRIX MULTIPLICATION ===\033[0m\n";
                cout << "Size of Matrix A (row,col): "; cin >> r1 >> ch >> c1;
                cout << "Size of Matrix B (row,col): "; cin >> r2 >> ch >> c2;

                Matrix<double> A(r1, c1), B(r2, c2);
                cout << "\nInput Matrix A:"; cin >> A;
                cout << "\nInput Matrix B:"; cin >> B;
                
                Matrix<double> product = A * B; 
                cout << GREEN << "\n***** MATRIX MULTIPLICATION RESULT *****" << RESET;
                cout << product;
                promptSave(product, "Matrix Multiplication Result");
                break;
            }

            case 5: // TRANSPOSE
            {
                int r1, c1;
                cout << "\n\033[36m=== TRANSPOSE ===\033[0m\n";
                cout << "Size of Matrix (row,col): "; cin >> r1 >> ch >> c1;
                
                Matrix<double> A(r1, c1);
                cout << "\nInput Matrix:"; cin >> A;
                
                Matrix<double> t = A.transpose();
                cout << GREEN << "\n******** TRANSPOSE RESULT ********" << RESET;
                cout << t;
                promptSave(t, "Transposed Matrix");
                break;
            }

            case 6: // ROW ECHELON FORM
            {
                int r1, c1;
                cout << "\n\033[36m=== ROW ECHELON FORM (REF) ===\033[0m\n";
                cout << "Size of Matrix (row,col): "; cin >> r1 >> ch >> c1;
                
                Matrix<double> A(r1, c1);
                cout << "\nInput Matrix:"; cin >> A;
                
                A.ref(); 
                cout << GREEN << "\n********** REF RESULT **********" << RESET;
                cout << A;
                promptSave(A, "Row Echelon Form");
                break;
            }

            case 7: // SOLVE SYSTEM
            {
                int n;
                cout << "\n\033[35m=== SOLVE LINEAR SYSTEM (A*x = B) ===\033[0m\n";
                cout << "Note: The coefficient matrix 'A' must be square.\n";
                cout << "Enter the number of variables/equations (n): "; cin >> n;

                Matrix<double> A(n, n);
                Vector<double> B(n);

                cout << "\nInput Coefficient Matrix A (" << n << "x" << n << "):"; cin >> A;
                cout << "\nInput Constants Vector B (" << n << " elements):"; cin >> B;

                Vector<double> solution = A.solveSystem(B); 

                cout << GREEN << "\n********** SOLUTION VECTOR (x) **********" << RESET;
                cout << solution;
                promptSave(solution, "Linear System Solution (x)");
                break;
            }

            case 8: // TRANSFORMATION PIPELINE
            {
                cout << "\n\033[35m=== 2D TRANSFORMATION PIPELINE ===\033[0m\n";

                Vector<double> point(2);
                cout << "Enter the X coordinate: "; cin >> point[0];
                cout << "Enter the Y coordinate: "; cin >> point[1];

                TransformPipeline<double> pipe(2);
                int pipeChoice;

                cout << GREEN << "\nTarget Vector created at (" << point[0] << ", " << point[1] << ")" << RESET << "\n";

                do {
                    cout << "\n--- Add Transformation ---\n";
                    cout << "1. Scale\n";
                    cout << "2. Rotate\n";
                    cout << "3. Reflect\n";
                    cout << "4. APPLY PIPELINE & VIEW RESULT\n";
                    cout << "Enter choice: ";
                    cin >> pipeChoice;

                    if (pipeChoice == 1) {
                        double sx, sy;
                        cout << "Enter Scale X factor: "; cin >> sx;
                        cout << "Enter Scale Y factor: "; cin >> sy;
                        pipe.addScale(sx, sy);
                        cout << YELLOW << "-> Scale added to pipeline.\n" << RESET;
                    }
                    else if (pipeChoice == 2) {
                        double angle;
                        cout << "Enter Rotation Angle (degrees): "; cin >> angle;
                        pipe.addRotation(angle);
                        cout << YELLOW << "-> Rotation added to pipeline.\n" << RESET;
                    }
                    else if (pipeChoice == 3) {
                        int rx, ry;
                        cout << "Reflect across X-axis? (1 for Yes, 0 for No): "; cin >> rx;
                        cout << "Reflect across Y-axis? (1 for Yes, 0 for No): "; cin >> ry;
                        pipe.addReflection(rx == 1, ry == 1);
                        cout << YELLOW << "-> Reflection added to pipeline.\n" << RESET;
                    }
                } while (pipeChoice != 4);

                pipe.showPipelineMatrix();

                Vector<double> transformedPoint = pipe.apply(point);

                cout << GREEN << "\n********** FINAL TRANSFORMED COORDINATE **********" << RESET;
                cout << transformedPoint;
                promptSave(transformedPoint, "Pipeline Transformed Coordinate");
                break;
            }

            default:
                cout << RED << "\n[Error] Invalid choice! Please select from the menu.\n" << RESET;
            }
        }
        catch (const std::exception& e) 
        {
            cout << RED << "\n[Fatal Math Error] " << e.what() << RESET << "\n";
            cout << YELLOW << "-> Operation safely aborted. Returning to Main Menu...\n" << RESET;
        }

    } while (choice != 9);

    return 0;
}