// Isolated leak experiment; execute only under ASan/LeakSanitizer.
#include "../auditorium.hpp"
#include <thread>
#if defined(__GNUC__)
__attribute__((noinline))
#endif
void omitDelete() {
    Auditorium* event=new Auditorium("Leaked",2,3);
    event->book(0,0);
    std::cout << "Intentionally forgotten delete, occupied=" << event->booked() << '\n';
}
int main() {
    // LeakSanitizer scans live stacks/registers conservatively. Completing this
    // worker removes its stack/register roots before the process leak check.
    std::thread worker(omitDelete);
    worker.join();
}
