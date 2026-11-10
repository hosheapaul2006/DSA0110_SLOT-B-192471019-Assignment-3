#include <iostream>
struct Book { int copies; Book(int count):copies(count) {} };
void print(Book value) { std::cout << "Copies=" << value.copies << '\n'; }
int main() { print(3); }
