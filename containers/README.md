# How to add and remove element in contigous and associative container
## 1. Add element in a map
### 1.1 Overview for map  
Here is the overview of  different methods:
It is divided in 2 columns: 1st one overwite value if element already present, the 2nd does not
 ![insert_emplace](https://www.fluentcpp.com/wp-content/uploads/2018/11/insertion_methods1.png)

### 1.2  operator[] VS insert()
 * Operator[] differs from insert() in how it handles a situation when an element with the given key already exists in the map. 
 * While operator[] simply overwrites the corresponding value, insert() doesn’t [1].

```cpp
// C++11 style 
auto m = std::map<int, std::string>{{1, "Anna"}};
m[1] = "Ben";
assert(m.at(1) == "Ben");

auto m = std::map<int, std::string>{{1, "Anna"}};
m.insert({1,"Ben"});
assert(m.at(1) == "Anna");
```

 * operator[], no way of finding out whether the insertion actually took place, unless a prior lookup is performed (eg find())
 * **insert()** returns a **pair<iterator, bool>** that provides this information.

```cpp
// C++17 structured bindings style
auto[it, ins] = m.insert({2, "Anna"});
```
 * operator[] requires a DefaultConstructible "value_type", which means that if the default constructor is explicitly or implicitly disabled,
  the code won’t compile.



## References
1. https://www.fluentcpp.com/2018/12/11/overview-of-std-map-insertion-emplacement-methods-in-cpp17/
