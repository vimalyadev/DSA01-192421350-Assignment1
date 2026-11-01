# Assignment 4: CO4 — Designing Reusable Class Hierarchies

**Student Name:** [Your Name]  
**Register Number:** [Your Register Number]  
**Course Code:** DSA01 - Object-Oriented Programming in C++  
**Compiler & Version:** `g++ (MinGW-W64 x86_64-ucrt-posix-seh) 13.2.0`  
**Standard Used:** C++17 (`-std=c++17 -Wall -Wextra`)

---

## Repository Structure & Overview
```text
ASSESSMENT 4/
├── Assignment4_Report.docx         <-- Complete 50-mark formatted Word Document Report
├── README.md                       <-- Compilation commands, flags, and test evidence
├── build_docx.py                   <-- Automated OpenXML Word document generator
├── Task01/                         <-- Task 1: Class Design Review (College Payroll)
│   └── task1_payroll.cpp
├── Task02/                         <-- Task 2: Hierarchy & Diamond Problem (University People)
│   ├── task2_predict_virtual.cpp
│   ├── task2_predict_nonvirtual.cpp
│   └── task2_university.cpp
├── Task03/                         <-- Task 3: Order of Construction & Member Classes
│   ├── task3_predict.cpp
│   ├── task3_student_address.cpp
│   └── task3_reorder_warning.cpp
├── Task04/                         <-- Task 4: Pointers to Derived Classes: Forensics (Bank Accounts)
│   ├── task4_predict.cpp
│   └── task4_repaired.cpp
├── Task05/                         <-- Task 5: Pointers to Objects and the `this` Pointer (Shopping Cart)
│   ├── task5_cart_complete.cpp
│   └── task5_cart_extended.cpp
└── Task06/                         <-- Task 6: Design Challenge (Vehicle Rental Fleet)
    └── task6_vehicle_fleet.cpp
```

---

## Compilation and Execution Guide

To execute each program with strict compiler flags, open PowerShell / Terminal and run:

### Task 1: Class Design Review (College Payroll Program)
```powershell
cd "ASSESSMENT 4\Task01"
g++ -std=c++17 -Wall -Wextra -o task1_payroll.exe task1_payroll.cpp
.\task1_payroll.exe
```
**Outcome:** Replaces duplicated classes and public variables with a polymorphic `Staff` hierarchy featuring pure virtual `pay()`. Eliminates magic number 0.42 with `DA_RATE`. Total expenditure calculated polymorphically in a single loop.

---

### Task 2: From Requirement to Hierarchy (University People Management)
```powershell
cd "..\Task02"
# Prediction with virtual base class:
g++ -std=c++17 -Wall -Wextra -o task2_predict_virtual.exe task2_predict_virtual.cpp
.\task2_predict_virtual.exe

# Prediction without virtual base class:
g++ -std=c++17 -Wall -Wextra -o task2_predict_nonvirtual.exe task2_predict_nonvirtual.cpp
.\task2_predict_nonvirtual.exe

# Full university hierarchy with resolved diamond:
g++ -std=c++17 -Wall -Wextra -o task2_university.exe task2_university.cpp
.\task2_university.exe
```
**Outcome:** Demonstrates diamond problem resolution using `virtual public Person`. Verifies output changes from duplicated `Person Student Person Staff TA ` to clean `Person Student Staff TA `.

---

### Task 3: Order of Construction and Destruction (Vehicle & Student Address)
```powershell
cd "..\Task03"
# Prediction for Vehicle/Engine/Car:
g++ -std=c++17 -Wall -Wextra -o task3_predict.exe task3_predict.cpp
.\task3_predict.exe

# Complete Student with Address composition:
g++ -std=c++17 -Wall -Wextra -o task3_student_address.exe task3_student_address.cpp
.\task3_student_address.exe

# Member order warning demonstration:
g++ -std=c++17 -Wall -Wextra -Wreorder task3_reorder_warning.cpp -o task3_reorder_warning.exe
```
**Outcome:** Validates construction rule (Base classes -> Member classes -> Derived class body) and destruction rule (exact reverse order: `Vehicle Engine Car | ~Car ~Engine ~Vehicle `). Captures `-Wreorder` diagnostic.

---

### Task 4: Pointers to Derived Classes: Forensics (Bank Accounts)
```powershell
cd "..\Task04"
# Prediction of non-virtual vs virtual dispatch & slicing:
g++ -std=c++17 -Wall -Wextra -o task4_predict.exe task4_predict.cpp
.\task4_predict.exe

# Repaired hierarchy with virtual destructor:
g++ -std=c++17 -Wall -Wextra -o task4_repaired.exe task4_repaired.cpp
.\task4_repaired.exe
```
**Outcome:** Analyzes compile-time early binding vs dynamic vtable late binding. Proves how missing virtual destructor causes severe memory leaks for dynamic derived members, and shows how object slicing truncates derived state.

---

### Task 5: Pointers to Objects and the `this` Pointer (Shopping Cart)
```powershell
cd "..\Task05"
# Complete the code with method chaining and this->:
g++ -std=c++17 -Wall -Wextra -o task5_cart_complete.exe task5_cart_complete.cpp
.\task5_cart_complete.exe

# Extended demonstration of the 4 ways to access objects:
g++ -std=c++17 -Wall -Wextra -o task5_cart_extended.exe task5_cart_extended.cpp
.\task5_cart_extended.exe
```
**Outcome:** Demonstrates (a) Direct name access, (b) Pointer with `->`, (c) Reference, and (d) Dynamic heap `new`/`delete`. Implements method chaining returning `*this` and finds highest cart total across an array of pointers.

---

### Task 6: Design Challenge (Vehicle Rental Fleet System)
```powershell
cd "..\Task06"
g++ -std=c++17 -Wall -Wextra -o task6_vehicle_fleet.exe task6_vehicle_fleet.cpp
.\task6_vehicle_fleet.exe
```
**Outcome:** Developed an extensible polymorphic fleet hierarchy (`Bicycle`, `TwoWheeler`, `Car`, `ElectricCar`, and newly added `ElectricBus`). Proves Open-Closed Principle (OCP) by adding new vehicle types without modifying existing classes. Handles late return penalty calculations and per-kWh vs per-litre charging.
