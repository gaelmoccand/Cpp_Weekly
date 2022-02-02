## use std::algorithm in your code

The aim of this page is to learn STL algorithms efficiently. As explained in this post, knowing STL algorithms provide you with a fantastic set of tools to express your intentions in code, making it clearer and more robust. Knowing your STL algorithms also gives you an idea of what types of abstractions are useful, which gives you a model to write your own ones. So we should strive to know our algorithms.

However there are many of them, more than 100 without counting overloads. This is a lot to learn[2].

Here is a cheat sheet of some of the most usefull [1].

![std::algo](https://hackingcpp.com/cpp/std/algorithms.png)
From https://hackingcpp.com/

## Non-Modyfing Sequence Operations
* std::find_if
```cpp
    auto v1 = std::vector{0,1,2,3,4,5,6,7,8};
    auto firstOdd = std::find_if(v1.cbegin(), v1.cend(), [&v1](const auto &elem) {
        return ((elem % 2) == 1);
    });
```

* std::minmax_element and min_element
```cpp
    auto [min, max] = std::minmax_element(v1.cbegin(), v1.cend());
    std::cout << "the min is: " << *min << " the max is: " << *max << "\n";
```
* std::all_of and std::any_of
```cpp
    auto inRange = [&v1](const auto &el){return (el >= 0) || (el <= 10);};
    auto is_in_range = std::all_of(v1.begin(), v1.end(), inRange);
```
* std::mismatch and std::equal
```cpp
    auto [mismatch_v1, mismatch_v2] = std::mismatch(v1.begin(), v1.end(), v2.begin());
```
* std::search -> 1st occurence
```cpp
    auto seq = std::vector{4,5,6};
    if (auto found_seq = search(v1.begin(), v1.end(), seq.begin(), seq.end()); found_seq != v1.end()) {
        ...
    }
```
#### code 
non_modif.cpp

## Reordering elements

```cpp

```

#### code 
reorder.cpp
## Changing values

```cpp

```

## Algo for sets

## Numeric Algo
using transform




1. https://hackingcpp.com/cpp/cheat_sheets.html
2. http://www.fluentcpp.com/STL/
3. https://www.fluentcpp.com/2017/01/09/know-your-algorithms-algos-on-sets/
4. https://www.fluentcpp.com/2017/10/10/partitioning-with-the-stl/
5. https://www.fluentcpp.com/2017/02/13/transform-central-algorithm/



