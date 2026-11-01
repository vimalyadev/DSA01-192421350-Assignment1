#include <iostream>
#include <string>

// Class with NO copy constructor or assignment operator (shallow copy)
class PrescriptionShallow {
public:
    int n;
    std::string* medicines;

    explicit PrescriptionShallow(int count) : n(count), medicines(new std::string[count]) {}
    ~PrescriptionShallow() {
        delete[] medicines;
    }
};

int main() {
    {
        PrescriptionShallow p(2);
        PrescriptionShallow q = p; // Shallow copy! Both point to same heap buffer
    } // CRASH here upon exiting block due to double-free / double-delete!
    return 0;
}
