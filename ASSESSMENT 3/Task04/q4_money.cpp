#include <iostream>
#include <iomanip>
#include <string>

class Money {
private:
    long long paise_; // Stored in exact paise

public:
    // Constructors
    Money() : paise_(0) {}
    explicit Money(long long paise) : paise_(paise) {}
    Money(long long rupees, int paise) : paise_(rupees * 100 + paise) {}

    // Arithmetic operators
    Money operator+(const Money& other) const {
        return Money(paise_ + other.paise_);
    }

    Money operator-(const Money& other) const {
        return Money(paise_ - other.paise_);
    }

    Money& operator+=(const Money& other) {
        paise_ += other.paise_;
        return *this;
    }

    // Comparison operators
    bool operator==(const Money& other) const {
        return paise_ == other.paise_;
    }

    bool operator<(const Money& other) const {
        return paise_ < other.paise_;
    }

    // Friend stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        long long r = m.paise_ / 100;
        long long p = m.paise_ % 100;
        if (p < 0) p = -p;
        os << "Rs " << r << '.' << std::setw(2) << std::setfill('0') << p << std::setfill(' ');
        return os;
    }

    // Friend stream extraction operator
    friend std::istream& operator>>(std::istream& is, Money& m) {
        double rupees;
        if (is >> rupees) {
            m.paise_ = static_cast<long long>(rupees * 100.0 + (rupees >= 0 ? 0.5 : -0.5));
        }
        return is;
    }

    long long getPaise() const { return paise_; }
};

int main() {
    std::cout << "=== Q4: Money Arithmetic for Billing Counter ===\n\n";

    // Expected output verification:
    // Rs 200.00
    // true
    Money item1(120, 50); // Rs 120.50
    Money item2(79, 50);  // Rs 79.50
    Money total = item1 + item2;
    std::cout << total << '\n';
    std::cout << std::boolalpha << (total == Money(200, 0)) << '\n';

    // Additional requirement: Test where paise carry into rupees (Rs 0.75 + Rs 0.50)
    std::cout << "\n--- Carry-over Test (Rs 0.75 + Rs 0.50) ---\n";
    Money p1(0, 75);
    Money p2(0, 50);
    Money pSum = p1 + p2;
    std::cout << p1 << " + " << p2 << " = " << pSum << " (Exact: Rs 1.25)\n";

    // Complete billing calculation
    std::cout << "\n--- Complete Billing Scenario ---\n";
    Money subtotal(0, 0);
    subtotal += Money(45, 0);   // Item A
    subtotal += Money(120, 25); // Item B
    subtotal += Money(34, 75);  // Item C
    std::cout << "Subtotal: " << subtotal << '\n';

    Money discount(15, 0);
    Money finalBill = subtotal - discount;
    std::cout << "Discount: " << discount << '\n';
    std::cout << "Final Bill: " << finalBill << '\n';
    std::cout << "Is final bill > Rs 150? " << (Money(150, 0) < finalBill) << '\n';

    return 0;
}
