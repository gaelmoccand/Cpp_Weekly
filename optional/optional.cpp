#include <iostream>
#include <optional>
#include <vector>
#include <complex>
#include <utility>



int main() {

    // 1. Create optional

    // empty:
    std::optional<int> emptyInt;
    std::optional<float> emptyFloat = std::nullopt;

    // direct specifying the type or not:
    std::optional<int> optInt(10);
    std::optional optIntDeduced(10); // deduction guides c++ 17

    // using make_optional
    auto optDouble = std::make_optional(3.0);
    // ctor with many arguments it is as efficent as in_place
    auto optComplex = std::make_optional<std::complex<double>>(3.0, 4.0); 

    // avoid creation of temp. will call vector with direct init of {1, 2, 3}
    std::optional<std::vector<int>> optVector(std::in_place, {1, 2, 3});

    // copy from other optional:
    auto oIntCopy = emptyInt;

    // 2. return optional from a function


}