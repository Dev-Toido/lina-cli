# LINA-CLI: Academic Matrix Operation System 🧮

LINA-CLI (v1.5) is a professional-grade, interactive Command-Line Interface designed for high-performance linear algebra operations. Originally built as a foundational tool for AI and Machine Learning computation, it has been expanded into a comprehensive academic engine covering Matrix Algebra, Vector Spaces, and Linear Transformations.

## 🚀 Version 1.5 Features

### 1. Matrix Algebra & Systems
* **Core Operations:** Matrix Addition, Subtraction, and Matrix/Scalar Multiplication.
* **Gauss-Jordan Inverse:** Calculates the true inverse ($A^{-1}$) of any square matrix using algorithmic augmented matrix reduction.
* **Row Echelon Form (REF):** Algorithmic reduction for matrix simplification.
* **Gaussian Elimination:** Solves complex systems of linear equations ($Ax = B$) dynamically.
* **Transposition:** Instantly transpose any $N \times M$ matrix.

### 2. Vector Spaces & Properties
* **Rank Calculation:** Programmatically determines the rank of a matrix to reveal the true dimension of datasets.
* **Linear Independence Verification:** Analyzes sets of $R^n$ vectors via Rank-Nullity logic to determine if they are linearly independent or dependent.

### 3. 2D Transformation Pipeline (Graphics/Physics)
A dedicated pipeline to chain geometric transformations into a single Master Matrix, applied to coordinate vectors:
* **Scale** (Expansion/Contraction)
* **Rotate**
* **Reflect** (Across X or Y axes)
* **Shear**
* **Projection** (Collapse to X or Y axes)

## 🛠️ Production-Grade Architecture

* **Memory Safety:** Built entirely on C++ `std::vector`, ensuring zero memory leaks and utilizing strict RAII principles (no manual `new`/`delete` pointers).
* **Bulletproof Input Handling:** Custom `std::cin` flushing prevents infinite loops and crashes from bad user inputs (e.g., typing chars instead of ints).
* **Exception Handling:** Robust `try-catch` architecture intercepts fatal math errors (division by zero, singular matrices, dimension mismatches) and safely aborts operations.
* **Data Persistence:** Integrated File I/O automatically appends complex calculations and ASCII-formatted results to `lina_history.txt`, with an in-app log viewer for continuous tracking.

## ⚙️ Compilation & Installation

LINA-CLI requires a standard C++11 (or higher) compiler (like GCC/G++).

1. Clone the repository:
   ```bash
   git clone [https://github.com/yourusername/lina-cli.git](https://github.com/yourusername/lina-cli.git)
   cd lina-cli
   ```
2. Compile the source code:
   
```bash
   g++ MatrixIO.cpp -o lina_cli
   ```
3. Run the engine:
   * **Windows:** `.\lina_cli.exe`
   * **Mac/Linux:** `./lina_cli`

## 💻 Usage Example

Upon launching, the interactive menu guides you through operations with fail-fast validation. 
```text
========= MAIN MENU =========
1. Matrix Arithmetics (+, -, *)
2. Transpose Matrix
3. Advanced Properties (Inverse, Rank, Independence)
4. Row Echelon Form (REF)
5. Solve Linear Systems (Ax = B)
6. Matrix Transformation (2D Pipeline)
7. View Operation Log
8. Exit
```

## 🤝 Contributing
Contributions, issues, and feature requests are welcome! Ideal future implementations include File Input Integration (reading `.csv` matrices) and Eigenvalue/Eigenvector calculators.