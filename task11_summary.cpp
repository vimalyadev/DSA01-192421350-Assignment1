// task11_summary.cpp  –  Integration summary: brings together all concepts
//   Types & literals, constexpr constants, control flow, OOP SmartCard,
//   formatted I/O, and a self-checking test table.
#include <iomanip>
#include <iostream>
#include <string>

// ── Constants (task2 concepts) ────────────────────────────────────
enum class Zone { Inner = 0, Middle = 1, Outer = 2 };
constexpr long long BASE_PAISE[]       = {1000, 2000, 3000};
constexpr int       PEAK_SURCHARGE_PC  = 20;
constexpr long long MIN_BALANCE_PAISE  = 5000;
const char* const   ZONE_NAMES[]       = {"Inner", "Middle", "Outer"};

// ── Free functions (task3 / task6 concepts) ───────────────────────
inline bool isPeak(int hour) {
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

long long computeFare(Zone z, bool peak) {
    long long base = BASE_PAISE[static_cast<int>(z)];
    if (peak) base += base * PEAK_SURCHARGE_PC / 100;
    return base;
}

// ── OOP SmartCard (task7 concepts) ───────────────────────────────
class SmartCard {
public:
    explicit SmartCard(long long initialPaise, std::string id)
        : balance_(initialPaise), id_(std::move(id)) {}

    bool travel(Zone z, int hour) {
        long long f = computeFare(z, isPeak(hour));
        if ((balance_ - f) < MIN_BALANCE_PAISE) return false;
        balance_ -= f;
        lastFare_ = f;
        ++tripCount_;
        return true;
    }

    void recharge(long long amount) {
        if (amount > 0) balance_ += amount;
    }

    long long balance()   const { return balance_;   }
    long long lastFare()  const { return lastFare_;  }
    int       tripCount() const { return tripCount_;  }
    const std::string& id() const { return id_; }

private:
    long long   balance_;
    long long   lastFare_  = 0;
    int         tripCount_ = 0;
    std::string id_;
};

// ── Formatted output helper (task8 concepts) ──────────────────────
void printMoney(long long paise) {
    std::cout << "Rs " << paise / 100 << '.'
              << std::setw(2) << std::setfill('0') << paise % 100
              << std::setfill(' ');
}

void printReceipt(const SmartCard& card, Zone z, int hour, bool ok) {
    std::cout << std::left  << std::setw(7) << ZONE_NAMES[static_cast<int>(z)]
              << " hr=" << std::setw(2) << hour
              << (isPeak(hour) ? " [PEAK]   " : " [off]    ");
    if (ok) {
        std::cout << "fare "; printMoney(card.lastFare());
        std::cout << "  bal ";  printMoney(card.balance());
    } else {
        std::cout << "DENIED          bal "; printMoney(card.balance());
    }
    std::cout << '\n';
}

// ── Self-checking test table (task9 concepts) ─────────────────────
void runSelfTest() {
    std::cout << "\n=== Self-check test table ===\n";
    struct TC { Zone z; int h; long long expFare; };
    TC tests[] = {
        {Zone::Inner,  12, 1000},   // off-peak
        {Zone::Inner,   8, 1200},   // peak
        {Zone::Middle, 17, 2400},   // peak
        {Zone::Outer,  11, 3000},   // off-peak
        {Zone::Outer,  18, 3600},   // peak
    };
    int pass = 0;
    for (const auto& t : tests) {
        long long actual = computeFare(t.z, isPeak(t.h));
        bool ok = (actual == t.expFare);
        if (ok) ++pass;
        std::cout << std::left << std::setw(8) << ZONE_NAMES[static_cast<int>(t.z)]
                  << " h=" << std::setw(2) << t.h
                  << "  exp=" << std::setw(5) << t.expFare
                  << "  act=" << std::setw(5) << actual
                  << "  " << (ok ? "PASS" : "FAIL") << '\n';
    }
    std::cout << pass << "/" << std::size(tests) << " tests passed\n";
}

// ── main ──────────────────────────────────────────────────────────
int main()
{
    std::cout << "╔══════════════════════════════════════╗\n"
              << "║   Namma Metro – Integration Summary  ║\n"
              << "╚══════════════════════════════════════╝\n\n";

    SmartCard card(15000, "192421350");  // Rs 150
    std::cout << "Card: " << card.id()
              << "  Initial balance: "; printMoney(card.balance());
    std::cout << "\n\n=== Journey log ===\n";

    // Simulate 5 journeys
    struct J { Zone z; int hour; };
    J journeys[] = {
        {Zone::Inner,  12},   // off-peak Rs 10
        {Zone::Outer,  17},   // peak     Rs 36
        {Zone::Middle, 14},   // off-peak Rs 20
        {Zone::Inner,   9},   // peak     Rs 12
        {Zone::Outer,  11},   // off-peak Rs 30 – may be denied
    };
    for (const auto& j : journeys) {
        bool ok = card.travel(j.z, j.hour);
        printReceipt(card, j.z, j.hour, ok);
    }

    std::cout << "\nTotal trips completed : " << card.tripCount() << '\n';
    std::cout << "Final balance         : "; printMoney(card.balance());
    std::cout << '\n';

    runSelfTest();
}
