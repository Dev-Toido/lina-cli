# LINA-CLI: Matrix Operation System 🧮

LINA-CLI is a professional-grade, interactive Command-Line Interface designed for high-performance linear algebra operations. Built as a foundational tool for AI and Machine Learning computation, it features memory-safe architecture, persistent file logging, and advanced matrix transformations.

## 🚀 Features

### Core Mathematical Engine
* **Standard Operations:** Matrix Addition, Subtraction, and Scalar Multiplication.
* **Matrix Multiplication:** Strict validation for N x M and M x P dot products.
* **Transposition:** Instantly transpose any N x M matrix.
* **Template Optimization:** Fully templated `Matrix` and `Vector` classes seamlessly support floating-point (`double`) calculations to prevent integer truncation during complex division.

### Advanced AI/ML & Physics Algorithms
* **Gaussian Elimination:** Solve complex systems of linear equations (Ax = B) dynamically.
* **Row Echelon Form (REF):** Algorithmic reduction for rank identification and matrix simplification.
* **2D Transformation Pipeline:** A dedicated graphics/physics engine pipeline to chain Scale, Rotate, and Reflect transformations into a single Master Matrix, applied to coordinate vectors.

### Production-Grade Architecture
* **Memory Safety:** Built entirely on C++ `std::vector`, ensuring zero memory leaks and utilizing RAII principles (no manual `new`/`delete` pointers).
* **Exception Handling:** Robust `try-catch` architecture intercepts fatal math errors (e.g., division by zero, singular matrices, dimension mismatches) and safely aborts operations without crashing the engine.
* **Data Persistence:** Integrated File I/O allows users to append complex calculations and ASCII-formatted results to `lina_history.txt` for continuous logging and external software interoperability.

## 🛠️ Compilation & Installation

LINA-CLI requires a standard C++ compiler (like GCC/G++).

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
1. Addition
2. Subtraction
3. Scalar Multiplication
4. Matrix Multiplication
5. Transpose
6. Row Echelon Form (REF)
7. Solve Linear Systems (Ax = B)
8. Matrix Transformation (2D Pipeline)
9. Exit
```

**File Logging:** After any calculation, LINA-CLI will prompt:
`Do you want to save this result to a file? (Y/N):`
Typing `log` automatically routes the beautifully formatted ASCII matrix output directly to your continuous history file.

## 🗺️ Roadmap (Future Implementations)
* **File Input Integration:** Read large-scale N x N weight/bias matrices directly from `.csv` or `.json` files for ML algorithm feeding.
* **3D Transformation Pipeline:** Expand the geometric pipeline to handle 3D space (X, Y, Z) and homogenous coordinates for translation matrices.

## 🤝 Contributing
Contributions, issues, and feature requests are welcome!