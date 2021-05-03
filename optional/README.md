## std:: optional and how to use it

### 1. Introduction what is std::optional

Sometime we want to achieve a so called "nullable" type. It represent a object that can be "empty", "null" or "not set". Before C++17, it was possible to achieve it using "-1" value , "nullptr" or boolean.

Optional comes from functional programming like Rust (std::option).

Typical use cases are the following :

1. If You Want to Represent a Nullable Type [1]
    Rather than using unique values (like -1, nullptr or boolean).
2. Return from a funtion to indicate that the process failed to produce a value
    A typical example would be that no key was found in a map using find()
3. Passing an argument to a function that can be optional

### 2. How to create optional

### 3. return optional in a function



## References
1. "C++17 in Detail: Learn the Exciting Features of the New C++ Standard!" By: Bart?omiej Filipek
2. https://www.fluentcpp.com/2016/11/24/clearer-interfaces-with-optionalt/


