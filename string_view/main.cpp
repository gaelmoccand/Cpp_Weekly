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
    std::cout << sv1 << ", len: " << sv1.size() << "\n\n"; // prints Hello World, len: 11

    // slice
    string_view sv2 {cstr, 5}; // not null-terminated!
    std::cout << sv2 << ", len: " << sv2.size() << "\n\n"; // prints Hello, len: 5

    // from string:
    string str = "Hello String";
    string_view sv3 = str;
    std::cout << sv3 << ", len: " << sv3.size() << "\n\n"; // prints Hello String, len: 12

    // ""sv literal
    using namespace std::literals;
    string_view sv4 = "Hello\0 Super World"sv;
    std::cout << sv4 << ", len: " << sv4.size() << '\n'; // prints Hello Super World, len: 18
    std::cout << sv4.data() << " - till zero\n\n"; // prints Hello - til


    string s = "Hello World";
    std::cout << s << ",len: "<< s.size() << '\n';
    string_view sv = s;
    auto sv5 = sv.substr(0, 5);
    std::cout << "display substring 'Hello' non null terminated:\n";
    std::cout << sv5 << "\n\n"; // prints "Hello"
    std::cout << "display the char array of underlying string using .data() :\n";
    std::cout << sv5.data() << "\n\n"; /// prints "Hello World" because data() returns char array of underlying string
    std::cout << "display string view using printf:\n";
    printf("%.*s\n", static_cast<int>(sv5.size()), sv5.data()); // you must add the siz
}
