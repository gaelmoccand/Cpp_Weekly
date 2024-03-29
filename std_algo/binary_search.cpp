#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <optional>
   
   
// 2. Binary search O(log(n))

template<typename T, typename K>
const typename T::iterator binarySearch(T& seq, const K elem) {
    auto first = seq.begin();
    auto last = seq.end();
    if (elem < *first || elem > *std::prev(last)) return seq.end();

    while (first < last) {
        auto const N = std::distance(first, last);
        auto const pivot = std::next(first, N / 2);
        if (elem == *pivot) {
            return (pivot);
        }
        else if (elem < *pivot) {
            last = pivot;
        }
        else {
            first = pivot;
        }
    }
    return seq.end();
}

int main() {

    auto v4 = std::vector{7,0,1,3,4,5,6,2,8};
    std::sort(v4.begin(), v4.end());

    //1. Binary search using std::lower_bound and not
    std::cout << "\n";
    if (std::vector<int>::iterator position = binarySearch(v4, 7); position != v4.end()) {
        std::cout << "found at pos :" << *position << "\n";
    }

    if (auto position = std::lower_bound(v4.begin(), v4.end(), 0); position != v4.end()) {
        std::cout << "found at pos using lower bound :" << *position << "\n";
    }

    //2. example of auto completion, searching a prefix using std::equal_range
    std::vector<std::string> dico {"auto","circus","deque","doctor","dog","done","doom","door","enough"};
    const std::string prefix {"do"};
    auto [foundStart, foundEnd] = std::equal_range(dico.begin(), dico.end(), prefix, [n = prefix.size()](auto prevElem, auto nextElem){
        return prevElem.substr(0,n) < nextElem.substr(0,n);
    });
    if(foundStart != dico.end() && foundEnd != dico.end()){
       for_each(foundStart, foundEnd, [](const auto & elem){ std::cout << elem <<":";}); 
    }


}