#ifndef LIBRARY_BOOK_HPP
#define LIBRARY_BOOK_HPP
#include <array>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string_view>
class Book {
    std::array<char,128> title{};
    std::array<char,96> author{};
    int copies;
    template<std::size_t N>
    static void store(std::array<char,N>& destination,std::string_view value) {
        if(value.size()>=N) throw std::length_error("book text exceeds fixed capacity");
        if(!value.empty()) std::memcpy(destination.data(),value.data(),value.size());
        destination[value.size()]='\0';
    }
public:
    Book():Book("","",0) {}
    explicit Book(std::string_view name):Book(name,"Donated",1) {}
    Book(std::string_view name,std::string_view writer,int count):title{},author{},copies(count) {
        if(count<0) throw std::invalid_argument("negative copy count");
        store(title,name);store(author,writer);
    }
    int count() const { return copies; }
    std::string_view name() const { return title.data(); }
    void print() const { std::cout << "Title=\"" << title.data() << "\" Author=\"" << author.data() << "\" Copies=" << copies << '\n'; }
};
#endif
