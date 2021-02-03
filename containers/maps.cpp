#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <cassert>

template <typename M>
void disp(const M& container){
    std::cout << " \n";
    std::cout << "{ " << "\n";
    for (const auto& [key,value] : container) {
        std::cout << key << " : " << value << "\n";
    }
    std::cout << " }\n";
    std::cout << " \n";

}

int main() {

    using namespace std;
    map<string,int> phone_book { {"John", 124}, {"Mary", 345}};


    phone_book["Leon"] = 4; // no way to find out if insertion took place
    phone_book["Leon"] += 2; // modify the entry

    phone_book.insert({"Marc", 7876}); // do not overwrite value if key present

    disp(phone_book);

    // [] and isnert differ when element with givern key exist
    cout << "[\"Leon\"] = 5; insert(std::pair{\"Marc\", 6}); \n"; 
    phone_book["Leon"] = 5; // overwrite
    phone_book.insert(std::pair{"Marc", 6}); // do not 

    // C++ 17 style insert with iterator
    auto [itins ,inserted] = phone_book.insert({"Marc", 7}); // do not overwrite
    assert(inserted == false); 
    disp(phone_book);

    // emplace enables in place construction , we can ommit {}
    // drawback create object even if not inserted + steals argument anyway (unique ptr !!!)
    phone_book.emplace("Gianna", 799); // do not 
    cout << "emplace(\"Gianna\", 799) : \n"; 
    disp(phone_book);

    // doesn't generate a redundant object & does not steals argument when insertation failed
    // try_emplace() makes sure that the argument remains untouched in case it wasn’t inserted (unique_ptr())
    phone_book.try_emplace("Ben", 47);
    phone_book.try_emplace("Ben", 47); // do not overwrite
    cout << "try_emplace(\"Ben\", 47) : \n"; 
    disp(phone_book);

    // like [] it modifies values if supplied with a key that is already present in the map
    // It doesn’t require default constructibility of the value_type
    // returns a pair<iterator, bool> 
    auto [it, insert] = phone_book.insert_or_assign("Ben", 4864654); // overwrite value
    assert(insert == false);

    cout << "insert_or_assign(\"Ben\", 4864654) : \n"; 
    disp(phone_book);

    phone_book.clear();


}