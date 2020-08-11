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

    double& operator[](int i);
    const double& operator[](int i) const;

    int size() const;
};

Vector::Vector(size_t s)
{
    if(s < 0)
    {
        throw std::length_error{"Vector ctor: negative size"};
    }
    elem = new double[s];
    sz=s;
}

Vector::Vector(const Vector& other):elem{new double[other.sz]},sz{other.sz}
{
    std::copy(other.elem, other.elem + sz, elem);
}

Vector& Vector::operator=(const Vector& other)
{
    if(this == &other) return *this;  // checkr for self assignment
    delete[] elem;
    elem = new double[other.sz];
    std::copy(other.elem, other.elem + other.sz, elem);
    sz = other.sz;
    return *this;
}

const double& Vector::operator[](int i) const 
{
    if(i < 0 || size() <= i)
    {
        std::cout << "Vector::operator[]";
        return elem[0];
    }
    return elem[i];
}

double& Vector::operator[](int i) 
{
    if(i < 0 || size() <= i)
    {
        std::cout << "Vector::operator[]";
        return elem[0];
    }
    return elem[i];
}

int Vector::size() const
{
    return sz;
}

int main()
{

    Vector vec1(3);

    vec1[0]=1;
    vec1[1]=2;
    vec1[2]=3;

    Vector vec2 = vec1; // copy ctor
    Vector vec3{vec1}; // copy ctor
    Vector vec4(3);
    vec4 = vec1; // copy assignement

    auto sz = vec1.size();

    std::cout << "vec1 size is : " << sz << "\n";
    std::cout << "vec1[0] is : " << vec1[2] << "\n";
    std::cout << "vec4[0] is : " << vec4[2] << "\n";

}


