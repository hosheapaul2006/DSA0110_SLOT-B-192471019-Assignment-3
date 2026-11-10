// Isolated leak experiment; execute only under ASan/LeakSanitizer.
#include "../auditorium.hpp"
#if defined(__GNUC__)
__attribute__((noinline))
#endif
void omitDelete() {
    Auditorium* event=new Auditorium("Leaked",2,3);
    event->book(0,0);
    std::cout << "Intentionally forgotten delete, occupied=" << event->booked() << '\n';
}
int main() { omitDelete(); }
