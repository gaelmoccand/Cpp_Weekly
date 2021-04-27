# Cpp daily 
Objective is to present, share and dicuss Modern Cpp features, best practice and more

## How to iterate over collections with for-range loop, for_each and more

### 1. using iterator 
 Bad Readability. Error Prone

```cpp
    std::vector<std::string> players {"Federer", "Djokovic", "Nadal"}; 
    for (auto iter = begin(players); iter != end(players); ++iter) {
        std::cout << *iter << std::endl;
    }
```

### 2. using range-for loop
Always Prefere a range-for loop to iterate 
* Use  & ref to avoid copying the element 
* Use const if u do not want to modify the element
```cpp
    for (const auto &player : players) {
        std::cout << player << std::endl;
    }
```
### 3. When using several elements of the containers
Can't be a range-for loop in some case
```cpp
    for (size_t idx = 1; idx < players.size(); ++idx) {
      std::cout << players[idx] << " VS " << players[idx-1] << std::endl;
    }
```

### 4. Use for_each to raise level of abstraction
* It does not show what is happening in the loop but call a function instead.
* You could also use a lambda instead of the function but it will then "break" the level of abstraction [2]
```cpp
    for_each(std::cbegin(players), std::cend(players), displayNationality);
```

### 5. range-for loop acessing the index
* First Question: Do I really need the index ? 
* Simple not so clean solution is to declare size_t idx = 0; outside the loop and increment it at each step 

```cpp
    size_t idx = 0;
    for (const auto &player : players) {
        std::cout << idx << " : " <<player << std::endl;
        ++idx;
    }
```
* A better approach would be to create getIndex fct[2]
```cpp
   template<typename Collection>
   auto getIndex(Collection const &collection, size_t offset = 0) {
       return [&collection, offset](auto const& iter) {
           return offset + std::distance(begin(collection), iter);
       };
   } 
```

```cpp
    std::set<std::string> competitions  {"Roland Garros", "Wimbledon", "US open"};
    const auto index = getIndex(competitions, 1);
    for (auto competition = begin(competitions); competition != cend(competitions); ++competition) {
        std::cout << index(competition) << " : " << *competition << std::endl;
    }
```
* alternatively use C++20 range [4]


### 6. Find and Insert using emplace
* Consider Emplacement instead of insertion. This is sometime more efficient because do not create temporary object. Item 42 from effective moderne C++ 14/17 Book [2]
* Prefer const_iterators ie: cbegin and cend. Item 13 from effective moderne C++ 14/17  [2]

```cpp
    template<typename C, typename V>
    void findAndEmplace(C& container, const V& targetVal, const V& insertVal) {
        auto iter = std::find(std::cbegin(container), std::cend(container), targetVal); 
        container.emplace(iter, insertVal);
    }
    findAndEmplace(players, std::string{"Federer"}, std::string{"Wawrinka"});
```


## Reference
1. https://www.fluentcpp.com/2018/10/26/how-to-access-the-index-of-the-current-element-in-a-modern-for-loop/
2. https://www.oreilly.com/library/view/effective-modern-c/9781491908419/
3. https://www.fluentcpp.com/2018/03/30/is-stdfor_each-obsolete/
4. https://en.cppreference.com/w/cpp/ranges
