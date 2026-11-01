#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <memory>

// Base Class: Common attributes and polymorphic interface
class Staff {
protected:
    std::string name_;
    int id_;

public:
    Staff(const std::string& name, int id) : name_(name), id_(id) {}
    virtual ~Staff() = default;

    virtual double pay() const = 0; // Pure virtual function for polymorphic payroll

    virtual void print() const {
        std::cout << "ID: " << std::setw(4) << id_ 
                  << " | Name: " << std::left << std::setw(18) << name_ 
                  << " | Pay: Rs " << std::right << std::fixed << std::setprecision(2) << pay() << '\n';
    }

    std::string getName() const { return name_; }
    int getId() const { return id_; }
};

// Derived Class: Regular Salaried Staff (Encapsulates DA constant)
class RegularStaff : public Staff {
protected:
    double basic_;
    static constexpr double DA_RATE = 0.42; // Replaces magic number 0.42

public:
    RegularStaff(const std::string& name, int id, double basic)
        : Staff(name, id), basic_(basic) {}

    double pay() const override {
        return basic_ + (basic_ * DA_RATE);
    }
};

// Derived Class: Professor (Adds research allowance)
class Professor : public RegularStaff {
private:
    double researchAllowance_;

public:
    Professor(const std::string& name, int id, double basic, double researchAllowance)
        : RegularStaff(name, id, basic), researchAllowance_(researchAllowance) {}

    double pay() const override {
        return RegularStaff::pay() + researchAllowance_;
    }
};

// Derived Class: Lab Assistant (Direct regular salaried employee)
class LabAssistant : public RegularStaff {
public:
    LabAssistant(const std::string& name, int id, double basic)
        : RegularStaff(name, id, basic) {}
};

// Derived Class: Guest Faculty (Hourly billing model)
class GuestFaculty : public Staff {
private:
    double hours_;
    double hourlyRate_;

public:
    GuestFaculty(const std::string& name, int id, double hours, double hourlyRate)
        : Staff(name, id), hours_(hours), hourlyRate_(hourlyRate) {}

    double pay() const override {
        return hours_ * hourlyRate_;
    }
};

int main() {
    std::cout << "=== TASK 1: Redesigned College Payroll System ===\n\n";

    // Polymorphic collection: processed in a single unified loop
    std::vector<std::unique_ptr<Staff>> payroll;
    payroll.push_back(std::make_unique<Professor>("Dr. Aris", 101, 85000.0, 15000.0));
    payroll.push_back(std::make_unique<LabAssistant>("Ramesh Kumar", 204, 32000.0));
    payroll.push_back(std::make_unique<GuestFaculty>("Prof. Anita (Visiting)", 305, 45.0, 1200.0));

    double totalExpenditure = 0.0;
    for (const auto& member : payroll) {
        member->print();
        totalExpenditure += member->pay();
    }

    std::cout << "--------------------------------------------------------\n";
    std::cout << "Total Payroll Expenditure: Rs " << std::fixed << std::setprecision(2) << totalExpenditure << '\n';

    return 0;
}
