#include <iostream>
class Order {
    int first;
    int second;
public:
    Order():second(20),first(10) {} // Deliberately listed out of declaration order.
    void print() const { std::cout << first << ' ' << second << '\n'; }
};
int main() { Order value;value.print(); }
