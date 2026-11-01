#include <iostream>

struct ReorderDemo {
    int b;
    int a;
    // Warning: 'ReorderDemo::a' will be initialized after 'ReorderDemo::b' [-Wreorder]
    ReorderDemo(int x) : a(x), b(a + 10) {}
};

int main() {
    ReorderDemo r(5);
    std::cout << "r.a: " << r.a << ", r.b: " << r.b << '\n';
    return 0;
}
