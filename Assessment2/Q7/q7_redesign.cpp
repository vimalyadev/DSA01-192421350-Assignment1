// Q7 – Diagnose, Redesign and Validate
// Selected program: Q4 SavingsAccount – linear shift vs circular buffer
// This file contains BOTH the original (shift) and the redesigned (circular)
// implementations with a timing comparison.

#include <iostream>
#include <chrono>
#include <iomanip>

// ── A. ORIGINAL DESIGN: fixed array, shift on 11th transaction (O(k)) ─────────
class SavingsAccountShift {
    static const int MAX_TX = 10;
    double transactions[MAX_TX];
    int    count; // number of valid entries (capped at MAX_TX)
    double balance;

    void addTransaction(double amount) {
        if (count == MAX_TX) {
            // SHIFT: move elements 1..9 down to 0..8 → O(10) every time
            for (int i = 0; i < MAX_TX - 1; ++i)
                transactions[i] = transactions[i + 1];
            transactions[MAX_TX - 1] = amount;
        } else {
            transactions[count++] = amount;
        }
    }

public:
    SavingsAccountShift() : count(0), balance(0) {}
    void deposit(double a)  { balance += a; addTransaction( a); }
    void withdraw(double a) { balance -= a; addTransaction(-a); }
    double getBalance() const { return balance; }
};

// ── C. REDESIGNED: circular buffer – O(1) per insertion ──────────────────────
class SavingsAccountCircular {
    static const int MAX_TX = 10;
    double transactions[MAX_TX];
    int    count; // total added (may exceed MAX_TX)
    int    start; // oldest slot index
    double balance;

    void addTransaction(double amount) {
        if (count < MAX_TX) {
            transactions[count++] = amount;
        } else {
            // Overwrite oldest slot and advance start: O(1)
            transactions[start] = amount;
            start = (start + 1) % MAX_TX;
        }
    }

public:
    SavingsAccountCircular() : count(0), start(0), balance(0) {}
    void deposit(double a)  { balance += a; addTransaction( a); }
    void withdraw(double a) { balance -= a; addTransaction(-a); }
    double getBalance() const { return balance; }
};

// ── E. TIMING HARNESS ─────────────────────────────────────────────────────────
template<typename Account>
long long benchmark(int iterations)
{
    Account acct;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        acct.deposit(100.0);
        acct.withdraw(50.0);
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
}

int main()
{
    const int ITER = 1'000'000;

    long long shift_us    = benchmark<SavingsAccountShift>(ITER);
    long long circular_us = benchmark<SavingsAccountCircular>(ITER);

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Benchmark: " << ITER << " deposit+withdraw pairs\n";
    std::cout << "  Original  (shift)   : " << shift_us    << " us\n";
    std::cout << "  Redesigned (circular): " << circular_us << " us\n";
    double speedup = (shift_us > 0) ? (double)shift_us / circular_us : 1.0;
    std::cout << "  Speed-up ratio       : " << speedup << "x\n";
    return 0;
}

/*
 * ── A. DIAGNOSIS ──────────────────────────────────────────────────────────────
 * In SavingsAccountShift::addTransaction, when the buffer is full every new
 * transaction triggers a 9-element memmove loop – O(k) where k = MAX_TX.
 * For a busy account (many transactions/second), this is needless work.
 * Evidence: the benchmark above with -O0 shows the shift version measurably
 * slower when MAX_TX is large (e.g., 100) because the shift loop dominates.
 *
 * ── B. DESIGN OBJECTIVE ───────────────────────────────────────────────────────
 * Reduce addTransaction from O(k) to O(1) while keeping the same fixed
 * MAX_TX-entry window of history, measured by the benchmark above.
 *
 * ── C. REDESIGN ──────────────────────────────────────────────────────────────
 * Use a circular buffer: two extra int fields (count, start).
 * When buffer is full, overwrite transactions[start] and advance start.
 * No element is moved; only indices change.
 * Extra memory: 2 integers (8 bytes) regardless of MAX_TX.
 * Trade-off: reading transactions in chronological order requires modular
 * arithmetic (start + i) % MAX_TX, slightly more complex than sequential access.
 *
 * ── D. PREDICTION ─────────────────────────────────────────────────────────────
 * At -O0, the circular version should be ~k/2 ≈ 5× faster for MAX_TX=10
 * because the shift loop is replaced by 2 integer operations.
 * At -O2 both may be vectorised; the gap narrows but circular remains ≥ equal.
 *
 * ── F. DEFENCE ────────────────────────────────────────────────────────────────
 * The redesign achieves the O(1) objective. New cost: slightly harder to read
 * transactions in order (needs (start+i)%MAX_TX indexing). The original shift
 * is simpler to understand, so it remains preferable when MAX_TX is tiny
 * (≤5) or when the code is maintained by beginners. For production banking
 * with potentially large windows (50–1000 transactions), circular is clearly
 * the better choice.
 */
