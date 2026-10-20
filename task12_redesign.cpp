// task12_redesign.cpp  –  Redesigned fare system:
//   4 zones (Inner, Middle, Outer, Airport)
//   Weekend 10% discount (does NOT combine with peak surcharge)
//   Monthly pass for 2 chosen zones (unlimited journeys)
#include <iomanip>
#include <iostream>
#include <set>
#include <string>
#include <vector>

// ============================================================
// CONSTANTS
// ============================================================
enum class Zone { Inner = 0, Middle = 1, Outer = 2, Airport = 3 };
constexpr int NUM_ZONES = 4;
constexpr long long BASE_PAISE[NUM_ZONES] = {1000, 2000, 3000, 6000};
constexpr int PEAK_SURCHARGE_PERCENT    = 20;
constexpr int WEEKEND_DISCOUNT_PERCENT  = 10;
constexpr long long MIN_BALANCE_PAISE   = 5000;

inline bool isPeak(int hour) {
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

// 0=Mon..6=Sun  (5,6 = weekend)
inline bool isWeekend(int dayOfWeek) {
    return dayOfWeek >= 5;
}

// ============================================================
// FARE TABLE (free function, pure logic)
// ============================================================
long long computeFare(Zone z, int hour, int dayOfWeek) {
    long long base = BASE_PAISE[static_cast<int>(z)];
    if (isWeekend(dayOfWeek)) {
        // Weekend discount – no peak surcharge
        return base - base * WEEKEND_DISCOUNT_PERCENT / 100;
    }
    if (isPeak(hour)) {
        return base + base * PEAK_SURCHARGE_PERCENT / 100;
    }
    return base;
}

// ============================================================
// MONTHLY PASS
// ============================================================
class MonthlyPass {
public:
    MonthlyPass(Zone z1, Zone z2) : zones_({z1, z2}) {}

    bool covers(Zone z) const {
        return zones_.count(z) > 0;
    }

private:
    std::set<Zone> zones_;
};

// ============================================================
// SMART CARD
// ============================================================
class SmartCard {
public:
    explicit SmartCard(long long initialPaise) : balance_(initialPaise) {}

    void attachPass(MonthlyPass p) { pass_ = p; hasPass_ = true; }

    bool canTravel(Zone z, long long fare) const {
        if (hasPass_ && pass_.covers(z)) return true;   // pass – no fare deducted
        return (balance_ - fare) >= MIN_BALANCE_PAISE;
    }

    // Returns fare paid (0 if pass covers zone), -1 if denied
    long long travel(Zone z, long long fare) {
        if (hasPass_ && pass_.covers(z)) return 0;       // free with pass
        if ((balance_ - fare) < MIN_BALANCE_PAISE) return -1;
        balance_ -= fare;
        return fare;
    }

    void recharge(long long amount) {
        if (amount <= 0) { std::cerr << "Bad recharge\n"; return; }
        balance_ += amount;
    }

    long long balance() const { return balance_; }

private:
    long long    balance_;
    bool         hasPass_ = false;
    MonthlyPass  pass_{Zone::Inner, Zone::Inner}; // dummy
};

// ============================================================
// HELPERS
// ============================================================
void printMoney(long long p) {
    std::cout << 'R' << 's' << ' ' << p/100 << '.'
              << std::setw(2) << std::setfill('0') << p%100 << std::setfill(' ');
}

const char* zoneName(Zone z) {
    switch(z) {
        case Zone::Inner:   return "Inner";
        case Zone::Middle:  return "Middle";
        case Zone::Outer:   return "Outer";
        case Zone::Airport: return "Airport";
    }
    return "?";
}

// ============================================================
// MAIN – demonstration test
// ============================================================
int main() {
    std::cout << "=== Namma Metro Redesigned Fare System ===\n\n";

    // Test matrix: {zone, hour, dayOfWeek(0=Mon,6=Sun), expectedFare description}
    struct T { Zone z; int h; int d; long long expFare; const char* desc; };
    T tests[] = {
        {Zone::Inner,   12, 0, 1000, "Inner weekday off-peak -> Rs10"},
        {Zone::Inner,    9, 0, 1200, "Inner weekday peak     -> Rs12"},
        {Zone::Outer,   11, 6,  2700, "Outer weekend discount -> Rs27"},
        {Zone::Airport, 11, 0, 6000, "Airport weekday off    -> Rs60"},
        {Zone::Airport,  9, 0, 7200, "Airport weekday peak   -> Rs72"},
        {Zone::Airport, 11, 6, 5400, "Airport weekend disc   -> Rs54"},
        {Zone::Middle,   9, 5, 1800, "Middle weekend (no pk) -> Rs18"},
    };

    std::cout << std::left << std::setw(40) << "Scenario"
              << std::setw(12) << "Exp (p)"
              << std::setw(12) << "Act (p)"
              << "Result\n";
    std::cout << std::string(70, '-') << '\n';
    int pass = 0;
    for (const auto& t : tests) {
        long long actual = computeFare(t.z, t.h, t.d);
        bool ok = actual == t.expFare;
        if (ok) ++pass;
        std::cout << std::left  << std::setw(40) << t.desc
                  << std::right << std::setw(10) << t.expFare << "  "
                  << std::setw(10) << actual << "  "
                  << (ok ? "PASS" : "FAIL") << '\n';
    }
    std::cout << std::string(70, '-') << '\n';
    std::cout << pass << "/" << std::size(tests) << " fare tests passed\n\n";

    // Monthly pass demonstration
    std::cout << "=== Monthly Pass Demo ===\n";
    SmartCard card(20000); // Rs 200
    MonthlyPass pass12(Zone::Inner, Zone::Middle);
    card.attachPass(pass12);

    auto journey = [&](Zone z, int h, int d) {
        long long f = computeFare(z, h, d);
        long long paid = card.travel(z, f);
        std::cout << "Zone " << zoneName(z);
        if (paid == 0)  std::cout << " [PASS – free]"
                                  << "  bal ";
        else if (paid < 0) std::cout << " [DENIED]       bal ";
        else { std::cout << " fare "; printMoney(paid); std::cout << "  bal "; }
        printMoney(card.balance()); std::cout << '\n';
    };

    journey(Zone::Inner,  12, 0);  // covered by pass – free
    journey(Zone::Middle,  9, 0);  // covered by pass – free
    journey(Zone::Outer,  11, 0);  // NOT covered – pay Rs 30
    journey(Zone::Airport,11, 0);  // NOT covered – pay Rs 60
}
