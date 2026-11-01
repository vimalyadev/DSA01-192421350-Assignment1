#include <iostream>
#include <string>

class BookNoDefault {
public:
    BookNoDefault(const std::string& t, int c) : title(t), copies(c) {}
    std::string title;
    int copies;
};

int main() {
    // Attempting to create an array without default constructor
    BookNoDefault library[3]; 
    return 0;
}
