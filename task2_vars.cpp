// task2_vars.cpp  –  Variables, constants, constexpr, and scope
// Models the named constants that drive the Namma Metro fare system
#include <iostream>

// ── File-scope named constants (constexpr preferred over #define) ──
constexpr long long BASE_PAISE_INNER  = 1000;   // Rs 10
constexpr long long BASE_PAISE_MIDDLE = 2000;   // Rs 20
constexpr long long BASE_PAISE_OUTER  = 3000;   // Rs 30
constexpr int       PEAK_SURCHARGE_PC = 20;     // 20 %
constexpr long long MIN_BALANCE_PAISE = 5000;   // Rs 50

int main()
{
    // ── Mutable variable vs immutable constant ────────────────────
    long long balance = 10000;          // mutable: balance changes
    const int cardId  = 192421350;     // immutable: card number fixed

    std::cout << "Card ID : " << cardId  << '\n';
    std::cout << "Balance : Rs " << balance / 100 << '\n';

    // ── constexpr evaluated at compile time ───────────────────────
    std::cout << "\n=== Fare table (compile-time constants) ===\n";
    std::cout << "Inner  base fare : Rs " << BASE_PAISE_INNER  / 100 << '\n';
    std::cout << "Middle base fare : Rs " << BASE_PAISE_MIDDLE / 100 << '\n';
    std::cout << "Outer  base fare : Rs " << BASE_PAISE_OUTER  / 100 << '\n';
    std::cout << "Peak surcharge   : "   << PEAK_SURCHARGE_PC        << "%\n";
    std::cout << "Min balance      : Rs " << MIN_BALANCE_PAISE / 100 << '\n';

    // ── Block scope ───────────────────────────────────────────────
    std::cout << "\n=== Scope demo ===\n";
    {
        long long peakFare = BASE_PAISE_INNER
                           + BASE_PAISE_INNER * PEAK_SURCHARGE_PC / 100;
        std::cout << "Inner peak fare (block scope): Rs "
                  << peakFare / 100 << '\n';
        balance -= peakFare;
    }
    // peakFare no longer accessible here
    std::cout << "Balance after Inner peak trip: Rs " << balance / 100 << '\n';

    // ── auto type deduction ───────────────────────────────────────
    auto trips = 1;          // deduced as int
    auto fare  = 1200LL;     // deduced as long long
    std::cout << "\nauto trips = " << trips
              << "  (sizeof=" << sizeof(trips) << ")\n";
    std::cout << "auto fare  = " << fare
              << "  (sizeof=" << sizeof(fare)  << ")\n";
}
