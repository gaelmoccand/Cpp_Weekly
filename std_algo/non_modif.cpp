#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

template<typename T>
void display(T cont){
    std::cout << "{";
    for_each(cont.cbegin(), cont.cend(), [&cont](const auto &elem){
        std::cout << elem << ":";
    });
    std::cout << "}\n";
}

int main() {

    auto v1 = std::vector{0,1,2,3,4,5,6,7,8};
    display(v1);

    // 1. Non-Modifying Sequence Operation
    // 1.1 use find_if to find the 1st odd element
    auto firstOdd = std::find_if(v1.cbegin(), v1.cend(), [&v1](const auto &elem) {
        return ((elem % 2) == 1);
    });

    if (firstOdd != v1.cend()) {
        std::cout << "1st Odd element found is: " << *firstOdd << "\n";
    }

    // When the sequence is already sorted then use Binary Search ->O(logn) the find 1st elem. not < value
    if (auto foundElem = std::lower_bound(v1.begin(), v1.end(), 6); foundElem != v1.end()){
        std::cout << "found element not < 6 using Binary Search at pos: " << std::distance(v1.begin(), foundElem) << "\n";
    }

    // Same also ->O(logn) to find 1st elem. > value
    if (auto foundElem = std::upper_bound(v1.begin(), v1.end(), 6); foundElem != v1.end()){
        std::cout << "found element > 6 using Binary Search at pos: " << std::distance(v1.begin(), foundElem) << "\n";
    }

    // 1.2 minmax find the minimum element inside the container
    auto [min, max] = std::minmax_element(v1.cbegin(), v1.cend());
    std::cout << "the min is: " << *min << " the max is: " << *max << "\n";

    // 1.3 all of  and any_of
    auto inRange = [&v1](const auto &el){return (el >= 0) || (el <= 10);};
    auto isInRange = std::all_of(v1.begin(), v1.end(), inRange);
    std::cout << "all elements have values between 0 and 10: " << isInRange << "\n";

    auto predIsVal7 = [&v1](const auto &el){return el == 7;};
    auto hasVal7 = std::any_of(v1.cbegin(), v1.cend(), predIsVal7); // see  none_of
    std::cout << "contains value of 7: " << std::boolalpha << hasVal7 << "\n";
    
    // 1.4 check that both ranges are equal return a bool
    auto v2{v1};
    auto isEqual = std::equal(v1.begin() + 1, v1.end(), v2.begin() + 1);
    std::cout << "v1 and v2 are equal: " << std::boolalpha << isEqual << "\n";

    // 1.5 mismatch return point to the first element in both sequences that did not compare equal to each other.
    // also a vesion exist with a predicate as 4th argument
    std::cout << "modify value at [3] \n";
    v2[3] += 10;
    display(v2);

    auto [mismatchV1, mismatchV2] = std::mismatch(v1.begin(), v1.end(), v2.begin());
    std::cout << "v1 element " << *mismatchV1 << " mismatch v2 element " << *mismatchV2 <<"\n";

    // 1.6 search a sequence and return the 1st occurence
    auto seq = std::vector{4,5,6};
    if (auto foundSeq = search(v1.begin(), v1.end(), seq.begin(), seq.end()); foundSeq != v1.end()) {
        std::cout << "found seq [4,5,6] inside container: " << *foundSeq <<"\n";
    }

    // 2. Binary search O(log(n))

    auto v4 = std::vector{0,1,2,3,4,5,6,7,8};
    auto first = v4.begin();
    auto last = v4.end();
    int elem = 1;
    while (first <= last) {
        auto middle = first + std::distance(first, last)/2;
        if (*middle == elem) {
            std::cout << "found at " << std::distance(v4.begin(), middle);
            break;
        }
        else if ( elem < *middle){
            last += std::distance(last, middle);
        }
        else {
            first += std::distance(first, middle);
        }
    }

}