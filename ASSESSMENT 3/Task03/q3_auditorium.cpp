#include <iostream>
#include <string>

class Auditorium {
private:
    std::string eventName_;
    int rows_;
    int cols_;
    char** seats_; // 2D dynamic array: 'O' = Open, 'X' = Booked
    int bookedCount_;

public:
    // Dynamic constructor
    Auditorium(const std::string& name, int rows, int cols)
        : eventName_(name), rows_(rows), cols_(cols), bookedCount_(0) {
        std::cout << "[Constructor] Allocating " << rows_ << "x" << cols_ 
                  << " seat map for event: \"" << eventName_ << "\"\n";
        seats_ = new char*[rows_];
        for (int r = 0; r < rows_; ++r) {
            seats_[r] = new char[cols_];
            for (int c = 0; c < cols_; ++c) {
                seats_[r][c] = 'O'; // Open
            }
        }
    }

    // Destructor: releases all allocations and prints closing summary
    ~Auditorium() {
        std::cout << "\n[Destructor] Event \"" << eventName_ << "\" Closing Summary:\n";
        std::cout << "  Total Seats: " << (rows_ * cols_) << '\n';
        std::cout << "  Seats Booked: " << bookedCount_ << '\n';
        std::cout << "  Seats Remaining: " << (rows_ * cols_ - bookedCount_) << '\n';
        std::cout << "  Releasing all dynamically allocated rows...\n";

        for (int r = 0; r < rows_; ++r) {
            delete[] seats_[r];
        }
        delete[] seats_;
        std::cout << "  Memory deallocation complete for \"" << eventName_ << "\".\n";
    }

    // Disable copy for raw pointer management in this demonstration
    Auditorium(const Auditorium&) = delete;
    Auditorium& operator=(const Auditorium&) = delete;

    bool bookSeat(int r, int c) {
        if (r < 0 || r >= rows_ || c < 0 || c >= cols_) {
            std::cout << "  Error: Invalid seat coordinate (" << r << ", " << c << ")\n";
            return false;
        }
        if (seats_[r][c] == 'X') {
            std::cout << "  Seat (" << r << ", " << c << ") is already booked!\n";
            return false;
        }
        seats_[r][c] = 'X';
        bookedCount_++;
        std::cout << "  Successfully booked seat (" << r << ", " << c << ") for \"" << eventName_ << "\"\n";
        return true;
    }

    bool cancelSeat(int r, int c) {
        if (r < 0 || r >= rows_ || c < 0 || c >= cols_) return false;
        if (seats_[r][c] == 'O') return false;
        seats_[r][c] = 'O';
        bookedCount_--;
        std::cout << "  Successfully cancelled seat (" << r << ", " << c << ") for \"" << eventName_ << "\"\n";
        return true;
    }
};

int main() {
    std::cout << "=== Demonstration of Object Lifecycles (Stack, Block-scope, Heap) ===\n\n";

    // 1. Stack object (main scope)
    Auditorium mainEvent("Annual Tech Summit", 3, 4);
    mainEvent.bookSeat(0, 0);
    mainEvent.bookSeat(1, 2);

    // 2. Block-scoped object
    {
        std::cout << "\n--- Entering Block Scope ---\n";
        Auditorium workshop("AI Workshop", 2, 3);
        workshop.bookSeat(0, 1);
        std::cout << "--- Exiting Block Scope ---\n";
    } // workshop destructor runs here immediately

    // 3. Dynamic Heap object (new / delete)
    std::cout << "\n--- Creating Dynamic Object on Heap ---\n";
    Auditorium* concert = new Auditorium("Symphony Orchestra", 4, 5);
    concert->bookSeat(2, 3);
    concert->bookSeat(3, 4);
    concert->cancelSeat(2, 3);
    
    std::cout << "--- Deleting Dynamic Object explicitly ---\n";
    delete concert; // concert destructor runs here

    std::cout << "\n--- Approaching end of main() ---\n";
    return 0;
} // mainEvent destructor runs here at function exit
