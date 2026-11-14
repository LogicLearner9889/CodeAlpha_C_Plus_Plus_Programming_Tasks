#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>

constexpr int N = 9;

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printBoard(const int grid[N][N]) {
    std::cout << "\n+-------+-------+-------+\n";
    for (int row = 0; row < N; ++row) {
        std::cout << "| ";
        for (int col = 0; col < N; ++col) {
            if (grid[row][col] == 0)
                std::cout << ". ";
            else
                std::cout << grid[row][col] << " ";

            if ((col + 1) % 3 == 0) std::cout << "| ";
        }
        std::cout << "\n";
        if ((row + 1) % 3 == 0) {
            std::cout << "+-------+-------+-------+\n";
        }
    }
}

// Check if placing 'num' at grid[row][col] obeys Sudoku constraints
bool isSafe(const int grid[N][N], int row, int col, int num) {
    // Check row & column
    for (int i = 0; i < N; ++i) {
        if (grid[row][i] == num || grid[i][col] == num) {
            return false;
        }
    }

    // Check 3x3 subgrid
    int startRow = row - (row % 3);
    int startCol = col - (col % 3);
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (grid[startRow + r][startCol + c] == num) {
                return false;
            }
        }
    }
    return true;
}

// Verify the initial board has no pre-existing rule violations
bool isValidInitialBoard(int grid[N][N], std::string& errorDetail) {
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            int val = grid[r][c];
            if (val < 0 || val > 9) {
                errorDetail = "Cell (" + std::to_string(r + 1) + "," + std::to_string(c + 1) + ") contains out-of-range value.";
                return false;
            }
            if (val != 0) {
                grid[r][c] = 0; // Temporarily clear to check against neighbors
                if (!isSafe(grid, r, c, val)) {
                    grid[r][c] = val;
                    errorDetail = "Conflict detected at row " + std::to_string(r + 1) +
                                  ", column " + std::to_string(c + 1) + " for digit " + std::to_string(val) + ".";
                    return false;
                }
                grid[r][c] = val;
            }
        }
    }
    return true;
}

// Find next empty cell (returns false if board is completely filled)
bool findEmptyCell(const int grid[N][N], int& row, int& col) {
    for (row = 0; row < N; ++row) {
        for (col = 0; col < N; ++col) {
            if (grid[row][col] == 0) return true;
        }
    }
    return false;
}

// Recursive backtracking solver
bool solveSudoku(int grid[N][N]) {
    int row = 0, col = 0;
    if (!findEmptyCell(grid, row, col)) {
        return true; // Base case: all cells assigned
    }

    for (int num = 1; num <= 9; ++num) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num;

            if (solveSudoku(grid)) {
                return true;
            }

            grid[row][col] = 0; // Backtrack
        }
    }
    return false;
}

void inputCustomBoard(int grid[N][N]) {
    std::cout << "\nEnter 9 rows of 9 integers (0 for empty cells, separated by spaces):\n";
    for (int r = 0; r < N; ++r) {
        while (true) {
            std::cout << "Row " << (r + 1) << ": ";
            bool rowValid = true;
            for (int c = 0; c < N; ++c) {
                if (!(std::cin >> grid[r][c]) || grid[r][c] < 0 || grid[r][c] > 9) {
                    rowValid = false;
                    break;
                }
            }
            if (rowValid) {
                clearInput();
                break;
            }
            std::cout << "[Error] Invalid row entry. Enter 9 integers between 0 and 9.\n";
            clearInput();
        }
    }
}

int main() {
    int grid[N][N] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    std::cout << "=========================================\n";
    std::cout << "SUDOKU SOLVER\n";
    std::cout << "=========================================\n";
    std::cout << "1. Solve built-in sample puzzle\n";
    std::cout << "2. Enter custom 9x9 puzzle\n";
    std::cout << "Choice (1-2): ";

    int choice;
    if (!(std::cin >> choice) || (choice != 1 && choice != 2)) {
        std::cout << "[Error] Invalid selection. Exiting.\n";
        return 1;
    }
    clearInput();

    if (choice == 2) {
        inputCustomBoard(grid);
    }

    std::cout << "\nInput Sudoku Grid:";
    printBoard(grid);

    std::string validationError;
    if (!isValidInitialBoard(grid, validationError)) {
        std::cerr << "\n[Error] Invalid Sudoku puzzle: " << validationError << "\n";
        return 1;
    }

    if (solveSudoku(grid)) {
        std::cout << "\n[Success] Solved Sudoku Grid:";
        printBoard(grid);
    } else {
        std::cout << "\n[Error] No valid solution exists for this Sudoku board.\n";
    }

    return 0;
}