#include <iostream>
#include <string>
#include <chrono>

class Prescription {
    int n;
    std::string* medicines; // dynamically allocated array

public:
    static int copy_count;

    explicit Prescription(int count) : n(count), medicines(new std::string[count]) {}

    // TODO 1: Copy constructor: allocate a NEW array of the same size and copy every medicine (deep copy)
    Prescription(const Prescription& other) : n(other.n), medicines(new std::string[other.n]) {
        copy_count++;
        for (int i = 0; i < n; ++i) {
            medicines[i] = other.medicines[i];
        }
    }

    // TODO 2: Copy assignment: guard against self-assignment, build a new array, copy, release old array and return *this
    Prescription& operator=(const Prescription& other) {
        if (this != &other) {
            std::string* new_meds = new std::string[other.n];
            for (int i = 0; i < other.n; ++i) {
                new_meds[i] = other.medicines[i];
            }
            delete[] medicines;
            medicines = new_meds;
            n = other.n;
        }
        return *this;
    }

    // TODO 3: Release the dynamically allocated array
    ~Prescription() {
        delete[] medicines;
    }

    void set(int i, const std::string& m) { medicines[i] = m; }
    std::string get(int i) const { return medicines[i]; }
    int size() const { return n; }
};

int Prescription::copy_count = 0;

void passByValue(Prescription p) {
    (void)p;
}

void passByConstRef(const Prescription& p) {
    (void)p;
}

int main() {
    std::cout << "=== Q2: Completed Code Test ===\n";
    {
        Prescription p(2);
        p.set(0, "Paracetamol");
        p.set(1, "ORS");

        Prescription q = p; // copy constructor
        q.set(1, "Cetirizine");

        Prescription r(1);
        r = p; // copy assignment
        p.set(0, "Ibuprofen");

        std::cout << p.get(0) << ' ' << p.get(1) << " | " 
                  << q.get(0) << ' ' << q.get(1) << " | " 
                  << r.get(0) << ' ' << r.get(1) << '\n';
        
        std::cout << "Expected:\nIbuprofen ORS | Paracetamol Cetirizine | Paracetamol ORS\n\n";

        // Self-assignment test
        p = p;
        std::cout << "Self-assignment test passed: " << p.get(0) << ' ' << p.get(1) << '\n';
    }

    std::cout << "\n=== Experimental Investigation: Pass by Value vs Pass by Const Reference ===\n";
    Prescription test(5);
    Prescription::copy_count = 0;
    passByConstRef(test);
    std::cout << "Copies made when passed by const reference: " << Prescription::copy_count << '\n';
    passByValue(test);
    std::cout << "Copies made when passed by value: " << Prescription::copy_count << '\n';

    std::cout << "\n=== Timing Benchmark for Deep Copies ===\n";
    int sizes[] = {10, 1000, 100000};
    for (int s : sizes) {
        Prescription src(s);
        for (int i = 0; i < s; ++i) src.set(i, "Med_" + std::to_string(i));

        auto start = std::chrono::high_resolution_clock::now();
        Prescription copy = src;
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::micro> elapsed = end - start;
        std::cout << "Time to copy " << s << " medicines: " << elapsed.count() << " microseconds\n";
    }

    return 0;
}
