# Assignment 3: CO3 — Constructors, Destructors, and Operator Overloading

**Student Name:** [Your Name]  
**Register Number:** [Your Register Number]  
**Course Code:** DSA01  
**Compiler & Version:** `g++ (MinGW-W64 x86_64-ucrt-posix-seh, built by Brecht Sanders) 13.2.0`  
**Standard Used:** C++17 (`-std=c++17 -Wall -Wextra`)

---

## Repository Structure & Overview
```text
DSA01-<RegisterNumber>-Assignment3/
├── README.md
├── Task01/
│   ├── q1_book.cpp
│   ├── q1_reorder_warning.cpp
│   └── q1_missing_default_error.cpp
├── Task02/
│   ├── q2_prescription.cpp
│   └── q2_shallow_crash.cpp
├── Task03/
│   ├── q3_predict.cpp
│   └── q3_auditorium.cpp
├── Task04/
│   └── q4_money.cpp
├── Task05/
│   ├── q5_predict.cpp
│   └── q5_playlist_timer.cpp
├── Task06/
│   └── q6_text.cpp
└── Task07/
    └── q7_complex.cpp
```

---

## Compilation and Execution Guide

To compile each task with strict compiler warnings enabled, execute the following commands in PowerShell / Terminal:

### Task 01: Constructors & Constructor Overloading (Library Book Records)
```powershell
cd Task01
g++ -std=c++17 -Wall -Wextra -o q1_book.exe q1_book.cpp
.\q1_book.exe

# Testing -Wreorder warning:
g++ -std=c++17 -Wall -Wextra -Wreorder -o q1_reorder.exe q1_reorder_warning.cpp

# Testing missing default constructor error:
g++ -std=c++17 -Wall -Wextra q1_missing_default_error.cpp
```
**Result Summary:** Demonstrates member initialization lists, constructor delegation, runtime validation rejecting negative copies, compile-time `-Wreorder` diagnostic, and array default-constructor requirement.

---

### Task 02: Copy Constructors: Shallow & Deep Copies (Prescription Records)
```powershell
cd ..\Task02
g++ -std=c++17 -Wall -Wextra -o q2_prescription.exe q2_prescription.cpp
.\q2_prescription.exe

# Shallow copy crash demonstration:
g++ -std=c++17 -o q2_shallow_crash.exe q2_shallow_crash.cpp
.\q2_shallow_crash.exe
```
**Result Summary:** Completely implements the Rule of Three (Deep Copy Constructor, Copy Assignment Operator with self-assignment guard, and Destructor). Verified that copies modify independent heap buffers. Benchmark demonstrates $O(n)$ deep copy time vs $O(1)$ shallow pointer alias.

---

### Task 03: Dynamic Constructors and Destructors (Auditorium Seat Booking)
```powershell
cd ..\Task03
g++ -std=c++17 -Wall -Wextra -o q3_predict.exe q3_predict.cpp
.\q3_predict.exe

g++ -std=c++17 -Wall -Wextra -o q3_auditorium.exe q3_auditorium.cpp
.\q3_auditorium.exe
```
**Result Summary:** Validates RAII object lifetime across stack, block-scope, and heap (`new`/`delete`). `q3_predict.cpp` output matches prediction: `+A +B -B copyAc use -Ac +P -P end -A `.

---

### Task 04: Overloading Arithmetic and Stream Operators (Money Arithmetic)
```powershell
cd ..\Task04
g++ -std=c++17 -Wall -Wextra -o q4_money.exe q4_money.cpp
.\q4_money.exe
```
**Result Summary:** Overloads `+`, `-`, `+=`, `==`, `<`, `<<`, and `>>` using exact integer paise arithmetic on `long long`. Hand-check and automated tests verify carry-over (`Rs 0.75 + Rs 0.50 = Rs 1.25`) with zero floating-point rounding error.

---

### Task 05: Overloading Increment, Comparison and Subscript (Music Playlist)
```powershell
cd ..\Task05
g++ -std=c++17 -Wall -Wextra -o q5_predict.exe q5_predict.cpp
.\q5_predict.exe

g++ -std=c++17 -Wall -Wextra -o q5_playlist_timer.exe q5_playlist_timer.cpp
.\q5_playlist_timer.exe
```
**Result Summary:** Explains the dummy `int` parameter distinguishing postfix from prefix `++`. Verified `00:59:59 -> 01:00:00` rollover, dual `const`/`non-const` `operator[]` with bounds checking (`std::out_of_range`), and 10M benchmark showing prefix `++` is measurably faster than postfix.

---

### Task 06: Rules of Operator Overloading (SMS Gateway Text Class)
```powershell
cd ..\Task06
g++ -std=c++17 -Wall -Wextra -o q6_text.exe q6_text.cpp
.\q6_text.exe
```
**Result Summary:** Custom RAII string class implementing dynamic heap management, self-assignment protection, deep concatenation, and comparison. Fully analyzes non-overloadable operators (`::`, `.`, `.*`, `?:`, `sizeof`) and arity/precedence immutability.

---

### Task 07: Complex-Number Class from First Principles (AC Circuit Calculator)
```powershell
cd ..\Task07
g++ -std=c++17 -Wall -Wextra -o q7_complex.exe q7_complex.cpp
.\q7_complex.exe
```
**Result Summary:** Designed from engineering specifications to compute AC series and parallel RLC circuit impedances. Verified through theoretical hand calculations ($Z_{\text{series}} = 100 + 50j\,\Omega$, $Z_{\text{parallel}} = 50 - 50j\,\Omega$) and handles division by zero safely with exception handling.
