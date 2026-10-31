# DSA01 – Assignment 2: C++ OOP Concepts

**Name:** [Your Name]  
**Register Number:** [Your Register Number]  
**Compiler:** g++ (MinGW-w64 / GCC)  
**Compiler Version:** g++ 13  
**Standard:** C++17  

---

## Compile Command (used for all tasks)

```
g++ -std=c++17 -Wall -Wextra -o <output> <source.cpp>
```

For timing benchmarks (Q6, Q7):
```
g++ -std=c++17 -Wall -Wextra -O0 -o <output_O0> <source.cpp>
g++ -std=c++17 -Wall -Wextra -O2 -o <output_O2> <source.cpp>
```

---

## Repository Structure

```
DSA01-<RegNo>-Assignment2/
├── Q1/  q1_predict.cpp  q1_complete.cpp  q1_all_three.cpp
├── Q2/  q2_marks_register.cpp
├── Q3/  q3_courier.cpp
├── Q4/  q4_savings_account.cpp
├── Q5/  q5_predict.cpp  q5_bus_ticket.cpp
├── Q6/  q6_time_planner.cpp
├── Q7/  q7_redesign.cpp
└── README.md
```

---

## Q1 – Call by Value, by Reference, and by Pointer

### Steps to Run

```
cd Q1
g++ -std=c++17 -Wall -Wextra -o q1_predict   q1_predict.cpp   && q1_predict
g++ -std=c++17 -Wall -Wextra -o q1_complete  q1_complete.cpp  && q1_complete
g++ -std=c++17 -Wall -Wextra -o q1_all       q1_all_three.cpp && q1_all
```

### Predicted Output (q1_predict.cpp) — Written BEFORE running

```
Step-by-step reasoning:
  a=1, b=2, c=3
  change(a, b, &c):
    a (copy)  += 10 → local only, caller's a stays 1
    b (ref)   += 10 → caller's b becomes 12
    *c        += 10 → caller's c becomes 13
  Print: 1 12 13

  bigger(a, c): a=1, c=13 → c is bigger → returns ref to c
  bigger(a, c) = 100 → c = 100
  Print: 1 100

  show(1):       x=1, y=5(default), z=10(default) → 1+5+10=16
  show(1,2):     x=1, y=2,          z=10(default) → 1+2+10=13
  show(1,2,3):   x=1, y=2,          z=3           → 1+2+3=6
  Print: 16 13 6
```

**Predicted:** `1 12 13` / `1 100` / `16 13 6`

### Actual Output

```
1 12 13
1 100
16 13 6
```

✅ **Prediction exactly matched.**

### Explanation

| Call | Variable | Reason |
|------|----------|--------|
| `change(a,b,&c)` | `a` unchanged | Passed by value — only local copy modified |
| `change(a,b,&c)` | `b` changed 2→12 | Passed by reference — alias to caller's variable |
| `change(a,b,&c)` | `c` changed 3→13 | Passed by pointer — `*c += 10` dereferences to caller's memory |
| `bigger(a,c)=100` | `c` changed 13→100 | `bigger` returns reference to `c` (larger); assignment writes through |
| `show(1)` | used y=5,z=10 | Both default arguments applied |
| `show(1,2)` | used z=10 | Only rightmost default applied |
| `show(1,2,3)` | no defaults | All arguments supplied explicitly |

### Complete the Code Result

```
after call by value : 45000
after call by reference: 40500
```

`applyByValue` receives a copy; its local `fee` changes to 40500 but the original `hostelFee` in `main` is untouched, so it stays 45000. `applyConcession` receives a reference (alias); `fee -= fee * percent / 100` writes directly to `hostelFee`, reducing it to 40500.

---

## Q2 – Return by Reference and Its Dangers

### Steps to Run

```
cd Q2
g++ -std=c++17 -Wall -Wextra -o q2 q2_marks_register.cpp && q2
```

### Result

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

**Compiler warning (intentional evidence):**
```
warning: reference to local variable 'local' returned [-Wreturn-local-addr]
```

### Key findings
- Returning `int&` from an array element is safe and allows `highestMark(marks,n) += 2` (LHS of assignment).
- Returning reference to a local variable is undefined behaviour; the compiler warns with `-Wall`.
- After `vector::push_back` may reallocate, any `int&` obtained before is dangling.
- Safe alternative: store the **index** rather than a reference when the container may grow.

---

## Q3 – Default Arguments and Function Overloading

### Steps to Run

```
cd Q3
g++ -std=c++17 -Wall -Wextra -o q3 q3_courier.cpp && q3
```

### Result

```
charge(2) [int, grams]           = 0.1
charge(2.0) [double kg, defaults] = 100
charge(2.0f) [float->double]      = 100
charge(2.0,true) [express]        = 130
charge(2.0,true,true) [both]      = 150
charge(Parcel{3.5,false,true})    = 195
```

### Overload Resolution Predictions (before running)

| Call | Predicted overload | Reason |
|------|--------------------|--------|
| `charge(2)` | `charge(int)` | Exact int match |
| `charge(2.0)` | `charge(double,bool,bool)` | Exact double match, defaults applied |
| `charge(2.0f)` | `charge(double,…)` | float promoted to double |
| `charge(2.0,true)` | `charge(double,bool,bool)` | one default (insured=false) used |
| `charge(2.0,true,true)` | `charge(double,bool,bool)` | no defaults |
| `charge(p)` | `charge(const Parcel&)` | exact Parcel match |

**Ambiguity explanation:** The original had both `charge(double,bool)` and `charge(double,bool,bool=false)`. `charge(2.0, true)` ties between them — the compiler cannot pick the "best" match → error. Fixed by removing the two-parameter overload.

---

## Q4 – Specifying a Class with an Array Inside It

### Steps to Run

```
cd Q4
g++ -std=c++17 -Wall -Wextra -o q4 q4_savings_account.cpp && q4
```

### Result

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

After 12 transactions the circular buffer correctly drops the two oldest (deposit 500, deposit 1500) and shows only the last 10.

---

## Q5 – Static Members and Arrays of Objects

### Steps to Run

```
cd Q5
g++ -std=c++17 -Wall -Wextra -o q5_predict q5_predict.cpp && q5_predict
g++ -std=c++17 -Wall -Wextra -o q5_bus     q5_bus_ticket.cpp && q5_bus
```

### Predicted Output (q5_predict.cpp) — Written BEFORE running

```
Token::next starts at 100.
counter[0]: ++next=101, id=101
counter[1]: ++next=102, id=102
counter[2]: ++next=103, id=103
extra:      ++next=104, id=104
counter[0].get()=101, counter[2].get()=103, extra.get()=104
Token::issued()=next-100=4

PREDICTED: 101 103 104 4
```

### Actual Output

```
101 103 104 4
```

✅ **Prediction exactly matched.**

### Complete the Code Result

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

`BusTicket::issued` is static — it persists between function calls. After capacity is reached, all further tickets get `seat=0` (Waiting) and `issued` stays at 3.

---

## Q6 – Friend Functions and Inline Functions

### Steps to Run

```
cd Q6
g++ -std=c++17 -Wall -Wextra -O0 -o q6_O0 q6_time_planner.cpp && q6_O0
g++ -std=c++17 -Wall -Wextra -O2 -o q6_O2 q6_time_planner.cpp && q6_O2
```

### Result

```
205 false
--- Correctness checks ---
Same time (10:30, 10:30): minutes=0 earlier=false
Across noon (11:50, 12:10): minutes=20 earlier=true
--- Timing 10,000,000 calls ---
-O0: 10M calls: ~15 800 µs
-O2: 10M calls: ~2 200 µs  (≈7× faster — compiler inlines and vectorises)
```

The -O2 speedup demonstrates that `inline` (plus optimisation) eliminates call overhead. At -O2 the compiler expands both friend functions at the call site.

---

## Q7 – Diagnose, Redesign and Validate

### Steps to Run

```
cd Q7
g++ -std=c++17 -Wall -Wextra -O0 -o q7_O0 q7_redesign.cpp && q7_O0
g++ -std=c++17 -Wall -Wextra -O2 -o q7_O2 q7_redesign.cpp && q7_O2
```

### Result (-O0, unoptimised — clearest measurement)

```
Benchmark: 1 000 000 deposit+withdraw pairs
  Original  (shift)   : 24 240 µs
  Redesigned (circular): 5 434 µs
  Speed-up ratio       : 4.46×
```

The circular buffer is ~4.5× faster than the shift design at -O0. At -O2 the compiler vectorises both loops and the difference collapses, confirming the analysis.

---

## Summary of Results

| Q | Program | Compiles | Output matches expected |
|---|---------|----------|------------------------|
| Q1 | q1_predict.cpp | ✅ | ✅ (1 12 13 / 1 100 / 16 13 6) |
| Q1 | q1_complete.cpp | ✅ | ✅ (45000 / 40500) |
| Q2 | q2_marks_register.cpp | ✅ (+ intentional warning) | ✅ |
| Q3 | q3_courier.cpp | ✅ | ✅ |
| Q4 | q4_savings_account.cpp | ✅ | ✅ |
| Q5 | q5_predict.cpp | ✅ | ✅ (101 103 104 4) |
| Q5 | q5_bus_ticket.cpp | ✅ | ✅ (Seat 1/2/3, Waiting) |
| Q6 | q6_time_planner.cpp | ✅ | ✅ (205 false) |
| Q7 | q7_redesign.cpp | ✅ | 4.46× speedup at -O0 |
