// Q5 – Predict the Output program (given, unmodified)
#include <iostream>

class Token {
    static int next;
    int id;

public:
    Token() { id = ++next; }
    int get() const { return id; }
    static int issued() { return next - 100; }
};

int Token::next = 100;

int main()
{
    Token counter[3];
    Token extra;
    std::cout << counter[0].get() << ' ' << counter[2].get() << ' ' << extra.get()
              << ' ' << Token::issued() << '\n';
}

/*
 * PREDICTION (written BEFORE running):
 *
 * Token::next starts at 100.
 * counter[3] is an array of 3 Tokens; their default constructors run in order:
 *   counter[0]: next becomes 101, id = 101
 *   counter[1]: next becomes 102, id = 102
 *   counter[2]: next becomes 103, id = 103
 * extra:        next becomes 104, id = 104
 *
 * counter[0].get() = 101
 * counter[2].get() = 103
 * extra.get()      = 104
 * Token::issued()  = next - 100 = 104 - 100 = 4
 *
 * PREDICTED OUTPUT:  101 103 104 4
 *
 * EXPLANATION:
 * static int next is shared by all Token objects. It is initialised to 100
 * by the out-of-class definition.  Each constructor call increments it BEFORE
 * assigning to id (pre-increment), so ids start at 101.
 * The constructor ran 4 times total (3 for the array + 1 for extra).
 * Token::issued() returns next - 100 = 4 (the count of tickets issued).
 */
