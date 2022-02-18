## Why do we need string_stream

In C++ **std::string**, solves the issue of C char* by providing a powerfull and rich interface and managing its characters and dealing with the associated memory.
Ownership and memory management of the array of characters is a big advantage, but it comes with with a price: each time we use a **std::string** its memory is allocated on the heap.
This can have a performance impact if your code need to build and destruct std::string many times.

To overcome this problem, string_view can be used. A _string_view_ is a lightweight object that reference a C or C++ string. 
As its name suggests it does noot use the original string but it use a non owning view instead. It is like a pointer or a referaence in that it **does not own** the characters it points to.

## std::string in action

Consider the example below [2]:

```cpp
string startFromWordStr(const string& strArg, const string& word) {
    return strArg.substr(strArg.find(word)); //3.  substr creates retrun new string
}                                           
//4. we might have a string copy or two for the return function (copy elision should take place here)

string str {"Hello Guys the online meeting starts now"}; //1. create a 1st string str

auto subStr = startFromWordStr(str, "the online meeting starts now"); //2. create another string to convert const char* into const string&
cout << subStr << '\n';
```
We can see that many temporaries are created here. The copy elision must take place since C++ 17 and if the string is 15 characters or less then small string optimization take place and no memory is allocated to the free store (heap).

Have a look at the version using string_view.

```cpp
string_view startFromWord(std::string_view str, std::string_view word) {
     return str.substr(str.find(word)); // substr creates now  only a new view
}

string str {"Hello Guys the online meeting starts now"};//1. create only one string str
auto subView = startFromWord(str, "the online meeting starts now");
cout << subView << '\n';
```

This time we have only one memory allocation when string str is created.
Of course, **string_view** is copied - but since it's only a pointer and a length, it's much more efficient than the copy of the whole string.

In any case, it's important to remember that it's only a non-owning view, so if the original string object is deallocated, the view becomes invalid and you might get into trouble.

Moreover, **string_view** might not contain null terminator so your code has to support that as well. For example, it's never a good idea to pass string_view to a function that accepts null-terminated strings.

## string_view might also be risky

Have a look at this code [1]:
```cpp
string_view getName() {
    auto const name = std::string{"Dragon"};
    return name;
}
```
It leads to undefined behaviour because, the string name lifetime is not extended when the function returned but the string_view is still pointing to it.

In the previous example what about calling the function this way.

```cpp
string_view startFromWord(std::string_view str, std::string_view word) {
     return str.substr(str.find(word)); // substr creates now  only a new view
}

auto str = "My Super"s;
auto str_v = startFromWord(str + " String", "Super");
// use 'str_v' later in the code...
```

“Super” is a temporary const char* literal and it's passed as _string_view_ word into the function. That's fine, as the temporary is guaranteed to live as long as the whole function invocation.
However, the result of string concatenation str + “ String” is a temporary and the function returns a _string_view_ _str_v_ of this temporary outside the call! Then Boom !
To sum up, when a _string_view_ is returned from a function, then we should be  about the state of the string the _string_view_ points to. 
### Reference lifetime extension

Any issue in the code below:

```cpp
vector<int> GenerateVec() {
    return vector<int>(5, 1);
}

const vector<int>& refv = GenerateVec();
```

This code over is OK. the C++ rules say that the lifetime of a temporary object bound to a const reference is prolonged to the lifetime of the reference itself.

However it does not work in this case :

```cpp
string func() {
    string s;
    // build s...
    return s;
}

string_view sv = func(); // no temp lifetime extension!
```
**string_view** is also a constant view, so should behave almost like a const reference. But according to existing C++ rules, it's not. The compiler immediately destroys the temporary object after the whole expression is done. The lifetime cannot be extended in this case [2].

A way to solve this issue would be to rewrite the call like this:

```cpp
auto temp = func();
std::string_view str_v {temp}; // fine lifetime of temporary is extended through 'temp'
```
You have to be sure the lifetime of the object is correct when string_view is returned from function !

## string_view creation and its operations

operator[], at, front, back, data - are also const - so you cannot change the underlying character sequence (it's only “read access”).
You have also iterators like cbegin()/ crbegin() etc.

```cpp
const char* cstr = "Hello World";
// the whole string:
string_view sv1 {cstr};
cout << sv1 << ", len: " << sv1.size() << '\n'; // prints Hello World, len: 11

// slice
string_view sv2 {cstr, 5}; // not null-terminated!
cout << sv2 << ", len: " << sv2.size() << '\n'; // prints Hello, len: 5

// from string:
string str = "Hello String";
string_view sv3 = str;
cout << sv3 << ", len: " << sv3.size() << '\n'; // prints Hello String, len: 12

// ""sv literal
using namespace std::literals;
string_view sv4 = "Hello\0 Super World"sv;
cout << sv4 << ", len: " << sv4.size() << '\n'; // prints Hello Super World, len: 18
cout << sv4.data() << " - till zero\n"; // rpints Hello - till zero
```
### Handling Non-Null Terminated Strings

Check the code below to see the difference between cout and printf()

```cpp
string s = "Hello World";
cout << s.size() << '\n';
string_view sv = s;
auto sv2 = sv.substr(0, 5);
cout << sv2; // prints "Hello"
cout << sv2.data() << '\n'; /// prints "Hello World" because data() returns char array of underlying string

printf("%.*s\n", static_cast<int>(sv2.size()), sv2.data()); // you must add the size
```

To convert string into numbers use C++17 function **from_chars** because it is more efficient [3].
Don't use atof because it takes only the pointer to a null-terminated string, so string_view is not compatible. Moreover it is less efficient than from_chars.

```cpp
string number = "123.456";
string_view svNum {number.data(), 3};
int res = 0;
std::from_chars(svNum.data(), svNum.data() + svNum.size(), res);
cout << res << '\n';
```

In some particular cases, API or legacy code supports only null terminated strings. look:

```cpp
void convertAndShow(const char *str) {
       auto f = atof(str);
       std::cout << f << '\n';
}

string number = "123.456";
string_view svNum {number.data(), 3};
// ... 
string tempStr {svNum.data(), svNum.size()}; // To create string from string_view pass data() AND size()
convertAndShow(tempStr.c_str());
```

convertAndShow only works with null terminated strings, so the only way we have is to create a temporary string tempStr and then pass it to the function.


### string vs string_view substr 

*substr* gives probably the best advantage over the standard string *substr*.
It has the complexity of O(1) and not O(n) as with regular strings.

```cpp
static  void StringSubStr(benchmark::State& state) {
    std::string s = "Hello Super Extra Programming World";
    for (auto _ : state) {
        auto oneStr = s.substr(0, 5);
        auto twoStr = s.substr(6, 5);
        auto threeStr = s.substr(12, 5);
    }
}
...

static void StringViewSubStr(benchmark::State& state) {
    std::string s = "Hello Super Extra Programming World";
    for (auto _ : state) {
        std::string_view sv = s;
        auto oneSv = sv.substr(0, 5);
        auto twoSv = sv.substr(6, 5);
    }
}
...
```

For this test, we have 10x speed-up using the string view.

For this test, we have 10x speed-up!

In this blog post from  Rainer Grimm: C++17 - Avoid Copying with std::string_view - ModernesCpp.com. The performance is even 45 x better using a text file for the test. 

[Rainer Grimm c++17 ](http://www.modernescpp.com/index.php/c-17-avoid-copying-with-std-string-view/).



## class member init using string_view

Does string_view is the best candidate for member initialisation ?

Look at the code below: 

```cpp
class UserName {
      std::string m_name;

public:
      UserName(const std::string& str) : m_name(str) {}
};
```

Is it worth to replace this code by the folllowing using string_view ?

```cpp
UserName(std::string_view sv) : m_name(sv) {}

```

The answer is not that straitforward because we need to take into consideration SSO for short string optimization.
The approach with passing by value is consistent with Scoot Meyers book [4]

However, is std::string cheap to move?

Although the C++ Standard doesn't specify that, usually, strings are implemented with Small String Optimisation (SSO) - the string object contains extra space to fit characters without additional memory allocation. That means that moving a string is the same as copying it. And since the string is short, the copy is also fast.

Let s compare those alternatives implementations in three cases for a long string: 
* creating from a string literal
* creating from an l-value 
* creating from an rvalue reference

```cpp
// creation from a string literal
UserName u1{"John With Very Long Name"};

// creation from l-value:
std::string s2 {"Marc With Very Long Name"};
UserName u2 {s2};
// use s2 later...

// from r-value reference
std::string s3 {"Marc With Very Long Name"};
UserName u3 {std::move(s3)};
```

For const std::string&:

* u1 - two allocations: the first one creates a temp string and binds it to the input parameter, and then there's a copy into mName.
* u2 - one allocation: we have a no-cost binding to the reference, and then there's a copy into the member variable
* u3 - one allocation: we have a no-cost binding to the reference, and then there's a copy into the member variable.

You'd have to write a ctor taking r-value reference to skip one allocation for the u1 case, and also that could skip one copy for the u3 case (since we could move from r-value reference).

For std::string_view:

* u1 - one allocation - no copy/allocation for the input parameter, there's only one allocation when mName is created.
* u2 - one allocation - there's a cheap creation of a string_view for the argument, and then there's a copy into the member variable.
* u3 - one allocation - there's a cheap creation of a string_view for the argument, and then there's a copy into the member variable.

You'd also have to write a constructor taking r-value reference if you want to save one allocation in the u3 case, as you could move from r-value reference.


While the string_view behaves better when you pass a string literal, it's no better when you use it with existing string, or you move from it. However, since the introduction of move semantics in C++11, it's usually better, and safer to pass string as a value and then move from it.

```cpp
class UserName {
       std::string m_name;

public :
        UserName(std::string str) : m_name(std::move(str)) { }
};
```

For std::string:

* u1 - one allocation - for the input argument and then one move into the mName. It's better than with const std::string& where we got two memory allocations in that case. And similar to the string_view approach.
* u2 - one allocation - we have to copy the value into the argument, and then we can move from it.
* u3 - no allocations, only two move operations - that's better than with string_view and const string&!

When you pass std::string by value not only is the code simpler, there's also no need to write separate overloads for r-value references.

Let's reconsider our example of passing by value when the string is **SSO**:

```cpp
UserName u1{"John"}; // fits in SSO buffer

std::string s2 {"Marc"}; // fits in SSO buffer
UserName u2 {s2};

std::string s3 {"Marc"}; // fits in SSO buffer
UserName u3 {std::move(s3)};

```

In  passed by value case with short string then :

* u1 - two copies: the input argument is created from a string literal, and then there's copy into mName.
* u2 - two copies: one copy into the argument and then there's the second copy into the member.
* u3 - two copies: one copy into the argument (move means copy) and then there's the second copy into the member.

As you see for short strings passing by value might be “slower” when you pass some existing string, simply because you have two copies rather than one. On the other hand, the compiler might optimise the code better when it sees a value. What's more, short strings are cheap to copy so the potential “slowdown” might not be even visible.

All in all, **passing by value and then moving from a string argument is the preferred solution**. You have simple code and better performance for larger strings [2].
## References

1. https://www.fluentcpp.com/2021/02/19/a-recap-on-string_view/
2. C++17 in Detail: Learn the Exciting Features of the New C++ Standard! by By: Bart?omiej Filipek
3. https://www.fluentcpp.com/2018/07/27/how-to-efficiently-convert-a-string-to-an-int-in-c/
4. item 41 - “Consider pass by value for copyable parameters that are cheap to move and always copied”  from Effective Modern C++ by Scott Meyers see section rvalue.



