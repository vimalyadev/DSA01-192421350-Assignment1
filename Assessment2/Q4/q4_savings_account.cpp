// Q4 – Specifying a Class with an Array Inside It: Savings Account Mini-Statement
// Keeps last 10 transactions using a circular index (O(1) per insertion)
#include <iostream>
#include <string>
#include <iomanip>

class SavingsAccount {
private:
    long long      accNumber;
    double         balance;
    double         transactions[10]; // circular buffer
    int            count;            // total transactions ever recorded
    int            start;            // index of oldest entry in circular buffer

    static const int MAX_TX = 10;

    // Helper: add one record to the circular buffer
    void addTransaction(double amount) {
        int slot = (start + (count < MAX_TX ? count : 0)) % MAX_TX;
        if (count < MAX_TX) {
            transactions[slot] = amount;
            ++count;
        } else {
            // Buffer full: overwrite oldest slot and advance start
            transactions[start] = amount;
            start = (start + 1) % MAX_TX;
        }
    }

public:
    // Constructor defined inside the class
    SavingsAccount(long long acc, double initialBalance)
        : accNumber(acc), balance(initialBalance), count(0), start(0)
    {
        for (double& d : transactions) d = 0.0;
    }

    // deposit defined outside (see below)
    void deposit(double amount);

    // withdraw defined outside (see below)
    bool withdraw(double amount);

    // miniStatement defined inside the class
    void miniStatement() const {
        std::cout << "\n=== Mini Statement ===\n";
        std::cout << "Account : " << accNumber << '\n';
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Balance : Rs " << balance << '\n';
        int shown = (count < MAX_TX) ? count : MAX_TX;
        std::cout << "Last " << shown << " transactions:\n";
        for (int i = 0; i < shown; ++i) {
            int idx = (start + i) % MAX_TX;
            double t = transactions[idx];
            std::cout << "  " << (t >= 0 ? "+Rs " : "-Rs ")
                      << std::abs(t) << '\n';
        }
        std::cout << "=====================\n";
    }
};

// deposit defined OUTSIDE using scope resolution operator
void SavingsAccount::deposit(double amount) {
    if (amount <= 0) { std::cerr << "Invalid deposit amount.\n"; return; }
    balance += amount;
    addTransaction(amount);
}

// withdraw defined OUTSIDE using scope resolution operator
bool SavingsAccount::withdraw(double amount) {
    if (amount <= 0) { std::cerr << "Invalid withdrawal amount.\n"; return false; }
    if (amount > balance) { std::cerr << "Insufficient funds.\n"; return false; }
    balance -= amount;
    addTransaction(-amount);
    return true;
}

int main()
{
    SavingsAccount acct(1001234567, 10000.00);

    // Make 12 transactions to demonstrate circular overwrite of oldest
    acct.deposit(500);
    acct.deposit(1500);
    acct.withdraw(200);
    acct.deposit(3000);
    acct.withdraw(750);
    acct.deposit(200);
    acct.withdraw(100);
    acct.deposit(800);
    acct.withdraw(300);
    acct.deposit(1000);   // 10th  – buffer now full
    acct.deposit(400);    // 11th  – overwrites 1st (circular)
    acct.withdraw(150);   // 12th  – overwrites 2nd (circular)

    acct.miniStatement(); // should show last 10 only

    return 0;
}

/*
 * WHY THE ARRAY MUST BE PRIVATE:
 * Encapsulation ensures balance and transactions stay consistent.
 * If public, any code could overwrite transactions[3] to hide a withdrawal,
 * violating data integrity.
 *
 * SHIFTING vs CIRCULAR INDEX for 11th transaction:
 * Shift: move elements 1..9 to 0..8, place new at 9. Cost: O(k) = O(10) always.
 * Circular: just advance 'start' and overwrite one slot. Cost: O(1).
 *
 * FOR 50 TRANSACTIONS:
 * Change MAX_TX = 50 and the array size to 50 – only one constant changes.
 * The circular design scales with no other code changes.
 * A std::deque alternative: push_back + pop_front if size>MAX_TX → also O(1)
 * amortised, but uses heap memory and pointer indirection overhead.
 */
