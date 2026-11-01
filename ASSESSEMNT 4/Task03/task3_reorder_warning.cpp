#include <iostream>
#include <string>

class Person {
public:
    std::string name;
    int age;
    Person(const std::string& n, int a) : name(n), age(a) {}
};

class Address {
    std::string city;
    int pin;
public:
    Address(const std::string& c, int p) : city(c), pin(p) {}
    std::string str() const { return city + "-" + std::to_string(pin); }
};

class StudentReorder : public Person {
    long long regNo;
    Address home;
public:
    // Warning: 'StudentReorder::home' will be initialized after 'long long int StudentReorder::regNo' [-Wreorder]
    StudentReorder(const std::string& n, int a, long long r, const std::string& c, int p)
        : Person(n, a), home(c, p), regNo(r) {}
};

int main() {
    StudentReorder s("Kavya", 19, 192311001, "Salem", 636001);
    return 0;
}
