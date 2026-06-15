/*
    Bank Management Application
    ----------------------------
    A simple console based banking system using OOP and file handling.

    Features:
        - Create new account
        - Deposit money
        - Withdraw money
        - Check balance
        - Display all account holders
        - Modify account details
        - Delete account

    All account data is stored in a file called "accounts.dat" so that
    the data stays even after the program closes.
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>

using namespace std;

class Account {
private:
    int accNo;
    char name[50];
    char accType[15];   // Savings or Current
    double balance;

public:
    // Used while creating a new account
    void createAccount() {
        cout << "\nEnter Account Number : ";
        cin >> accNo;
        cin.ignore();

        cout << "Enter Account Holder Name : ";
        cin.getline(name, 50);

        cout << "Enter Account Type (Savings/Current) : ";
        cin.getline(accType, 15);

        cout << "Enter Initial Deposit Amount : ";
        cin >> balance;

        cout << "\nAccount Created Successfully!\n";
    }

    // Show one account's details
    void showAccount() const {
        cout << left;
        cout << setw(15) << accNo
             << setw(20) << name
             << setw(12) << accType
             << setw(10) << fixed << setprecision(2) << balance << endl;
    }

    void deposit(double amount) {
        balance += amount;
    }

    bool withdraw(double amount) {
        if (amount > balance) {
            return false; // not enough balance
        }
        balance -= amount;
        return true;
    }

    int getAccNo() const {
        return accNo;
    }

    string getName() const {
        return string(name);
    }

    double getBalance() const {
        return balance;
    }

    void setBalance(double b) {
        balance = b;
    }

    void modify() {
        cin.ignore();
        cout << "Enter New Account Holder Name : ";
        cin.getline(name, 50);

        cout << "Enter New Account Type : ";
        cin.getline(accType, 15);

        cout << "Enter New Balance : ";
        cin >> balance;
    }
};

// ---------------- Function Prototypes ----------------
void openAccount();
void showAllAccounts();
void depositAmount();
void withdrawAmount();
void checkBalance();
void modifyAccount();
void deleteAccount();

const string FILE_NAME = "accounts.dat";

int main() {
    int choice;

    do {
        cout << "\n===================================\n";
        cout << "      BANK MANAGEMENT SYSTEM\n";
        cout << "===================================\n";
        cout << "1. Open New Account\n";
        cout << "2. Deposit Amount\n";
        cout << "3. Withdraw Amount\n";
        cout << "4. Balance Enquiry\n";
        cout << "5. Show All Accounts\n";
        cout << "6. Modify Account\n";
        cout << "7. Delete Account\n";
        cout << "8. Exit\n";
        cout << "===================================\n";
        cout << "Enter your choice : ";
        cin >> choice;

        switch (choice) {
            case 1:
                openAccount();
                break;
            case 2:
                depositAmount();
                break;
            case 3:
                withdrawAmount();
                break;
            case 4:
                checkBalance();
                break;
            case 5:
                showAllAccounts();
                break;
            case 6:
                modifyAccount();
                break;
            case 7:
                deleteAccount();
                break;
            case 8:
                cout << "\nThank you for using our Bank Management System!\n";
                break;
            default:
                cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 8);

    return 0;
}

// ---------------- Function Definitions ----------------

// Adds a new account record at the end of the file
void openAccount() {
    Account acc;
    acc.createAccount();

    ofstream outFile(FILE_NAME, ios::binary | ios::app);
    if (!outFile) {
        cout << "Error opening file!\n";
        return;
    }

    outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
    outFile.close();
}

// Displays all accounts stored in the file
void showAllAccounts() {
    ifstream inFile(FILE_NAME, ios::binary);
    if (!inFile) {
        cout << "\nNo records found.\n";
        return;
    }

    Account acc;
    bool found = false;

    cout << "\n--------------------------------------------------\n";
    cout << left << setw(15) << "Acc No"
         << setw(20) << "Name"
         << setw(12) << "Type"
         << setw(10) << "Balance" << endl;
    cout << "--------------------------------------------------\n";

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        acc.showAccount();
        found = true;
    }

    if (!found) {
        cout << "No records found.\n";
    }

    inFile.close();
}

// Searches account by account number, returns true if found and updates balance
void depositAmount() {
    int accNo;
    double amount;

    cout << "\nEnter Account Number : ";
    cin >> accNo;

    cout << "Enter Amount to Deposit : ";
    cin >> amount;

    fstream file(FILE_NAME, ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "Error opening file!\n";
        return;
    }

    Account acc;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccNo() == accNo) {
            acc.deposit(amount);

            // move back to overwrite this record
            file.seekp(-static_cast<int>(sizeof(Account)), ios::cur);
            file.write(reinterpret_cast<char*>(&acc), sizeof(Account));

            found = true;
            cout << "\nAmount Deposited Successfully!\n";
            cout << "New Balance : " << fixed << setprecision(2) << acc.getBalance() << endl;
            break;
        }
    }

    if (!found) {
        cout << "\nAccount not found!\n";
    }

    file.close();
}

void withdrawAmount() {
    int accNo;
    double amount;

    cout << "\nEnter Account Number : ";
    cin >> accNo;

    cout << "Enter Amount to Withdraw : ";
    cin >> amount;

    fstream file(FILE_NAME, ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "Error opening file!\n";
        return;
    }

    Account acc;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccNo() == accNo) {
            found = true;

            if (acc.withdraw(amount)) {
                file.seekp(-static_cast<int>(sizeof(Account)), ios::cur);
                file.write(reinterpret_cast<char*>(&acc), sizeof(Account));

                cout << "\nAmount Withdrawn Successfully!\n";
                cout << "New Balance : " << fixed << setprecision(2) << acc.getBalance() << endl;
            } else {
                cout << "\nInsufficient Balance!\n";
                cout << "Available Balance : " << fixed << setprecision(2) << acc.getBalance() << endl;
            }
            break;
        }
    }

    if (!found) {
        cout << "\nAccount not found!\n";
    }

    file.close();
}

void checkBalance() {
    int accNo;

    cout << "\nEnter Account Number : ";
    cin >> accNo;

    ifstream file(FILE_NAME, ios::binary);
    if (!file) {
        cout << "Error opening file!\n";
        return;
    }

    Account acc;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccNo() == accNo) {
            found = true;
            cout << "\nAccount Holder Name : " << acc.getName() << endl;
            cout << "Current Balance     : " << fixed << setprecision(2) << acc.getBalance() << endl;
            break;
        }
    }

    if (!found) {
        cout << "\nAccount not found!\n";
    }

    file.close();
}

void modifyAccount() {
    int accNo;

    cout << "\nEnter Account Number to Modify : ";
    cin >> accNo;

    fstream file(FILE_NAME, ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "Error opening file!\n";
        return;
    }

    Account acc;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccNo() == accNo) {
            found = true;
            acc.modify();

            file.seekp(-static_cast<int>(sizeof(Account)), ios::cur);
            file.write(reinterpret_cast<char*>(&acc), sizeof(Account));

            cout << "\nAccount Updated Successfully!\n";
            break;
        }
    }

    if (!found) {
        cout << "\nAccount not found!\n";
    }

    file.close();
}

// Deletes an account by rewriting the file without that record
void deleteAccount() {
    int accNo;

    cout << "\nEnter Account Number to Delete : ";
    cin >> accNo;

    ifstream inFile(FILE_NAME, ios::binary);
    if (!inFile) {
        cout << "Error opening file!\n";
        return;
    }

    ofstream tempFile("temp.dat", ios::binary);

    Account acc;
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccNo() == accNo) {
            found = true;
            continue; // skip writing this record (i.e. delete it)
        }
        tempFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
    }

    inFile.close();
    tempFile.close();

    remove(FILE_NAME.c_str());
    rename("temp.dat", FILE_NAME.c_str());

    if (found) {
        cout << "\nAccount Deleted Successfully!\n";
    } else {
        cout << "\nAccount not found!\n";
    }
}
