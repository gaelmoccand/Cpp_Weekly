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
    if (elem < *first)
        return std::nullopt;
    if (elem > *std::prev(last)) 
        return std::nullopt;

    auto N = std::distance(first, last);
    while (first < last) {
        auto const pivot = std::next(first, N/2);
        if (*pivot == elem) {
            auto found = std::distance(seq.begin(), pivot);
            return std::make_optional(found);
        }
        else if ( elem < *pivot) {
            last = pivot;
        }
        else {
            first = pivot;
        }
        N = std::distance(first, last);
    }
    return std::nullopt;
}

int main() {

    auto v4 = std::vector{0,1,2,3,4,5,6,7,8}; // sorted vector
    
    if (auto position = searchElem(v4, 9); position) {
        std::cout << "found at pos :" << *position;
    }
}