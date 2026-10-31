// Q3 – Default Arguments and Function Overloading: Courier Charge Calculator
// Resolves the ambiguous-call compiler error and demonstrates overload resolution
#include <iostream>
#include <string>

// ── Parcel structure ─────────────────────────────────────────────────────────
struct Parcel {
    double weightKg;
    bool   express;
    bool   insured;
};

// ── Overloaded charge functions ───────────────────────────────────────────────
// Overload 1 – weight in grams (int), no express, no insurance
double charge(int weightGrams)
{
    double kg = weightGrams / 1000.0;
    return kg * 50.0; // Rs 50 per kg base
}

// Overload 2 – weight in kg (double), optional express/insurance
//   Default arguments given from RIGHT to LEFT (C++ rule)
double charge(double weightKg, bool express = false, bool insured = false)
{
    double total = weightKg * 50.0;
    if (express) total += 30.0;
    if (insured) total += 20.0;
    return total;
}

// Overload 3 – full Parcel object
double charge(const Parcel& p)
{
    return charge(p.weightKg, p.express, p.insured);
}

// ── Demonstration ─────────────────────────────────────────────────────────────
int main()
{
    // Call 1: charge(2)   => int literal => Overload 1 (exact int match)
    std::cout << "charge(2) [int, grams]           = " << charge(2)           << '\n';

    // Call 2: charge(2.0) => double literal => Overload 2, defaults applied
    std::cout << "charge(2.0) [double kg, defaults] = " << charge(2.0)         << '\n';

    // Call 3: charge(2.0f) => float => promoted to double => Overload 2
    std::cout << "charge(2.0f) [float->double]      = " << charge(2.0f)        << '\n';

    // Call 4: express only
    std::cout << "charge(2.0,true) [express]        = " << charge(2.0, true)   << '\n';

    // Call 5: express + insured
    std::cout << "charge(2.0,true,true) [both]      = " << charge(2.0,true,true)<< '\n';

    // Call 6: Parcel object
    Parcel p{3.5, false, true};
    std::cout << "charge(Parcel{3.5,false,true})    = " << charge(p)           << '\n';

    return 0;
}

/*
 * ── AMBIGUITY EXPLANATION ─────────────────────────────────────────────────────
 * The original code had:
 *   charge(double, bool)                  // overload A
 *   charge(double, bool, bool = false)    // overload B
 * A call like charge(2.0, true) matches BOTH A and B equally well.
 * The compiler cannot choose → "ambiguous call" error.
 * FIX: Remove overload A and keep only the three-parameter version with
 * two default arguments, or make the overloads differ in the type of the
 * first argument (int vs double vs Parcel) so every call has exactly ONE
 * best match.
 *
 * ── OVERLOAD RESOLUTION RULES ────────────────────────────────────────────────
 * 1. Exact match (e.g., int literal → int parameter)
 * 2. Promotion (float → double, short → int)
 * 3. Standard conversion (int → double)
 * If two overloads tie at the same level, the call is ambiguous.
 *
 * ── DEFAULT ARGUMENTS MUST BE FROM THE RIGHT ─────────────────────────────────
 * charge(double, bool = false, bool insured) is illegal because once a
 * parameter has a default, every parameter to its RIGHT must also have one.
 * This ensures the compiler can match positional arguments unambiguously.
 *
 * ── ALTERNATIVE: SINGLE Parcel STRUCT OR TEMPLATE ────────────────────────────
 * struct Parcel { double kg; bool express=false; bool insured=false; };
 * double charge(Parcel p) { ... }
 * Pros: one function, easy to add fields.
 * Cons: every call must construct a Parcel explicitly; no overload resolution.
 * Time: compile-time resolution → no runtime cost in both designs.
 * Space per call: int(4B) / double(8B) / Parcel(~16B) vs always Parcel(~16B).
 */
