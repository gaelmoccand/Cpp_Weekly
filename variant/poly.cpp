#include <string>
#include <iostream>
#include <variant>
#include <vector>

class Triangle
{
public:
    void Render()  { std::cout << "Drawing a triangle!\n"; }
};

class Polygon
{
public:
    void Render() { std::cout << "Drawing a polygon!\n"; }
};

class Sphere
{
public:
    void Render() { std::cout << "Drawing a sphere!\n"; }
};

int main() 
{
    std::vector<std::variant<Triangle, Polygon, Sphere>> objects { 
        Polygon(), 
        Triangle(),
        Sphere(),
        Triangle()
    };

    auto CallRender = [](auto& obj) { obj.Render(); };

    for (auto& obj : objects)
        std::visit(CallRender, obj);

}