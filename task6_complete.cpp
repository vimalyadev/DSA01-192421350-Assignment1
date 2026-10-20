#include <iomanip>
#include <iostream>

enum class Zone { Inner, Middle, Outer };

constexpr long long BASE_PAISE[] = {1000, 2000, 3000}; // Rs 10, Rs 20, Rs 30
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000; // Rs 50 must remain on the card

long long fare(Zone z, bool peak)
{
    long long base = 0;
    switch (z) {
        case Zone::Inner:  base = BASE_PAISE[0]; break;
        case Zone::Middle: base = BASE_PAISE[1]; break;
        case Zone::Outer:  base = BASE_PAISE[2]; break;
    }
    if (peak) {
        base = base + base * PEAK_SURCHARGE_PERCENT / 100;
    }
    return base;
}

bool canTravel(long long balance, long long f)
{
    return (balance - f) >= MIN_BALANCE_PAISE;
}

int main()
{
    long long balance = 10000; // Rs 100.00 in paise
    struct Trip { Zone zone; bool peak; };
    Trip trips[] = {{Zone::Inner, false}, {Zone::Outer, true}, {Zone::Middle, false}};
    for (const Trip& t : trips) {
        long long f = fare(t.zone, t.peak);
        if (canTravel(balance, f)) {
            balance -= f;
            std::cout << "Fare Rs " << f / 100 << '.' << std::setw(2)
                      << std::setfill('0') << f % 100 << std::setfill(' ');
        } else {
            std::cout << "Denied (low balance)";
        }
        std::cout << "  balance Rs " << balance / 100 << '\n';
    }

    std::cout << "\n--- Boundary: 10 AM (should be OFF-peak) ---\n";
    // 10 AM: hour == 10, peak is 8<=h<10 OR 17<=h<20
    // 10 is NOT < 10, so NOT peak
    int hour = 10;
    bool isPeak = (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    std::cout << "Hour 10: isPeak = " << std::boolalpha << isPeak << '\n';

    hour = 20;
    isPeak = (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    std::cout << "Hour 20: isPeak = " << isPeak << '\n';

    hour = 8;
    isPeak = (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    std::cout << "Hour  8: isPeak = " << isPeak << '\n';

    hour = 17;
    isPeak = (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    std::cout << "Hour 17: isPeak = " << isPeak << '\n';
}
