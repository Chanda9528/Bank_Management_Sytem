#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>

using namespace std;

struct Account {
    int accountNumber;
    string name;
    double balance;
};

class BankManagementSystem {
private:
    vector<Account> accounts;
    const string filename = "accounts.dat";

    void saveToFile() {
        ofstream file(filename, ios::binary);
        if (!file) {
            throw runtime_error("Error: Unable to open file for saving.");
        }
        for (const auto& acc : accounts) {
            file.write((char*)&acc, sizeof(Account));
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file(filename, ios::binary);
        if (!file) {
            // If file does not exist, skip loading
            return;
        }
        Account acc;
        while (file.read((char*)&acc, sizeof(Account))) {
            accounts.push_back(acc);
        }
        file.close();
    }

    Account* findAccount(int accountNumber) {
        for (auto& acc : accounts) {
            if (acc.accountNumber == accountNumber) {
                return &acc;
            }
        }
        return nullptr;
    }

public:
    BankManagementSystem() {
        loadFromFile();
    }

    ~BankManagementSystem() {
        try {
            saveToFile();
        } catch (const exception& e) {
            cerr << e.what() << endl;
        }
    }

    void createAccount() {
        Account acc;
        cout << "Enter Account Number: ";
        cin >> acc.accountNumber;

        if (findAccount(acc.accountNumber)) {
            cout << "Error: Account with this number already exists.\n";
            return;
        }

        cin.ignore(); // Clear input buffer
        cout << "Enter Name: ";
        getline(cin, acc.name);

        cout << "Enter Initial Balance: ";
        cin >> acc.balance;

        accounts.push_back(acc);
        cout << "Account created successfully!\n";
    }

    void deposit() {
        int accountNumber;
        double amount;

        cout << "Enter Account Number: ";
        cin >> accountNumber;

        Account* acc = findAccount(accountNumber);
        if (!acc) {
            cout << "Error: Account not found.\n";
            return;
        }

        cout << "Enter Amount to Deposit: ";
        cin >> amount;

        if (amount <= 0) {
            cout << "Error: Invalid deposit amount.\n";
            return;
        }

        acc->balance += amount;
        cout << "Deposit successful! New Balance: " << fixed << setprecision(2) << acc->balance << "\n";
    }

    void withdraw() {
        int accountNumber;
        double amount;

        cout << "Enter Account Number: ";
        cin >> accountNumber;

        Account* acc = findAccount(accountNumber);
        if (!acc) {
            cout << "Error: Account not found.\n";
            return;
        }

        cout << "Enter Amount to Withdraw: ";
        cin >> amount;

        if (amount <= 0 || amount > acc->balance) {
            cout << "Error: Invalid withdrawal amount.\n";
            return;
        }

        acc->balance -= amount;
        cout << "Withdrawal successful! New Balance: " << fixed << setprecision(2) << acc->balance << "\n";
    }

    void viewAccount() {
        int accountNumber;
        cout << "Enter Account Number: ";
        cin >> accountNumber;

        Account* acc = findAccount(accountNumber);
        if (!acc) {
            cout << "Error: Account not found.\n";
            return;
        }

        cout << "\nAccount Details:\n";
        cout << "Account Number: " << acc->accountNumber << "\n";
        cout << "Name: " << acc->name << "\n";
        cout << "Balance: " << fixed << setprecision(2) << acc->balance << "\n";
    }

    void listAccounts() {
        if (accounts.empty()) {
            cout << "No accounts available.\n";
            return;
        }

        cout << "\nAll Accounts:\n";
        cout << left << setw(15) << "Account Number" << setw(20) << "Name" << setw(15) << "Balance" << "\n";
        cout << string(50, '-') << "\n";
        for (const auto& acc : accounts) {
            cout << left << setw(15) << acc.accountNumber << setw(20) << acc.name << setw(15) << fixed << setprecision(2) << acc.balance << "\n";
        }
    }
};

int main() {
    BankManagementSystem bms;
    int choice;

    while (true) {
        cout << "\n--- Bank Management System ---\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. View Account\n";
        cout << "5. List All Accounts\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        try {
            switch (choice) {
                case 1:
                    bms.createAccount();
                    break;
                case 2:
                    bms.deposit();
                    break;
                case 3:
                    bms.withdraw();
                    break;
                case 4:
                    bms.viewAccount();
                    break;
                case 5:
                    bms.listAccounts();
                    break;
                case 6:
                    cout << "Exiting...\n";
                    return 0;
                default:
                    cout << "Invalid choice! Please try again.\n";
            }
        } catch (const exception& e) {
            cerr << e.what() << endl;
        }
    }
}
