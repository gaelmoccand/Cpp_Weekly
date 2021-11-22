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
    auto firstOdd = std::find_if(v1.cbegin(), v1.cend(), [&v1](const auto &elem) {
        return ((elem % 2) == 1);
    });

    if (firstOdd != v1.cend()) {
        std::cout << " 1st Odd element found is: " << *firstOdd << "\n";
    }

    // 1.2 minmax find the minimum element inside the container
    auto [min, max] = std::minmax_element(v1.cbegin(), v1.cend());

    std::cout << " the min is: " << *min << "the max is: " << *max << "\n";

    // 1.3 any_of
    auto predIsVal7 = [&v1](const auto &el){return el == 7;};
    if (auto hasVal7 = std::any_of(v1.cbegin(), v1.cend(), predIsVal7)) { // see also all_of, none_of
        std::cout << "contains value of 7: " << std::boolalpha << hasVal7 << "\n" ;
    }

    // 1.4 check that both ranges are equal return a bool
    auto v2{v1};
    auto isEqual = std::equal(v1.begin() + 1, v1.end(), v2.begin() + 1);
    std::cout << "v1 and v2 are equal: " << std::boolalpha << isEqual << "\n";

    // 1.5 mismatch return point to the first element in both sequences that did not compare equal to each other.
    // also a vesion exist with a predicate as 4th argument
    v2[3] += 10;
    display(v2);

    auto [mismatchV1, mismatchV2] = std::mismatch(v1.begin(), v1.end(), v2.begin());
    std::cout << "v1 element " << *mismatchV1 << " mismatch v2 element " << *mismatchV2 <<"\n";

    // 1.6 search a sequence and return the 1st occurence
    auto seq = std::vector{4,5,6};
    if (auto foundSeq = search(v1.begin(), v1.end(), seq.begin(), seq.end()); foundSeq != v1.end()) {
        std::cout << "found seq [4,5,6] inside container: " << *foundSeq;
    }


}