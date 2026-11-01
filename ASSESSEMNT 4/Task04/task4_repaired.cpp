#include <iostream>
#include <vector>
#include <memory>

// Base Class: Polymorphic Account with Virtual Rate & Virtual Destructor
class Account {
public:
    virtual double rate() const { return 3.5; }
    virtual ~Account() {
        std::cout << "[~Account destructor executed]\n";
    }
};

// Derived Class: Senior Citizen Account
class SeniorAccount : public Account {
private:
    double* history; // Dynamic array allocation

public:
    SeniorAccount() : history(new double[100]) {
        for (int i = 0; i < 100; ++i) history[i] = 0.0;
        std::cout << "[SeniorAccount allocated 100 doubles on heap]\n";
    }

    double rate() const override { return 4.0; }

    ~SeniorAccount() override {
        std::cout << "[~SeniorAccount destructor executed: Releasing 100 doubles]\n";
        delete[] history;
    }
};

int main() {
    std::cout << "=== TASK 4: Forensics and Repaired Bank Account Hierarchy ===\n\n";

    std::cout << "--- 1. Polymorphic Array via Base-Class Pointers ---\n";
    std::vector<std::unique_ptr<Account>> accounts;
    accounts.push_back(std::make_unique<Account>());
    accounts.push_back(std::make_unique<SeniorAccount>());

    for (size_t i = 0; i < accounts.size(); ++i) {
        std::cout << "Account " << i << " Interest Rate: " << accounts[i]->rate() << "%\n";
    }

    std::cout << "\n--- 2. Destruction Trace (Zero Memory Leaks via Virtual Destructor) ---\n";
    accounts.clear(); // Invokes virtual destructors properly!

    std::cout << "\n--- 3. Slicing Demonstration vs Reference ---\n";
    SeniorAccount sa;
    Account sliced = sa; // Object slicing: Senior part truncated
    Account& ref = sa;   // Polymorphic reference: Preserves identity

    std::cout << "Sliced object rate: " << sliced.rate() << "% (Lost senior behavior)\n";
    std::cout << "Polymorphic ref rate: " << ref.rate() << "% (Senior behavior preserved!)\n";

    return 0;
}
