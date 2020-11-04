/* 
Only works with c++17

1) g++ -std=c++17 -O2 -Wall -pedantic dedguide.cpp -o dedguide

*/

#include <iostream>
#include <string>
#include <sstream>

template<class T>


class TextDisplayer
{
    public:
        explicit TextDisplayer(T&& text) : m_text(std::forward<T>(text)) {}

        void show() {
            std::cout << m_text << std::endl;
        }
    private:
    T m_text;
};

template<class T> TextDisplayer(T&&) -> TextDisplayer<T>; // deduction guide ! Won't compile if commented


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
    TextDisplayer displayer2(get_string_from_file()); // error if deduction guide is missing 
    TextDisplayer displayer3("Hello World");    // error if deeduction guide is missing 

    displayer1.show();
    displayer2.show();
    displayer3.show();
}