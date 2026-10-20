// task1_types.cpp  –  C++ fundamental types: sizes, ranges, and literals
// Demonstrates why long long is chosen for paise-based fare storage
#include <climits>
#include <iostream>

int main()
{
    // ── Type sizes ────────────────────────────────────────────────
    std::cout << "=== sizeof each type ===\n";
    std::cout << "char        : " << sizeof(char)        << " byte(s)\n";
    std::cout << "short       : " << sizeof(short)       << " byte(s)\n";
    std::cout << "int         : " << sizeof(int)         << " byte(s)\n";
    std::cout << "long        : " << sizeof(long)        << " byte(s)\n";
    std::cout << "long long   : " << sizeof(long long)   << " byte(s)\n";
    std::cout << "float       : " << sizeof(float)       << " byte(s)\n";
    std::cout << "double      : " << sizeof(double)      << " byte(s)\n";
    std::cout << "bool        : " << sizeof(bool)        << " byte(s)\n\n";

    // ── Ranges ────────────────────────────────────────────────────
    std::cout << "=== Key ranges ===\n";
    std::cout << "INT_MAX     : " << INT_MAX  << '\n';
    std::cout << "LLONG_MAX   : " << LLONG_MAX << '\n';

    // ── Integer literals ──────────────────────────────────────────
    std::cout << "\n=== Integer literal forms ===\n";
    int decimal     = 255;
    int octal       = 0377;        // leading 0
    int hexadecimal = 0xFF;        // leading 0x
    std::cout << "decimal 255  = " << decimal     << '\n';
    std::cout << "octal 0377   = " << octal       << '\n';
    std::cout << "hex 0xFF     = " << hexadecimal << '\n';

    // ── Why paise? ────────────────────────────────────────────────
    // Rs 10 fare = 1000 paise; a city network does ~3 million trips/day
    long long daily_collection = 3'000'000LL * 2000LL;  // avg Rs 20 = 2000 paise
    std::cout << "\n=== Daily collection (3M trips @ Rs 20 avg) ===\n";
    std::cout << "In paise (long long): " << daily_collection << '\n';
    std::cout << "Fits in int?          " << (daily_collection <= INT_MAX ? "Yes" : "No – use long long") << '\n';
}
