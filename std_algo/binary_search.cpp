#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <optional>
   
   
// 2. Binary search O(log(n))

template<typename T, typename K>
std::optional<int> searchElem(const T& seq, const K elem) {
    auto first = seq.begin();
    auto last = seq.end();
    if (elem < *first || elem > *std::prev(last)) return std::nullopt;

    while (first < last) {
        auto const N = std::distance(first, last);
        auto const pivot = std::next(first, N / 2);
        if (elem == *pivot) {
            auto found = std::distance(seq.begin(), pivot);
            return std::make_optional(found);
        }
        else if (elem < *pivot) {
            last = pivot;
        }
        else {
            first = pivot;
        }
    }
    return std::nullopt;
}

template<typename FwdIt, typename Compare = std::less<>>
void quickSort(FwdIt first, FwdIt last, Compare cmp = Compare{}) {
    auto const N = std::distance(first, last);
    if (N <= 1) return; 
    auto const pivot = std::next(first, N / 2);
    std::nth_element(first, pivot, last, cmp);
    quickSort(first, pivot, cmp); 
    quickSort(pivot, last, cmp); 
}

int main() {

    auto v4 = std::vector{7,0,1,3,4,5,6,2,8};
    quickSort(v4.begin(), v4.end());

    if (auto position = searchElem(v4, 8); position) {
        std::cout << "found at pos :" << *position;
    }
}