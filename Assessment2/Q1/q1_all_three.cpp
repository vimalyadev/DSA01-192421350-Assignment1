// Q1 – Three implementations: by value, by reference, by pointer
// Demonstrates which versions change the caller's fee
#include <iostream>

// 1. By value – does NOT change caller's fee
void concessionByValue(long long fee, int percent)
{
    fee -= fee * percent / 100;
    std::cout << "  [inside byValue]  local fee = " << fee << '\n';
}

// 2. By reference – CHANGES caller's fee
void concessionByRef(long long& fee, int percent)
{
    fee -= fee * percent / 100;
}

// 3. By pointer – CHANGES caller's fee through dereferencing
void concessionByPtr(long long* fee, int percent)
{
    *fee -= (*fee) * percent / 100;
}

int main()
{
    long long fee = 45000;
    std::cout << "Original fee       : " << fee << '\n';

    concessionByValue(fee, 10);
    std::cout << "After byValue  (10%): " << fee << "  (unchanged)\n";

    concessionByRef(fee, 10);
    std::cout << "After byRef    (10%): " << fee << "  (changed)\n";

    fee = 45000; // reset
    concessionByPtr(&fee, 15);
    std::cout << "After byPointer(15%): " << fee << "  (changed)\n";

    return 0;
}

/*
 * STORAGE EXPLANATION:
 * byValue  – parameter lives in its own stack frame; caller's variable untouched.
 * byRef    – parameter is an alias for the caller's variable; same memory location.
 * byPtr    – parameter holds the address; *fee dereferences to caller's memory.
 *
 * CONST REFERENCE USAGE:
 * Use const long long& fee when the function only needs to READ the fee
 * (e.g., for computing a display total) so large objects are not copied
 * but accidental modification is prevented at compile time.
 *
 * DESIGN DECISION – in-place vs return new value:
 * Several concessions can apply to one student, so changing in place
 * (by reference) is cleaner: applyConcession(fee, MERIT);
 *                                              applyConcession(fee, SPORTS);
 * The caller always sees the current balance without extra variables.
 * Returning a new value would force the caller to write:
 *   fee = applyC(fee, MERIT);
 *   fee = applyC(fee, SPORTS);
 * which is error-prone if a developer forgets to assign the return value.
 */
