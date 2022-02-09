#include <iostream>
#include <any>
#include <string>
#include <cassert>


int main() {

    // 1.1 Create the variable
    std::any anyEmpty;
    std::any anyInit {10};
    std::any anyInPlace {std::in_place_type<std::string>,"Hi Guys !"}; // create in place object
    std::any anyMake = std::make_any<std::string>("Yeap !"); // also create in place object

    assert(!anyEmpty.has_value());

    std::cout << std::any_cast<int>(anyInit) << '\n';
    std::cout << std::any_cast<std::string>(anyInPlace) << '\n';
    std::cout << std::any_cast<std::string>(anyMake) << '\n';

    // 1.2 Change the value
    anyInit = 11;
    anyInPlace.emplace<std::string>("Bye Bye");
    
    // 1.3 Accessing the stored Value
    try
    {
        std::any_cast<int>(anyInit); // read
        std::any_cast<std::string&>(anyInPlace) = "I'll be back";
        std::cout << std::any_cast<std::string&>(anyInPlace) << '\n';
    }
    catch (const std::bad_any_cast& e)
    {
        std::cerr << e.what() << '\n';
    }

    // use pointer if do not want to use exception
    if (auto* pt = std::any_cast<int>(&anyInit); pt)
    {
        std::cout << "value before: " <<*pt << '\n';
        *pt=100;
        std::cout << *pt << '\n';
    }
    
    //

}
