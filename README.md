# Cpp daily 
Objective is to present, share and dicuss Modern Cpp features, best practice and more

## How to iterate over containers for-range loop, for_each

### 1 using iterator 


 Bad Readability. Error Prone

```cpp
    std::vector<std::string> players {"Federer", "Djokovic", "Nadal"}; 
    for (auto iter = begin(players); iter != end(players); ++iter) {
        std::cout << *iter << std::endl;
    }
```

### 2 using range-for loop
Always Prefere a range-for loop to iterate 
* Use  & ref to avoid copying the element 
* Use const if u do not want to modify the element
```cpp
    for (const auto &player : players) {
        std::cout << player << std::endl;
    }
```




## Reference
1. https://www.fluentcpp.com/2018/10/26/how-to-access-the-index-of-the-current-element-in-a-modern-for-loop/
2. https://www.oreilly.com/library/view/effective-modern-c/9781491908419/
[3]
