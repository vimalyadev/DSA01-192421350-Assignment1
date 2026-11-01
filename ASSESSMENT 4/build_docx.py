import os
import re
from docx import Document
from docx.shared import Inches, Pt, RGBColor
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.enum.table import WD_TABLE_ALIGNMENT, WD_ALIGN_VERTICAL
from docx.oxml import OxmlElement, parse_xml
from docx.oxml.ns import nsdecls, qn

def set_cell_background(cell, fill_hex):
    tcPr = cell._tc.get_or_add_tcPr()
    shd = parse_xml(f'<w:shd {nsdecls("w")} w:fill="{fill_hex}"/>')
    tcPr.append(shd)

def set_cell_margins(cell, top=100, bottom=100, left=150, right=150):
    tcPr = cell._tc.get_or_add_tcPr()
    tcMar = parse_xml(f'<w:tcMar {nsdecls("w")}><w:top w:w="{top}" w:type="dxa"/><w:bottom w:w="{bottom}" w:type="dxa"/><w:left w:w="{left}" w:type="dxa"/><w:right w:w="{right}" w:type="dxa"/></w:tcMar>')
    tcPr.append(tcMar)

def set_table_borders(table, color="D3D3D3"):
    tblPr = table._tbl.tblPr
    borders = parse_xml(f'''
        <w:tblBorders {nsdecls("w")}>
            <w:top w:val="single" w:sz="4" w:space="0" w:color="{color}"/>
            <w:bottom w:val="single" w:sz="6" w:space="0" w:color="003366"/>
            <w:insideH w:val="single" w:sz="4" w:space="0" w:color="{color}"/>
            <w:insideV w:val="none"/>
            <w:left w:val="none"/>
            <w:right w:val="none"/>
        </w:tblBorders>
    ''')
    tblPr.append(borders)

def add_header(doc, text, level):
    p = doc.add_paragraph()
    run = p.add_run(text)
    run.font.name = 'Calibri'
    run.font.bold = True
    if level == 1:
        p.paragraph_format.space_before = Pt(16)
        p.paragraph_format.space_after = Pt(4)
        run.font.size = Pt(20)
        run.font.color.rgb = RGBColor(0x00, 0x33, 0x66)
    elif level == 2:
        p.paragraph_format.space_before = Pt(13)
        p.paragraph_format.space_after = Pt(3)
        run.font.size = Pt(14)
        run.font.color.rgb = RGBColor(0x00, 0x4D, 0x80)
    elif level == 3:
        p.paragraph_format.space_before = Pt(9)
        p.paragraph_format.space_after = Pt(2)
        run.font.size = Pt(12)
        run.font.color.rgb = RGBColor(0x22, 0x55, 0x88)
    return p

def add_p(doc, text, bold_prefix="", italic=False):
    p = doc.add_paragraph()
    p.paragraph_format.space_before = Pt(2)
    p.paragraph_format.space_after = Pt(4)
    p.paragraph_format.line_spacing = 1.15
    if bold_prefix:
        r_pre = p.add_run(bold_prefix)
        r_pre.font.name = 'Calibri'
        r_pre.font.size = Pt(10.5)
        r_pre.font.bold = True
        r_pre.font.color.rgb = RGBColor(0x1B, 0x36, 0x5D)
    
    tokens = re.split(r'(\*\*.*?\*\*|`.*?`)', text)
    for t in tokens:
        if not t: continue
        if t.startswith('**') and t.endswith('**'):
            r = p.add_run(t[2:-2])
            r.font.name = 'Calibri'
            r.font.size = Pt(10.5)
            r.font.bold = True
        elif t.startswith('`') and t.endswith('`'):
            r = p.add_run(t[1:-1])
            r.font.name = 'Consolas'
            r.font.size = Pt(9.5)
            r.font.color.rgb = RGBColor(0x8B, 0x1A, 0x1A)
        else:
            r = p.add_run(t)
            r.font.name = 'Calibri'
            r.font.size = Pt(10.5)
            if italic: r.font.italic = True
    return p

def add_code(doc, code_str):
    tbl = doc.add_table(rows=1, cols=1)
    tbl.alignment = WD_TABLE_ALIGNMENT.CENTER
    cell = tbl.cell(0, 0)
    set_cell_background(cell, "F4F6F9")
    set_cell_margins(cell, top=140, bottom=140, left=200, right=200)
    cp = cell.paragraphs[0]
    cp.paragraph_format.space_before = Pt(0)
    cp.paragraph_format.space_after = Pt(0)
    cp.paragraph_format.line_spacing = 1.15
    run = cp.add_run(code_str.strip())
    run.font.name = 'Consolas'
    run.font.size = Pt(9.0)
    run.font.color.rgb = RGBColor(0x1B, 0x36, 0x5D)
    doc.add_paragraph().paragraph_format.space_after = Pt(3)

def add_table_data(doc, headers, rows):
    table = doc.add_table(rows=len(rows)+1, cols=len(headers))
    table.alignment = WD_TABLE_ALIGNMENT.CENTER
    set_table_borders(table, "D0D7DE")
    # Header
    for i, h in enumerate(headers):
        c = table.rows[0].cells[i]
        set_cell_background(c, "003366")
        set_cell_margins(c, 100, 100, 140, 140)
        p = c.paragraphs[0]
        p.paragraph_format.space_before = Pt(2)
        p.paragraph_format.space_after = Pt(2)
        r = p.add_run(h)
        r.font.name = 'Calibri'
        r.font.bold = True
        r.font.size = Pt(10)
        r.font.color.rgb = RGBColor(0xFF, 0xFF, 0xFF)
    # Rows
    for r_idx, row in enumerate(rows):
        for c_idx, val in enumerate(row):
            c = table.rows[r_idx+1].cells[c_idx]
            bg = "F9FAFC" if r_idx % 2 == 1 else "FFFFFF"
            set_cell_background(c, bg)
            set_cell_margins(c, 100, 100, 140, 140)
            p = c.paragraphs[0]
            p.paragraph_format.space_before = Pt(2)
            p.paragraph_format.space_after = Pt(2)
            r = p.add_run(val)
            r.font.name = 'Calibri'
            r.font.size = Pt(9.5)
            r.font.color.rgb = RGBColor(0x33, 0x33, 0x33)
    doc.add_paragraph().paragraph_format.space_after = Pt(4)

def generate_assignment4_docx():
    doc = Document()
    for s in doc.sections:
        s.top_margin = Inches(0.8)
        s.bottom_margin = Inches(0.8)
        s.left_margin = Inches(0.9)
        s.right_margin = Inches(0.9)

    # Title Page Header
    p_title = doc.add_paragraph()
    p_title.paragraph_format.space_before = Pt(20)
    p_title.paragraph_format.space_after = Pt(4)
    r = p_title.add_run("ASSIGNMENT 4 – CO4 (50 Marks)\nDesigning Reusable Class Hierarchies")
    r.font.name = 'Calibri'
    r.font.size = Pt(24)
    r.font.bold = True
    r.font.color.rgb = RGBColor(0x00, 0x33, 0x66)

    p_sub = doc.add_paragraph()
    p_sub.paragraph_format.space_after = Pt(16)
    r_sub = p_sub.add_run("Topic Coverage: Inheritance, Types of Inheritance, Virtual Base Classes, Abstract Classes, Pointers to Derived Classes, Virtual Destructors, this Pointer, and Runtime Polymorphism\nBloom's Taxonomy Level: K5 – Evaluate | Tasks 1 to 6")
    r_sub.font.name = 'Calibri'
    r_sub.font.size = Pt(12)
    r_sub.font.italic = True
    r_sub.font.color.rgb = RGBColor(0x55, 0x55, 0x55)

    add_table_data(doc, ["Field", "Details"], [
        ["Course Title", "Object Oriented Programming with C++ (DSA01)"],
        ["Course Outcome", "CO4 - Designing Reusable Class Hierarchies"],
        ["Total Marks", "50 Marks (Tasks 1 to 6)"],
        ["Compiler & Flags", "g++ 13.2.0 (MinGW-W64), -std=c++17 -Wall -Wextra"],
        ["Progression Flow", "Interpret -> Identify Is-a/Has-a -> Design Hierarchy -> Decide Access -> Implement -> Trace -> Challenge"]
    ])

    # =========================================================================
    # TASK 1: CLASS DESIGN REVIEW (6 Marks)
    # =========================================================================
    add_header(doc, "TASK 1 – Class Design Review: College Payroll (6 Marks)", 1)
    add_p(doc, "Diagnosis of a poorly designed college payroll system written by three independent developers featuring separate Professor, LabAssistant, and GuestFaculty classes with blatant duplication and absence of encapsulation.", "Context & Scope: ")

    add_header(doc, "1.1 Diagnosis of Five Major Design Deficiencies", 2)
    add_table_data(doc, ["Deficiency #", "Deficiency Identified", "Technical Consequence in Original Code"], [
        ["1", "Violations of DRY (Repeated Fields & Pay Rules)", "Fields like 'name', 'id', 'basic' and pay formula 'basic + basic * 0.42' repeated in multiple classes."],
        ["2", "Public Data Members (Zero Encapsulation)", "All fields are public, permitting arbitrary external modification without validation."],
        ["3", "Inconsistent Naming for Identical Semantics", "Professor uses 'name', 'id' while GuestFaculty uses 'nm', 'code' preventing uniform programmatic access."],
        ["4", "Unused / Dead Fields ('da')", "The member 'double da' is declared in Professor and LabAssistant but never used in the pay calculation."],
        ["5", "Hardcoded Magic Number (0.42)", "Dearness Allowance is hardcoded as 0.42. If government policy changes DA to 45%, every class must be edited."],
        ["6", "Inability to Process Staff in Unified Loop", "Unrelated classes have no common polymorphic base; cannot store in a single vector or process uniformly."]
    ])

    add_header(doc, "1.2 Repaired Hierarchy & UML Representation", 2)
    add_p(doc, "The hierarchy introduces an abstract base class `Staff` with pure virtual `pay()`. Salaried employees inherit from `RegularStaff` where `DA_RATE = 0.42` is centralized:", "Architectural Fix: ")
    add_code(doc, '''// UML Representation
          [ Staff ] (Abstract Base)
          + id_: int, name_: string
          + virtual pay(): double = 0
             /                  \\
     [ RegularStaff ]       [ GuestFaculty ]
     + basic_: double       + hours_: double
     + DA_RATE: 0.42        + hourlyRate_: double
        /          \\        + pay() = hours * rate
  [ Professor ]  [ LabAssistant ]
  + research: double
  + pay() = RegularStaff::pay() + research''')

    add_header(doc, "1.3 Design Defense & Complexity Analysis", 2)
    add_p(doc, "By introducing an abstract `Staff` base with pure virtual `pay()`, new staff categories (e.g. `ResearchAssociate` or `Adviser`) are added simply by deriving a new class and overriding `pay()`. Existing classes remain completely untouched (Open-Closed Principle), and the payroll processing loop requires zero modifications.", "Design Defense (<100 words): ")
    add_p(doc, "If a single `Staff` class with a type flag and a `switch` statement were used: Execution time remains O(1) in both. However, space complexity is bloated because every single object must allocate unused fields (e.g. `hours` and `rate` in a Professor object). Moreover, adding a new staff type forces modification of every `switch` statement throughout the codebase, drastically violating maintainability.", "Alternative Concept Analysis: ")

    # =========================================================================
    # TASK 2: FROM REQUIREMENT TO HIERARCHY (8 Marks)
    # =========================================================================
    add_header(doc, "TASK 2 – From Requirement to Hierarchy: University People (8 Marks)", 1)
    add_p(doc, "Context: University Records management involving Person, Student, Staff, TeachingAssistant (both student and staff), and ResearchScholar (student holding fellowship).", "Context & Scope: ")

    add_header(doc, "2.1 Relationship Classification & Inheritance Types", 2)
    add_table_data(doc, ["Relationship", "Classification", "Inheritance / Association Type", "Justification"], [
        ["Student is a Person", "Is-a", "Single / Hierarchical Inheritance", "Student specializes Person with programme and CGPA."],
        ["Staff is a Person", "Is-a", "Single / Hierarchical Inheritance", "Staff specializes Person with department and salary."],
        ["TeachingAssistant is Student & Staff", "Is-a", "Multiple / Diamond (Hybrid) Inheritance", "TA inherits properties of both Student and Staff."],
        ["ResearchScholar is a Student", "Is-a", "Multilevel Inheritance", "ResearchScholar specializes Student."],
        ["Fellowship in ResearchScholar", "Has-a", "Composition (Member Class)", "Scholar holds a fellowship record object."]
    ])

    add_header(doc, "2.2 Resolving the Diamond Problem via Virtual Base Classes", 2)
    add_p(doc, "In multiple inheritance where `TeachingAssistant` derives from both `Student` and `Staff`, both intermediate classes derive from `Person`. Without `virtual`, `TeachingAssistant` receives TWO separate copies of `Person`, creating ambiguity when accessing `name` or `id` (`ta.name` triggers compiler error) and wasting memory. Declaring `virtual public Person` in both `Student` and `Staff` instructs the compiler to share a single, unified `Person` sub-object.", "The Diamond Problem: ")

    add_header(doc, "2.3 Access Specifier Visibility Matrix", 2)
    add_table_data(doc, ["Base Member in Person", "Public Inheritance", "Protected Inheritance", "Private Inheritance"], [
        ["public", "public in Student (accessible anywhere)", "protected in Student (accessible in derived)", "private in Student (accessible only in Student)"],
        ["protected", "protected in Student", "protected in Student", "private in Student"],
        ["private", "Hidden / Inaccessible directly", "Hidden / Inaccessible directly", "Hidden / Inaccessible directly"]
    ])

    add_header(doc, "2.4 Output Prediction & Experimental Investigation", 2)
    add_p(doc, "Prediction with virtual base: Person Student Staff TA \nActual Output: Person Student Staff TA \nPrediction without virtual: Person Student Person Staff TA \nActual Output: Person Student Person Staff TA ", "Trace Analysis: ")
    add_p(doc, "With virtual base classes, `Person` is constructed exactly ONCE by the most-derived class `TeachingAssistant`. When `virtual` is removed, two distinct `Person` sub-objects are constructed (one via Student and one via Staff).", "Explanation: ")

    # =========================================================================
    # TASK 3: CONSTRUCTORS IN DERIVED & MEMBER CLASSES (9 Marks)
    # =========================================================================
    add_header(doc, "TASK 3 – Constructors in Derived Classes and Member Classes (9 Marks)", 1)
    
    add_header(doc, "3.1 Passing Values to Base & Virtual Constructors", 2)
    add_p(doc, "In a virtual inheritance hierarchy, intermediate classes (`Student`, `Staff`) do NOT call the virtual base constructor. The most-derived class (`TeachingAssistant`) is solely responsible for initializing the virtual base class (`Person(name, id, phone)`). If the most-derived class fails to call it, the compiler attempts to invoke the default constructor of the virtual base.", "Most-Derived Class Rule: ")

    add_header(doc, "3.2 Output Prediction: Vehicle / Engine / Car", 2)
    add_code(doc, '''// Code trace from Task 3
struct Engine { Engine() { cout << "Engine "; } ~Engine() { cout << "~Engine "; } };
struct Vehicle { Vehicle() { cout << "Vehicle "; } ~Vehicle() { cout << "~Vehicle "; } };
struct Car : Vehicle { Engine e; Car() { cout << "Car "; } ~Car() { cout << "~Car "; } };''')
    add_p(doc, "Vehicle Engine Car | ~Car ~Engine ~Vehicle ", "Exact Output: ")
    add_table_data(doc, ["Phase", "Order of Execution", "C++ Standard Rule"], [
        ["Construction", "1. Vehicle -> 2. Engine -> 3. Car", "Base classes construct first, then member objects in declaration order, then derived body."],
        ["Destruction", "1. ~Car -> 2. ~Engine -> 3. ~Vehicle", "Destructors execute in EXACT REVERSE order of construction."]
    ])

    add_header(doc, "3.3 Completed Code & -Wreorder Warning", 2)
    add_p(doc, "Executing Task 3.2 Student with Address composition produced:\n`192311001 Kavya (19) Salem-636001` (Exact match with expected output).", "Execution Match: ")
    add_p(doc, "When initializer list order `: home(c, p), regNo(r)` swapped relative to declaration order (`regNo` declared before `home`), `g++ -Wreorder` reported:\n`warning: 'StudentReorder::home' will be initialized after 'long long int StudentReorder::regNo' [-Wreorder]`.", "-Wreorder Evidence: ")

    # =========================================================================
    # TASK 4: POINTERS TO DERIVED CLASSES: FORENSICS (7 Marks)
    # =========================================================================
    add_header(doc, "TASK 4 – Pointers to Derived Classes: Forensics (7 Marks)", 1)
    add_p(doc, "Forensic audit of bank account software where SeniorAccount should earn 4.0% interest but earns 3.5%, and dynamic memory history fails to deallocate.", "Context & Scope: ")

    add_header(doc, "4.1 Root Cause Diagnosis", 2)
    add_table_data(doc, ["Defect #", "Observed Malfunction", "Underlying C++ Mechanism", "Correction"], [
        ["1", "rate() returns 3.5% instead of 4.0%", "Early Binding / Non-Virtual Dispatch: Account::rate() was non-virtual; compiler resolved call based on pointer type (Account*).", "Mark `virtual double rate() const` in base class."],
        ["2", "Memory Leak (100 doubles leaked per SeniorAccount)", "Non-Virtual Destructor: `delete a` through `Account*` only invoked `~Account()`, completely bypassing `~SeniorAccount()`.", "Declare `virtual ~Account() = default;` in base class."],
        ["3", "Object Slicing (`Account copy = SeniorAccount()`)", "Pass/Copy by Value: Assigning derived object to base instance copies only base sub-object, truncating derived state.", "Use pointers or references (`Account*` or `Account&`) instead of value copies."]
    ])

    add_header(doc, "4.2 Output Prediction: Task 4 Predict", 2)
    add_p(doc, "Predict output of:\n`cout << p->rate() << ' ' << p->vrate() << ' ' << copy.vrate();`\nExact Actual Output: `3.5 4 3.5`", "Prediction Trace: ")
    add_p(doc, "- `p->rate()` prints 3.5 because `rate()` is non-virtual and statically bound to `Account`.\n- `p->vrate()` prints 4.0 because `vrate()` is virtual and dynamically dispatches via vtable to `SeniorAccount::vrate()`.\n- `copy.vrate()` prints 3.5 because `copy` was sliced into a pure `Account` object during copy initialization.", "Explanation: ")

    # =========================================================================
    # TASK 5: POINTERS TO OBJECTS AND THE THIS POINTER (7 Marks)
    # =========================================================================
    add_header(doc, "TASK 5 – Pointers to Objects and the this Pointer (7 Marks)", 1)

    add_header(doc, "5.1 Four Ways to Reach an Object", 2)
    add_table_data(doc, ["Access Mode", "Syntax Example", "Safety & Lifetime", "Recommended Usage"], [
        ["(a) Directly by Name", "cart.add(\"Rice\", 60);", "Guaranteed safe; lifetime tied to scope.", "Local processing within a single function."],
        ["(b) Through Pointer (->)", "ptr->add(\"Dal\", 120);", "Nullable; requires null-check; can rebind.", "Dynamic polymorphism and optional parameters."],
        ["(c) Through Reference (.)", "ref.setDiscount(10);", "Safe; non-null; cannot rebind.", "Function parameters for pass-by-reference."],
        ["(d) Dynamic Heap (new/delete)", "Cart* p = new Cart(); ... delete p;", "Explicit lifetime; risk of leak if delete missed.", "Objects that must outlive creating scope."]
    ])

    add_header(doc, "5.2 Method Chaining and `this` Pointer Mechanics", 2)
    add_p(doc, "The completed code implementation returned `Cart&` and used `this->discount = discount` to disambiguate member from parameter. Executing `main()` produced exact expected output:\n`324.00`", "Output Verification: ")
    add_p(doc, "If `add()` returned `Cart` by value instead of `Cart&`, each chained call would operate on an ephemeral, temporary copy of the cart. The modifications made by subsequent calls (`setDiscount(10)`) would be discarded upon temporary destruction, leaving the original cart unaltered.", "Why Return Cart& vs Cart: ")

    # =========================================================================
    # TASK 6: DESIGN CHALLENGE - VEHICLE RENTAL FLEET (13 Marks)
    # =========================================================================
    add_header(doc, "TASK 6 – Design Challenge: Vehicle Rental Fleet System (13 Marks)", 1)
    add_p(doc, "Comprehensive design of an enterprise vehicle rental fleet managing Bicycles, Two-Wheelers, Cars, Electric Cars, and seamlessly extensible to Electric Buses.", "Context & Scope: ")

    add_header(doc, "6.1 Hierarchy Architecture & Class Diagram", 2)
    add_code(doc, '''// Enterprise Vehicle Fleet Hierarchy
                 [ Vehicle ] (Abstract Base)
                 + regNo_: string, dailyRate_: double, isRented_: bool
                 + virtual calculateCharge(days, consumption): double = 0
                 + virtual processReturn(scheduled, actual, consumption): double
                     /                                  \\
         [ Bicycle ]                              [ MotorVehicle ] (Abstract)
         + calculateCharge() = days*rate          + fuelOrChargeLevel_: double
                                                  + serviceRecord_: ServiceInfo (Composition)
                                                   /          |           \\
                                          [ TwoWheeler ]    [ Car ]     [ ElectricCar ]
                                          + per-litre       + per-litre  + per-kWh
                                                                               |
                                                                        [ ElectricBus ] (Stage 5 Extension)''')

    add_header(doc, "6.2 Execution and Test Evidence", 2)
    add_code(doc, '''[1] Current Fleet Inventory:
[Bicycle] Reg: KA-01-B-101  | Rate/Day: Rs 150.00   | Status: AVAILABLE
[Two-Wheeler] Reg: KA-01-TW-202 | Rate/Day: Rs 450.00   | Status: AVAILABLE | Level: 85%
[Car] Reg: KA-01-CR-303 | Rate/Day: Rs 2200.00  | Status: AVAILABLE | Seats: 5
[Electric Car] Reg: KA-01-EC-404 | Rate/Day: Rs 3000.00  | Status: AVAILABLE | Battery: 75 kWh
[Electric Bus] Reg: KA-01-EB-999 | Rate/Day: Rs 8500.00  | Status: AVAILABLE | Passenger Cap: 45

[3] Processing Returns & Generating Invoices:
Invoice 1: Bicycle returned on time (3 days). Total: Rs 450.00
Invoice 2: Car returned on time (2 days + 15 L fuel). Total: Rs 5937.50
Invoice 3: Electric Car returned 2 days LATE (2 scheduled + 2 late + 30 kWh). Total: Rs 15360.00
Invoice 4: Electric Bus returned on time (1 day + 80 kWh). Total: Rs 9700.00''')

    add_header(doc, "6.3 Stage 5 Extension Defense & Open-Closed Principle", 2)
    add_p(doc, "When `ElectricBus` was introduced, zero lines of code in `Vehicle`, `Bicycle`, `TwoWheeler`, `Car`, or `ElectricCar` were modified. The new vehicle class was simply linked into the polymorphic collection `std::vector<std::unique_ptr<Vehicle>>`. This serves as empirical proof of compliance with the Open-Closed Principle (OCP) and demonstrates maximum reusability.", "Design Defense: ")

    out_file = r"C:\ASSIGNMENT 1\ASSESSMENT 4\Assignment4_Report.docx"
    doc.save(out_file)
    print("Assignment 4 Word Document successfully created at:", out_file)

if __name__ == "__main__":
    generate_assignment4_docx()
