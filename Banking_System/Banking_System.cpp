#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <ctime>
#include <sstream>
#include <stdexcept>

std::string getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm* localTm = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(localTm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void clearStdin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

class Transaction {
private:
    std::string transactionId;
    std::string type;
    double amount;
    double postBalance;
    std::string timestamp;
    std::string details;

public:
    Transaction(std::string id, std::string t, double amt, double bal, std::string info)
        : transactionId(std::move(id)), type(std::move(t)), amount(amt),
          postBalance(bal), timestamp(getCurrentTimestamp()), details(std::move(info)) {}

    void display() const {
        std::cout << std::left << std::setw(12) << transactionId
                  << std::setw(21) << timestamp
                  << std::setw(14) << type
                  << "$" << std::setw(11) << std::fixed << std::setprecision(2) << amount
                  << "$" << std::setw(11) << postBalance
                  << details << "\n";
    }
};

class Account {
private:
    int accountNumber;
    int customerId;
    std::string accountType;
    double balance;
    std::vector<Transaction> history;
    int txCounter = 1;

    std::string nextTxId() {
        return "TX" + std::to_string(accountNumber) + "_" + std::to_string(txCounter++);
    }

public:
    Account(int accNum, int custId, std::string type, double initialDeposit)
        : accountNumber(accNum), customerId(custId), accountType(std::move(type)), balance(0.0) {
        if (initialDeposit < 0.0) {
            throw std::invalid_argument("Initial deposit cannot be negative.");
        }
        if (initialDeposit > 0.0) {
            deposit(initialDeposit, "Initial Opening Deposit");
        }
    }

    int getAccountNumber() const { return accountNumber; }
    int getCustomerId() const { return customerId; }
    std::string getAccountType() const { return accountType; }
    double getBalance() const { return balance; }

    void deposit(double amount, const std::string& note = "Cash/Online Deposit") {
        if (amount <= 0.0) {
            throw std::invalid_argument("Deposit amount must be greater than $0.00.");
        }
        balance += amount;
        history.emplace_back(nextTxId(), "DEPOSIT", amount, balance, note);
    }

    void withdraw(double amount, const std::string& note = "Cash Withdrawal") {
        if (amount <= 0.0) {
            throw std::invalid_argument("Withdrawal amount must be greater than $0.00.");
        }
        if (amount > balance) {
            throw std::runtime_error("Insufficient balance for withdrawal.");
        }
        balance -= amount;
        history.emplace_back(nextTxId(), "WITHDRAWAL", amount, balance, note);
    }

    void recordTransferOut(double amount, int targetAcc) {
        balance -= amount;
        history.emplace_back(nextTxId(), "TRANSFER_OUT", amount, balance,
                             "To Account #" + std::to_string(targetAcc));
    }

    void recordTransferIn(double amount, int sourceAcc) {
        balance += amount;
        history.emplace_back(nextTxId(), "TRANSFER_IN", amount, balance,
                             "From Account #" + std::to_string(sourceAcc));
    }

    void printSummary() const {
        std::cout << "Account #" << accountNumber
                  << " | Owner ID: " << customerId
                  << " | Type: " << accountType
                  << " | Balance: $" << std::fixed << std::setprecision(2) << balance << "\n";
    }

    void printTransactions() const {
        std::cout << "\n--- Transaction History for Account #" << accountNumber << " ---\n";
        if (history.empty()) {
            std::cout << "No transactions recorded yet.\n";
            return;
        }
        std::cout << std::left << std::setw(12) << "TX ID"
                  << std::setw(21) << "Timestamp"
                  << std::setw(14) << "Type"
                  << std::setw(12) << "Amount"
                  << std::setw(12) << "Balance"
                  << "Details\n";
        std::cout << std::string(85, '-') << "\n";
        for (const auto& tx : history) {
            tx.display();
        }
    }
};

class Customer {
private:
    int customerId;
    std::string fullName;
    std::string email;
    std::string phone;

public:
    Customer(int id, std::string name, std::string mail, std::string ph)
        : customerId(id), fullName(std::move(name)), email(std::move(mail)), phone(std::move(ph)) {
        if (fullName.empty()) {
            throw std::invalid_argument("Customer name cannot be empty.");
        }
    }

    int getId() const { return customerId; }
    std::string getName() const { return fullName; }

    void display() const {
        std::cout << "Customer ID: " << customerId
                  << " | Name: " << fullName
                  << " | Email: " << email
                  << " | Phone: " << phone << "\n";
    }
};

class BankSystem {
private:
    std::vector<Customer> customers;
    std::vector<Account> accounts;
    int nextCustomerId = 101;
    int nextAccountNumber = 10001;

    Customer* findCustomer(int id) {
        for (auto& c : customers) {
            if (c.getId() == id) return &c;
        }
        return nullptr;
    }

    Account* findAccount(int accNum) {
        for (auto& a : accounts) {
            if (a.getAccountNumber() == accNum) return &a;
        }
        return nullptr;
    }

public:
    void createCustomer() {
        std::string name, email, phone;
        std::cout << "Full Name: ";
        std::getline(std::cin, name);
        std::cout << "Email: ";
        std::getline(std::cin, email);
        std::cout << "Phone: ";
        std::getline(std::cin, phone);

        try {
            Customer c(nextCustomerId, name, email, phone);
            customers.push_back(c);
            std::cout << "[Success] Customer created! Assigned Customer ID: " << nextCustomerId++ << "\n";
        } catch (const std::exception& e) {
            std::cout << "[Error] " << e.what() << "\n";
        }
    }

    void createAccount() {
        int custId;
        std::cout << "Enter Customer ID: ";
        if (!(std::cin >> custId)) {
            clearStdin();
            std::cout << "[Error] Invalid Customer ID.\n";
            return;
        }
        clearStdin();

        if (!findCustomer(custId)) {
            std::cout << "[Error] Customer ID " << custId << " does not exist.\n";
            return;
        }

        std::string accType;
        std::cout << "Account Type (Savings / Checking): ";
        std::getline(std::cin, accType);
        if (accType.empty()) accType = "Savings";

        double initialDeposit = 0.0;
        std::cout << "Initial Deposit Amount ($): ";
        if (!(std::cin >> initialDeposit)) {
            clearStdin();
            std::cout << "[Error] Invalid deposit amount.\n";
            return;
        }
        clearStdin();

        try {
            Account acc(nextAccountNumber, custId, accType, initialDeposit);
            accounts.push_back(acc);
            std::cout << "[Success] Account created! Account Number: " << nextAccountNumber++ << "\n";
        } catch (const std::exception& e) {
            std::cout << "[Error] " << e.what() << "\n";
        }
    }

    void handleDeposit() {
        int accNum;
        double amount;
        std::cout << "Account Number: ";
        if (!(std::cin >> accNum)) { clearStdin(); std::cout << "[Error] Invalid account number.\n"; return; }
        std::cout << "Deposit Amount ($): ";
        if (!(std::cin >> amount)) { clearStdin(); std::cout << "[Error] Invalid amount.\n"; return; }
        clearStdin();

        Account* acc = findAccount(accNum);
        if (!acc) {
            std::cout << "[Error] Account #" << accNum << " not found.\n";
            return;
        }

        try {
            acc->deposit(amount);
            std::cout << "[Success] Deposited $" << std::fixed << std::setprecision(2) << amount
                      << ". New Balance: $" << acc->getBalance() << "\n";
        } catch (const std::exception& e) {
            std::cout << "[Error] " << e.what() << "\n";
        }
    }

    void handleWithdraw() {
        int accNum;
        double amount;
        std::cout << "Account Number: ";
        if (!(std::cin >> accNum)) { clearStdin(); std::cout << "[Error] Invalid account number.\n"; return; }
        std::cout << "Withdrawal Amount ($): ";
        if (!(std::cin >> amount)) { clearStdin(); std::cout << "[Error] Invalid amount.\n"; return; }
        clearStdin();

        Account* acc = findAccount(accNum);
        if (!acc) {
            std::cout << "[Error] Account #" << accNum << " not found.\n";
            return;
        }

        try {
            acc->withdraw(amount);
            std::cout << "[Success] Withdrew $" << std::fixed << std::setprecision(2) << amount
                      << ". Remaining Balance: $" << acc->getBalance() << "\n";
        } catch (const std::exception& e) {
            std::cout << "[Error] " << e.what() << "\n";
        }
    }

    void handleTransfer() {
        int fromAccNum, toAccNum;
        double amount;

        std::cout << "Source Account Number: ";
        if (!(std::cin >> fromAccNum)) { clearStdin(); std::cout << "[Error] Invalid account number.\n"; return; }
        std::cout << "Destination Account Number: ";
        if (!(std::cin >> toAccNum)) { clearStdin(); std::cout << "[Error] Invalid account number.\n"; return; }
        std::cout << "Transfer Amount ($): ";
        if (!(std::cin >> amount)) { clearStdin(); std::cout << "[Error] Invalid amount.\n"; return; }
        clearStdin();

        if (fromAccNum == toAccNum) {
            std::cout << "[Error] Cannot transfer funds to the same account.\n";
            return;
        }

        Account* source = findAccount(fromAccNum);
        Account* dest = findAccount(toAccNum);

        if (!source || !dest) {
            std::cout << "[Error] One or both account numbers do not exist.\n";
            return;
        }
        if (amount <= 0.0) {
            std::cout << "[Error] Transfer amount must be positive.\n";
            return;
        }
        if (source->getBalance() < amount) {
            std::cout << "[Error] Insufficient funds in source account.\n";
            return;
        }

        // Atomic transfer update
        source->recordTransferOut(amount, toAccNum);
        dest->recordTransferIn(amount, fromAccNum);

        std::cout << "[Success] Transferred $" << std::fixed << std::setprecision(2) << amount
                  << " from #" << fromAccNum << " to #" << toAccNum << ".\n";
    }

    void viewAccountDetails() {
        int accNum;
        std::cout << "Enter Account Number: ";
        if (!(std::cin >> accNum)) { clearStdin(); std::cout << "[Error] Invalid account number.\n"; return; }
        clearStdin();

        Account* acc = findAccount(accNum);
        if (!acc) {
            std::cout << "[Error] Account #" << accNum << " not found.\n";
            return;
        }

        Customer* owner = findCustomer(acc->getCustomerId());
        std::cout << "\n=== ACCOUNT DETAILS ===\n";
        if (owner) owner->display();
        acc->printSummary();
        acc->printTransactions();
    }
};

int main() {
    BankSystem bank;
    while (true) {
        std::cout << "\n=========================================\n";
        std::cout << "BANKING SYSTEM\n";
        std::cout << "=========================================\n";
        std::cout << "1. Create Customer\n";
        std::cout << "2. Open Bank Account\n";
        std::cout << "3. Deposit Funds\n";
        std::cout << "4. Withdraw Funds\n";
        std::cout << "5. Transfer Funds\n";
        std::cout << "6. View Account & Transaction History\n";
        std::cout << "7. Exit\n";
        std::cout << "Select option (1-7): ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cout << "[Error] Enter a number between 1 and 7.\n";
            clearStdin();
            continue;
        }
        clearStdin();

        switch (choice) {
            case 1: bank.createCustomer(); break;
            case 2: bank.createAccount(); break;
            case 3: bank.handleDeposit(); break;
            case 4: bank.handleWithdraw(); break;
            case 5: bank.handleTransfer(); break;
            case 6: bank.viewAccountDetails(); break;
            case 7: std::cout << "Thank you for using CodeAlpha Banking System.\n"; return 0;
            default: std::cout << "[Error] Invalid option.\n";
        }
    }
}