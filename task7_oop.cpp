// task7_oop.cpp  –  Procedural vs Object-Oriented comparison
#include <iomanip>
#include <iostream>
#include <string>

// ============================================================
// PART A – PROCEDURAL STYLE
// ============================================================
enum class Zone { Inner = 0, Middle = 1, Outer = 2 };
constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr int    PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;

// free functions – procedural
long long proc_fare(Zone z, bool peak) {
    long long base = BASE_PAISE[static_cast<int>(z)];
    if (peak) base += base * PEAK_SURCHARGE_PERCENT / 100;
    return base;
}

bool proc_canTravel(long long balance, long long f) {
    return (balance - f) >= MIN_BALANCE_PAISE;
}

// ============================================================
// PART B – OBJECT-ORIENTED STYLE
// ============================================================
class SmartCard {
public:
    explicit SmartCard(long long initialPaise) : balance_(initialPaise) {}

    bool canTravel(long long fare) const {
        return (balance_ - fare) >= MIN_BALANCE_PAISE;
    }

    // Returns true if deduction was made; false if balance is too low
    bool deduct(long long fare) {
        if (!canTravel(fare)) return false;
        balance_ -= fare;
        return true;
    }

    long long balance() const { return balance_; }

    void recharge(long long amount) {
        if (amount <= 0) {
            std::cerr << "Error: recharge amount must be positive\n";
            return;
        }
        balance_ += amount;
    }

private:
    long long balance_;   // stored in paise; NEVER exposed directly
};

class FareTable {
public:
    static long long computeFare(Zone z, bool peak) {
        long long base = BASE_PAISE[static_cast<int>(z)];
        if (peak) base += base * PEAK_SURCHARGE_PERCENT / 100;
        return base;
    }
};

void printMoney(long long paise) {
    std::cout << 'R' << 's' << ' ' << paise / 100 << '.'
              << std::setw(2) << std::setfill('0') << paise % 100
              << std::setfill(' ');
}

int main() {
    std::cout << "=== PROCEDURAL STYLE ===\n";
    long long balance = 10000;
    struct Trip { Zone zone; bool peak; };
    Trip trips[] = {{Zone::Inner,false},{Zone::Outer,true},{Zone::Middle,false}};
    for (const auto& t : trips) {
        long long f = proc_fare(t.zone, t.peak);
        if (proc_canTravel(balance, f)) {
            balance -= f;
            std::cout << "Fare "; printMoney(f); std::cout << "  bal "; printMoney(balance);
        } else {
            std::cout << "Denied  bal "; printMoney(balance);
        }
        std::cout << '\n';
    }

    std::cout << "\n=== OOP STYLE ===\n";
    SmartCard card(10000); // Rs 100
    for (const auto& t : trips) {
        long long f = FareTable::computeFare(t.zone, t.peak);
        if (card.deduct(f)) {
            std::cout << "Fare "; printMoney(f); std::cout << "  bal "; printMoney(card.balance());
        } else {
            std::cout << "Denied  bal "; printMoney(card.balance());
        }
        std::cout << '\n';
    }

    std::cout << "\n--- Testing negative recharge (OOP prevents it) ---\n";
    card.recharge(-500);
    std::cout << "Balance after bad recharge: "; printMoney(card.balance()); std::cout << '\n';
}
