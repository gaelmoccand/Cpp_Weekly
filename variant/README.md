## use std::variant

std::variant could be seen as type-safe union

### C style union

Let's have a look at the DSA example

```cpp

```

This is quite low level C style code. What if we want to use a kind of union in high level code in C++.
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
    std::visit(SampleVisitor{}, intFloatString);

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
* All the places where you might get a few types for a single field: so things like parsing command lines, ini files, ...
* Error handling - for example you can return variant **<Object, ErrorCode>**. If the value is available, then you return Object otherwise you assign some error code.
* State machines
* Polymorphism without vtables and inheritance (no Base class)

### std::variant creation

```cpp

```

## changing the value

```cpp

```

## accesing the stored value

```cpp

```

## Visitors

```cpp

```

1. https://www.cppstories.com/2018/06/variant/
