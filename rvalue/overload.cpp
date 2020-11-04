/* 
Only works with c++17

1) g++ -std=c++17 -O2 -Wall -pedantic dedguide.cpp -o dedguide

*/

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

std::string get_from_index(int idx) {
    std::vector<std::string> book;
    book.emplace_back("Hello");
    book.emplace_back("Bye");

    return book[idx];
}

template<class T>


class TextDisplayer
{
    public:
        explicit TextDisplayer(T&& text) : m_text(std::forward<T>(text)) {
            std::cout << "ctor fw" << std::endl;
        }

        explicit TextDisplayer(int idx) : m_text(get_from_index(idx)) {
            std::cout << "ctor int" << std::endl;
        }

        void show() {
            std::cout << m_text << std::endl;
        }
    private:
    T m_text;
};

template<class T> TextDisplayer(T&&) -> TextDisplayer<T>; // deduction guide ! Won't compile if commented only from C++17


std::string get_string_from_file() {
    std::stringstream strs("Hello World, Bye Bye!"); 
    std::string substr;
    std::getline(strs, substr,',');
    return substr;
}



int main()
{

    std::string txt = "Hello World";
    TextDisplayer displayer1(txt); // ok
    TextDisplayer displayer2(get_string_from_file()); // compilation error if deduction guide is missing 
    TextDisplayer displayer3("Hello World");    // compilation error if deeduction guide is missing 
   // TextDisplayer displayer4(0); // compilation error overload is ambiguous 


    displayer1.show();
    displayer2.show();
    displayer3.show();
}