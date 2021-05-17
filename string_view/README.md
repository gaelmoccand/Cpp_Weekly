## Why do we need string_stream

In C++ **std::string**, solves the issue of C char* by providing a powerfull and rich interface and managing its characters and dealing with the associated memory.
Ownership and memory management of the array of characters is a big advantage, but it comes with with a price: each time we use a **std::string** its memory is allocated on the heap.
This can have a performance impact if your code need to build and destruct std::string many times.

To overcome this problem, string_view can be used. A **string_view** is a lightweight object that reference a C or C++ string. As its name suggests it does noot use the original string but it use a non owning view instead.

