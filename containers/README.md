# How to add and remove element in contigous and associative container
## 1. Add element in a map
### 1.1 Overview for map  
Here is the overview of  different methods:
It is divided in 2 columns: 1st one **overwite value** if element already present, the 2nd does not
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

### 1.3  emplace() 
* C++11 introduced **emplace()** which has the same functionality as **insert()** but it enables in-place construction.
In-place construction is a technique that bypasses construction and destruction of temporaries by constructing the objects directly in the map [1].

```cpp
struct A
{
    std::string name;
    int age;
    ...
}
auto m = std::map<int, A> {};
// Ctor. Copy ctor. Move ctor. Dtor. Dtor. Dtor. A copy constructor call because of copy-list-initialization.
m.insert({1, {"Anna", 63}});
// Ctor. Move ctor. Dtor. Dtor. Performance gain
m.emplace(1, A("Anna", 63)):
```
Notice that **emplace()** do not need {} like insert()

Scott Meyers is in favor of **emplace()** and other experts like Bjarne and [Abseil](https://abseil.io/tips/112) are in favor of insert() for safety reason 

### 1.4 try_emplace()

**try_emplace()** is a safer successor of insert() or emplace(). In line with insert() and emplace(), try_emplace() doesn’t modify values for already inserted elements. 
However, it prevents stealing from original arguments that happens both with insert() and emplace() in case of a failed insertion [1].
```cpp
auto m = std::map<int, std::unique_ptr<A>> {};
m.emplace(1, std::make_unique<A>("Anna",63)); 

auto p = std::make_unique<A>("John",47);
// p won't be inserted because the key 1 already exists
m.emplace(1, std::move(p)); // but steals arguments
//but it still might get plundered!!!
assert(p != nullptr); // this will most likely fail
```

try_emplace() makes sure that the argument remains untouched in case it wasn’t inserted:

```cpp
m.try_emplace(1, std::move(p));
// no plundering in case insertion failed
assert(p != nullptr);
```

The second advantage of **try_emplace()** is it will not generate a object for already existing keys in the map.
Remember try_emplace do not modify already inserted element.

```cpp
// std::map m with the original object
auto m = std::map<int, A> {};
m.emplace(1, A("Ann", 63));

// doesn't generate a redundant object
m.try_emplace(1, "Ben", 47);
```

### 1.5 insert_or_assign

**insert_or_assign()** is a “smarter” successor of operator[]. Just like operator[] it modifies values if supplied with a key that is already present in the map.
 However, unlike operator[], insert_or_assign() doesn’t require default constructibility of the value_type. 
 It also returns a **pair<iterator, bool>**. 
 * The bool is true when insertion took place and false in case of assignment.

```cpp
    std::map<std::string, int> phoneBook;
    phoneBook.insert_or_assign("Mary", 1234);
    auto [it, inserted] = phoneBook.insert_or_assign("Mary", 666);
    assert(inserted == false);
```

## References
1. https://www.fluentcpp.com/2018/12/11/overview-of-std-map-insertion-emplacement-methods-in-cpp17/
2. https://www.oreilly.com/library/view/effective-modern-c/9781491908419/item42

