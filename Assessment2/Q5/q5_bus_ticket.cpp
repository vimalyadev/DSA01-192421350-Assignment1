// Q5 – Complete the Code: BusTicket with static members
// TODO 1 and TODO 2 completed
#include <iostream>
#include <string>

class BusTicket {
    static int issued;               // shared by all BusTicket objects
    static const int CAPACITY = 3;
    int seat;
    std::string name;

public:
    explicit BusTicket(const std::string& n) : seat(0), name(n)
    {
        // TODO 1 COMPLETED:
        // If fewer than CAPACITY tickets have been issued, increase issued
        // and give this ticket the next seat number; otherwise seat stays 0 (waiting list)
        if (issued < CAPACITY) {
            ++issued;
            seat = issued;
        }
        // else seat remains 0 → waiting list
    }

    static int count()
    {
        // TODO 2 COMPLETED: Return the number of seats issued so far
        return issued;
    }

    void print() const
    {
        std::cout << name << ": "
                  << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting"))
                  << '\n';
    }
};

int BusTicket::issued = 0; // definition of the static data member

// ── Demonstrate count behaviour across two function calls ─────────────────────
void bookTrip(const std::string& name)
{
    BusTicket t(name);
    t.print();
    // 'issued' persists between calls because it is static (class-level storage)
}

int main()
{
    BusTicket trip[4] = {BusTicket("Asha"), BusTicket("Bala"),
                         BusTicket("Chitra"), BusTicket("Dev")};
    for (const BusTicket& t : trip) t.print();
    std::cout << "Issued: " << BusTicket::count() << '\n';

    // Extra: show count persists across function calls
    std::cout << "\n--- Extra booking attempts (capacity already full) ---\n";
    bookTrip("Elan");
    bookTrip("Fiona");
    std::cout << "Issued after extra calls: " << BusTicket::count() << '\n';
    // Still 3 – static member is NOT reset between calls
    return 0;
}

/*
 * EXPECTED OUTPUT (first block):
 * Asha: Seat 1
 * Bala: Seat 2
 * Chitra: Seat 3
 * Dev: Waiting
 * Issued: 3
 *
 * COUNT BEHAVIOUR WITH FUNCTION CALLS:
 * BusTicket::issued is a static data member stored in the data segment (once).
 * It is NOT a local variable; it persists for the entire program lifetime.
 * Calling bookTrip("Elan") after capacity is full leaves issued at 3.
 * Elan and Fiona both get seat=0 (Waiting).
 *
 * STATIC MEMBER FUNCTION CANNOT USE 'this':
 * A static function has no receiver object, so 'this' pointer does not exist.
 * It can access only static members (which belong to the class, not an object).
 *
 * ALTERNATIVE DESIGNS vs STATIC DATA MEMBER:
 * Global variable: accessible everywhere → poor encapsulation, no class coupling.
 * TicketOffice object: encapsulates counter but must be passed to every BusTicket
 *   constructor → enables two buses (two TicketOffice instances) easily.
 * Static member: single counter per class; two buses require two classes or a
 *   workaround.  For a single bus, static is simplest and cleanest.
 * Time: O(1), Space: one int in all designs.
 */
