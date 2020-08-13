// g++ -std=c++17 -O2 -Wall -pedantic -fno-elide-constructors main.cpp -o main

#include <iostream>
#include <string>
#include <stdexcept>
#include <exception>

class Vector{
 private:
    double* elem;
    size_t sz;
 public:
    explicit Vector(size_t s);
    ~Vector(){delete[] elem;}

    Vector(const Vector& vec);
    Vector& operator=(const Vector& vec);

    Vector( Vector&& vec);
    Vector& operator=( Vector&& vec);

    double& operator[](int i);
    const double& operator[](int i) const;

    int size() const;
    Vector factory(size_t s);
};

Vector::Vector(size_t s)
{
    std::cout << "ctor" << std::endl;
    if(s < 0)
    {
        throw std::length_error{"Vector ctor: negative size"};
    }
    elem = new double[s];
    sz=s;
}

Vector::Vector(const Vector& other)
:elem{new double[other.sz]},sz{other.sz}            // input const -> left untouched; create a new array with the same size
{
    std::cout << "copy ctor" << std::endl;
    std::copy(other.elem, other.elem + sz, elem);   // deep copy is required
}

Vector::Vector(Vector&& other)
:elem{other.elem},sz{other.sz} // steal the data first for the rvalue reference
{
                                // no deep copy involved here just moving ressources
    std::cout << "mv ctor" << std::endl;
    other.elem = nullptr;       // important to set rvalue ref data to valid state
    other.sz = 0;               // to preven it being accidentally delted when the temporary object dies
}

Vector& Vector::operator=(const Vector& other) // input const -> left untouched
{
    std::cout << "copy assignement" << std::endl;
    if(this == &other) return *this;    // checkr for self assignment
    delete[] elem;
    elem = new double[other.sz];
    std::copy(other.elem, other.elem + other.sz, elem);
    sz = other.sz;
    return *this;                       // by convention a reference to this class is returned
}

Vector& Vector::operator=(Vector&& other)
{
    std::cout << "mv assignement" << std::endl;

    if(this == &other) return *this;

    delete[] elem;          // clean of actual ressource

    elem = other.elem;
    sz = other.sz;

    other.elem = nullptr;   // put temp. object in valid state
    other.sz = 0;
    return *this;
}

const double& Vector::operator[](int i) const 
{
    if(i < 0 || size() <= i)
    {
        throw std::out_of_range{"Vector ctor: negative size"};
    }
    return elem[i];
}

double& Vector::operator[](int i) 
{
    if(i < 0 || size() <= i)
    {
        throw std::out_of_range{"Vector ctor: negative size"};
    }
    return elem[i];
}

int Vector::size() const
{
    return sz;
}

Vector Vector::factory(size_t s)
{
    return Vector(s);
}


int main()
{

    Vector vec1(3);         // ctor

    vec1[0]=1;
    vec1[1]=2;
    vec1[2]=3;

    Vector vec2 = vec1;     // copy ctor
    Vector vec3{vec1};      // copy ctor
    Vector vec4(3);         // ctor
    vec4 = vec1;            // copy assignement

    Vector vec5 = std::move(vec2);// mv ctor 

    //vec2.size();             // NOOOOO MUST not use vec2 anymore !

    std::cout << "vec1[0] is : " << vec1[2] << "\n";
    std::cout << "vec4[0] is : " << vec4[2] << "\n";
    std::cout << "vec4[0] is : " << vec5[2] << "\n";

}


