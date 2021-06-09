#include <iostream>
#include <optional>
#include <vector>
#include <complex>
#include <utility>
#include <algorithm>
#include <ctype.h>
#include <string>

std::optional<std::vector<int>::const_iterator> findElem(const std::vector<int> vec, int target) {
    // copy elision is mandatory in C++17 it's more optimal to use - not named optional - if possible -> not temp. created
    auto posElem = std::find(vec.begin(), vec.end(), target);
    if (posElem != vec.end()) {
        return posElem;
    }
    return std::nullopt; 
}

std::optional<int> newFct(const std::string& str_dec) {
    if (isdigit(str_dec[0])) {
        return std::stoi (str_dec, nullptr, 10);
    }
    return std::nullopt;
}

int legacyFct(const std::string& str_dec) {
    return newFct(str_dec).value_or(-1);
}

int main() {

    // 1. Create optional
    std::cout << "Create some Optionals \n";

    // optional set to empty:
    std::optional<int> optEmptyInt;
    if (optEmptyInt) {
    }
    else {
        std::cout << "optEmptyInt is empty!\n";
    }

    std::optional<float> optEmptyFloat = std::nullopt;
    if (optEmptyFloat) {
    }
    else {
        std::cout << "optEmptyFloat is empty!\n";
    }

    // direct specifying the type and using deduction guides:
    std::optional<int> optInt(10);
    std::optional optIntDeduced(10); // deduction guides c++17
    std::optional optFloatDeduced(10.0); // deduction guides c++17

    // avoid creation of temp. will call vector with direct init of {1, 2, 3} -> more efficient
    std::optional<std::vector<int>> optVector(std::in_place, {1, 2, 3});
    if (optVector) {
        std::cout << "optVector is not null!\n";
    }

    // using make_optional
    auto optDouble = std::make_optional(3.0);
    if (optDouble) {
        std::cout << "optDouble is not null!\n";
        std::cout << optDouble.value() << "\n";
    }
    // ctor with many arguments it is as efficent as using in_place 
    auto optComplex = std::make_optional<std::complex<double>>(3.0, 4.0); 
    if (optComplex) {
        std::cout << "optComplex is not null!\n";
        std::cout << *optComplex << "\n";
    }

    // copy from other optional:
    auto optIntCopy = optEmptyInt;

    // 2. return optional from a function
    std::vector myVec {1, 2, 3, 4};
    if (auto pos = std::find(myVec.begin(), myVec.end(), 0); pos != myVec.end()) {
        std::cout << "found elem 0 !\n";
    }

    // use optional respect the level of abstraction compare to the if()
    auto posElem = findElem(myVec, 2);
    if (posElem) {
        std::cout << "found elem  in myVec {1, 2, 3, 4}; \n";
    }

    //using optional to refactor legacy code without breakin the API
    auto intRes = legacyFct({"1234"}); // by convention -1 means no value
    if (intRes != -1) {
        std::cout << "converted into integer :" << intRes <<"\n";
    }

    auto optRes = newFct({"189"}); // now optional is used. Interface is cleaner
    if (optRes) {
        std::cout << "converted into int :" << optRes.value() <<"\n";
    }





}