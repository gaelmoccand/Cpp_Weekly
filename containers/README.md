# How to add and remove element in contigous and associative container

## 0. Add element in a vector

### 0.1 using operator[] vs at() vs insert()

Let' start with **operator[]**. It is simple and return the element at the specified postion.
However it does not guarantee range checking !!! 
```cpp
    vector<int> v1 = {1, 10, 20, 5, 40, 50}; 
    v1[0] = 11;
    v1[1] += 2; // modify directly
    v1[2]++;
    cout << v1[6] // boom No out of range check !! Undefined behavior
```
A safer alternative is to use **at()**. You must then catch the an exception otherwise the program will terminate
```cpp
try {
    v1.at(0) = 10;
    v1.at(7) = 10;
}
catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
}
```

Another option is to use **insert()**. Important difference is it does not overwrite.
Vector is extended by inserting new elements before the element at the specified position effectively increasing the container size by the number of elements inserted. It returns an iterator that points to the first of the newly inserted elements.
```cpp
    auto it = v1.insert(v1.end(), 3);
    it = v1.insert(v1.end(), 4, 1); // fill up with 1 values 4 times
```

This is an expensive operation if not insert at the **end()** it cause container to relocate all the elements that were after position to their new positions.

### 0.2 how to add several elements into a vector

The suboptimal way to inserting several elements by using **std::back_inserter** is to couple it with **std::copy**:
```cpp
    std::vector<int> v1;
    std::vector<int> newElements = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 , 12};
    copy(newElements.begin(), newElements.end(), std::back_inserter(v1));
```

Here std::copy successively passes each of the elements in newElements to the output iterator, that adds them to v by calling its push_back method. And this does the job: after the execution of **std::copy**, all the elements from newElements have been effectively copied into v1 [2].

The problem here is before calling std::copy we know how many elements are going to be copied but this information is not used and push_back method is repeatedly called causing possibly multiple reallocations in the vector.

If the number of elements to be copied is known, it would reallocate once insted of several time using push_back().
So just use range insertion method.
```cpp
    std::vector<int> newElements = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 , 12};
    std::vector<int> v1{begin(newElements), end(newElements)}; // at vector init.
    
    std::vector<int> v2;
    v2.insert(end(v2), begin(newElements), end(newElements)); // appending new elements at the end of vector
```

If you need to replace elements and not appending them just user **assign()** instead

```cpp    
    std::vector<int> v2;
    v2.assign(begin(newElements), end(newElements)); // replace elements
```
In some case you do not have the information in advance of how many elements are going to be copied . Typical case using **copy_if()**. 

```cpp    
    auto pred = [](auto const &elem){ return elem % 2 == 0;};
    vector<int> v4;
    copy_if(v1.begin(), v1.end(), back_inserter(v4), pred);
    print(v4);
```

back_inserter is in this case required.

## 1. Add elements in a map
### 1.1 Overview for map  
Here is the overview of  different methods:
It is divided in 2 columns: 1st one **overwite value** if element already present, the 2nd does not
<img src="">
![insert methods](https://www.fluentcpp.com/wp-content/uploads/2018/11/insertion_methods1.png)
From www.fluentcpp.com


### 1.2  operator[] VS insert()
 * Operator[] differs from insert() in how it handles a situation when an element with the given key already exists in the map. 
 * While operator[] simply overwrites the corresponding value, insert() does not [1].

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
C++11 introduced **emplace()** which has the same functionality as **insert()** but it enables in-place construction.
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
* Notice that **emplace()** do not need {} like insert()

Scott Meyers is in favor of **emplace()** and other experts like Bjarne and [Abseil](https://abseil.io/tips/112) are in favor of insert() for safety reason 

### 1.4 try_emplace()

**try_emplace()** is a safer successor of insert() or emplace(). In line with insert() and emplace(), try_emplace() does not modify values for already inserted elements. 
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

try_emplace() makes sure that the argument remains untouched in case it was not inserted:

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
 However, unlike operator[], insert_or_assign() doesn’t require default constructibility of the value_type [1]. 
 It also returns a **pair<iterator, bool>**. 
 * The bool is true when insertion took place and false in case of assignment.

```cpp
std::map<std::string, int> phoneBook;
phoneBook.insert_or_assign("Mary", 1234);
auto [it, inserted] = phoneBook.insert_or_assign("Mary", 666);
assert(inserted == false);
```

Amazing cheat sheet from https://www.walletfox.com/course/cheatsheets_cpp.php

<img src="https://www.walletfox.com/course/cheatsheetsSource/stdmap_tryemplace_insertorassign_corr.png" width="400" height="400">

## 2. Remove elements in sequence container

Sequence container can be **vector, string, deque, list**

The 4 different ways to remove elements into a seq containers are :
* Removing the elements at a given position (or a range),
* Removing the elements equal to a certain value,
* Removing the elements satisfying a certain predicate,
* Removing the duplicates.

### 2.1 Remove at a give position

This is the most simple way to remove an element

```cpp
// 1. remove element at specific position
myvec.erase(myvec.begin() + 1);
// for a subrange (important for next point)
myvec.erase(myvec.begin(), myvec.begin() + 2); // erase the first 2 element of myvec
```
Like all the STL ranges represented by iterators:
* 1st arg. is included in the subrange.
* 2nd arg. is not included in the subrange. It points to the “past-the-end” element, like for instance myvec.end().

Note that for vector and string, all iterators pointing to elements at and after the one removed are invalidated. 
Indeed, all those elements have been shifted up by the call to erase [3].

### 2.2 Remove element for a certain value

Until C++20 vector, deque, string containers don’t have a method to remove a value so we must use the algo **std::remove**. 
This algorithm takes a range and a value to remove, and shifts up all the elements that are to be kept [3].

check this code and the figure below

```cpp
vector<int> myvec{1, 3, 42, 2, 42, 7, 1, 4};
Check std::remove(myvec.begin(), myvec.end(), 42);
```

![remove](https://www.fluentcpp.com/wp-content/uploads/2018/01/remove2.png)

Note that the values of the elements left at the end of the range are unspecified.
A bit like std::move doesn’t move , **_std::remove_ doesn’t remove**.

Indeed, bearing in mind that, in the design of the STL, algorithms interact only with iterators, and not directly with the container, the container is not aware of the effect of the algorithm. For instance its size has not been reduced.

So we need to use the erase & remove C++ idiom.
The elements to remove are in the range defined by the iterator **returned by _std::remove_** and the end of the collection.
```cpp
vector<int> myvec{1, 2, 3, 4, 5, 6, 7, 8, 8, 9, 9, -1, -2};
string mystr{"Hi Folks !!"};
// using erase_remove C++ idiom
auto iter = std::remove(begin(myvec), end(myvec), 6);
myvec.erase(iter, end(myvec));
mystr.erase(std::remove(begin(mystr), end(mystr), '!'), end(mystr)); // in 1 line
```

#### 2.2.1 Remove element in a std::list

Note for a **list** just use _mylist.remove(42)_

Indeed, since it does not offer random-access iterators, using the algorithm std::remove on a list would make list even slower than it already is.

std::list<T,Allocator>::remove Complexity is Linear in the size of the container 

#### 2.2.2 Remove complexity
std::remove & std::remove_if
EComplexity equal exactly std::distance(first, last) applications of the predicate. 

std::vector<T,Allocator>::erase
Complexity is Linear: the number of calls to the destructor of T is the same as the number of elements erased, the assignment operator of T is called the number of times equal to the number of elements in the vector after the erased elements 

### 2.3 Remove element with a predicate

Untill C++17 we still use the erase remove Idiom for a predicate as well using _std::remove_if_.

```cpp
auto negativeNumber = [](auto const & elem) { return elem < 0;};
auto it = std::remove_if(myvec.begin(), myvec.end(), negativeNumber);
myvec.erase(it, myvec.end());
```

### 2.4 remove adjacent duplicates using unique

_std::unique_ only removes adjacent duplicates, and not duplicates in the collection as a whole. 
It has a linear complexity.

```cpp
vector<int> myvec{1, 2, 3, 4, 5, 6, 7, 8, 8, 9, 9, -1, -2};
myvec.erase(std::unique(myvec.begin(),myvec.end()),myvec.end());
```

### 2.5 The C++20 way to remove element

Convenient erase and erase_if finally available in C++20. Yeah ! [6]

```cpp
vector<int> myvec{1, 2, 3, 4, 5, 6, 7, 8, 8, 9, 9, -1, -2};
experimental::erase(myvec, 8);
cout << "erase value 8 in C++ 20 \n";

auto divisibleBy3 = [](auto const& elem) { return (elem % 3) == 0; };
experimental::erase_if(myvec, divisibleBy3);
cout << "erase value divisible by 3 in  C++ 20 \n";
```

### 2.5.1 Complexity

Exactly N applications of the corresponding predicate and any projection, where N = (last - first), and N-1 move operations at worst. 

## 3. Remove elements in associative containers

Associative containers can be maps & sets and their different types likee unordered and multi

* How to remove the elements at a given position (or between two given positions),
* How to remove the elements equivalent to a certain value,
* How to remove the elements satisfying a certain predicate,
* [How to remove duplicates in associative containers](https://www.fluentcpp.com/2018/09/25/remove-duplicates-associative-container-cpp/)

### 3.1 Remove elements at a position

```cpp
myvec.erase(position);
```
removes the entry at that position.

And:

```cpp
myvec.erase(first, last);
```
removes all the entries between first (included) and last (not included).

### 3.2 Remove element equivalent to a specific key 

Piece of cake !

```cpp
myvec.erase(myKey);
```

To remove an elemement of a certain value then we have to use predicates. 
It is a bit more complicated.

### 

### 3.2 Remove element that satisfy a predicate
In a vector we can simply use std::remove_if. It is not possible with associative containers.
Associative containers have stronger constraints[4]: 
* must find keys pretty fast (in O(log(n)) for non-hash and O(1) for hash).

So it is not possible to just shuffle the elements like std::remove_if does,
otherwise we would break the internal (Tree or table).

We need to use the interface and must be carefull to not invalidate the iterator (using erase) before incrementing it !

Neetd to write code this way: 

```cpp
map<int,int> mymap {{1, 10},{2, 20}};    
auto predEvenKey = [](auto const& elem){ auto const [key, val] = elem; return key % 2 == 0;};
// trick to remove evenKey using loop < C++20
for(auto it = mymap.begin(); it != mymap.end(); /*not increament here*/) {
    if(predEvenKey(*it)) {
        it = mymap.erase(it); // erase returns the iter. following the removed elements
    }
    else {
        ++it;
    }
}
```
### 3.3 Remove element that satisfy a predicate with C++20

From C++20  **_erase_if_** can be used. It is clearer now ! [5]

```cpp
// remove using predicates evenKey with erase_if C++20
auto predEvenKey = [](auto const& elem){ auto const [key, val] = elem; return key % 2 == 0;};
std::experimental::erase_if(mymap, predEvenKey);
```
### 3.4 Remove complexity 

```cpp
1) iterator erase( iterator pos );
    Complexity: Amortized constant
2) void erase( iterator first, iterator last );
    Complexity:  log(mymap.size()) + std::distance(first, last)
3) size_type erase( const key_type& key );
    Complexity: log(mymap.size()) + mymap.count(key)
```

## References
1. https://www.fluentcpp.com/2018/12/11/overview-of-std-map-insertion-emplacement-methods-in-cpp17/
2. https://www.oreilly.com/library/view/effective-modern-c/9781491908419/item42
3. https://www.fluentcpp.com/2018/09/14/how-to-remove-elements-from-a-sequence-container
4. https://www.fluentcpp.com/2018/09/21/remove-elements-associative-container-cpp/
5. https://en.cppreference.com/w/cpp/container/map/erase_if
6. https://en.cppreference.com/w/cpp/container/vector/erase2
7. https://www.fluentcpp.com/2017/03/28/inserting-several-elements-into-an-stl-container/

