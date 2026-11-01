#include <iostream>
#include <string>

// Virtual Base Class
class Person {
protected:
    std::string name_;
    int id_;
    std::string phone_;

public:
    Person(const std::string& name, int id, const std::string& phone)
        : name_(name), id_(id), phone_(phone) {
        std::cout << "[Person Created: " << name_ << "]\n";
    }
    virtual ~Person() = default;

    std::string getName() const { return name_; }
};

// Intermediate Derived Class 1: Virtual Inheritance
class Student : virtual public Person {
protected:
    std::string programme_;
    double cgpa_;

public:
    Student(const std::string& name, int id, const std::string& phone,
            const std::string& prog, double cgpa)
        : Person(name, id, phone), programme_(prog), cgpa_(cgpa) {
        std::cout << "[Student Created: " << programme_ << "]\n";
    }
};

// Intermediate Derived Class 2: Virtual Inheritance
class Staff : virtual public Person {
protected:
    std::string department_;
    double salary_;

public:
    Staff(const std::string& name, int id, const std::string& phone,
          const std::string& dept, double salary)
        : Person(name, id, phone), department_(dept), salary_(salary) {
        std::cout << "[Staff Created: " << department_ << "]\n";
    }
};

// Most Derived Class: Multiple Inheritance resolving Diamond via Virtual Base
class TeachingAssistant : public Student, public Staff {
private:
    int hoursPerWeek_;

public:
    TeachingAssistant(const std::string& name, int id, const std::string& phone,
                      const std::string& prog, double cgpa,
                      const std::string& dept, double salary, int hours)
        : Person(name, id, phone), // Most-derived class initializes virtual base directly!
          Student(name, id, phone, prog, cgpa),
          Staff(name, id, phone, dept, salary),
          hoursPerWeek_(hours) {
        std::cout << "[TeachingAssistant Created: " << hoursPerWeek_ << " hrs/wk]\n";
    }

    void display() const {
        std::cout << "\n=== Teaching Assistant Record ===\n"
                  << "Name: " << name_ << " (Accessible without ambiguity!)\n"
                  << "ID: " << id_ << " | Phone: " << phone_ << '\n'
                  << "Programme: " << programme_ << " | CGPA: " << cgpa_ << '\n'
                  << "Dept: " << department_ << " | Salary: Rs " << salary_ << '\n'
                  << "Teaching Workload: " << hoursPerWeek_ << " hours/week\n";
    }
};

int main() {
    std::cout << "--- Constructing TeachingAssistant ---\n";
    TeachingAssistant ta("Kavya Sharma", 401, "9876543210",
                          "M.Tech AI", 9.42, "Computer Science", 25000.0, 15);
    ta.display();

    std::cout << "\nMemory layout check: sizeof(TeachingAssistant) = " 
              << sizeof(TeachingAssistant) << " bytes\n";
    return 0;
}
