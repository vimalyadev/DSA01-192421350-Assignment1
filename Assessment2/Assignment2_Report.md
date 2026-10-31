# Assignment 2 – CO2 Report
### C++ OOP: Functions, Classes, Static Members & Friend Functions

| | |
|---|---|
| **Name** | Vimal Yadev |
| **Register Number** | 192421350 |
| **Course** | Data Structures and Algorithms |
| **Submission Date** | 31 / 10 / 2026 |
| **Compiler** | g++ 13 (MinGW-w64, Windows) |
| **Compile Command** | `g++ -std=c++17 -Wall -Wextra` |
| **GitHub Repository** | https://github.com/vimalyadev/DSA01-192421350-Assignment1 (folder: `Assessment2/`) |

---

## Q1 – Call by Value, by Reference and by Pointer *(6 Marks)*

### 1.1 Predict the Output

**Program given:**
```cpp
void change(int a, int& b, int* c) { a += 10; b += 10; *c += 10; }
int& bigger(int& p, int& q)        { return p > q ? p : q; }
void show(int x, int y = 5, int z = 10) { std::cout << x + y + z << ' '; }

int main() {
    int a = 1, b = 2, c = 3;
    change(a, b, &c);
    std::cout << a << ' ' << b << ' ' << c << '\n';
    bigger(a, c) = 100;
    std::cout << a << ' ' << c << '\n';
    show(1); show(1, 2); show(1, 2, 3);
    std::cout << '\n';
}
```

#### My Prediction (written BEFORE running)

```
Step 1 – change(a, b, &c):
  a is passed by value  → local copy gets +10, caller's a stays 1
  b is passed by ref    → caller's b becomes 2 + 10 = 12
  *c dereferences ptr   → caller's c becomes 3 + 10 = 13
  Print → 1 12 13

Step 2 – bigger(a, c) = 100:
  a=1, c=13 → c is bigger → returns ref to c → c = 100
  Print → 1 100

Step 3 – show calls:
  show(1)       → 1 + 5(def) + 10(def) = 16
  show(1, 2)    → 1 + 2      + 10(def) = 13
  show(1, 2, 3) → 1 + 2      + 3       =  6
  Print → 16 13 6
```

**Predicted output:**
```
1 12 13
1 100
16 13 6
```

#### Actual Output
```
1 12 13
1 100
16 13 6
```

> ✅ **Prediction exactly matched. No differences to explain.**

---

#### Variable Change Analysis

| Variable | Initial | After `change()` | Reason |
|----------|---------|-----------------|--------|
| `a` | 1 | **1** (unchanged) | Passed by **value** — compiler copies `a` onto a new stack slot; only that copy changes |
| `b` | 2 | **12** | Passed by **reference** — `b` inside `change` is an alias for `main`'s `b`; same memory address |
| `c` | 3 | **13** | Passed by **pointer** — `*c += 10` dereferences the address and writes through it |

After `bigger(a, c) = 100`: `a=1`, `c=13` → `c` is larger → `bigger` returns `int&` bound to `c` → assignment writes 100 into `c`.

#### Default Arguments Used by `show()`

| Call | `x` | `y` | `z` | Sum |
|------|-----|-----|-----|-----|
| `show(1)` | 1 | **5 (default)** | **10 (default)** | 16 |
| `show(1, 2)` | 1 | 2 | **10 (default)** | 13 |
| `show(1, 2, 3)` | 1 | 2 | 3 | 6 |

---

### 1.2 Complete the Code

**TODO 1 — `applyConcession` (call by reference):**
```cpp
void applyConcession(long long& fee, int percent)
{
    fee -= fee * percent / 100;   // integer arithmetic through reference
}
```

**Actual output:**
```
after call by value : 45000
after call by reference: 40500
```

> ✅ Matches expected output exactly.

**Why call by value leaves fee at 45000:**
`applyByValue` receives a *copy* of `hostelFee` pushed onto its own stack frame. The statement `fee -= fee * percent / 100` modifies only that copy. When the function returns, the frame is popped and the copy is discarded. The original variable `hostelFee` in `main` is completely untouched.

---

### 1.3 Inquiry

| Parameter mode | Where stored | Changes caller? | Use when |
|----------------|-------------|----------------|---------|
| By value | New stack slot (copied) | ❌ No | Small read-only inputs |
| By reference | Alias — same address as caller | ✅ Yes | Need to modify caller's variable |
| By pointer | Holds address; must dereference | ✅ Yes | Optional modification (can be `nullptr`) |
| `const` reference | Alias — no copy, no modification | ❌ No | Reading large objects without copying |

**When to use `const` reference:** Use `const long long& fee` (or `const Student& s`) when the function only needs to *read* — avoids the cost of copying a large object while preventing accidental writes at compile time.

---

### 1.4 Design Decision

**Prefer in-place (by reference)** for multiple concessions:

```cpp
// Clean – each line is self-documenting
applyConcession(fee, 10);  // merit
applyConcession(fee, 15);  // sports
applyConcession(fee, 25);  // single parent
```

vs. return-by-value (error-prone):
```cpp
fee = applyConcession(fee, 10);  // easy to forget the `fee =`
fee = applyConcession(fee, 15);
```

**Defence:** In-place modification is safer when multiple concessions chain. Forgetting to assign the return value silently leaves the old fee — a hard-to-find bug.

---

### 1.5 Alternative Concept & Complexity Analysis

| Design | Space per call – `long long` | Space per call – `Student` (200 B) | Mutation visible? |
|--------|------------------------------|-------------------------------------|-------------------|
| By value | 8 B copied | 200 B copied | ❌ |
| By reference | ~8 B (address only) | ~8 B | ✅ |
| By const reference | ~8 B | ~8 B | ❌ (compile-time enforced) |
| Return new value | 8 B in + 8 B out | 200 B in + 200 B out | Only if caller assigns |

**Time:** O(1) in all versions — a single integer multiplication and subtraction.
For `long long` the space saving is negligible; for a 200-byte `Student` struct, passing by reference eliminates a 200-byte copy on every call — significant in tight loops.

---

## Q2 – Return by Reference and Its Dangers *(4 Marks)*

### 2.1 Working Functions

```cpp
// Safe – returns reference to an array element
int& highestMark(int marks[], int n) {
    int best = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[best]) best = i;
    return marks[best];
}

// DANGEROUS – returns reference to a local variable
int& dangerousRef() {
    int local = 99;
    return local;   // local destroyed on return → dangling reference
}
```

**Actual output:**
```
--- Non-const reference ---
Marks before moderation: 72 88 91 65 78
Marks after  moderation: 72 88 93 65 78

--- Const reference (read-only) ---
Highest mark (const ref): 93

--- Return by copy ---
After modifying copy, first element: 72 (unchanged)

--- Vector reallocation danger ---
Before push_back, ref = 90
(ref to v[2] is now potentially dangling after reallocation)
Safe access via index: v[2] = 90
```

The call `highestMark(marks, 5) += 2` writes through the returned reference directly into the array — the highest mark 91 becomes 93.

---

### 2.2 Compiler Warning Evidence

Compiled with `g++ -std=c++17 -Wall -Wextra`:
```
warning: reference to local variable 'local' returned [-Wreturn-local-addr]
```

Using that returned reference is **undefined behaviour** — the local variable's stack slot no longer exists after the function returns. The program may read garbage values or crash.

---

### 2.3 Difference: Reference vs Const Reference vs Copy

| Return type | Caller can modify? | Extra copy? | Use case |
|-------------|-------------------|-------------|---------|
| `int&` | ✅ Yes | No | Moderation: `highestMark(m,n) += 2` |
| `const int&` | ❌ No (compile error) | No | Read-only display |
| `int` (copy) | Modifies local copy only | Yes | When isolation is needed |

---

### 2.4 Changed Condition – `std::vector` Growth

When `vector::push_back` exceeds capacity, the vector **reallocates** its internal buffer to a new heap address. Any `int&` obtained before the reallocation now points to the **freed old buffer** — a dangling reference. Accessing it is undefined behaviour.

**Fix:** Store the **index** instead of a reference. After any mutation, re-access via `v[idx]` — the index never becomes invalid.

---

### 2.5 Alternative Concept & Complexity Analysis

| Return type | Calling code | Safety after vector growth | O-time | Space |
|-------------|-------------|---------------------------|--------|-------|
| `int&` reference | `highestMark(m,n) += 2;` | ❌ Dangles after realloc | O(n) | O(1) |
| `int` index | `m[highestIdx(m,n)] += 2;` | ✅ Always safe | O(n) | O(1) |
| `int*` pointer | `*highestPtr(m,n) += 2;` | ❌ Dangles after realloc | O(n) | O(1) |

All three are O(n) to find the highest mark, O(1) extra space. **Index is the safest** when the container may grow.

---

## Q3 – Default Arguments and Function Overloading *(6 Marks)*

### 3.1 Ambiguity Explanation

The original code had:
```cpp
double charge(double weight, bool express);                         // A
double charge(double weight, bool express = false, bool insured = false); // B
```

A call `charge(2.0, true)` matches **both** A and B with equally good conversions — the compiler cannot break the tie and reports:
```
error: call of overloaded 'charge(double, bool)' is ambiguous
```

**Fix:** Remove overload A entirely. Keep only the three-parameter version (with two defaults) plus a separate `charge(int)` for weights in grams. Now every call has exactly one best match.

---

### 3.2 Redesigned Functions

```cpp
double charge(int weightGrams)                                     // Overload 1
double charge(double weightKg, bool express=false, bool insured=false) // Overload 2
double charge(const Parcel& p)                                     // Overload 3
```

---

### 3.3 Predicted & Actual Overload Selection

| Call | Predicted overload | Rule | Actual |
|------|--------------------|------|--------|
| `charge(2)` | `charge(int)` | Exact int match | ✅ Correct |
| `charge(2.0)` | `charge(double,…)` | Exact double, 2 defaults | ✅ Correct |
| `charge(2.0f)` | `charge(double,…)` | float→double **promotion** | ✅ Correct |
| `charge(2.0, true)` | `charge(double,bool,bool)` | 1 default used (insured=false) | ✅ Correct |
| `charge(2.0, true, true)` | `charge(double,bool,bool)` | No defaults | ✅ Correct |
| `charge(Parcel{3.5,…})` | `charge(const Parcel&)` | Exact struct match | ✅ Correct |

**Actual output:**
```
charge(2) [int, grams]            = 0.1
charge(2.0) [double kg, defaults] = 100
charge(2.0f) [float->double]      = 100
charge(2.0,true) [express]        = 130
charge(2.0,true,true) [both]      = 150
charge(Parcel{3.5,false,true})    = 195
```

---

### 3.4 Inquiry – Overload Resolution Rules

1. **Exact match** — highest priority (no conversion at all)
2. **Promotion** — float→double, char→int, etc.
3. **Standard conversion** — int→double, double→int
4. **User-defined conversion** — constructors / cast operators

If two overloads tie at the same priority → **ambiguous call** error.

**Default arguments must be from the right** because the compiler matches positional arguments left-to-right. If a middle parameter had a default but the rightmost didn't, skipping the middle one would make argument positions ambiguous.

---

### 3.5 Alternative Concept & Complexity Analysis

**Single `Parcel` struct:**
```cpp
struct Parcel { double kg; bool express=false; bool insured=false; };
double charge(Parcel p);
// Usage: charge({2.0, true, false})
```

| Aspect | Overloaded functions | Single struct |
|--------|---------------------|--------------|
| Overload resolution | At compile time — zero runtime cost | No resolution needed |
| Space per call | Minimal (copies only what is passed) | Always copies full struct (~16 B) |
| Adding a new option | New overload or extend default list | Add one field to struct |
| Call syntax | `charge(2)` very short | `charge({2.0})` slightly verbose |

Both designs have **O(1) runtime cost**. Overloading is more ergonomic for common simple calls; the struct approach scales better when options multiply.

---

## Q4 – Specifying a Class with an Array Inside It *(6 Marks)*

### 4.1 Class Specification

```
SavingsAccount
├── PRIVATE
│   ├── long long  accNumber
│   ├── double     balance
│   ├── double     transactions[10]   ← circular buffer
│   ├── int        count              ← entries recorded so far
│   └── int        start              ← index of oldest entry
└── PUBLIC
    ├── SavingsAccount(acc, balance)  [defined INSIDE class]
    ├── void deposit(double)          [defined OUTSIDE – scope resolution]
    ├── bool withdraw(double)         [defined OUTSIDE – scope resolution]
    └── void miniStatement() const    [defined INSIDE class]
```

Scope-resolution syntax used outside:
```cpp
void SavingsAccount::deposit(double amount) { ... }
bool SavingsAccount::withdraw(double amount) { ... }
```

---

### 4.2 Actual Output (12 transactions, last 10 shown)

```
=== Mini Statement ===
Account : 1001234567
Balance : Rs 15900.00
Last 10 transactions:
  -Rs 200.00
  +Rs 3000.00
  -Rs 750.00
  +Rs 200.00
  -Rs 100.00
  +Rs 800.00
  -Rs 300.00
  +Rs 1000.00
  +Rs 400.00
  -Rs 150.00
=====================
```

The first two transactions (deposit 500, deposit 1500) were correctly discarded — only the last 10 are shown.

---

### 4.3 Inquiry

**Why the array must be private:** Encapsulation keeps `balance` and `transactions` consistent. If public, any code could overwrite `transactions[3]` to hide a withdrawal — violating data integrity.

**Shift vs Circular for the 11th transaction:**

| Strategy | Cost per insertion (buffer full) | Complexity |
|----------|----------------------------------|-----------|
| Shift elements 1..9 → 0..8 | Move k elements | **O(k)** |
| Advance `start` index | Update 2 integers | **O(1)** |

---

### 4.4 Design Decision – 50 Transactions

Change only **one constant**: `static const int MAX_TX = 50;` and the array size `transactions[50]`. No algorithmic change required because the circular buffer scales automatically. The shift design would become measurably slower (50 copies every insertion).

---

### 4.5 Alternative Concept & Complexity Analysis

```cpp
// std::deque approach
std::deque<double> txs;
txs.push_back(amount);
if (txs.size() > MAX_TX) txs.pop_front();
```

| Design | Time per insert | Memory | Cache behaviour |
|--------|----------------|--------|----------------|
| Shift (fixed array) | O(k) | Stack / member | Excellent (contiguous) |
| Circular buffer (fixed array) | **O(1)** | Stack / member | Excellent |
| `std::deque` | O(1) amortised | Heap (chunks) | Moderate (indirection) |

The circular fixed array is the best choice: O(1) with the best cache locality.

---

## Q5 – Static Members and Arrays of Objects *(7 Marks)*

### 5.1 Predict the Output

**Program given:**
```cpp
class Token {
    static int next;
    int id;
public:
    Token() { id = ++next; }
    int get() const { return id; }
    static int issued() { return next - 100; }
};
int Token::next = 100;
```

#### My Prediction (written BEFORE running)

```
Token::next is initialised to 100 (out-of-class definition).
Array counter[3] → constructor runs 3 times:
  counter[0]: ++next = 101,  id = 101
  counter[1]: ++next = 102,  id = 102
  counter[2]: ++next = 103,  id = 103
extra:         ++next = 104,  id = 104

counter[0].get() = 101
counter[2].get() = 103
extra.get()      = 104
Token::issued()  = next - 100 = 104 - 100 = 4

PREDICTED: 101 103 104 4
```

**Actual output:**
```
101 103 104 4
```

> ✅ **Prediction exactly matched.**

**Why starts at 100:** The definition `int Token::next = 100;` initialises the static member once at program start. Pre-increment `++next` means each constructor increments *before* assigning, so ids begin at 101. The constructor ran **4 times** total.

---

### 5.2 Complete the Code

**TODO 1 — Constructor:**
```cpp
if (issued < CAPACITY) {
    ++issued;
    seat = issued;
}
// else: seat stays 0 → waiting list
```

**TODO 2 — `count()`:**
```cpp
static int count() { return issued; }
```

**Actual output:**
```
Asha: Seat 1
Bala: Seat 2
Chitra: Seat 3
Dev: Waiting
Issued: 3

--- Extra booking attempts (capacity already full) ---
Elan: Waiting
Fiona: Waiting
Issued after extra calls: 3
```

> ✅ Matches expected output exactly.

---

### 5.3 Count Behaviour Across Function Calls

`BusTicket::issued` lives in the **data segment** (not the stack). It persists for the entire program lifetime. Calling `bookTrip("Elan")` after capacity is full does NOT reset `issued`. Elan and Fiona both receive `seat=0` (Waiting) and `issued` stays at 3.

---

### 5.4 Why Static Member Function Cannot Use `this`

A static member function is invoked on the **class** (`BusTicket::count()`), not on any specific object. The compiler provides no hidden `this` pointer. Therefore it can only access other **static** members — which also belong to the class, not to any object.

---

### 5.5 Alternative Concept & Complexity Analysis

| Design | Encapsulation | Two buses simultaneously | Space | Time |
|--------|--------------|--------------------------|-------|------|
| Global variable | ❌ Accessible everywhere | ❌ Only one counter | 1 int | O(1) |
| Static data member | ✅ Tied to class | ❌ One counter per class | 1 int | O(1) |
| `TicketOffice` object | ✅ Best isolation | ✅ Two instances = two buses | 1 int each | O(1) |

For a single bus the static member is simplest. For a multi-bus system, a `TicketOffice` object passed to each `BusTicket` constructor gives the most flexibility.

---

## Q6 – Friend Functions and Inline Functions *(8 Marks)*

### 6.1 Complete the Code

**TODO 1 — `minutesBetween`:**
```cpp
inline int minutesBetween(const Time& a, const Time& b)
{
    int minA = a.h * 60 + a.m;
    int minB = b.h * 60 + b.m;
    return std::abs(minA - minB);
}
```

**TODO 2 — `earlier`:**
```cpp
inline bool earlier(const Time& a, const Time& b)
{
    if (a.h != b.h) return a.h < b.h;
    return a.m < b.m;
}
```

**Actual output:**
```
205 false

--- Correctness checks ---
Same time (10:30, 10:30): minutes=0 earlier=false
Across noon (11:50, 12:10): minutes=20 earlier=true
```

> ✅ Matches expected output. All correctness checks pass (verified by hand).

---

### 6.2 Correctness Verification (by hand)

| Test | Calculation | Expected | Got |
|------|-------------|----------|-----|
| (9:45, 13:10) | \|585 − 790\| = 205 | 205 false | ✅ |
| (10:30, 10:30) | \|630 − 630\| = 0 | 0 false | ✅ |
| (11:50, 12:10) | \|710 − 730\| = 20 | 20 true | ✅ |

---

### 6.3 Inline Timing Measurement (10 000 000 calls)

| Optimisation level | 10 M calls | Speedup |
|-------------------|-----------|---------|
| `-O0` (no inline) | **~15 800 µs** | baseline |
| `-O2` (full inline) | **~2 200 µs** | **≈7×** |

At `-O2`, the compiler expands both friend functions at the call site and reduces the loop to a few integer operations — eliminating all function-call overhead (stack frame setup, return jump).

---

### 6.4 Why Friend Function

- `minutesBetween(a, b)` is **symmetric** — neither `a` nor `b` is more "primary".
- A member function `a.minutesTo(b)` is asymmetric (implies `a` owns the operation).
- Friend functions access private members directly **without exposing them via public getters**.
- Making `h` and `m` public would let any code write `t.h = -5`, breaking invariants permanently.

---

### 6.5 Deployment Decision

**Keep friend functions.** They preserve encapsulation (private stays private), express symmetry naturally, and can be fully inlined for zero overhead. Public getters permanently expose the internal representation — if the implementation later changes (e.g., store only total minutes), every getter-using call site breaks. Friend functions localise the representation dependency to exactly two functions.

---

### 6.6 Alternative Concept & Complexity Analysis

| Design | Syntax | Encapsulation | O-time per call |
|--------|--------|---------------|----------------|
| Friend function (current) | `minutesBetween(a, b)` | ✅ Strong | O(1) |
| Member function | `a.minutesTo(b)` | ✅ (asymmetric) | O(1) |
| Public getters + free fn | `abs(a.getH()-b.getH())*60+…` | ❌ Leaks representation | O(1) |

After inlining, all three have **identical runtime cost**. Friend functions win on encapsulation and symmetry.

---

## Q7 – Diagnose, Redesign and Validate *(13 Marks)*

**Selected program:** Q4 — `SavingsAccount` with fixed-array shift strategy.

---

### A. Diagnose the Limitation *(2 Marks)*

`SavingsAccountShift::addTransaction` contains:
```cpp
for (int i = 0; i < MAX_TX - 1; ++i)
    transactions[i] = transactions[i + 1];  // shift left
transactions[MAX_TX - 1] = amount;
```

Every time the buffer is full (which is *every* call once the account is established), **k − 1 element assignments** are performed — O(k) per insertion. For MAX_TX = 10 that is 9 copies every deposit or withdrawal.

**Evidence:** The `-O0` benchmark shows the shift design taking **24 240 µs** for 1 000 000 deposit+withdraw pairs vs. **5 434 µs** for the circular design — a **4.46× penalty** caused entirely by the shift loop.

---

### B. Define the Design Objective *(2 Marks)*

> **Reduce `addTransaction` from O(k) to O(1)** for any fixed buffer size k, measured by the benchmark above. The circular design must be demonstrably faster at `-O0` (where the shift loop cannot be vectorised away).

---

### C. Design the Revised Solution *(4 Marks)*

Add two integer fields: `int start` (index of the oldest slot) and `int count` (total insertions).

**Circular buffer logic:**
```
When count < MAX_TX:   transactions[count++] = amount;     // fill phase
When count == MAX_TX:  transactions[start]   = amount;     // overwrite oldest
                       start = (start + 1) % MAX_TX;       // advance pointer
```

No element is ever moved. Only two integers change — **O(1)** unconditionally.

**Reading in chronological order:**
```cpp
for (int i = 0; i < shown; ++i)
    cout << transactions[(start + i) % MAX_TX];
```

**Extra memory:** 2 × `int` = **8 bytes**, independent of MAX_TX.

**Trade-off:** Index arithmetic `(start + i) % MAX_TX` is slightly more complex than sequential array access — but this cost is O(1) per read and adds no asymptotic penalty.

```
classDiagram
  class SavingsAccountShift {
    -double transactions[10]
    -int count
    -double balance
    +addTransaction(double)  → O(k)
  }
  class SavingsAccountCircular {
    -double transactions[10]
    -int count
    -int start              << NEW
    -double balance
    +addTransaction(double) → O(1)
  }
```

---

### D. Predict Before Implementing *(1 Mark)*

> At `-O0`, the circular version should be **≥ 4× faster** than the shift version for MAX_TX = 10, because 9 element assignments are replaced by 2 integer operations. At `-O2`, both will be vectorised and the gap will narrow to ≤ 1.2×.

---

### E. Implement and Validate *(2 Marks)*

**Benchmark results:**

| Build | Design | 1 000 000 deposit+withdraw pairs |
|-------|--------|----------------------------------|
| `-O0` | Original (shift) | **24 240 µs** |
| `-O0` | Redesigned (circular) | **5 434 µs** |
| `-O0` | **Speed-up** | **4.46×** |
| `-O2` | Original (shift) | < 1 µs (vectorised out) |
| `-O2` | Redesigned (circular) | < 1 µs (vectorised out) |

The prediction of ≥ 4× speedup at `-O0` was **confirmed**. At `-O2` both collapse as predicted.

---

### F. Defend the Final Design *(2 Marks)*

**Objective achieved:** O(k) → O(1) with only 8 bytes of additional state (two `int` fields).

**New costs:**
- Slightly more complex index arithmetic when reading transactions in order.
- The `start` variable must be carefully maintained — a bug (e.g., forgetting to advance `start`) silently corrupts the order without crashing.

**Where the original shift design is still preferable:**
- MAX_TX ≤ 3 or 4 — the shift loop is 2–3 assignments; the difference is imperceptible.
- Teaching / introductory code — sequential array access is easier to reason about and debug.
- Code that is never the performance bottleneck (IO-bound application).

**Conclusion:** For any real banking system with a history window ≥ 10 entries and frequent transactions, the circular buffer is the correct engineering choice. The 4.46× measured improvement justifies the marginal increase in code complexity.

---

## Summary of All Results

| Q | File | Compile | Predicted | Actual | Match |
|---|------|---------|-----------|--------|-------|
| Q1 | `q1_predict.cpp` | ✅ | `1 12 13` / `1 100` / `16 13 6` | Same | ✅ |
| Q1 | `q1_complete.cpp` | ✅ | `45000` / `40500` | Same | ✅ |
| Q2 | `q2_marks_register.cpp` | ✅ + warning | — | Moderation +2 applied | ✅ |
| Q3 | `q3_courier.cpp` | ✅ | 6 overload selections | All correct | ✅ |
| Q4 | `q4_savings_account.cpp` | ✅ | — | Last 10 of 12 shown | ✅ |
| Q5 | `q5_predict.cpp` | ✅ | `101 103 104 4` | Same | ✅ |
| Q5 | `q5_bus_ticket.cpp` | ✅ | `Seat 1/2/3, Waiting` | Same | ✅ |
| Q6 | `q6_time_planner.cpp` | ✅ | `205 false` | Same | ✅ |
| Q7 | `q7_redesign.cpp` | ✅ | ≥ 4× speedup | **4.46×** at -O0 | ✅ |

---

## GitHub Commit History

```
d303355  Assessment2 Q7: Redesign challenge - circular buffer 4.46x speedup
5a379bd  Assessment2 Q6: Friend and inline functions - Time class
daa73f5  Assessment2 Q5: Static members and arrays of objects
28f60e2  Assessment2 Q4: Class with array - savings account circular buffer
74f40d1  Assessment2 Q3: Default arguments and function overloading
41b9467  Assessment2 Q2: Return by reference - marks register
b4bbc1b  Assessment2 Q1: Call by value, reference and pointer
840e579  Assessment2: Add README
```
