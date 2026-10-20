// task10_precision.cpp  –  Compare long long paise vs double rupees precision
#include <climits>
#include <iomanip>
#include <iostream>

int main() {
    std::cout << "=== Type Sizes and Ranges ===\n";
    std::cout << "sizeof(int)       = " << sizeof(int)       << " bytes, max = " << INT_MAX  << '\n';
    std::cout << "sizeof(long long) = " << sizeof(long long) << " bytes, max = " << LLONG_MAX << '\n';
    std::cout << "sizeof(double)    = " << sizeof(double)    << " bytes\n\n";

    // Average fare Rs 20 -> 2000 paise
    constexpr long long AVG_FARE = 2000;
    long long counts[] = {20000LL, 500000LL, 3000000LL};
    const char* labels[] = {"1 station (20k)", "1 line (500k)", "Network (3M)"};

    std::cout << "=== Daily Collection Totals ===\n";
    std::cout << std::left << std::setw(20) << "Scale"
              << std::setw(20) << "long long (paise)"
              << std::setw(20) << "double (rupees)"
              << "Difference (paise)\n";
    std::cout << std::string(75, '-') << '\n';

    for (int i = 0; i < 3; ++i) {
        long long ll_total = counts[i] * AVG_FARE;          // exact
        double    db_total = static_cast<double>(counts[i]) * (AVG_FARE / 100.0); // rupees
        // convert db_total back to paise for comparison
        long long db_as_paise = static_cast<long long>(db_total * 100.0 + 0.5);
        long long diff = ll_total - db_as_paise;

        std::cout << std::left << std::setw(20) << labels[i]
                  << std::setw(20) << ll_total
                  << std::fixed << std::setprecision(4) << std::setw(20) << db_total
                  << diff << '\n';
    }

    // int overflow check
    std::cout << "\n=== int Overflow Check ===\n";
    long long network_paise = 3000000LL * AVG_FARE;
    std::cout << "Network total (paise) = " << network_paise << '\n';
    std::cout << "INT_MAX               = " << INT_MAX << '\n';
    std::cout << "Overflows int?          " << (network_paise > INT_MAX ? "YES – use long long" : "No") << '\n';

    // Accumulated rounding error with double
    std::cout << "\n=== Accumulated Rounding Error (double) ===\n";
    double running = 0.0;
    long long ll_running = 0;
    double fare_rs = 20.0; // Rs 20
    long long fare_pa = 2000; // 2000 paise
    long long iters[] = {1000, 100000, 3000000};
    int prev = 0;
    for (long long n : iters) {
        for (long long j = prev; j < n; ++j) {
            running    += fare_rs;
            ll_running += fare_pa;
        }
        prev = static_cast<int>(n);
        double ll_as_rs = static_cast<double>(ll_running) / 100.0;
        std::cout << "n=" << std::setw(8) << n
                  << "  double=" << std::fixed << std::setprecision(6) << running
                  << "  ll=" << ll_running << " (Rs " << ll_as_rs << ")"
                  << "  err=" << (running - ll_as_rs) << '\n';
    }
}
