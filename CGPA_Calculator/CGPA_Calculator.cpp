#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <cctype>

struct Course {
    std::string name;
    std::string letterGrade;
    double gradePoint;
    double credits;
};

struct Semester {
    int semesterNumber;
    std::vector<Course> courses;
    double totalCredits = 0.0;
    double totalGradePoints = 0.0;
    double gpa = 0.0;
};

// Clear invalid input stream safely
void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Convert letter grade to 10-point / 4-point scale (4.0 standard scale used here)
bool parseGrade(const std::string& input, double& outPoint, std::string& normalizedLabel) {
    std::string upper = input;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

    if (upper == "O" || upper == "A+" || upper == "A") { outPoint = 4.0; normalizedLabel = upper; return true; }
    if (upper == "A-") { outPoint = 3.7; normalizedLabel = upper; return true; }
    if (upper == "B+") { outPoint = 3.3; normalizedLabel = upper; return true; }
    if (upper == "B")  { outPoint = 3.0; normalizedLabel = upper; return true; }
    if (upper == "B-") { outPoint = 2.7; normalizedLabel = upper; return true; }
    if (upper == "C+") { outPoint = 2.3; normalizedLabel = upper; return true; }
    if (upper == "C")  { outPoint = 2.0; normalizedLabel = upper; return true; }
    if (upper == "D")  { outPoint = 1.0; normalizedLabel = upper; return true; }
    if (upper == "F")  { outPoint = 0.0; normalizedLabel = upper; return true; }

    // Allow direct numeric grade point entry (0.0 to 4.0)
    try {
        size_t pos = 0;
        double val = std::stod(input, &pos);
        if (pos == input.length() && val >= 0.0 && val <= 4.0) {
            outPoint = val;
            normalizedLabel = input + " pts";
            return true;
        }
    } catch (...) {
        return false;
    }
    return false;
}

int getValidatedInt(const std::string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minVal && value <= maxVal) {
            clearInputBuffer();
            return value;
        }
        std::cout << "[Error] Please enter a valid integer between " << minVal << " and " << maxVal << ".\n";
        clearInputBuffer();
    }
}

double getValidatedDouble(const std::string& prompt, double minVal, double maxVal) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minVal && value <= maxVal) {
            clearInputBuffer();
            return value;
        }
        std::cout << "[Error] Please enter a valid number between " << minVal << " and " << maxVal << ".\n";
        clearInputBuffer();
    }
}

int main() {
    std::cout << "=========================================\n";
    std::cout << "CGPA CALCULATOR\n";
    std::cout << "=========================================\n";

    int numSemesters = getValidatedInt("Enter number of semesters to calculate (1-12): ", 1, 12);
    std::vector<Semester> semesters;

    double cumulativeCredits = 0.0;
    double cumulativeGradePoints = 0.0;

    for (int s = 1; s <= numSemesters; ++s) {
        Semester sem;
        sem.semesterNumber = s;
        std::cout << "\n--- Semester " << s << " ---\n";
        int numCourses = getValidatedInt("Enter number of courses taken (1-15): ", 1, 15);

        for (int c = 1; c <= numCourses; ++c) {
            Course course;
            std::cout << "\nCourse #" << c << " Name: ";
            std::getline(std::cin, course.name);
            if (course.name.empty()) {
                course.name = "Course_" + std::to_string(c);
            }

            course.credits = getValidatedDouble("  Credit Hours (0.5 - 10.0): ", 0.5, 10.0);

            while (true) {
                std::string gradeInput;
                std::cout << "  Grade (A+, A, A-, B+, B, B-, C+, C, D, F or 0.0-4.0): ";
                std::cin >> gradeInput;
                clearInputBuffer();

                if (parseGrade(gradeInput, course.gradePoint, course.letterGrade)) {
                    break;
                }
                std::cout << "  [Error] Invalid grade format. Try again.\n";
            }

            sem.totalCredits += course.credits;
            sem.totalGradePoints += (course.gradePoint * course.credits);
            sem.courses.push_back(course);
        }

        if (sem.totalCredits > 0.0) {
            sem.gpa = sem.totalGradePoints / sem.totalCredits;
        }

        cumulativeCredits += sem.totalCredits;
        cumulativeGradePoints += sem.totalGradePoints;
        semesters.push_back(sem);
    }

    // Final Report Display
    std::cout << "\n=========================================================\n";
    std::cout << "                  ACADEMIC GRADE REPORT                  \n";
    std::cout << "=========================================================\n";
    std::cout << std::fixed << std::setprecision(2);

    for (const auto& sem : semesters) {
        std::cout << "\nSemester " << sem.semesterNumber << ":\n";
        std::cout << std::left << std::setw(25) << "Course"
                  << std::setw(12) << "Credits"
                  << std::setw(12) << "Grade"
                  << "Grade Points\n";
        std::cout << "---------------------------------------------------------\n";

        for (const auto& c : sem.courses) {
            std::cout << std::left << std::setw(25) << c.name.substr(0, 23)
                      << std::setw(12) << c.credits
                      << std::setw(12) << c.letterGrade
                      << (c.gradePoint * c.credits) << "\n";
        }
        std::cout << ">> Semester Total Credits: " << sem.totalCredits
                  << " | Semester GPA: " << sem.gpa << "\n";
    }

    double cgpa = (cumulativeCredits > 0.0) ? (cumulativeGradePoints / cumulativeCredits) : 0.0;
    std::cout << "\n=========================================================\n";
    std::cout << "OVERALL TOTAL CREDITS : " << cumulativeCredits << "\n";
    std::cout << "FINAL CUMULATIVE CGPA : " << cgpa << " / 4.00\n";
    std::cout << "=========================================================\n";

    return 0;
}