#include <iostream>
#include <optional>
#include <vector>
#include <complex>
#include <utility>
#include <algorithm>

std::optional<std::vector<int>::const_iterator> findElem(const std::vector<int> vec, int target) {
    // copy elision is mandatory in C++17 it's more optimal to use - not named optional - if possible -> not temp. created
    std::optional<std::vector<int>::const_iterator> posElem = std::find(vec.begin(), vec.end(), target);
    if (posElem != vec.end()) {
        return posElem;
    }
    return std::nullopt; 
}


int main() {

    // 1. Create optional
    std::cout << "Create Optionals \n";

    // optional set to empty:
    std::optional<int> optEmptyInt;
    std::optional<float> optEmptyFloat = std::nullopt;

    // direct specifying the type and using deduction guides:
    std::optional<int> optInt(10);
    std::optional optIntDeduced(10); // deduction guides c++17
    std::optional optFloatDeduced(10.0); // deduction guides c++17

    // avoid creation of temp. will call vector with direct init of {1, 2, 3} -> more efficient
    std::optional<std::vector<int>> optVector(std::in_place, {1, 2, 3});

    // using make_optional
    auto optDouble = std::make_optional(3.0);
    // ctor with many arguments it is as efficent as using in_place 
    auto optComplex = std::make_optional<std::complex<double>>(3.0, 4.0); 

    // copy from other optional:
    auto optIntCopy = optEmptyInt;

    // 2. return optional from a function
    std::vector myVec {1, 2, 3, 4};
    if (auto pos = std::find(myVec.begin(), myVec.end(), 0); pos != myVec.end()) {
        std::cout << "found elem 0 !";
    }

    // use optional respect the level of abstraction compare to the if()
    auto posElem = findElem(myVec, 2);
    if (posElem) {
        std::cout << "found elem 2 !";
    }



}