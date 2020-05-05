#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // for_each
#include <set>

    void displayNationality(const std::string &player) {

        std::cout << player << " is ";
        if(player == "Djokovic") {
            std::cout << "Serbian" << std::endl;
        }
        else if (player == "Nadal"){
            std::cout << "Spanish" << std::endl;
        }
        else {
            std::cout << "Swiss" << std::endl;
        }
    }


template<typename Collection>
auto getIndex(Collection const &collection, size_t offset = 0) {

    return [&collection, offset](auto const& iter) {
        return offset + std::distance(begin(collection), iter);
    };
} 

int main() {

    std::vector<std::string> players {"Federer","Djokovic","Nadal","Wawrinka"}; 
    std::set<std::string> competitions  {"Roland Garros", "Wimbledon", "US open"};



    //Always Prefere a range-for loop to iterate 

    std::cout << "Players using for loop" << std::endl;
    // 1. Bad Readability. Error Prone
    for (auto iter = begin(players); iter != end(players); ++iter) {
        std::cout << *iter << std::endl;
    }

    std::cout << "Players using for range loop " << std::endl;
    /* 2. Better:
      a. Use  & ref to avoid copying the element 
      b . Use const if u do not want to modify the element*/
    for (const auto &player : players) {
        std::cout << player << std::endl;
    }

    std::cout << "players Games " << std::endl;
    /* 3. In this case Can't be a range loop. It touches two elements */
    for (size_t idx = 1; idx < players.size(); ++idx) {
        std::cout << players[idx] << " VS " << players[idx-1] << std::endl;
    }

    /* 4. "for_each" can be used to increase the level of abstraction
    It does not show what is happening in the loop but call a function instead.
    You could also use a lambda instead the function but it will then break the level of abstraction */
    for_each(std::cbegin(players),std::cend(players),displayNationality);


    /* 5: How to find the index in a loop
    a. Do I really need the index ? 
    b. Simple not clean solution is to declare size_t idx = 0; outside the loop and increment it at each step 
        size_t idx = 0; 
    c. use C++20 range  */
    const auto index = getIndex(competitions,1);
    for (auto competition = begin(competitions); competition != cend(competitions); ++competition) {
        std::cout << index(competition) << " : " << *competition << std::endl;
    }

    //auto std::find_if()
}
