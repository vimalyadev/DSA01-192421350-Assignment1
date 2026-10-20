// task3_control.cpp  –  Control flow: if/else, switch, for, while, do-while
// Applied to peak-hour detection and zone-based fare selection
#include <iostream>

enum class Zone { Inner = 0, Middle = 1, Outer = 2 };

constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr int       PEAK_PC      = 20;
constexpr long long MIN_BAL      = 5000;

// ── if / else ─────────────────────────────────────────────────────
bool isPeak(int hour)
{
    if (hour >= 8 && hour < 10) {
        return true;
    } else if (hour >= 17 && hour < 20) {
        return true;
    } else {
        return false;
    }
}

// ── switch ────────────────────────────────────────────────────────
long long baseFare(Zone z)
{
    switch (z) {
        case Zone::Inner:  return BASE_PAISE[0];
        case Zone::Middle: return BASE_PAISE[1];
        case Zone::Outer:  return BASE_PAISE[2];
        default:           return 0;
    }
}

int main()
{
    // ── Peak-hour table (for loop) ────────────────────────────────
    std::cout << "=== Peak-hour schedule ===\n";
    for (int h = 0; h < 24; ++h) {
        if (isPeak(h)) {
            std::cout << "  Hour " << h << ":00  -> PEAK\n";
        }
    }

    // ── Zone fare table (for loop over enum values) ───────────────
    std::cout << "\n=== Zone fare table ===\n";
    Zone zones[] = {Zone::Inner, Zone::Middle, Zone::Outer};
    const char* names[] = {"Inner", "Middle", "Outer"};
    for (int i = 0; i < 3; ++i) {
        long long b = baseFare(zones[i]);
        long long p = b + b * PEAK_PC / 100;
        std::cout << names[i]
                  << "  off-peak Rs " << b / 100
                  << "  peak Rs "     << p / 100 << '\n';
    }

    // ── Balance deduction (while loop) ────────────────────────────
    std::cout << "\n=== Simulate journeys until balance too low ===\n";
    long long balance = 10000; // Rs 100
    int trip = 0;
    while ((balance - BASE_PAISE[0]) >= MIN_BAL) {   // inner off-peak
        balance -= BASE_PAISE[0];
        ++trip;
    }
    std::cout << "Completed " << trip << " Inner off-peak trips\n";
    std::cout << "Remaining balance: Rs " << balance / 100 << '\n';

    // ── do-while: at least one attempt ────────────────────────────
    std::cout << "\n=== do-while: try outer peak once ===\n";
    long long outerPeak = BASE_PAISE[2] + BASE_PAISE[2] * PEAK_PC / 100; // Rs 36
    do {
        if ((balance - outerPeak) >= MIN_BAL) {
            balance -= outerPeak;
            std::cout << "Trip OK  balance Rs " << balance / 100 << '\n';
        } else {
            std::cout << "Denied   balance Rs " << balance / 100 << '\n';
            break;
        }
    } while ((balance - outerPeak) >= MIN_BAL);
}
