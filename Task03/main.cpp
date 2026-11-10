#include "auditorium.hpp"
#include "../tools/checks.hpp"
#include <type_traits>
int main(int argc,char** argv) {
    Tests tests(argc,argv);
    Auditorium mainEvent("Main",2,3);
    tests.check(mainEvent.book(0,0) && mainEvent.book(1,2),"book valid seats at both boundaries");
    tests.check(!mainEvent.book(0,0),"duplicate booking rejected");
    tests.check(mainEvent.cancel(0,0) && !mainEvent.cancel(0,0),"cancellation and duplicate cancellation");
    tests.check(mainEvent.booked()==1,"booking statistics after cancellation");
    std::cout << "Current occupied seats=" << mainEvent.booked() << '\n';
    { Auditorium nested("Nested",1,2);tests.check(nested.book(0,1),"nested event booking"); }
    Auditorium* heap=new Auditorium("Heap",1,1);
    tests.check(heap->book(0,0),"heap event booking");delete heap;
    { Auditorium zero("Zero",0,5);tests.check(zero.booked()==0 && zero.allocations()==0 && zero.payloadBytes()==0,"zero rows allocate nothing");
      bool rejected=false;try { zero.book(0,0); } catch(const std::out_of_range&) { rejected=true; }
      tests.check(rejected,"booking a zero-row map rejected"); }
    bool negative=false,invalidSeat=false;
    try { Auditorium invalid("Invalid",-1,2); } catch(const std::invalid_argument&) { negative=true; }
    try { mainEvent.book(-1,0); } catch(const std::out_of_range&) { invalidSeat=true; }
    tests.check(negative && invalidSeat,"negative dimension and negative seat rejected");
    tests.check(!std::is_copy_constructible_v<Auditorium> && !std::is_copy_assignable_v<Auditorium>,"owning auditorium is noncopyable");
    std::cout << "Leaving main scope\n";
}
