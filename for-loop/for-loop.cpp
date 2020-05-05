#include <iostream>
#include <string>
#include <vector>




int main() {

    std::vector<std::string> players {"Federer","Djokovic","Nadal","Wawrinka"}; 

    //Prefere a range-for loop to iterate 

    std::cout << "Players using for loop" << std::endl;
    // Bad Readability. Error Prone
    for (auto iter = players.begin(); iter != players.end(); ++iter) {
        std::cout << *iter << std::endl;
    }

    std::cout << "Players using for range loop " << std::endl;
    // Better:
    // 1. Use  & ref to avoid copying the element 
    // 2. Use const if u do not want to modify the element
    for (const auto &player : players) {
        std::cout << player << std::endl;
    }

    std::cout << "players Games " << std::endl;
    // In this case Can't be a range loop. It touches two elements
    for (size_t idx = 1; idx < players.size(); ++idx) {
        std::cout << players[idx] << " VS " << players[idx-1] << std::endl;
    }
}
