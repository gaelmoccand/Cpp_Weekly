## use std::variant

std::variant could be seen as type-safe union

### C style union

In the embedded system world we use a lot C union. 
Different endianness for different processor achitecture eg (ARM vs x86) are very important.

```cpp
typedef union
{
    struct {
        unsigned char byte1;
        unsigned char byte2;
        unsigned char byte3;
        unsigned char byte4;
    } bytes;
    unsigned int dword;
} HW_Register;
HW_Register reg;

reg.dword = 0x12345678;
reg.bytes.byte3 = 4;

```
While it is a valid option for low level programming such register access and byte manipulation. Is it a good fit for high level C++ programming ?
Union requires are very basic bud you do not know which type is currenbtly active in the union and dtor of th4e underlying type will not be called when switching type. 
A lot of boiler code is required to know which type is active and call the approriate ctor/dtor before switching to new type.

What could make unions better ?

* ability to use complex type : lifetime, dtor call & do not leak
* which type is active


## Basic Example

```cpp
struct SampleVisitor {
    void operator()(int i) const { 
        std::cout << "int: " << i << "\n"; 
    }
    void operator()(float f) const { 
        std::cout << "float: " << f << "\n"; 
    }
    void operator()(const std::string& s) const { 
        std::cout << "string: " << s << "\n"; 
    }
};

int main() {
    std::variant<int, float, std::string> intFloatString;
    static_assert(std::variant_size_v<decltype(intFloatString)> == 3);

    // default initialized to the first alternative, should be 0
    // index will show the currently used 'type'
    std::cout << "index = " << intFloatString.index() << std::endl;
    intFloatString = 100.0f;
    std::cout << "index = " << intFloatString.index() << std::endl;
    intFloatString = "hello super world";
    std::cout << "index = " << intFloatString.index() << std::endl;

    // try with get_if:
    if (const auto intPtr (std::get_if<int>(&intFloatString)); intPtr) 
        std::cout << "int!" << *intPtr << "\n";
    else if (const auto floatPtr (std::get_if<float>(&intFloatString)); floatPtr) 
        std::cout << "float!" << *floatPtr << "\n";

    if (std::holds_alternative<int>(intFloatString))
        std::cout << "the variant holds an int!\n";
    else if (std::holds_alternative<float>(intFloatString))
        std::cout << "the variant holds a float\n";
    else if (std::holds_alternative<std::string>(intFloatString))
        std::cout << "the variant holds a string\n";  

    // try/catch and bad_variant_access
    try {
        auto f = std::get<float>(intFloatString); 
        std::cout << "float! " << f << "\n";
    }
    catch (std::bad_variant_access&) {
        std::cout << "our variant doesn't hold float at this moment...\n";
    }

    // visit:
    std::visit(SampleVisitor{}, intFloatString);
    intFloatString = 10;
    std::visit(SampleVisitor{}, intFloatString);
    intFloatString = 10.0f;
    std::visit(SampleVisitor{}, intFloatString);
}
```
* You know what’s the currently used type via **index()** or check via **holds_alternative**.
* You can access the value by using **get_if** or **get** (might throw bad_variant_access exception)
* Type Safety - the variant doesn’t allow to get a value of the type that’s not active
* If you don’t initialize a variant with a value, then the variant is initialized with the **first type**. In that case the first alternative type must have a default constructor.
* No extra heap allocation happens
* You can use a visitor to invoke some action on a currently hold type.
* Important point, The variant class calls dtor and ctor of non-trivial types, so here the string object is cleaned up before we switch to new variants.


## Use cases 
* All the places where you might get a few types for a single field: so things like parsing command lines, ini Config files, ...
* Error handling - for example you can return variant **<Object, ErrorCode>**. If the value is available, then you return Object otherwise you assign some error code.
* State machines
* Polymorphism without vtables and inheritance (no Base class)

## std::variant creation

* By default the variant is init. with the first type.
    * if it is not possible when the type does not have a default ctor then compiler error
    * use **std::monostate** to use an empty type to represent empty space (eg no default ctor)
* Init by value then the best match type is used
    * if it is ambigous then use std::in_place_index

* use in_place to for complex type like vector

```cpp

// default initialization: (type has to has a default ctor)
std::variant<int, float> intFloat;

// pass a value:
std::variant<int, float, std::string> intFloatString { 10.5f };

// ambiguity
// double might convert to float or int, so the compiler cannot decide
//std::variant<int, float, std::string> intFloatString { 10.5 };

// ambiguity resolved by in_place
std::variant<long, float, std::string> longFloatString { std::in_place_index<1>, 7.6 }; // double!

// in_place for complex types
std::variant<std::vector<int>, std::string> vecStr { std::in_place_index<0>, { 0, 1, 2, 3 }};

```

## changing the value

We can change the value using:
* the assignment operator
* emplace
* get and then assign a new value for the currently active type

```cpp
std::variant<int, float, std::string> intFloatString { "Hello" };

intFloatString = 10; // we're now an int

intFloatString.emplace<2>(std::string("Hello")); // we're now string again

// std::get returns a reference, so you can change the value:
std::get<std::string>(intFloatString) += std::string(" World");

intFloatString = 10.1f;
if (auto pFloat = std::get_if<float>(&intFloatString); pFloat)
    *pFloat *= 2.0f;

```

## accesing the stored value

2 ways:
* get might throw exeption
* get_if returns a pointer to the active type or nullptr but it will not throw

```cpp
std::variant<int, float, std::string> intFloatString;
try {
    auto f = std::get<float>(intFloatString); 
    std::cout << "float! " << f << "\n";
}
catch (std::bad_variant_access&) {
    std::cout << "our variant doesn't hold float at this moment...\n";
}

if (const auto intPtr = std::get_if<int>(&intFloatString)) 
    std::cout << "int!" << *intPtr << "\n";


```

## Visitors for std::variant

You can use **std::visitor**. It can call a given visitir on all passed variants.
You can use either lambda or operator(). 
 
```cpp
// a generic lambda:
auto PrintVisitor = [](const auto& t) { std::cout << t << "\n"; };

std::variant<int, float, std::string> intFloatString { "Hello" };
std::visit(PrintVisitor, intFloatString);

auto PrintVisitor = [](const auto& t) { std::cout << t << "\n"; };
auto TwiceMoreVisitor = [](auto& t) { t*= 2; };

std::variant<int, float> intFloat { 20.4f };
std::visit(PrintVisitor, intFloat);
std::visit(TwiceMoreVisitor, intFloat);
std::visit(PrintVisitor, intFloat);

```

Generic Lambda work but sometime you need different actions for different underlying types.

```cpp
// visitor with operator()
struct MultiplyVisitor {
    float mFactor;

    MultiplyVisitor(float factor) : mFactor(factor) { }

    void operator()(int& i) const {
        i *= static_cast<int>(mFactor);
    }
    void operator()(float& f) const {
        f *= mFactor;
    }
    void operator()(std::string& ) const {
        // nothing to do here...
    }
};

std::visit(MultiplyVisitor(0.5f), intFloat);
std::visit(PrintVisitor, intFloat);

```

## Memory consideration

std::variant uses the memory in a similar way to union: so it will take the **max size of the underlying types**. But since we need something that will know what’s the currently active alternative, then we need to add some more space.

Plus everything needs to honour the alignment rules.

What’s more interesting is that std::variant won’t allocate any extra space! **No dynamic allocation** happens to hold variants.

To sum up you pay a little extra space for a type-safe functionality.

## Polymorphism example

no v-table needed

have a look at  code poly.cpp

1. https://www.cppstories.com/2018/06/variant/
2. C++17 in Detail: Learn the Exciting Features of the New C++ Standard! by By: Bartomiej Filipek
