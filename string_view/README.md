## Why do we need string_stream

In C++ **std::string**, solves the issue of C char* by providing a powerfull and rich interface and managing its characters and dealing with the associated memory.
Ownership and memory management of the array of characters is a big advantage, but it comes with with a price: each time we use a **std::string** its memory is allocated on the heap.
This can have a performance impact if your code need to build and destruct std::string many times.

To overcome this problem, string_view can be used. A **string_view** is a lightweight object that reference a C or C++ string. As its name suggests it does noot use the original string but it use a non owning view instead.

## std::string in action

Consider the example below [2]:

```cpp
string startFromWordStr(const string& strArg, const string& word) {
    return strArg.substr(strArg.find(word)); //3.  substr creates retrun new string
} //4. we might have a string copy or two for the return function (copy elision should take place here)

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

“Super” is a temporary const char* literal and it's passed as string_view word into the function. That's fine, as the temporary is guaranteed to live as long as the whole function invocation.
However, the result of string concatenation str + “ String” is a temporary and the function returns a string_view str_v of this temporary outside the call! Then Boom !
To sum up, when a string_view is returned from a function, then we should be  about the state of the string the string_view points to. 
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

### Handling Non-Null Terminated Strings

TBD


## string_view creation and its operations


## References

1. https://www.fluentcpp.com/2021/02/19/a-recap-on-string_view/
2. C++17 in Detail: Learn the Exciting Features of the New C++ Standard! by By: Bart?omiej Filipek



