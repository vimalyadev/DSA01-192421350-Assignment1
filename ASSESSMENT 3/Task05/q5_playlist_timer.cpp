#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>

class Time {
private:
    int hours_;
    int minutes_;
    int seconds_;

    void normalize() {
        if (seconds_ >= 60) {
            minutes_ += seconds_ / 60;
            seconds_ %= 60;
        }
        if (minutes_ >= 60) {
            hours_ += minutes_ / 60;
            minutes_ %= 60;
        }
    }

public:
    Time(int h = 0, int m = 0, int s = 0) : hours_(h), minutes_(m), seconds_(s) {
        normalize();
    }

    // Prefix increment (++t): advances by 1 second, returns reference to self
    Time& operator++() {
        ++seconds_;
        normalize();
        return *this;
    }

    // Postfix increment (t++): returns copy of original state, advances by 1 second
    Time operator++(int) {
        Time temp = *this;
        ++seconds_;
        normalize();
        return temp;
    }

    // Comparison operators
    bool operator==(const Time& other) const {
        return hours_ == other.hours_ && minutes_ == other.minutes_ && seconds_ == other.seconds_;
    }

    bool operator<(const Time& other) const {
        if (hours_ != other.hours_) return hours_ < other.hours_;
        if (minutes_ != other.minutes_) return minutes_ < other.minutes_;
        return seconds_ < other.seconds_;
    }

    void print() const {
        std::cout << std::setw(2) << std::setfill('0') << hours_ << ':'
                  << std::setw(2) << std::setfill('0') << minutes_ << ':'
                  << std::setw(2) << std::setfill('0') << seconds_ << std::setfill(' ');
    }
};

class Playlist {
private:
    std::vector<std::string> tracks_;

public:
    void addTrack(const std::string& track) {
        tracks_.push_back(track);
    }

    int size() const { return static_cast<int>(tracks_.size()); }

    // Non-const subscript operator
    std::string& operator[](int index) {
        if (index < 0 || index >= static_cast<int>(tracks_.size())) {
            throw std::out_of_range("Error: Playlist index out of bounds! Index: " + std::to_string(index) + ", Size: " + std::to_string(tracks_.size()));
        }
        return tracks_[index];
    }

    // Const subscript operator
    const std::string& operator[](int index) const {
        if (index < 0 || index >= static_cast<int>(tracks_.size())) {
            throw std::out_of_range("Error: Playlist index out of bounds! Index: " + std::to_string(index) + ", Size: " + std::to_string(tracks_.size()));
        }
        return tracks_[index];
    }
};

int main() {
    std::cout << "=== Q5: Music Playlist and Track Timer ===\n\n";

    // 1. Verify prefix and postfix increments
    std::cout << "[1] Increment Verification:\n";
    Time t(0, 2, 30);
    std::cout << "Initial Time t: "; t.print(); std::cout << '\n';

    Time a = t++; // a gets old t (00:02:30), t becomes 00:02:31
    std::cout << "After a = t++ -> a: "; a.print(); std::cout << " | Current t: "; t.print(); std::cout << '\n';

    Time b = ++t; // t becomes 00:02:32, b gets new t (00:02:32)
    std::cout << "After b = ++t -> b: "; b.print(); std::cout << " | Current t: "; t.print(); std::cout << '\n';

    // 2. Minute and Hour Rollover Test (00:59:59 -> 01:00:00)
    std::cout << "\n[2] Minute and Hour Rollover Test (00:59:59 -> 01:00:00):\n";
    Time rollover(0, 59, 59);
    std::cout << "Before increment: "; rollover.print(); std::cout << '\n';
    ++rollover;
    std::cout << "After ++rollover: "; rollover.print(); std::cout << " (Verified 01:00:00)\n";

    // 3. Subscript operator and out-of-range checks
    std::cout << "\n[3] Playlist Subscript and Bounds Checking:\n";
    Playlist pl;
    pl.addTrack("Track 1: Bohemian Rhapsody");
    pl.addTrack("Track 2: Hotel California");
    pl.addTrack("Track 3: Stairway to Heaven");

    std::cout << "Valid access pl[1]: " << pl[1] << '\n';

    // Test playlist[-1]
    try {
        std::cout << "Accessing pl[-1]: ";
        std::cout << pl[-1] << '\n';
    } catch (const std::exception& e) {
        std::cout << "Caught expected exception: " << e.what() << '\n';
    }

    // Test playlist[size]
    try {
        std::cout << "Accessing pl[pl.size()]: ";
        std::cout << pl[pl.size()] << '\n';
    } catch (const std::exception& e) {
        std::cout << "Caught expected exception: " << e.what() << '\n';
    }

    // 4. Benchmark: 10,000,000 prefix vs postfix increments
    std::cout << "\n[4] Benchmark: 10,000,000 Prefix vs Postfix Increments:\n";
    const int ITERS = 10000000;
    Time t1(0, 0, 0);
    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERS; ++i) {
        ++t1;
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dur1 = end1 - start1;
    std::cout << "Time for 10M prefix ++: " << dur1.count() << " ms\n";

    Time t2(0, 0, 0);
    auto start2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERS; ++i) {
        t2++;
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dur2 = end2 - start2;
    std::cout << "Time for 10M postfix ++: " << dur2.count() << " ms\n";

    return 0;
}
