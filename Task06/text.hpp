#ifndef OWNED_TEXT_HPP
#define OWNED_TEXT_HPP
#include <cstddef>
#include <cstring>
#include <iostream>
#include <limits>
#include <stdexcept>
class Text {
    std::size_t length;
    char* characters;
    struct Uninitialized {};
    static std::size_t checkedLength(const char* value) {
        if(!value) throw std::invalid_argument("null text input");
        return std::strlen(value);
    }
    static char* allocate(std::size_t size) {
        if(size==std::numeric_limits<std::size_t>::max()) throw std::length_error("text size overflow");
        char* result=new char[size+1];++allocations;return result;
    }
    Text(std::size_t count,Uninitialized):length(count),characters(allocate(count)) { characters[length]='\0';++constructions; }
public:
    inline static unsigned long long constructions=0,copies=0,allocations=0;
    explicit Text(const char* value=""):length(checkedLength(value)),characters(allocate(length)) {
        std::memcpy(characters,value,length+1);++constructions;
    }
    Text(const Text& other):length(other.length),characters(allocate(length)) {
        std::memcpy(characters,other.characters,length+1);++copies;++constructions;
    }
    Text& operator=(const Text& other) {
        if(this==&other) return *this;
        char* replacement=allocate(other.length);
        std::memcpy(replacement,other.characters,other.length+1);
        delete[] characters;characters=replacement;length=other.length;return *this;
    }
    ~Text() { delete[] characters; }
    std::size_t size() const { return length; }
    const char* c_str() const { return characters; }
    Text& operator+=(const Text& other) {
        if(length>std::numeric_limits<std::size_t>::max()-1-other.length) throw std::length_error("text concatenation overflow");
        const std::size_t combined=length+other.length;
        char* replacement=allocate(combined);
        std::memcpy(replacement,characters,length);
        std::memcpy(replacement+length,other.characters,other.length+1);
        delete[] characters;characters=replacement;length=combined;return *this;
    }
    friend Text operator+(const Text& a,const Text& b) {
        if(a.length>std::numeric_limits<std::size_t>::max()-1-b.length) throw std::length_error("text concatenation overflow");
        Text result(a.length+b.length,Uninitialized{});
        std::memcpy(result.characters,a.characters,a.length);
        std::memcpy(result.characters+a.length,b.characters,b.length);
        return result;
    }
    bool operator==(const Text& other) const { return length==other.length && std::memcmp(characters,other.characters,length)==0; }
    bool operator!=(const Text& other) const { return !(*this==other); }
    friend std::ostream& operator<<(std::ostream& out,const Text& value) { return out<<value.characters; }
};
inline Text plusOnCopy(Text left,const Text& right) { left+=right;return left; }
#endif
