// task9_test.cpp  –  Complete implementation with automated test table
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

enum class Zone { Inner = 0, Middle = 1, Outer = 2 };
constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;

long long computeFare(Zone z, bool peak) {
    long long base = BASE_PAISE[static_cast<int>(z)];
    if (peak) base += base * PEAK_SURCHARGE_PERCENT / 100;
    return base;
}

bool isPeak(int hour) {
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

struct TestCase {
    std::string name;
    Zone zone;
    int  hour;
    long long expectedFare;
    long long initialBalance;
    bool      expectedAllowed;
    long long expectedBalance;  // balance after journey (if allowed)
};

void runTests(const std::vector<TestCase>& tests) {
    std::cout << std::left
              << std::setw(35) << "Test Case"
              << std::setw(10) << "Exp Fare"
              << std::setw(10) << "Act Fare"
              << std::setw(10) << "Allowed?"
              << std::setw(12) << "Exp Bal"
              << std::setw(12) << "Act Bal"
              << "Result\n";
    std::cout << std::string(89, '-') << '\n';

    int passed = 0;
    for (const auto& tc : tests) {
        long long actualFare = computeFare(tc.zone, isPeak(tc.hour));
        long long balAfter = tc.initialBalance;
        bool allowed = false;
        if ((tc.initialBalance - actualFare) >= MIN_BALANCE_PAISE) {
            balAfter = tc.initialBalance - actualFare;
            allowed = true;
        }

        bool ok = (actualFare == tc.expectedFare) &&
                  (allowed    == tc.expectedAllowed) &&
                  (balAfter   == tc.expectedBalance);
        if (ok) ++passed;

        std::cout << std::left  << std::setw(35) << tc.name
                  << std::right << std::setw(8)  << tc.expectedFare
                  << "  "
                  << std::setw(8)  << actualFare
                  << "  "
                  << std::setw(8)  << (allowed ? "YES" : "NO")
                  << "  "
                  << std::setw(10) << tc.expectedBalance
                  << "  "
                  << std::setw(10) << balAfter
                  << "  " << (ok ? "PASS" : "FAIL") << '\n';
    }
    std::cout << std::string(89, '-') << '\n';
    std::cout << passed << "/" << tests.size() << " tests passed\n";
}

int main() {
    std::vector<TestCase> tests = {
        // name,                     zone,          hour, expFare, initBal, expAllow, expBal
        // Normal journeys
        {"Inner off-peak",        Zone::Inner,  12,   1000,  20000,  true,   19000},
        {"Middle off-peak",       Zone::Middle, 14,   2000,  20000,  true,   18000},
        {"Outer off-peak",        Zone::Outer,  11,   3000,  20000,  true,   17000},
        {"Inner peak (8AM)",      Zone::Inner,   8,   1200,  20000,  true,   18800},
        {"Outer peak (5PM)",      Zone::Outer,  17,   3600,  20000,  true,   16400},
        // Boundary: exactly 8:00 -> peak
        {"Boundary 8:00 (peak)",  Zone::Inner,   8,   1200,  20000,  true,   18800},
        // Boundary: exactly 10:00 -> off-peak
        {"Boundary 10:00 (off)",  Zone::Inner,  10,   1000,  20000,  true,   19000},
        // Boundary: exactly 17:00 -> peak
        {"Boundary 17:00 (peak)",Zone::Middle,  17,   2400,  20000,  true,   17600},
        // Boundary: exactly 20:00 -> off-peak
        {"Boundary 20:00 (off)", Zone::Middle,  20,   2000,  20000,  true,   18000},
        // Balance leaves exactly Rs 50
        {"Exact Rs50 remain",    Zone::Outer,   12,   3000,   8000,  true,    5000},
        // Not enough – would leave < Rs 50
        {"Balance too low",      Zone::Inner,   12,   1000,   5500,  false,   5500},
        // Exactly at the edge – balance - fare == 4999 paise (< 5000)
        {"Edge denied",          Zone::Outer,   12,   3000,   7999,  false,   7999},
        // Five-journey hand-total test (each uses a fresh 20000 balance card)
        {"5-journey #1 Inner off",Zone::Inner,  12,   1000,  20000,  true,   19000},
        {"5-journey #2 Outer pk", Zone::Outer,  17,   3600,  20000,  true,   16400},
        {"5-journey #3 Middle off",Zone::Middle,14,   2000,  20000,  true,   18000},
        {"5-journey #4 Inner pk", Zone::Inner,   9,   1200,  20000,  true,   18800},
        {"5-journey #5 Middle pk",Zone::Middle, 18,   2400,  20000,  true,   17600},
    };
    runTests(tests);

    // Cumulative 5-journey sequence
    std::cout << "\n=== Cumulative 5-Journey Sequence (start Rs 200) ===\n";
    long long bal = 20000;
    struct J { Zone z; int h; };
    J seq[] = {{Zone::Inner,12},{Zone::Outer,17},{Zone::Middle,14},{Zone::Inner,9},{Zone::Middle,18}};
    long long handTotal = 0;
    for (const auto& j : seq) {
        long long f = computeFare(j.z, isPeak(j.h));
        handTotal += f;
        if ((bal-f) >= MIN_BALANCE_PAISE) { bal -= f; }
    }
    std::cout << "Sum of fares (hand): " << (1000+3600+2000+1200+2400) << " paise = Rs "
              << (1000+3600+2000+1200+2400)/100 << ".00\n";
    std::cout << "Sum of fares (code): " << handTotal << " paise = Rs "
              << handTotal/100 << "." << std::setw(2) << std::setfill('0') << handTotal%100 << '\n';
    std::cout << "Final balance: Rs " << bal/100 << '\n';
}
