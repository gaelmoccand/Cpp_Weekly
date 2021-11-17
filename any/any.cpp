#include <iostream>
#include <any>
#include <string>
#include <cassert>

int main() {

    std::any anyEmpty;
    std::any anyInit {10};
    std::any anyInPlace {std::in_place_type<std::string>,"Hi Guys !"}; // create in place object
    std::any anyMake = std::make_any<std::string>("Yeap !"); // also create in place object

    assert(!anyEmpty.has_value());

    std::cout << std::any_cast<int>(anyInit) << '\n';
    std::cout << std::any_cast<std::string>(anyInPlace) << '\n';
    std::cout << std::any_cast<std::string>(anyMake) << '\n';







}