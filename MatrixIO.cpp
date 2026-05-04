/**
 * LINA-CLI: Matrix Operation System (v1.0)
 * A professional-grade, memory-safe C++ Linear Algebra engine.
 * Features: Auto-logging, Exception Handling, std::vector architecture, and 2D Graphics Transformations.
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <limits>

using namespace std;

// --- ANSI COLOR CODES ---
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
// TEMPLATE MATRIX CLASS
// ==========================================
template <typename T>
class Matrix
{
protected:
    int nrow, ncol;
    std::vector<std::vector<T>> matx;

public:
    // CONSTRUCTOR: Initializes a vector grid with 0s.
    Matrix(int row = 0, int col = 0) : nrow(row), ncol(col)
    {
        if (nrow > 0 && ncol > 0)
        {
            matx.assign(nrow, std::vector<T>(ncol, 0));
        }
    }

    // OVERLOADED INPUT (>>): Formatted CLI input
    friend istream &operator>>(istream &is, Matrix<T> &m)
    {
        cout << "\n+-----------------------------------+\n";
        cout << "| \033[42m\033[30m ENTER ELEMENTS ROW WISE \033[0m         |\n";
        cout << "+-----------------------------------+\n";
        for (int i = 0; i < m.nrow; i++)
        {
            // FIX 4: Explicitly states how many elements are required per row
            cout << " Row " << i + 1 << " (" << m.ncol << " elements) : ";
            for (int j = 0; j < m.ncol; j++)
                is >> m.matx[i][j];
        }
        return is;
    }

    // OVERLOADED OUTPUT (<<): Dynamic ASCII Box rendering
    friend ostream &operator<<(ostream &os, const Matrix<T> &m)
    {
        if (m.nrow == 0 || m.ncol == 0)
            return os;
        int width = 8;
        int total = (m.ncol * width) + 2;
        if (total < 14)
            total = 14;

        os << "\n+";
        for (int i = 0; i < total; i++)
            os << "-";
        os << "+\n|";

        int leftSpace = (total - 12) / 2;
        int rightSpace = total - 12 - leftSpace;
        for (int i = 0; i < leftSpace; i++)
            os << " ";
        os << "FINAL MATRIX";
        for (int i = 0; i < rightSpace; i++)
            os << " ";
        os << "|\n+";

        for (int i = 0; i < total; i++)
            os << "-";
        os << "+\n";

        for (int i = 0; i < m.nrow; i++)
        {
            os << "| ";
            for (int j = 0; j < m.ncol; j++)
                os << setw(width) << m.matx[i][j];
            int remainingSpace = total - (m.ncol * width) - 1;
            for (int j = 0; j < remainingSpace; j++)
                os << " ";
            os << "|\n";
        }
        os << "+";
        for (int i = 0; i < total; i++)
            os << "-";
        os << "+\n";
        return os;
    }

    // ADDITION
    Matrix<T> operator+(const Matrix<T> &m) const
    {
        if (nrow != m.nrow || ncol != m.ncol)
            throw std::invalid_argument("Addition requires matrices of the exact same size!");

        Matrix<T> result(nrow, ncol);
        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++)
                result.matx[i][j] = matx[i][j] + m.matx[i][j];
        return result;
    }

    // SUBTRACTION
    Matrix<T> operator-(const Matrix<T> &m) const
    {
        if (nrow != m.nrow || ncol != m.ncol)
            throw std::invalid_argument("Subtraction requires matrices of the exact same size!");

        Matrix<T> result(nrow, ncol);
        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++)
                result.matx[i][j] = matx[i][j] - m.matx[i][j];
        return result;
    }

    // SCALAR MULTIPLICATION
    Matrix<T> operator*(T scalar) const
    {
        Matrix<T> result(nrow, ncol);
        for (int i = 0; i < nrow; i++)
            for (int j = 0; j < ncol; j++)
                result.matx[i][j] = matx[i][j] * scalar;
        return result;
    }

    // MATRIX MULTIPLICATION
    Matrix<T> operator*(const Matrix<T> &m) const
    {
        if (ncol != m.nrow)
            throw std::invalid_argument("Matrix Multiplication failed: Matrix A columns must equal Matrix B rows!");

        Matrix<T> result(nrow, m.ncol);
        for (int i = 0; i < nrow; i++)
        {
            for (int j = 0; j < m.ncol; j++)
            {
                result.matx[i][j] = 0;
                for (int k = 0; k < ncol; k++)
                    result.matx[i][j] += matx[i][k] * m.matx[k][j];
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

    // ROW ECHELON FORM (REF)
    void ref()
    {
        int limit = (nrow < ncol) ? nrow : ncol;
        for (int i = 0; i < limit; i++)
        {
            T pivot = (matx[i][i] < 0) ? -matx[i][i] : matx[i][i];
            if (pivot < 1e-9)
            {
                for (int k = i + 1; k < nrow; k++)
                {
                    T check = (matx[k][i] < 0) ? -matx[k][i] : matx[k][i];
                    if (check > 1e-9)
                    {
                        matx[i].swap(matx[k]);
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
                        // --- NEW: Snap floating-point dust to absolute zero ---
                        if (std::abs(matx[k][j]) < 1e-9)
                        {
                            matx[k][j] = 0;
                        }
                    }
                }
            }
        }
    }

    // GAUSSIAN ELIMINATION (Ax = B)
    Matrix<double> solveSystem(const Matrix<T> &B) const
    {
        if (nrow != ncol)
            throw std::invalid_argument("System must be square (n equations with n variables)!");
        if (nrow != B.nrow)
            throw std::invalid_argument("Dimension mismatch between Coefficient Matrix A and Constants Vector B!");

        int n = nrow;
        std::vector<std::vector<double>> aug(n, std::vector<double>(n + 1));

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                aug[i][j] = (double)matx[i][j];
            aug[i][n] = (double)B.matx[i][0];
        }

        for (int i = 0; i < n; i++)
        {
            if (std::abs(aug[i][i]) < 1e-9)
            {
                for (int k = i + 1; k < n; k++)
                {
                    if (std::abs(aug[k][i]) > 1e-9)
                    {
                        aug[i].swap(aug[k]);
                        break;
                    }
                }
            }
            if (std::abs(aug[i][i]) < 1e-9)
            {
                throw std::runtime_error("Matrix is singular! System has no unique solution (Infinite or No Solutions).");
            }
            for (int k = i + 1; k < n; k++)
            {
                double factor = aug[k][i] / aug[i][i];
                for (int j = i; j <= n; j++)
                    aug[k][j] -= factor * aug[i][j];
            }
        }

        Matrix<double> solution(n, 1);
        for (int i = n - 1; i >= 0; i--)
        {
            solution.matx[i][0] = aug[i][n];
            for (int j = i + 1; j < n; j++)
                solution.matx[i][0] -= aug[i][j] * solution.matx[j][0];
            solution.matx[i][0] /= aug[i][i];
        }

        return solution;
    }

    // STATIC TRANSFORMATION HELPERS
    static Matrix<T> Identity(int size)
    {
        Matrix<T> I(size, size);
        for (int i = 0; i < size; i++)
            I.matx[i][i] = 1;
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
        if (acrossXAxis)
            R.matx[1][1] = -1;
        if (acrossYAxis)
            R.matx[0][0] = -1;
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
// TEMPLATE VECTOR CLASS (Inherits Matrix)
// ==========================================
template <typename T>
class Vector : public Matrix<T>
{
public:
    Vector(int size = 0) : Matrix<T>(size, 1) {}
    Vector(const Matrix<T> &m) : Matrix<T>(m)
    {
        if (this->ncol != 1 && this->nrow > 0)
            cout << YELLOW << "Warning: Converting N x M matrix to vector!\n"
                 << RESET;
    }

    int getrow() const { return this->nrow; }
    T &operator[](int index) { return this->matx[index][0]; }
    T operator[](int index) const { return this->matx[index][0]; }
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
    void showPipelineMatrix() const { cout << "\n[ Current Master Transformation Matrix ]" << masterTransform; }
    void addScale(T sx, T sy)
    {
        if (dimensions == 2)
            masterTransform = Matrix<T>::Scale2D(sx, sy) * masterTransform;
    }
    void addRotation(double degrees)
    {
        if (dimensions == 2)
            masterTransform = Matrix<T>::Rotate2D(degrees) * masterTransform;
    }
    void addReflection(bool acrossX, bool acrossY)
    {
        if (dimensions == 2)
            masterTransform = Matrix<T>::Reflect2D(acrossX, acrossY) * masterTransform;
    }

    Vector<T> apply(const Vector<T> &targetVector) const
    {
        if (targetVector.getrow() != dimensions)
            throw std::invalid_argument("Vector dimension mismatch!");
        return masterTransform * targetVector;
    }
};

// ==========================================
// FILE HANDLING UTILITIES
// ==========================================
template <typename T>
void autoLogAndPromptSave(const Matrix<T> &m, const string &operationName)
{
    // FIX 2: Automatic silent logging to standard history file
    ofstream logFile("lina_history.txt", ios::app);
    if (logFile.is_open())
    {
        logFile << "\n=== " << operationName << " ===\n";
        logFile << m << "\n";
        logFile.close();
        cout << GREEN << "\n[Auto-Saved to lina_history.txt]\n"
             << RESET;
    }

    // FIX 2: Prompt for custom absolute/relative path
    char choice;
    cout << "Save a copy to a custom path? (Y/N): ";
    cin >> choice;

    if (choice == 'Y' || choice == 'y')
    {
        string path;
        cout << "Enter path (e.g., C:\\output.txt or ./res.txt): ";
        cin >> path;

        ofstream customFile(path, ios::app);
        if (customFile.is_open())
        {
            customFile << "\n=== " << operationName << " ===\n"
                       << m << "\n";
            customFile.close();
            cout << GREEN << "[Success] Result copied to " << path << "!\n"
                 << RESET;
        }
        else
        {
            cout << RED << "[Error] Could not access or create file at that path.\n"
                 << RESET;
        }
    }
}

// FIX 5: Utility to read the log back into the terminal
void viewLog()
{
    ifstream file("lina_history.txt");
    if (!file.is_open())
    {
        cout << YELLOW << "\n[Notice] No history log found yet. Do some math first!\n"
             << RESET;
        return;
    }

    cout << "\n\033[35m" << BOLD << "=== LINA-CLI OPERATION LOG ===" << RESET << "\n";
    string line;
    while (getline(file, line))
    {
        cout << line << "\n";
    }
    cout << "\033[35m" << BOLD << "==============================" << RESET << "\n";
    file.close();
}

// ==========================================
// MAIN CLI LOOP
// ==========================================
int main()
{
    // Enable cin to throw exceptions on wrong data types (e.g., letters instead of numbers)
    cin.exceptions(ios_base::failbit); 

    int choice = 0; // Initialize choice to 0

    cout << "\n\033[34m+=======================================================+\033[0m\n";
    cout << "\033[34m|\033[0m" << BOLD << CYAN << "             LINA-CLI: MATRIX OPERATION SYSTEM         " << RESET << "\033[34m|\033[0m\n";
    cout << "\033[34m+=======================================================+\033[0m\n";

    do
    {
        // --- NEW: The try block now protects the ENTIRE loop, including the menu selection! ---
        try 
        {
            cout << "\n\n" << BOLD << "========= MAIN MENU =========" << RESET << "\n";
            cout << "1. Matrix Arithmetics (+, -, *)\n";
            cout << "2. Transpose Matrix\n";
            cout << "3. Row Echelon Form (REF)\n";
            cout << "4. Solve Linear Systems (Ax = B)\n";
            cout << "5. Matrix Transformation (2D Pipeline)\n";
            cout << "6. View Operation Log\n";
            cout << RED << "7. Exit\n" << RESET;
            cout << "Enter your preferred operation: ";
            
            cin >> choice; // If user types a letter here, it instantly jumps to the catch block!

            if (choice == 7) {
                cout << CYAN << "\nExiting LINA-CLI... Goodbye!\n\n" << RESET;
                break;
            }

            switch (choice)
            {
            case 1: // MATRIX ARITHMETICS SUB-MENU
            {
                int arithChoice;
                cout << "\n" << BOLD << "--- Matrix Arithmetics ---" << RESET << "\n";
                cout << "1. Addition\n2. Subtraction\n3. Multiplication\n";
                cout << "Select arithmetic operation: ";
                cin >> arithChoice;

                if (arithChoice == 1 || arithChoice == 2) 
                {
                    int r1, c1, r2, c2;
                    cout << "Size of Matrix A (rows cols): "; cin >> r1 >> c1;
                    cout << "Size of Matrix B (rows cols): "; cin >> r2 >> c2;

                    if (r1 != r2 || c1 != c2) {
                        throw std::invalid_argument("Addition/Subtraction requires matrices of the exact same size!");
                    }

                    Matrix<double> A(r1, c1), B(r2, c2);
                    cout << "\nInput Matrix A:"; cin >> A;
                    cout << "\nInput Matrix B:"; cin >> B;
                    
                    if (arithChoice == 1) {
                        Matrix<double> sum = A + B; 
                        cout << GREEN << "\n*********** ADDITION RESULT ***********" << RESET;
                        cout << sum;
                        autoLogAndPromptSave(sum, "Addition Result");
                    } else {
                        Matrix<double> diff = A - B; 
                        cout << GREEN << "\n********* SUBTRACTION RESULT **********" << RESET;
                        cout << diff;
                        autoLogAndPromptSave(diff, "Subtraction Result");
                    }
                }
                else if (arithChoice == 3) // MULTIPLICATION SUB-MENU
                {
                    int multChoice;
                    cout << "\nMultiply by:\n1. Scalar (Single Number)\n2. Another Matrix\nSelect option: ";
                    cin >> multChoice;

                    if (multChoice == 1) {
                        int r1, c1;
                        double s;
                        cout << "Size of Matrix A (rows cols): "; cin >> r1 >> c1;
                        
                        Matrix<double> A(r1, c1);
                        cout << "\nInput Matrix A:"; cin >> A;
                        cout << "Enter Scalar multiplier: "; cin >> s;

                        Matrix<double> result = A * s;
                        cout << GREEN << "\n*********** SCALAR RESULT ***********" << RESET;
                        cout << result;
                        autoLogAndPromptSave(result, "Scalar Multiplication");
                    } 
                    else if (multChoice == 2) {
                        int r1, c1, r2, c2;
                        cout << "Size of Matrix A (rows cols): "; cin >> r1 >> c1;
                        cout << "Size of Matrix B (rows cols): "; cin >> r2 >> c2;

                        if (c1 != r2) {
                            throw std::invalid_argument("Matrix A columns must equal Matrix B rows!");
                        }

                        Matrix<double> A(r1, c1), B(r2, c2);
                        cout << "\nInput Matrix A:"; cin >> A;
                        cout << "\nInput Matrix B:"; cin >> B;
                        
                        Matrix<double> product = A * B; 
                        cout << GREEN << "\n***** MATRIX MULTIPLICATION RESULT *****" << RESET;
                        cout << product;
                        autoLogAndPromptSave(product, "Matrix Multiplication");
                    }
                }
                break;
            }

            case 2: // TRANSPOSE
            {
                int r1, c1;
                cout << "\n\033[36m=== TRANSPOSE ===\033[0m\n";
                cout << "Size of Matrix (rows cols): "; cin >> r1 >> c1;
                
                Matrix<double> A(r1, c1);
                cout << "\nInput Matrix:"; cin >> A;
                
                Matrix<double> t = A.transpose();
                cout << GREEN << "\n******** TRANSPOSE RESULT ********" << RESET;
                cout << t;
                autoLogAndPromptSave(t, "Transposed Matrix");
                break;
            }

            case 3: // ROW ECHELON FORM
            {
                int r1, c1;
                cout << "\n\033[36m=== ROW ECHELON FORM (REF) ===\033[0m\n";
                cout << "Size of Matrix (rows cols): "; cin >> r1 >> c1;
                
                Matrix<double> A(r1, c1);
                cout << "\nInput Matrix:"; cin >> A;
                
                A.ref(); 
                cout << GREEN << "\n********** REF RESULT **********" << RESET;
                cout << A;
                autoLogAndPromptSave(A, "Row Echelon Form");
                break;
            }

            case 4: // SOLVE SYSTEM
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
                autoLogAndPromptSave(solution, "Linear System Solution (x)");
                break;
            }

            case 5: // TRANSFORMATION PIPELINE
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
                    cout << "1. Scale\n2. Rotate\n3. Reflect\n4. APPLY PIPELINE & VIEW RESULT\n";
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
                autoLogAndPromptSave(transformedPoint, "Pipeline Transformed Coordinate");
                break;
            }

            case 6: // VIEW OPERATION LOG
                viewLog();
                break;

            default:
                cout << RED << "\n[Error] Invalid choice! Please select from the menu.\n" << RESET;
            }
        }
        // --- CATCHING THE EXCEPTIONS ---
        catch (const std::exception& e) 
        {
            if (cin.fail()) {
                // If the user typed letters instead of numbers
                cout << RED << "\n[Input Error] Invalid input detected! Please enter numbers only.\n" << RESET;
                cin.clear(); // Clear the "panic" state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Flush out the bad letters
                choice = 0; // Reset choice so the menu safely loops again
            } 
            else {
                // If it was a math error
                cout << RED << "\n[Fatal Math Error] " << e.what() << RESET << "\n";
            }
            
            cout << YELLOW << "-> Operation safely aborted. Returning to Main Menu...\n" << RESET;
        }

    } while (choice != 7);

    return 0;
}