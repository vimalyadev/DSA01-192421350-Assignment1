#include <iostream>
#include <iomanip>
#include <string>

class Cart {
private:
    std::string customerName_;
    std::string items_[10];
    double prices_[10];
    int n_ = 0;
    double discount_ = 0.0;

public:
    Cart() : customerName_("Guest") {}
    explicit Cart(const std::string& name) : customerName_(name) {}

    Cart& add(const std::string& item, double price) {
        if (n_ < 10) {
            items_[n_] = item;
            prices_[n_] = price;
            n_++;
        }
        return *this;
    }

    Cart& setDiscount(double discount) {
        this->discount_ = discount; // Disambiguation via this->
        return *this;
    }

    double total() const {
        double t = 0;
        for (int i = 0; i < n_; ++i) t += prices_[i];
        return t * (100.0 - discount_) / 100.0;
    }

    std::string getName() const { return customerName_; }
};

int main() {
    std::cout << "=== TASK 5: Four Ways to Reach an Object & Chaining ===\n\n";

    // (a) Directly by name
    Cart cartA("Customer_A");
    cartA.add("Apples", 150.0).add("Milk", 50.0);
    std::cout << "(a) Direct access: Total = Rs " << cartA.total() << '\n';

    // (b) Through a pointer with ->
    Cart* ptrB = &cartA;
    ptrB->add("Bread", 40.0);
    std::cout << "(b) Pointer -> access: Total = Rs " << ptrB->total() << '\n';

    // (c) Through a reference
    Cart& refC = cartA;
    refC.setDiscount(5.0);
    std::cout << "(c) Reference access: Total = Rs " << refC.total() << '\n';

    // (d) Through a dynamic pointer (heap with new and delete)
    Cart* heapCart = new Cart("Customer_D");
    heapCart->add("Cereal", 220.0).setDiscount(10.0);
    std::cout << "(d) Dynamic new/delete access: Total = Rs " << heapCart->total() << '\n';
    delete heapCart;

    // Task 5.2: Array of pointers to Cart objects and finding highest total
    std::cout << "\n--- Task 5.2: Finding Highest Cart Total in Pointer Array ---\n";
    Cart c1("Alice"); c1.add("Item1", 300).add("Item2", 200).setDiscount(10); // 450
    Cart c2("Bob");   c2.add("Item3", 100).add("Item4", 150);                  // 250
    Cart c3("Charlie"); c3.add("Item5", 600).add("Item6", 400).setDiscount(20); // 800
    Cart c4("Diana"); c4.add("Item7", 500);                                    // 500

    Cart* activeCarts[4] = { &c1, &c2, &c3, &c4 };
    int count = 4;

    Cart* highestCart = activeCarts[0];
    for (int i = 1; i < count; ++i) {
        if (activeCarts[i]->total() > highestCart->total()) {
            highestCart = activeCarts[i];
        }
    }

    std::cout << "Highest Active Cart: " << highestCart->getName() 
              << " with Total of Rs " << std::fixed << std::setprecision(2) << highestCart->total() << '\n';

    return 0;
}
