struct Book { int copies; explicit Book(int count):copies(count) {} };
void accept(Book) {}
int main() { accept(3); }
