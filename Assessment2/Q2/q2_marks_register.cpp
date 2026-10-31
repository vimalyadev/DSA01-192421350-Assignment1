// Q2 – Return by Reference and Its Dangers
// Marks Register: highest mark returned by reference; dangling reference demo
#include <iostream>
#include <vector>

const int CLASS_SIZE = 5;

// --- Part A: Return reference to array element (safe) ---
int& highestMark(int marks[], int n)
{
    int best = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[best]) best = i;
    return marks[best]; // reference to element inside the caller's array
}

// Return a const reference – caller can READ but not modify
const int& highestMarkConst(const int marks[], int n)
{
    int best = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[best]) best = i;
    return marks[best];
}

// Return by copy (no alias)
int highestMarkCopy(const int marks[], int n)
{
    int best = marks[0];
    for (int i = 1; i < n; ++i)
        if (marks[i] > best) best = marks[i];
    return best;
}

// --- Part B: DANGEROUS – return reference to local variable ---
// Compiler will warn: warning: reference to local variable 'local' returned
int& dangerousRef()
{
    int local = 99;
    return local; // local is destroyed when function returns => dangling reference
}

// --- Part C: std::vector grown after reference captured ---
void vectorGrowthDemo()
{
    std::vector<int> v = {70, 85, 90, 60};
    int& ref = v[2]; // reference into internal buffer
    std::cout << "Before push_back, ref = " << ref << '\n';
    v.push_back(95); // may reallocate; ref now INVALID (dangling)
    // Accessing ref here is undefined behaviour; commented out for safety:
    // std::cout << "After push_back, ref = " << ref << '\n'; // UB!
    std::cout << "(ref to v[2] is now potentially dangling after reallocation)\n";
    std::cout << "Safe access via index: v[2] = " << v[2] << '\n';
}

int main()
{
    int marks[CLASS_SIZE] = {72, 88, 91, 65, 78};

    // 1. Modify highest mark via reference (moderation +2)
    std::cout << "--- Non-const reference ---\n";
    std::cout << "Marks before moderation: ";
    for (int m : marks) std::cout << m << ' ';
    std::cout << '\n';

    highestMark(marks, CLASS_SIZE) += 2; // call on LEFT of =
    std::cout << "Marks after  moderation: ";
    for (int m : marks) std::cout << m << ' ';
    std::cout << '\n';

    // 2. Const reference – read only
    std::cout << "\n--- Const reference (read-only) ---\n";
    std::cout << "Highest mark (const ref): " << highestMarkConst(marks, CLASS_SIZE) << '\n';

    // 3. Copy – modifying return value has no effect on array
    std::cout << "\n--- Return by copy ---\n";
    int copy = highestMarkCopy(marks, CLASS_SIZE);
    copy += 5; // modifies local copy only, array unchanged
    std::cout << "After modifying copy, first element: " << marks[0] << " (unchanged)\n";

    // 4. Vector growth invalidation
    std::cout << "\n--- Vector reallocation danger ---\n";
    vectorGrowthDemo();

    return 0;
}

/*
 * COMPILER WARNING (when dangerousRef is compiled with -Wall):
 *   warning: reference to local variable 'local' returned [-Wreturn-local-addr]
 * Using the returned reference is undefined behaviour because 'local' no longer
 * exists on the stack after dangerousRef() returns.
 *
 * ALTERNATIVES (index vs pointer vs reference):
 *   int index approach: int idx = highestIdx(marks,n); marks[idx]+=2;
 *   int* ptr approach : *highestPtr(marks,n) += 2;
 *   int& ref approach : highestMark(marks,n) += 2;   (most expressive)
 *
 * All three: O(n) time to find highest, O(1) extra space.
 * Safety with growing vector:
 *   index – always safe (recompute marks[idx] after growth).
 *   pointer & reference – may dangle after vector reallocation.
 *   => prefer index or always re-call the finder after mutations.
 */
