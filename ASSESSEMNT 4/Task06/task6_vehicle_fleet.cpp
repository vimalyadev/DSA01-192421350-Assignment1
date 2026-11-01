#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

// Member Class (Composition): Service Information
class ServiceInfo {
private:
    std::string dueDate_;
    int lastServiceKm_;

public:
    ServiceInfo(const std::string& dueDate = "2026-12-31", int km = 0)
        : dueDate_(dueDate), lastServiceKm_(km) {}

    std::string getDueDate() const { return dueDate_; }
    int getLastKm() const { return lastServiceKm_; }
};

// Abstract Base Class: Unified interface for all rentable fleet assets
class Vehicle {
protected:
    std::string regNo_;
    double dailyRate_;
    bool isRented_;

public:
    Vehicle(const std::string& regNo, double dailyRate)
        : regNo_(regNo), dailyRate_(dailyRate), isRented_(false) {}

    virtual ~Vehicle() = default;

    // Pure virtual function: Each vehicle calculates charges according to domain rules
    virtual double calculateCharge(int daysRented, double consumption) const = 0;

    // Common virtual return calculation with late fee support
    virtual double processReturn(int daysScheduled, int daysActual, double consumption, double lateRateMultiplier = 1.5) {
        if (!isRented_) {
            std::cout << "  [Warning] Vehicle " << regNo_ << " was not marked as rented.\n";
        }
        isRented_ = false;

        int regularDays = std::min(daysScheduled, daysActual);
        int lateDays = std::max(0, daysActual - daysScheduled);

        double baseCharge = calculateCharge(regularDays, consumption);
        double latePenalty = lateDays * (dailyRate_ * lateRateMultiplier);

        return baseCharge + latePenalty;
    }

    virtual void rentVehicle() {
        if (isRented_) {
            throw std::runtime_error("Vehicle " + regNo_ + " is already currently rented!");
        }
        isRented_ = true;
        std::cout << "  Vehicle " << regNo_ << " rented successfully.\n";
    }

    virtual void display(std::ostream& os) const {
        os << "[" << getType() << "] Reg: " << std::left << std::setw(12) << regNo_
           << " | Rate/Day: Rs " << std::fixed << std::setprecision(2) << std::setw(8) << dailyRate_
           << " | Status: " << (isRented_ ? "RENTED" : "AVAILABLE");
    }

    virtual std::string getType() const = 0;
    std::string getRegNo() const { return regNo_; }
    bool isRented() const { return isRented_; }
};

// Concrete Class 1: Bicycle (Non-motorized, clean base rate)
class Bicycle : public Vehicle {
public:
    Bicycle(const std::string& regNo, double dailyRate)
        : Vehicle(regNo, dailyRate) {}

    std::string getType() const override { return "Bicycle"; }

    double calculateCharge(int daysRented, double consumption) const override {
        (void)consumption; // No fuel/charge consumption
        return daysRented * dailyRate_;
    }
};

// Abstract Class: Motorized Vehicle with Service Tracking
class MotorVehicle : public Vehicle {
protected:
    double fuelOrChargeLevel_;
    ServiceInfo serviceRecord_;

public:
    MotorVehicle(const std::string& regNo, double dailyRate,
                 double level, const ServiceInfo& service)
        : Vehicle(regNo, dailyRate), fuelOrChargeLevel_(level), serviceRecord_(service) {}

    void display(std::ostream& os) const override {
        Vehicle::display(os);
        os << " | Level: " << fuelOrChargeLevel_ << "%"
           << " | Service Due: " << serviceRecord_.getDueDate();
    }
};

// Concrete Class 2: Two-Wheeler (Motorcycle/Scooter with Petrol Per-Litre Billing)
class TwoWheeler : public MotorVehicle {
private:
    static constexpr double FUEL_PRICE_PER_LITRE = 102.50;

public:
    TwoWheeler(const std::string& regNo, double dailyRate, double fuelLevel, const ServiceInfo& service)
        : MotorVehicle(regNo, dailyRate, fuelLevel, service) {}

    std::string getType() const override { return "Two-Wheeler"; }

    double calculateCharge(int daysRented, double litresConsumed) const override {
        return (daysRented * dailyRate_) + (litresConsumed * FUEL_PRICE_PER_LITRE);
    }
};

// Concrete Class 3: Car (Seating capacity + Fuel Surcharge)
class Car : public MotorVehicle {
protected:
    int seatingCapacity_;
    static constexpr double FUEL_PRICE_PER_LITRE = 102.50;

public:
    Car(const std::string& regNo, double dailyRate, double fuelLevel,
        const ServiceInfo& service, int seats)
        : MotorVehicle(regNo, dailyRate, fuelLevel, service), seatingCapacity_(seats) {}

    std::string getType() const override { return "Car"; }

    double calculateCharge(int daysRented, double litresConsumed) const override {
        return (daysRented * dailyRate_) + (litresConsumed * FUEL_PRICE_PER_LITRE);
    }

    void display(std::ostream& os) const override {
        MotorVehicle::display(os);
        os << " | Seats: " << seatingCapacity_;
    }
};

// Concrete Class 4: Electric Car (Charged per kWh, battery capacity)
class ElectricCar : public MotorVehicle {
private:
    double batteryCapacityKWh_;
    static constexpr double RATE_PER_KWH = 12.00; // Charged per kWh

public:
    ElectricCar(const std::string& regNo, double dailyRate, double chargeLevel,
                const ServiceInfo& service, double batteryKWh)
        : MotorVehicle(regNo, dailyRate, chargeLevel, service), batteryCapacityKWh_(batteryKWh) {}

    std::string getType() const override { return "Electric Car"; }

    double calculateCharge(int daysRented, double kWhConsumed) const override {
        return (daysRented * dailyRate_) + (kWhConsumed * RATE_PER_KWH);
    }

    void display(std::ostream& os) const override {
        MotorVehicle::display(os);
        os << " | Battery: " << batteryCapacityKWh_ << " kWh";
    }
};

// Stage 5 Demonstration: Adding an Electric Bus WITHOUT touching existing classes!
class ElectricBus : public MotorVehicle {
private:
    int passengerCapacity_;
    double batteryKWh_;
    static constexpr double COMMERCIAL_KWH_RATE = 15.00;

public:
    ElectricBus(const std::string& regNo, double dailyRate, double chargeLevel,
                const ServiceInfo& service, int passengers, double batteryKWh)
        : MotorVehicle(regNo, dailyRate, chargeLevel, service),
          passengerCapacity_(passengers), batteryKWh_(batteryKWh) {}

    std::string getType() const override { return "Electric Bus"; }

    double calculateCharge(int daysRented, double kWhConsumed) const override {
        return (daysRented * dailyRate_) + (kWhConsumed * COMMERCIAL_KWH_RATE);
    }

    void display(std::ostream& os) const override {
        MotorVehicle::display(os);
        os << " | Passenger Cap: " << passengerCapacity_ << " | Battery: " << batteryKWh_ << " kWh";
    }
};

int main() {
    std::cout << "=== TASK 6: Vehicle Rental Fleet System (Design Challenge) ===\n\n";

    // Polymorphic array of base-class unique pointers
    std::vector<std::unique_ptr<Vehicle>> fleet;
    fleet.push_back(std::make_unique<Bicycle>("KA-01-B-101", 150.0));
    fleet.push_back(std::make_unique<TwoWheeler>("KA-01-TW-202", 450.0, 85.0, ServiceInfo("2026-11-20", 8500)));
    fleet.push_back(std::make_unique<Car>("KA-01-CR-303", 2200.0, 60.0, ServiceInfo("2026-12-15", 14200), 5));
    fleet.push_back(std::make_unique<ElectricCar>("KA-01-EC-404", 3000.0, 92.0, ServiceInfo("2027-01-10", 5300), 75.0));

    // STAGE 5 EXTENSION: Newly introduced vehicle added seamlessly
    fleet.push_back(std::make_unique<ElectricBus>("KA-01-EB-999", 8500.0, 100.0, ServiceInfo("2027-03-01", 21000), 45, 250.0));

    // 1. List fleet
    std::cout << "[1] Current Fleet Inventory:\n";
    for (const auto& v : fleet) {
        v->display(std::cout);
        std::cout << '\n';
    }

    // 2. Renting vehicles
    std::cout << "\n[2] Processing Rentals:\n";
    fleet[0]->rentVehicle(); // Bicycle
    fleet[2]->rentVehicle(); // Car
    fleet[3]->rentVehicle(); // Electric Car

    // 3. Returning vehicles with and without late charges
    std::cout << "\n[3] Processing Returns & Generating Invoices:\n";

    // Normal return: Bicycle rented 3 days, returned on time
    double fee1 = fleet[0]->processReturn(3, 3, 0.0);
    std::cout << "Invoice 1: Bicycle returned on time (3 days). Total: Rs " << fee1 << '\n';

    // Normal return with petrol: Car rented 2 days, returned on time, used 15 litres petrol
    double fee2 = fleet[2]->processReturn(2, 2, 15.0);
    std::cout << "Invoice 2: Car returned on time (2 days + 15 L fuel). Total: Rs " << fee2 << '\n';

    // Late return with kWh: Electric Car scheduled for 2 days, returned after 4 days (2 late days), used 30 kWh
    // Daily rate = Rs 3000, 2 days = 6000 + (30 * 12 = 360) = 6360 base. Late penalty = 2 * (3000 * 1.5) = 9000. Total = 15360.
    double fee3 = fleet[3]->processReturn(2, 4, 30.0);
    std::cout << "Invoice 3: Electric Car returned 2 days LATE (2 scheduled + 2 late + 30 kWh). Total: Rs " << fee3 << '\n';

    // 4. Extended Fleet Return Test
    fleet[4]->rentVehicle();
    double fee4 = fleet[4]->processReturn(1, 1, 80.0); // 80 kWh for Bus
    std::cout << "Invoice 4: Electric Bus returned on time (1 day + 80 kWh). Total: Rs " << fee4 << '\n';

    return 0;
}
