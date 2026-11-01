#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>
#include <complex>

class Complex {
private:
    double real_;
    double imag_;

public:
    // Constructors
    Complex() : real_(0.0), imag_(0.0) {}
    Complex(double r) : real_(r), imag_(0.0) {}
    Complex(double r, double i) : real_(r), imag_(i) {}

    double real() const { return real_; }
    double imag() const { return imag_; }

    double magnitude() const {
        return std::sqrt(real_ * real_ + imag_ * imag_);
    }

    // Arithmetic operators as member / friend functions
    Complex operator+(const Complex& o) const {
        return Complex(real_ + o.real_, imag_ + o.imag_);
    }

    Complex operator-(const Complex& o) const {
        return Complex(real_ - o.real_, imag_ - o.imag_);
    }

    Complex operator*(const Complex& o) const {
        return Complex(real_ * o.real_ - imag_ * o.imag_, real_ * o.imag_ + imag_ * o.real_);
    }

    Complex operator/(const Complex& o) const {
        double denom = o.real_ * o.real_ + o.imag_ * o.imag_;
        if (denom == 0.0) {
            throw std::runtime_error("Error: Division by zero complex impedance!");
        }
        return Complex((real_ * o.real_ + imag_ * o.imag_) / denom,
                       (imag_ * o.real_ - real_ * o.imag_) / denom);
    }

    bool operator==(const Complex& o) const {
        return std::abs(real_ - o.real_) < 1e-9 && std::abs(imag_ - o.imag_) < 1e-9;
    }

    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << std::fixed << std::setprecision(3) << c.real_;
        if (c.imag_ >= 0) os << " + " << c.imag_ << "j";
        else os << " - " << (-c.imag_) << "j";
        return os;
    }
};

int main() {
    std::cout << "=== Q7: AC Circuit Impedance Calculator (First Principles Design) ===\n\n";

    // Component specifications at angular frequency omega = 1000 rad/s
    // Resistor: R = 100 Ohms -> ZR = 100 + 0j
    // Inductor: L = 0.1 H -> ZL = 0 + j*omega*L = 0 + 100j
    // Capacitor: C = 20 uF -> ZC = 0 - j/(omega*C) = 0 - 50j
    Complex ZR(100.0, 0.0);
    Complex ZL(0.0, 100.0);
    Complex ZC(0.0, -50.0);

    std::cout << "Impedances:\n";
    std::cout << "  Resistor (ZR)  : " << ZR << " Ohms\n";
    std::cout << "  Inductor (ZL)  : " << ZL << " Ohms\n";
    std::cout << "  Capacitor (ZC) : " << ZC << " Ohms\n\n";

    // 1. Series RLC Circuit: Z_series = ZR + ZL + ZC
    Complex Z_series = ZR + ZL + ZC;
    std::cout << "[1] Series RLC Impedance: " << Z_series << " Ohms\n";
    std::cout << "    Magnitude |Z_series|: " << Z_series.magnitude() << " Ohms\n";
    std::cout << "    Hand check: 100 + (100 - 50)j = 100 + 50j, |Z| = sqrt(100^2 + 50^2) = 111.803 Ohms. Verified!\n\n";

    // 2. Parallel RLC Circuit:
    // Pairwise: Z_LC = (ZL * ZC) / (ZL + ZC)
    // Z_total = (ZR * Z_LC) / (ZR + Z_LC)
    Complex Z_LC = (ZL * ZC) / (ZL + ZC);
    Complex Z_parallel = (ZR * Z_LC) / (ZR + Z_LC);
    std::cout << "[2] Parallel RLC Impedance:\n";
    std::cout << "    Z_LC (Inductor || Capacitor): " << Z_LC << " Ohms\n";
    std::cout << "    Total Parallel Impedance    : " << Z_parallel << " Ohms\n";
    std::cout << "    Magnitude |Z_parallel|      : " << Z_parallel.magnitude() << " Ohms\n";
    std::cout << "    Hand check: Z_LC = (100j * -50j) / (50j) = 5000 / 50j = -100j\n";
    std::cout << "    Z_parallel = (100 * -100j) / (100 - 100j) = 50 - 50j, |Z| = sqrt(50^2 + 50^2) = 70.711 Ohms. Verified!\n\n";

    // 3. Division by Zero Test
    std::cout << "[3] Division by Zero Safety Test:\n";
    try {
        Complex zeroImpedance(0.0, 0.0);
        Complex invalid = ZR / zeroImpedance;
        std::cout << "Result: " << invalid << '\n';
    } catch (const std::exception& e) {
        std::cout << "    Caught expected exception: " << e.what() << '\n';
    }

    return 0;
}
