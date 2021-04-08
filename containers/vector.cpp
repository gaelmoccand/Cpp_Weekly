#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <experimental/vector>
#include <functional>


template <typename T>
void print(std::vector<T> & vecnum)
{
    std::cout << "vec: { ";
    for (const auto & val : vecnum) {
        std::cout << val << ", ";
    }
    std::cout << "} \n";
}

int main() {

    using namespace std;

    vector<int> v0 (10); // 10 zero initialized elements 
    vector<int> v1 = {1, 10, 20, 5, 40, 50}; 

    // 1. using [] to acess and modify element

    cout << v0[0] << "\n";

    v1[0] = 11;
    v1[1] += 2; // modify directly
    v1[2]++;
    //v1[6] = 0; // boom no out of range check !! Undefined behavior

    print(v1);

    // 2. using at() is safer. Throw out of range exception

    try {
        v1.at(0) = 10;
        v1.at(7) = 10;
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    
    // 3. using insert to add element but does not overwrite
    // This is an expensive operation if not insert at the end()
    // it cause container to relocate all the elements that were after position to their new positions.

    auto it = v1.insert(v1.end(), 3);
    print(v1);

    it = v1.insert(v1.end(), 4, 1); // fill up with 1 values 4 times
    print(v1);


    // 4. using emplace_back or push_back to insert

    v1.emplace_back(60); // more performant construct the object  directly (in place) in container
    v1.push_back(70); // safer according to abseil because more readable
    print(v1);

    cout << v1.front() << "\n";
    cout << v1.back() << "\n";
    v1.pop_back();

    // 5. some different ways to copy vectors

    //auto v2{v1};
    std::vector<int> v2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 , 12};
    copy(v1.begin(), v1.end(), begin(v2)); // do not need back_inserter but v2 has to be big enough
    print(v2);

    std::vector<int> appendElements = {13, 14 ,15};
    v2.insert(v2.end(), appendElements.begin(), appendElements.end());
    print(v2);

    std::vector<int> replaceElements = {0, 0 ,0 ,0};
    v2.assign(replaceElements.begin(), replaceElements.end());
    print(v2);
    
    auto pred = [](auto const &elem){ return elem % 2 == 0;};
    vector<int> v4;
    copy_if(v1.begin(), v1.end(), back_inserter(v4), pred);
    print(v4);
    
    auto v5{v1};
    std::experimental::erase_if(v5, std::not_fn(pred)); // copy then remove elements
    print(v5);

    // 6.  some more algorithms
    auto iter = min_element(v1.begin(),v1.end()); // O(n) N comparisons
    cout << "min element is: " << *iter << "\n";
    reverse(v1.begin(), v1.end()); // O(last - first)/2 swaps.
    print(v1);
    sort(v1.begin(), v1.end()); // O(nlogn)
    print(v1);

    // 7. vector of vector
    vector<vector<int>> v3 = {{1, 2, 3}, {4 ,5 ,6}};
    cout << v3[1][1]; // row colum access 2d vec

    v3.emplace_back(3);
    cout << v3.at(0).at(1);

}


