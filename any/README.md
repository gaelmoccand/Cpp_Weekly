# std::any learn how to use it

## Introduction
The goal of **std::any** is to replace the use _void*_. You could still use _void*_ but it is an unsafe pattern and like any raw pointer you have to manage the lifetime of your object. So _std::any_ is the Modern version of _void*_.

## Basic usage

### 1.1 create the variable

There are several ways you can create std::any object:

* a default initialisation - then the object is empty
* a direct initialisation with a value/object
* in place _std::in_place_type_
* via _std::make_any_

Both _std::make_any_ and _std::in_place_type_ efficently create in place object.

```cpp
    std::any anyEmpty;
    std::any anyInit {10};
    std::any anyInPlace {in_place_type<string>,"Hi Guys !"}; // create in place object
    std::any anyMake = make_any<string>("Yeap !"); // also create in place object
```

### 1.2 change the Value

To change the value you have two options: assignement operator or _emplace_.

```cpp
    anyInit = 11;
    anyInPlace.emplace<std::string>("Bye Bye");
```


### 1.3 access the stored Value

In order to access the value you should use std::any_cast

This function has 3 modes:
* read access - returns a copy of the value, and throws _std::bad_any_cast_ when it fails
* read/write access - returns a reference, and throws _std::bad_any_cast_ when it fails
* read/write access - returns a pointer to the value (const or not) or nullptr on failure

```cpp
    try
    {
        std::any_cast<int>(anyInit); // read
        std::any_cast<std::string&>(anyInPlace) = "I'll be back";
    }
    catch (const std::bad_any_cast& e)
    {
        ...
    }

    // use pointer if do not want to use exception
    if (auto* pt = std::any_cast<int>(&anyInit); pt) // noexcept.
    {
        *pt=100;
    }
```

### 1.5 Examples

* Message Passing eg Windows API
* Parsing config files [https://www.cppstories.com/2018/06/variant/#examples-of-stdvariant]
* Properties class 

```cpp
struct property
{
    property();
    property(const std::string &, const std::any &);

    std::string name;
    std::any value;
};

typedef std::vector<property> properties;
```



### 1.6 Performance and memory usage 

In c++ and in life in general there is no free lunch. So this cool _std::any_ feature is not an exception come with an extra cost.

The cost is the use of extra dynamic memory allocations.

_std::variant_ and _std::optional_ don't require any extra memory allocations but this is because they know which type (or types) will be stored in the object. std::any does not know which types might be stored and that's why it might use some free store memory.

Fortunately for us, Implementations are encouraged to use **SBO** (Small Buffer Optimisation).  
So if your type is an _int_ std::any won't use the free store memory.
But that also comes at some cost: it will make the type larger - to fit the buffer.
   
* sizeof(any) for GCC 8.1:  16 bytes
* sizeof(any) for Clang 7:  32 bytes

In general, as you see, std::any is not a “simple” type and it brings some of overhead with it. 
It's usually not small - due to SBO - it takes 16 or 32 bytes (GCC, Clang, or even 64 bytes in MSVC).

# References:

[https://www.cppstories.com/2018/06/variant/#examples-of-stdvariant]
