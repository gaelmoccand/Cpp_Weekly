#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <optional>
   
   
// 2. Binary search O(log(n))

template<typename T, typename K>
std::optional<int> searchElem(const T& seq, const K elem){
    auto first = seq.begin();
    auto last = seq.end();
    while (first < last) {
        auto middle = first + std::distance(first, last)/2;
        if (*middle == elem) {
            auto found = std::distance(seq.begin(), middle);
            return std::make_optional(found);
        }
        else if ( elem < *middle){
            last += std::distance(last, middle);
        }
        else {
            first -= std::distance(first, middle);
        }
    }
    return std::nullopt;
}

int main() {

    auto v4 = std::vector{0,1,2,3,4,5,6,7,8}; // sorted vector
    
    if(auto position = searchElem(v4, 7); position) {
        std::cout << "found at pos :" << *position;
    }
}