// Isolated allocator/deallocator mismatch; execute only under ASan.
#include <iostream>
int main() { char* data=new char[32]{};data[0]='X';std::cout << data[0] << std::endl;delete data; }
