<div align="center">

<!-- Animated Typing SVG Banner -->
<a href="https://git.io/typing-svg">
  <img src="https://readme-typing-svg.demolab.com?font=Fira+Code&weight=700&size=28&duration=3000&pause=1000&color=00F7FF&center=true&vCenter=true&multiline=true&repeat=true&width=850&height=110&lines=CodeAlpha+C%2B%2B+Programming+Internship;4+Production-Ready+Projects+with+Full+Error+Handling" alt="Typing SVG" />
</a>

<!-- Animated Wave Divider -->
<img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/aqua.png" width="100%" />

<p align="center">
  <img src="https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
  <img src="https://img.shields.io/badge/Internship-CodeAlpha-00C853?style=for-the-badge&logo=checkmarx&logoColor=white" />
  <img src="https://img.shields.io/badge/Error_Handling-100%25_Validated-FF6D00?style=for-the-badge&logo=shield&logoColor=white" />
  <img src="https://img.shields.io/badge/Status-Completed-8E24AA?style=for-the-badge" />
</p>

<p align="center">
  <b>Clean Architecture • Defensive Input Validation • Modern STL • Zero-Crash Execution</b>
</p>

</div>

---

## Overview

This repository contains complete, fault-tolerant **C++17** implementations developed for the **CodeAlpha Software Development Internship**. Every module is engineered with strict stream recovery, boundary validation, exception handling, and modular design.

---

## Featured Projects

| # | Project Name | Core Concepts & Algorithms | Key Error-Handling Highlights |
| :-: | :--- | :--- | :--- |
| **01** |CGPA Calculator| `STL Vectors`, `Formatting`, `Weighted Averages` | Recovers from non-numeric `std::cin` corruption, validates credit/grade ranges, prevents division by zero. |
| **02** |Login & Registration| `Salted Hashing`, `File I/O`, `String Parsing` | Enforces password complexity, blocks duplicate usernames, handles missing/corrupted DB files, locks brute-force attempts. |
| **03** |Sudoku Solver| `Recursive Backtracking`, `Constraint Satisfaction` | Validates initial 9×9 board conflicts prior to recursion, catches out-of-range inputs, detects unsolvable boards. |
| **04** |Banking System| `OOP Encapsulation`, `Exception Handling`, `RAII` | Prevents negative deposits, blocks overdrafts, guarantees atomic account-to-account transfers, logs timestamps. |

<img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/aqua.png" width="100%" />

## Repository Structure

```text
CodeAlpha_CPP_Internship/
├──CGPA_Calculator/
│   └──CGPA_Calculator.cpp              # Multi-semester GPA & CGPA engine
├──Login_Registration/
│   ├──Login_Registration.cpp           # Salted authentication & file persistence
│   └── users_db.txt                    # Auto-generated user credential store
├──Sudoku_Solver/
│   └──Sudoku_Solver.cpp                # 9x9 recursive backtracking solver
├── Banking_System/
│   └──Banking_System.cpp               # Customer, Account & Transaction OOP suite
└── README.md                           # Project documentation
