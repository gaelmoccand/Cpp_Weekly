#include <map>
#include <algorithm>
#include <iostream>
#include <experimental/map>

template <typename C>
void disp(const C& cont) {

    for (const auto& [key, val] : cont){
        std::cout << key << " : ";
        std::cout << val << "\n";
    }
    std::cout << "\n";
}

int main() {

    using namespace std;

    map<int,int> mymap {{1, 10}, {2, 20}, {3, 30}, {4, 40}, {5, 50}, {6, 60}};
    cout << "mymap" << "\n";
    disp(mymap);

    // 1. remove element using a key is a piece ok cake !
    mymap.erase(5);
    cout << "remove key 5 in mymap \n";
    disp(mymap);

    // 2.remove element using iterator
    mymap.erase(mymap.begin());
    cout << "remove 1st element in my map \n";
    disp(mymap);
    cout << "remove last elements using range \n";
    auto start = ++mymap.begin();
    mymap.erase(++start, mymap.end());
    disp(mymap);

    // 3. trick to remove evenKey using loop < C++20
    auto predEvenKey = [](auto const& elem){ auto const [key, val] = elem; return key % 2 == 0;};

    cout << "remove using predicate < C++20\n";
    for(auto it = mymap.begin(); it != mymap.end(); /*not increament here*/) {
        if(predEvenKey(*it)) {
            it = mymap.erase(it); // erase returns the iter. following the removed elements
        }
        else {
            ++it;
        }

    }
    disp(mymap);

    // 4. remove evenKey using erase_if C++20
    cout << "erase_if(mymap,evenKey) in C++20 \n";
    std::experimental::erase_if(mymap, predEvenKey);
    disp(mymap);
    
}
