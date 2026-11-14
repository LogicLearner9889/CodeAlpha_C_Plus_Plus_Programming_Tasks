#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>
#include <functional>
#include <random>

const std::string DB_FILE = "users_db.txt";

struct UserRecord {
    std::string username;
    std::string salt;
    std::string passwordHash;
};

void clearBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Generate a random 16-character alphanumeric salt
std::string generateSalt(size_t length = 16) {
    const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, static_cast<int>(chars.size() - 1));

    std::string salt;
    for (size_t i = 0; i < length; ++i) {
        salt += chars[dist(gen)];
    }
    return salt;
}

// Compute salted hash string
std::string hashPassword(const std::string& password, const std::string& salt) {
    std::hash<std::string> hasher;
    size_t h1 = hasher(salt + password);
    size_t h2 = hasher(password + salt + std::to_string(h1));
    std::stringstream ss;
    ss << std::hex << h1 << h2;
    return ss.str();
}

// Validate username: 4-20 alphanumeric/underscore chars, no spaces/delimiters
bool isValidUsername(const std::string& username, std::string& errorMsg) {
    if (username.length() < 4 || username.length() > 20) {
        errorMsg = "Username must be between 4 and 20 characters.";
        return false;
    }
    for (char ch : username) {
        if (!std::isalnum(static_cast<unsigned char>(ch)) && ch != '_') {
            errorMsg = "Username can only contain letters, numbers, and underscores.";
            return false;
        }
    }
    return true;
}

// Validate password strength: min 8 chars, upper, lower, digit, special char
bool isValidPassword(const std::string& password, std::string& errorMsg) {
    if (password.length() < 8) {
        errorMsg = "Password must be at least 8 characters long.";
        return false;
    }
    if (password.find('|') != std::string::npos || password.find(' ') != std::string::npos) {
        errorMsg = "Password cannot contain spaces or '|' characters.";
        return false;
    }
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    for (unsigned char ch : password) {
        if (std::isupper(ch)) hasUpper = true;
        else if (std::islower(ch)) hasLower = true;
        else if (std::isdigit(ch)) hasDigit = true;
        else hasSpecial = true;
    }
    if (!hasUpper || !hasLower || !hasDigit || !hasSpecial) {
        errorMsg = "Password must include uppercase, lowercase, digit, and special character.";
        return false;
    }
    return true;
}

// Load all users from file safely
bool loadUsers(std::vector<UserRecord>& users) {
    users.clear();
    std::ifstream inFile(DB_FILE);
    if (!inFile.is_open()) {
        // File does not exist yet; create an empty database file
        std::ofstream createFile(DB_FILE);
        if (!createFile.is_open()) {
            std::cerr << "[Critical Error] Unable to initialize database file '" << DB_FILE << "'.\n";
            return false;
        }
        return true;
    }

    std::string line;
    int lineNum = 0;
    while (std::getline(inFile, line)) {
        ++lineNum;
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string u, s, h;
        if (std::getline(ss, u, '|') && std::getline(ss, s, '|') && std::getline(ss, h)) {
            users.push_back({u, s, h});
        } else {
            std::cerr << "[Warning] Skipping corrupted record on line " << lineNum << ".\n";
        }
    }
    return true;
}

bool usernameExists(const std::vector<UserRecord>& users, const std::string& username) {
    for (const auto& u : users) {
        if (u.username == username) return true;
    }
    return false;
}

void registerUser() {
    std::vector<UserRecord> users;
    if (!loadUsers(users)) return;

    std::string username, password, err;
    std::cout << "\n--- USER REGISTRATION ---\n";

    while (true) {
        std::cout << "Enter Username (4-20 alphanumeric chars): ";
        std::getline(std::cin, username);
        if (!isValidUsername(username, err)) {
            std::cout << "[Error] " << err << "\n";
            continue;
        }
        if (usernameExists(users, username)) {
            std::cout << "[Error] Username '" << username << "' is already taken. Choose another.\n";
            continue;
        }
        break;
    }

    while (true) {
        std::cout << "Enter Password (min 8 chars, upper, lower, digit, symbol): ";
        std::getline(std::cin, password);
        if (!isValidPassword(password, err)) {
            std::cout << "[Error] " << err << "\n";
            continue;
        }
        break;
    }

    std::string salt = generateSalt();
    std::string hashed = hashPassword(password, salt);

    std::ofstream outFile(DB_FILE, std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "[Error] Failed to open database file for writing.\n";
        return;
    }

    outFile << username << "|" << salt << "|" << hashed << "\n";
    if (outFile.fail()) {
        std::cerr << "[Error] Failed to write user credentials to disk.\n";
        return;
    }

    std::cout << "[Success] Registration completed for user '" << username << "'!\n";
}

void loginUser() {
    std::vector<UserRecord> users;
    if (!loadUsers(users)) return;

    if (users.empty()) {
        std::cout << "\n[Info] No registered users found. Please register first.\n";
        return;
    }

    std::cout << "\n--- USER LOGIN ---\n";
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;

    while (attempts < MAX_ATTEMPTS) {
        std::string username, password;
        std::cout << "Username: ";
        std::getline(std::cin, username);
        std::cout << "Password: ";
        std::getline(std::cin, password);

        bool authenticated = false;
        for (const auto& user : users) {
            if (user.username == username) {
                if (hashPassword(password, user.salt) == user.passwordHash) {
                    authenticated = true;
                }
                break;
            }
        }

        if (authenticated) {
            std::cout << "\n[Success] Welcome back, " << username << "! Authentication successful.\n";
            return;
        } else {
            ++attempts;
            std::cout << "[Error] Invalid username or password. Attempts remaining: "
                      << (MAX_ATTEMPTS - attempts) << "\n";
        }
    }

    std::cout << "[Locked] Maximum login attempts exceeded. Returning to main menu.\n";
}

int main() {
    while (true) {
        std::cout << "\n=========================================\n";
        std::cout << "   LOGIN & REGISTRATION      \n";
        std::cout << "=========================================\n";
        std::cout << "1. Register\n";
        std::cout << "2. Login\n";
        std::cout << "3. Exit\n";
        std::cout << "Select option (1-3): ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cout << "[Error] Invalid input. Please enter 1, 2, or 3.\n";
            clearBuffer();
            continue;
        }
        clearBuffer();

        switch (choice) {
            case 1: registerUser(); break;
            case 2: loginUser(); break;
            case 3: std::cout << "Exiting system.\n"; return 0;
            default: std::cout << "[Error] Choice must be between 1 and 3.\n";
        }
    }
}