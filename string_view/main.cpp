#include <string>
#include <iostream>
#include <string>

std::string_view startFromWord(std::string_view str, std::string_view word) {
    return str.substr(str.find(word)); // substr creates now  only a new view
}

int main() {

    using std::string;
    using std::string_view;
    const char* cstr = "Hello World";

    // the whole string:
    string_view sv1 {cstr};
    std::cout << sv1 << ", len: " << sv1.size() << '\n'; // prints Hello World, len: 11

    // slice
    string_view sv2 {cstr, 5}; // not null-terminated!
    std::cout << sv2 << ", len: " << sv2.size() << '\n'; // prints Hello, len: 5

    // from string:
    string str = "Hello String";
    string_view sv3 = str;
    std::cout << sv3 << ", len: " << sv3.size() << '\n'; // prints Hello String, len: 12

    // ""sv literal
    using namespace std::literals;
    string_view sv4 = "Hello\0 Super World"sv;
    std::cout << sv4 << ", len: " << sv4.size() << '\n'; // prints Hello Super World, len: 18
    std::cout << sv4.data() << " - till zero\n"; // prints Hello - til
}
