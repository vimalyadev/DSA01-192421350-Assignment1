// Q6 – Friend Functions and Inline Functions: Train Journey Planner
// TODO 1 and TODO 2 completed; includes timing measurement for inline vs non-inline
#include <iostream>
#include <chrono>
#include <cstdlib> // abs

class Time {
    int h, m;

public:
    Time(int hh, int mm) : h(hh), m(mm) {}

    // Declare friend functions (can access private h and m)
    friend int  minutesBetween(const Time& a, const Time& b);
    friend bool earlier(const Time& a, const Time& b);
};

// TODO 1 COMPLETED – absolute minute difference between two times
inline int minutesBetween(const Time& a, const Time& b)
{
    int minA = a.h * 60 + a.m;
    int minB = b.h * 60 + b.m;
    return std::abs(minA - minB);
}

// TODO 2 COMPLETED – true if a is earlier in the day than b
inline bool earlier(const Time& a, const Time& b)
{
    if (a.h != b.h) return a.h < b.h;
    return a.m < b.m;
}

int main()
{
    // --- Original test ---
    Time departure(9, 45), arrival(13, 10);
    std::cout << minutesBetween(departure, arrival) << ' '
              << std::boolalpha << earlier(arrival, departure) << '\n';
    // Expected: 205 false

    // --- Correctness checks ---
    std::cout << "\n--- Correctness checks ---\n";

    // Same time twice
    Time t1(10, 30), t2(10, 30);
    std::cout << "Same time (10:30, 10:30): minutes=" << minutesBetween(t1, t2)
              << " earlier=" << earlier(t1, t2) << '\n';
    // Expected: 0 false

    // Either side of noon
    Time morning(11, 50), afternoon(12, 10);
    std::cout << "Across noon (11:50, 12:10): minutes=" << minutesBetween(morning, afternoon)
              << " earlier=" << earlier(morning, afternoon) << '\n';
    // Expected: 20 true

    // --- Timing: 10,000,000 calls ---
    std::cout << "\n--- Timing 10,000,000 calls ---\n";
    Time a(9, 0), b(17, 30);
    volatile int sink = 0; // prevent dead-code elimination

    auto t_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10'000'000; ++i)
        sink += minutesBetween(a, b);
    auto t_end = std::chrono::high_resolution_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count();
    std::cout << "10M calls: " << ms << " microseconds (sink=" << sink << ")\n";
    std::cout << "(Compile with -O0 vs -O2 to observe inlining effect)\n";

    return 0;
}

/*
 * WHY A FRIEND FUNCTION:
 * minutesBetween and earlier operate symmetrically on TWO Time objects.
 * Writing as a member function: departure.minutesTo(arrival) is asymmetric –
 * it suggests 'departure' is the primary object, which is arbitrary.
 * A friend free function treats both arguments equally and reads more naturally.
 *
 * FRIEND vs MEMBER ACCESS:
 * Member: called as obj.func(other); accesses 'this->h' directly.
 * Friend: called as func(a, b);      accesses a.h and b.h explicitly.
 * Both can reach private members; friend just isn't tied to one receiver.
 *
 * INLINE:
 * 'inline' is a hint to the compiler to expand the function body at the call
 * site, avoiding function-call overhead (stack frame setup, return jump).
 * At -O2 the compiler may inline automatically even without the keyword;
 * at -O0 the keyword has more visible effect.
 * Measurement: typically 2-5× faster at -O2 than at -O0 for tight loops.
 *
 * WHAT WOULD BE LOST IF private BECAME public:
 * Any code could freely set t.h = -5 or t.m = 90, breaking invariants.
 * Encapsulation is lost; the class becomes a plain struct.
 *
 * DEPLOYMENT DECISION:
 * Keep friend functions. They preserve encapsulation (private stays private),
 * express symmetry naturally, and can be inlined for zero overhead.
 * Getters (int getH() const) would expose internal representation permanently;
 * if the internal format ever changes (e.g., store total minutes only),
 * every getter-based call site breaks. Friend functions localise the
 * representation dependency to just those two functions.
 *
 * ALTERNATIVE – member functions / getters:
 * int minutesTo(const Time& other) const { return abs((h-other.h)*60 + (m-other.m)); }
 * Works but is asymmetric. Also requires getters if kept as non-friend free functions,
 * which leaks representation.
 */
