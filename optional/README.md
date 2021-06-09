## std:: optional and how to use it

### 1. Introduction what is std::optional

Sometime we want to achieve a so called "nullable" type. It represent a object that can be "empty", "null" or "not set". Before C++17, it was possible to achieve it using "-1" value , "nullptr" or boolean.

Optional comes from functional programming like Rust (std::option).

For a given type T, **optional<T>** represent an object that can be:

* either a value of type T,
* or an “empty” value or not_set

Typical use cases are the following :

1. If You Want to Represent a Nullable Type [1]
    Rather than using unique values (like -1, nullptr or boolean).
2. Return from a funtion to indicate that the process failed to produce a value
    A typical example would be that no key was found in a map using find()
3. Passing an argument to a function that can be empty or not_set

Even if optional looks similar to pointer, they are different. Optional does not model a pointer at all. optional has value semantics, not pointer semantics. For instance copying a optional copies the wrapped value, and comparing 2 optionals compares the wrapped values.

### 2. How to create optional

Simply create optionals that are set to empty.

```cpp
    std::optional<int> emptyInt;
    std::optional<float> emptyFloat = std::nullopt;
```
std::optional is a wrapper type, so you should be able to create optional objects almost in the same way as the wrapped object. And in most cases you can:
It possible also to set to a value specifying the type or using deduction guide.

```cpp
    std::optional<int> optInt(10);
    std::optional optIntDeduct(10); // deduction guides c++17
```



```cpp
class UserName
{
public:
   UserName() : m_name("Default") {}
   // ...
};
    
std::optional<UserName> u0; // empty optional
std::optional<UserName> u1{}; // also empty

// optional with default constructed object:
std::optional<UserName> u2{UserName()};

```

That works but it creates an additional temporary object.
The code creates a temporary object and then moves it into the object stored in optional.

Here we can use a more efficient constructor - specifically by using **std::in_place** tag:

```cpp
std::optional<MyType> opt{std::in_place};
```
Then no additional temp object is created.
    
It is also possible to use **in_place** tag to specify that the default constructor must be used and to avoid the creation of temporary object and use of move ctor.
As it can be seen in the code below, using in_place tag avoid the unnecessary call to a move ctor with a temporary. The object stored in the optional is created in place, in the same way as you would call **MyType{}**. No additional copy or move is needed [1].
The tag **in_place** is also useful when the ctor have more than one argument. By default optional can work with a single argument ( Rvalue reference), and efficiently pass it to the wrapped type[1].

use **in_place** and the version of the constructor that handles variable argument list:
```cpp
std::optional<std::vector<int>> optVector(std::in_place, {1, 2, 3});
```

The last option is to use the factory method **make_optional**.
```cpp

auto opt = std::make_optional<Point>(0, 0);

auto opt = std::make_optional<UserName>();
```
    
The make_optional tag implements in place construction equivalent to:
```cpp
return std::optional<T>(std::in_place, std::forward<Args>(args)...);
```
And also thanks to mandatory copy elision from C++17 there is no temporary object involved.

However you will still need to use **in_place** tag for optional if you are using not movable or not copyable wrapped Type like mutex for instance.

### 3. return optional in a function

Returning with optional<T> from a function is a cleaner approach than returning using "-1" or "nullptr".

Have a look at the example below:

```cpp
std::vector myVec {1, 2, 3, 4};
std::optional<std::vector<int>::const_iterator> findElem(const std::vector<int> vec, int target) {
    std::optional<std::vector<int>::const_iterator> posElem = std::find(vec.begin(), vec.end(), target);
    if (posElem != vec.end()) {
        return posElem;
    }
    return std::nullopt; 
}

auto posElem = findElem(myVec, 2);
if (posElem) {
    std::cout << "found elem 2 !";
}
```

As it can be seen from the example over, using **optional<T>** to return from function respect the  level of abstraction. 

Always try to not named optional. The reason is that in C++17 copy elision is mandatory. If an local optional is declared inside the findElem()function then a temporary will be unnecessary created.  

Also note that according to the Standard if you wrap a return value into braces {} then you prevent move operations from happening. The returned object will be copied only[1].

### 4. Access the stored value

Before accessing the stored value, we must check if the value is present. This can be done using **has_value()** or simply using **if (optional<T>)**.

They are different ways to access the stored value:

* **operator*** and **operator->** - if there's no value the behaviour is undefined!
* **value()** - returns the value, or throws std::bad_optional_access
* **value_or(defaultVal)** - returns the value if available, or defaultVal otherwise

Warning, if we try to access an optional that does not hold a value, the results is **undefined** and exception is not thrown.

### 5. Using optional in function parameters

It is possible to use optional<T> as an argument to leverage on its not_set semantic.
Let's have a look at the following example to create partial db queries [3].

```cpp
std::string buildQuery(const boost::optional<std::string>& gender,
                       const boost::optional<std::string>& nationality,
                       const boost::optional<std::string>& eyeColor) {
    std::ostringstream oss;
    oss << "SELECT * FROM persons ";

    if (gender)
        oss << "WHERE gender = '" << *gender << "' ";
    if (nationality)
        oss << "AND nationality = '" << *nationality << "' ";git
    if (eyeColor)
        oss << "AND eyeColor = '" << *eyeColor << "'";
    return oss.str();
}

...
// caller with optional
buildQuery({"male"}, std::nullopt, {"brown"});
buildQuery({"female"}, {}, {"blue"});

```

### refactoring 

TBC

## References
1. "C++17 in Detail: Learn the Exciting Features of the New C++ Standard!" By: Bart?omiej Filipek
2. https://www.fluentcpp.com/2016/11/24/clearer-interfaces-with-optionalt/
3. https://www.fluentcpp.com/2016/12/01/partial-queries-with-optionalt/



