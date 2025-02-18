#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits> 
#include <windows.h>
using namespace std;

// Function to set color
void SetColor(int foreground, int background)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)((background << 4) | foreground));
}

// Transaction class to represent a banking transaction
class Transaction 
{
public:
    string type; // Deposit, Withdrawal or Transfer
    double amount;

    Transaction(string t, double a) : type(t), amount(a) {}
};

// Account class to represent a bank account
class Account 
{
private:
    string accountNumber;
    double balance;
    vector<Transaction> transactions;

public:
    Account(string accNum, double initialBalance) : accountNumber(accNum), balance(initialBalance) {}

    string getAccountNumber() const 
    {
        return accountNumber;
    }

    double getBalance() const 
    {
        return balance;
    }

    void deposit(double amount) 
    {
        balance += amount;
        transactions.push_back(Transaction("Deposit", amount));
        cout << "Deposit successful. New balance: " << balance << endl;
    }

    void withdraw(double amount)
    {
        if (amount > balance)
        {
            cout << "Insufficient funds!" << endl;
        }
        else
        {
            balance -= amount;
            transactions.push_back(Transaction("Withdrawal", amount));
            cout << "Withdrawal successful." << endl;
            cout << "New balance: " << balance << endl;
        }
    }

    void transfer(Account& toAccount, double amount) 
    {
        if (amount > balance) 
        {
            cout << "Insufficient funds for transfer!" << endl;
        }
        else 
        {
            withdraw(amount);
            toAccount.deposit(amount);
            transactions.push_back(Transaction("Transfer", amount));
            cout << "Transfer successful." << endl;
        }
    }

    void displayTransactions() const 
    {
        cout << endl;
        cout << "Transaction History for Account " << accountNumber << ":" << endl;
        cout << setw(20)  << left << "Type" << setw(20) << "Amount" << endl;
        for (const auto& transaction : transactions)
        {
            cout << setw(20) << left << transaction.type << setw(20) << transaction.amount << endl;
        }
    }
};

// Customer class to represent a bank customer
class Customer 
{
private:
    string name;
    vector<Account> accounts;

public:
    Customer(string n) : name(n) {}

    void createAccount(string accNum, double initialBalance) 
    {
        accounts.push_back(Account(accNum, initialBalance));
        cout << "Account created successfully!" << endl;
    }

    Account* getAccount(string accNum) 
    {
        for (auto& account : accounts) 
        {
            if (account.getAccountNumber() == accNum) 
            {
                return &account;
            }
        }
        return nullptr;
    }

    void displayAccounts() const
    {
        cout << "Accounts for Customer " << name << ":" << endl;
        for (const auto& account : accounts)
        {
            cout << "Account Number: " << account.getAccountNumber()
                << ", Balance: " << account.getBalance() << endl;
        }
    }
};

// BankingService class to manage banking operations
class BankingService 
{
public:
    static void performDeposit(Account& account, double amount) 
    {
        account.deposit(amount);
    }

    static void performWithdrawal(Account& account, double amount)
    {
        account.withdraw(amount);
    }

    static void performTransfer(Account& fromAccount, Account& toAccount, double amount) 
    {
        fromAccount.transfer(toAccount, amount);
    }
};

// Function to display the main menu
void displayMenu()
{
    SetColor(7, 0);
    cout << endl;
    SetColor(13, 7);
    cout << " Menu: ";
    SetColor(7, 0);
    cout << "\n1. Create Customer\n";
    cout << "2. Create Account\n";
    cout << "3. Deposit\n";
    cout << "4. Withdraw\n";
    cout << "5. Transfer\n";
    cout << "6. View Account Information\n";
    cout << "7. Exit\n\n";
    SetColor(13, 7);
    cout << "Enter your choice:";
    SetColor(7, 0);
    cout << " ";
}

// Function to clear the input buffer
void clearInputBuffer() 
{
    cin.clear(); // Clear any error flags
    cin.ignore(10000, '\n'); // Discard leftover characters
}

int main() 
{
    vector<Customer> customers;
    int choice;
    string name, accNum, fromAccNum, toAccNum;
    double amount, initialBalance;

    SetColor(5, 0);
    cout << " ___   _   _  _  _  _ _  _  _   __       __ __ __ __  ___  ___  _   _ " << endl;
    cout << "| o ) / \\ | \\| || |//| || \\| | / _|     / _|\\ V // _||_ _|| __|| \\_/ |" << endl;
    cout << "| o \\| o || \\\\ ||  ( | || \\\\ |( |_n     \\_ \\ \\ / \\_ \\ | | | _| | \\_/ |" << endl;
    cout << "|___/|_n_||_|\\_||_|\\\\|_||_|\\_| \\__/     |__/ |_| |__/ |_| |___||_| |_|" << endl;

    while (true) 
    {
        displayMenu();
        if (!(cin >> choice))
        {
            clearInputBuffer(); 
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }
        clearInputBuffer(); 

        switch (choice) 
        {
        case 1: 
        {
            cout << "Enter customer name: ";
            getline(cin, name); 
            customers.push_back(Customer(name));
            cout << "Customer created successfully!" << endl;
            system("pause");
            system("cls");
            break;
        }
        case 2: 
        {
            cout << "Enter customer name: ";
            getline(cin, name); 
            cout << "Enter account number: ";
            getline(cin, accNum); 
            cout << "Enter initial balance: ";
            if (!(cin >> initialBalance)) 
            {
                clearInputBuffer();
                cout << "Invalid input. Please enter a number." << endl;
                break;
            }
            clearInputBuffer(); 

            bool customerFound = false;
            for (auto& customer : customers) 
            {
                if (customer.getAccount(accNum) == nullptr)
                {
                    customer.createAccount(accNum, initialBalance);
                    customerFound = true;
                    break;
                }
            }
            if (!customerFound)
            {
                cout << "Customer not found or account already exists!" << endl;
            }
            system("pause");
            system("cls");
            break;
        }
        case 3: 
        {
            cout << "Enter account number: ";
            getline(cin, accNum);
            cout << "Enter deposit amount: ";
            if (!(cin >> amount)) 
            {
                clearInputBuffer(); 
                cout << "Invalid input. Please enter a number." << endl;
                break;
            }
            clearInputBuffer();

            bool accountFound = false;
            for (auto& customer : customers)
            {
                Account* account = customer.getAccount(accNum);
                if (account) 
                {
                    BankingService::performDeposit(*account, amount);
                    accountFound = true;
                    break;
                }
            }
            if (!accountFound)
            {
                cout << "Account not found!" << endl;
            }
            system("pause");
            system("cls");
            break;
        }
        case 4: 
        {
            cout << "Enter account number: ";
            getline(cin, accNum); 
            cout << "Enter withdrawal amount: ";
            if (!(cin >> amount)) 
            {
                clearInputBuffer();
                cout << "Invalid input. Please enter a number." << endl;
                break;
            }
            clearInputBuffer();

            bool accountFound = false;
            for (auto& customer : customers) 
            {
                Account* account = customer.getAccount(accNum);
                if (account) {
                    BankingService::performWithdrawal(*account, amount);
                    accountFound = true;
                    break;
                }
            }
            if (!accountFound) 
            {
                cout << "Account not found!" << endl;
            }
            system("pause");
            system("cls");
            break;
        }
        case 5: 
        {
            cout << "Enter source account number: ";
            getline(cin, fromAccNum); 
            cout << "Enter destination account number: ";
            getline(cin, toAccNum); 
            cout << "Enter transfer amount: ";
            if (!(cin >> amount)) 
            {
                clearInputBuffer(); 
                cout << "Invalid input. Please enter a number." << endl;
                break;
            }
            clearInputBuffer(); 

            Account* fromAccount = nullptr;
            Account* toAccount = nullptr;

            for (auto& customer : customers) 
            {
                if (!fromAccount) fromAccount = customer.getAccount(fromAccNum);
                if (!toAccount) toAccount = customer.getAccount(toAccNum);
                if (fromAccount && toAccount) break;
            }

            if (fromAccount && toAccount)
            {
                BankingService::performTransfer(*fromAccount, *toAccount, amount);
            }
            else 
            {
                cout << "One or both accounts not found!" << endl;
            }
            system("pause");
            system("cls");
            break;
        }
        case 6:
        {
            cout << "Enter account number: ";
            getline(cin, accNum); 

            bool accountFound = false;
            for (auto& customer : customers) 
            {
                Account* account = customer.getAccount(accNum);
                if (account)
                {
                    cout << "Account Number: " << account->getAccountNumber() << endl;
                    cout << "Balance: " << account->getBalance() << endl;
                    account->displayTransactions();
                    accountFound = true;
                    break;
                }
            }
            if (!accountFound) 
            {
                cout << "Account not found!" << endl;
            }
            system("pause");
            system("cls");
            break;
        }
        case 7:
        {
            cout << "Exiting the system." << endl;
            return 0;
        }
        default:
        {
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
        }
    }

    return 0;
}

