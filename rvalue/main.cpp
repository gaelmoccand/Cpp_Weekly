/* 

Try both versions and check the differnces.
To disable Rreturn Value OPtimization RVO use flag -fno-elide-constructors 

1) g++ -std=c++17 -O2 -Wall -pedantic -fno-elide-constructors main.cpp -o main
2) g++ -std=c++11 -O2 -Wall -pedantic -fno-elide-constructors main.cpp -o main
3) g++ -std=c++17 -O2 -Wall -pedantic main.cpp -o main


Note: this code is not optimal yet. Copy-and-swap idiom should be used to avoid code duplication and self assignement check. 

*/

#include <iostream>
#include <string>
#include <stdexcept>
#include <exception>




class Vector{
 private:
    double* m_elem;
    size_t m_sz;
 public:
    explicit Vector(size_t s);
    ~Vector(){delete[] m_elem;}

    Vector(const Vector& vec);
    Vector& operator=(const Vector& vec);

    Vector( Vector&& vec);
    Vector& operator=( Vector&& vec);

    //Vector( Vector&& vec) = delete;
    //Vector(const Vector& vec) = delete;


    double& operator[](int i);
    const double& operator[](int i) const;

    int size() const;
};

Vector::Vector(size_t s)
{
    std::cout << "ctor" << std::endl;
    if (s < 0)
    {
        throw std::length_error{"Vector ctor: negative size"};
    }
    m_elem = new double[s];
    m_sz=s;
}

 Vector::Vector(const Vector& other)
:m_elem{new double[other.m_sz]},m_sz{other.m_sz}            // input const -> left untouched; create a new array with the same size
{
    std::cout << "copy ctor" << std::endl;
    std::copy(other.m_elem, other.m_elem + m_sz, m_elem);   // deep copy is required
}

Vector& Vector::operator=(const Vector& other) // input const -> left untouched
{
    std::cout << "copy assignement" << std::endl;
    if(this == &other) return *this;    // check for self assignment
    delete[] m_elem;
    m_elem = new double[other.m_sz];
    std::copy(other.m_elem, other.m_elem + other.m_sz, m_elem);
    m_sz = other.m_sz;
    return *this;                       // by convention a reference to this class is returned
}

 Vector::Vector(Vector&& other)
:m_elem{other.m_elem},m_sz{other.m_sz}          // steal the data first for the rvalue reference
{
                                        // no deep copy involved here just moving ressources
    std::cout << "mv ctor" << std::endl;
    other.m_elem = nullptr;               // important to set rvalue ref data to valid state
    other.m_sz = 0;                       // to preven it being accidentally delted when the temporary object dies
}
 
Vector& Vector::operator=(Vector&& other)
{
    std::cout << "mv assignement" << std::endl;

    if (this == &other) return *this;    // check for self assignment
    delete[] m_elem;                      // clean of actual ressource
    m_elem = other.m_elem;
    m_sz = other.m_sz;

    other.m_elem = nullptr;               // put temp. object in valid state
    other.m_sz = 0;
    return *this;                       // by convention a reference to this class is returned
}

const double& Vector::operator[](int i) const 
{
    if (i < 0 || size() <= i)
    {
        throw std::out_of_range{"Vector ctor: negative size"};
    }
    return m_elem[i];
}

double& Vector::operator[](int i) 
{
    if (i < 0 || size() <= i)
    {
        throw std::out_of_range{"Vector ctor: negative size"};
    }
    return m_elem[i];
}

int Vector::size() const
{
    return m_sz;
}

Vector MakeVectorRVO(size_t s)
{
    return Vector(s);
}


Vector MakeVectorNRVO()
{
    Vector vec{2};
    return vec; // compiler must elide copy OR std:move(vec) 
    //return std::move(vec); // try to use std::move and observe the impact on RVO 
}

Vector&& MakeVectorWrong()
{
    Vector vec{2};
    return std::move(vec); //  warning: function returns address of local variable !!!
}

int main()
{

    std::cout << "Vector vec1(3); \n";
    Vector vec1(3);         // ctor
    vec1[0] = 1;
    vec1[1] = 2;
    vec1[2] = 3;
    std::cout << std::endl;

    std::cout << "Vector vec2 = vec1; \n";
    Vector vec2 = vec1;     // copy ctor
    std::cout << std::endl;

    std::cout << "Vector vec3{vec1}; \n";
    Vector vec3{vec1};      // copy ctor
    std::cout << std::endl;

    std::cout << "Vector vec4(3); \n";
    Vector vec4(3);         // ctor
    std::cout << "vec4 = vec1; \n";
    vec4 = vec1;            // copy assignement
    std::cout << std::endl;

    std::cout << "Vector vec5 = std::move(vec2); \n";
    Vector vec5 = std::move(vec2);// mv ctor 
    //vec2.size();             // NOOOOO MUST not use vec2 anymore vec2 value is unknown !
    std::cout << std::endl; 

    std::cout << "Vector vec6 = MakeVectorRVO(); \n";
    Vector vec6 = MakeVectorRVO(3);   // RVO should occur
    std::cout << std::endl;

    std::cout << "vec6 = MakeVectorRVO(); \n";
    vec6 = MakeVectorRVO(4);          // RVO should occur
    std::cout << std::endl;

    std::cout << "Vector vec7 = MakeVectorNRVO(); \n";
    Vector vec7 = MakeVectorNRVO(); // RVO should occur
    std::cout << std::endl;

    std::cout << "Vector vec9 = MakeVectorWrong(); \n";
    //auto vec9 = MakeVectorWrong(); // have compilator warning
    //std::cout << vec9.size(); 
    std::cout << std::endl;
}


