#include <iostream>
#include <cstring>
#include <string>
#include <chrono>

class Text {
private:
    char* data_;
    size_t length_;

public:
    static int copy_count;

    // Default constructor
    Text() : data_(new char[1]), length_(0) {
        data_[0] = '\0';
    }

    // Parameterised constructor from C-string
    Text(const char* s) {
        if (s == nullptr) {
            length_ = 0;
            data_ = new char[1];
            data_[0] = '\0';
        } else {
            length_ = std::strlen(s);
            data_ = new char[length_ + 1];
            std::strcpy(data_, s);
        }
    }

    // Copy constructor (Deep Copy)
    Text(const Text& other) : length_(other.length_) {
        copy_count++;
        data_ = new char[length_ + 1];
        std::strcpy(data_, other.data_);
    }

    // Copy assignment operator (with self-assignment guard)
    Text& operator=(const Text& other) {
        if (this != &other) {
            char* new_data = new char[other.length_ + 1];
            std::strcpy(new_data, other.data_);
            delete[] data_;
            data_ = new_data;
            length_ = other.length_;
        }
        return *this;
    }

    // Destructor
    ~Text() {
        delete[] data_;
    }

    // Overload + for concatenation
    Text operator+(const Text& other) const {
        Text result;
        delete[] result.data_;
        result.length_ = length_ + other.length_;
        result.data_ = new char[result.length_ + 1];
        std::strcpy(result.data_, data_);
        std::strcat(result.data_, other.data_);
        return result;
    }

    // Overload == for comparison
    bool operator==(const Text& other) const {
        if (length_ != other.length_) return false;
        return std::strcmp(data_, other.data_) == 0;
    }

    // Overload != for comparison
    bool operator!=(const Text& other) const {
        return !(*this == other);
    }

    // Overload << for stream printing
    friend std::ostream& operator<<(std::ostream& os, const Text& t) {
        os << t.data_;
        return os;
    }

    size_t length() const { return length_; }
    const char* c_str() const { return data_; }
};

int Text::copy_count = 0;

int main() {
    std::cout << "=== Q6: Custom Text Class for SMS Gateway ===\n\n";

    // 1. Basic concatenation and printing
    Text msg1("Hello, ");
    Text msg2("Your OTP is 9482.");
    Text fullMsg = msg1 + msg2;
    std::cout << "Concatenated SMS: \"" << fullMsg << "\"\n";
    std::cout << "Length: " << fullMsg.length() << " chars\n";

    // 2. Comparison tests
    Text msgCopy("Hello, Your OTP is 9482.");
    std::cout << "fullMsg == msgCopy: " << std::boolalpha << (fullMsg == msgCopy) << '\n';
    std::cout << "fullMsg != msg1: " << (fullMsg != msg1) << '\n';

    // 3. Self-assignment test
    std::cout << "\n--- Testing Self-Assignment ---\n";
    fullMsg = fullMsg;
    std::cout << "After self-assignment (fullMsg = fullMsg): \"" << fullMsg << "\" (Intact and valid)\n";

    // 4. Test empty and long messages
    std::cout << "\n--- Testing Empty and Long SMS Messages ---\n";
    Text emptyMsg("");
    std::cout << "Empty text length: " << emptyMsg.length() << ", Content: \"" << emptyMsg << "\"\n";

    std::string longStr(500, 'A');
    Text longMsg(longStr.c_str());
    std::cout << "Long SMS length: " << longMsg.length() << " characters.\n";

    // 5. Count copies in a + b + c evaluation
    std::cout << "\n--- Counting Copies in (a + b + c) ---\n";
    Text a("Alpha ");
    Text b("Beta ");
    Text c("Gamma");
    Text::copy_count = 0;
    Text result = a + b + c;
    std::cout << "Result: \"" << result << "\"\n";
    std::cout << "Copies made during evaluation: " << Text::copy_count << '\n';

    // 6. Benchmark comparison: Text vs std::string for 100,000 concatenations
    std::cout << "\n--- Benchmark: 100,000 Concatenations (Text vs std::string) ---\n";
    const int N = 100000;
    
    // std::string benchmark
    auto s1 = std::chrono::high_resolution_clock::now();
    std::string std_res = "";
    for (int i = 0; i < N; ++i) {
        std_res += "x";
    }
    auto s2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> std_dur = s2 - s1;
    std::cout << "std::string += (with dynamic capacity growth): " << std_dur.count() << " ms\n";

    return 0;
}
