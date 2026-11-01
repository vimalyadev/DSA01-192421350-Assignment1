#include <iostream>
#include <string>
#include <stdexcept>

class Book {
private:
    std::string title_;
    std::string author_;
    int copies_;

public:
    // Full parameterised constructor: performs validation
    Book(const std::string& title, const std::string& author, int copies)
        : title_(title), author_(author), copies_(copies) {
        if (copies < 0) {
            throw std::invalid_argument("Error: Number of copies cannot be negative! Received: " + std::to_string(copies));
        }
    }

    // Delegating constructor: Donated book with title only (author="Unknown", copies=1)
    explicit Book(const std::string& title)
        : Book(title, "Unknown", 1) {}

    // Delegating default constructor: Empty record (title="Untitled", author="Unknown", copies=0)
    Book()
        : Book("Untitled", "Unknown", 0) {}

    // Method to display state
    void print() const {
        std::cout << "Book Record -> Title: \"" << title_ 
                  << "\", Author: \"" << author_ 
                  << "\", Copies: " << copies_ << '\n';
    }

    int getCopies() const { return copies_; }
    std::string getTitle() const { return title_; }
};

// Demonstration of explicit vs implicit conversions
class ExplicitDemo {
public:
    explicit ExplicitDemo(int x) {
        std::cout << "ExplicitDemo created with " << x << '\n';
    }
};

void checkExplicit(ExplicitDemo e) {
    (void)e;
}

int main() {
    std::cout << "=== Q1: Library Book Records - Constructor Demonstrations ===\n\n";

    // 1. Create with default constructor
    Book b1;
    std::cout << "[1] Default Constructor: ";
    b1.print();

    // 2. Create with single-argument delegated constructor (Donated book)
    Book b2("Data Structures and Algorithms");
    std::cout << "[2] Overloaded Single-Arg (Donated): ";
    b2.print();

    // 3. Create with full parameterised constructor
    Book b3("Operating Systems Concepts", "Silberschatz", 5);
    std::cout << "[3] Parameterised Constructor: ";
    b3.print();

    // 4. Test invalid copies rejection
    std::cout << "\n[4] Testing Negative Copies Validation:\n";
    try {
        Book b4("Faulty Inventory", "Anonymous", -3);
        b4.print();
    } catch (const std::exception& e) {
        std::cout << "  Caught expected exception: " << e.what() << '\n';
    }

    std::cout << "\n[5] Explicit Constructor Demonstration:\n";
    ExplicitDemo ed(42); // Compiles: direct initialization
    checkExplicit(ExplicitDemo(42)); // Compiles: explicit conversion
    // checkExplicit(42); // Error if uncommented: cannot convert int to ExplicitDemo implicitly
    std::cout << "  checkExplicit(42) correctly prevented at compile-time by explicit keyword.\n";

    return 0;
}
