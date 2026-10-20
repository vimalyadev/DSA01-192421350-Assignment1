// task8_io.cpp  –  Safe I/O with cin validation and iomanip formatting
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

enum class Zone { Inner = 0, Middle = 1, Outer = 2 };
constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;

long long computeFare(Zone z, bool peak) {
    long long base = BASE_PAISE[static_cast<int>(z)];
    if (peak) base += base * PEAK_SURCHARGE_PERCENT / 100;
    return base;
}

bool readInt(const std::string& prompt, int& val, int lo, int hi) {
    while (true) {
        std::cout << prompt;
        std::cin >> val;
        if (std::cin.fail()) {
            std::cout << "  [Error] Non-numeric input. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (val < lo || val > hi) {
            std::cout << "  [Error] Value must be between " << lo << " and " << hi << ". Try again.\n";
        } else {
            return true;
        }
    }
}

void printReceipt(int journeyNo, Zone z, bool peak, long long fare, long long balance) {
    const char* zoneNames[] = {"Inner", "Middle", "Outer"};
    std::cout << "+----------------------------------+\n";
    std::cout << "| Journey #" << std::setw(24) << journeyNo << " |\n";
    std::cout << "| Zone   : " << std::left << std::setw(23) << zoneNames[static_cast<int>(z)] << " |\n";
    std::cout << "| Peak   : " << std::setw(23) << (peak ? "Yes" : "No") << " |\n";
    std::cout << "| Fare   : Rs " << std::right << std::setw(7) << fare/100 << '.' 
              << std::setw(2) << std::setfill('0') << fare%100 << std::setfill(' ') << "          |\n";
    std::cout << "| Balance: Rs " << std::setw(7) << balance/100 << '.' 
              << std::setw(2) << std::setfill('0') << balance%100 << std::setfill(' ') << "          |\n";
    std::cout << "+----------------------------------+\n";
}

int main() {
    long long balance = 20000; // Rs 200 initial balance
    int journeys = 0;

    std::cout << "=== Namma Metro Gate System ===\n";
    std::cout << "Initial balance: Rs " << balance/100 << ".00\n\n";
    std::cout << "Enter number of journeys (1-5): ";

    int n;
    if (!(std::cin >> n) || n < 1 || n > 5) {
        std::cout << "Invalid input. Defaulting to 3 journeys for demo.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        n = 3;
    }

    for (int i = 0; i < n; ++i) {
        std::cout << "\n--- Journey " << (i+1) << " ---\n";
        int zoneNum;
        readInt("Zone (0=Inner,1=Middle,2=Outer): ", zoneNum, 0, 2);
        int hour;
        readInt("Departure hour (0-23): ", hour, 0, 23);

        Zone z = static_cast<Zone>(zoneNum);
        bool peak = (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
        long long f = computeFare(z, peak);

        if ((balance - f) >= MIN_BALANCE_PAISE) {
            balance -= f;
            ++journeys;
            printReceipt(journeys, z, peak, f, balance);
        } else {
            std::cout << "[DENIED] Insufficient balance. Minimum Rs 50 must remain.\n";
        }
    }
    std::cout << "\nTotal journeys completed: " << journeys << '\n';
    std::cout << "Final balance: Rs " << balance/100 << '.' 
              << std::setw(2) << std::setfill('0') << balance%100 << '\n';
}
