# 🚀 LINA-CLI: The Engineering Journey
> **Project Scope:** A memory-safe, command-line Linear Algebra and 2D Graphics engine built in C++.
> **Status:** Version 1.5 (Stable)

## 📍 Phase 1: The Foundation (v0.1 - v0.9)
**The Objective:** Build a functional mathematical calculator that handles basic arithmetic and standard operations without relying on massive external libraries like NumPy or Eigen.
*   **Milestones Achieved:** 
    *   Engineered the core matrix structures using `double` arrays.
    *   Implemented standard matrix addition, subtraction, and multiplication with strict dimension validation.
    *   Developed the Transposition algorithm.
*   **The Problem Faced:** **Memory Management.** Initially, using raw pointers and arrays meant the system was vulnerable to memory leaks if a matrix was overwritten or destroyed improperly.
*   **The Breakthrough:** Completely refactored the underlying architecture to use C++ `std::vector`. This embraced RAII (Resource Acquisition Is Initialization) principles, guaranteeing zero memory leaks and making the codebase significantly safer.

## 📍 Phase 2: Production Readiness & Algorithms (v1.0)
**The Objective:** Transform a basic calculator into a robust, crash-proof engine capable of solving complex systems and visualizing data.
*   **Milestones Achieved:**
    *   Programmed the **Row Echelon Form (REF)** and **Gaussian Elimination** algorithms to solve $Ax = B$.
    *   Built the **2D Transformation Pipeline**, allowing users to chain Scale, Rotate, and Reflect matrices into a single Master Matrix for graphics/physics applications.
    *   Integrated **Persistent File Logging**, automatically saving beautifully formatted ASCII matrix outputs to a `.txt` file.
*   **The Problem Faced:** **The "Floating-Point Ghost" & Input Crashes.** 
    1.  *Math Bug:* Computers store decimals imperfectly. When subtracting rows in REF, perfect zeros were returning as infinitesimal garbage data (e.g., `-2.22045e-016`), ruining downstream math. 
    2.  *Input Bug:* If a user accidentally typed a letter instead of a number, the `cin` stream failed, causing an infinite, terminal-crashing loop.
*   **The Breakthrough:** 
    1.  Implemented a mathematical "snap-to-zero" threshold (`1e-9`) to instantly crush floating-point dust into absolute zeros.
    2.  Wrapped the entire main loop in a global `try-catch` net and activated `cin.exceptions(ios_base::failbit)`. This allowed the engine to flush bad input characters from the memory buffer and safely redraw the menu instead of crashing.

## 📍 Phase 3: The Academic Expansion (v1.5 - Current)
**The Objective:** Align the engine perfectly with the university's advanced Linear Algebra syllabus, building specific tools for numerical analysis.
*   **Milestones Achieved:**
    *   **Rank Calculation:** Leveraged the REF algorithm to dynamically determine the true rank of any matrix.
    *   **Gauss-Jordan Inverse:** Engineered an augmented matrix solver $[A | I]$ to accurately calculate true inverse matrices.
    *   **Determinant via Laplace Expansion:** Built a recursive algorithm using sub-matrix minors to calculate determinants.
    *   **Linear Independence:** Automated the verification of vector spaces using Rank-Nullity logic.
    *   Expanded the 2D Pipeline with **Shear** and **Projection** matrices.
*   **The Problem Faced:** **Inheritance & Type Casting.** When returning data from the pipeline, the compiler threw fatal errors because it did not know how to seamlessly convert a standard `Matrix` object down into a child `Vector` object.
*   **The Breakthrough:** Re-architected the `Vector` class to include explicit Conversion Constructors, allowing the C++ compiler to safely downcast $N \times 1$ matrices into specialized vectors.

---

## 🔭 The Horizon: Looking Toward v2.0
The immediate goal for LINA-CLI is to transition from an exact calculator to an iterative numerical analysis engine.

*   **Eigenvalues & Eigenvectors:** Standard formulas cannot find these for large matrices. v2.0 will introduce the **Power Iteration Method**, using looping algorithms to mathematically converge on dominant Eigenvectors.
*   **Diagonalization & The Spectral Theorem:** Once Eigen-functions are stable, the engine will be able to automatically factor matrices into $PDP^{-1}$.
*   **Data Ingestion (File I/O):** Upgrading the file handling to parse massive $N \times N$ matrices directly from `.csv` or `.json` files, allowing LINA-CLI to interact with real-world Machine Learning datasets.
