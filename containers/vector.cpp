#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

template <typename T>
void print(std::vector<T> & vecnum)
{
    for (const auto & val : vecnum) {
        std::cout << val;
    }
}

int main() {

    using namespace std;

    vector<int> v1 = {10,20,5,40,50}; 
    vector<int> v2;

    v1[0] = 11;
    v1[1] += 1;

    v1.emplace_back(60);
    v1.push_back(70);

    cout << v1.front() << endl;
    cout << v1.back() << endl;
    v1.pop_back();

    v1.erase(v1.begin()+1);
    v1.erase(remove(v1.begin(), v1.end(), 30), v1.end());
    //v1.erase(remove_if(v1.begin(), v1.end(), [](int val){ return val >= 50;}), v1.end());

    auto iter = min_element(v1.begin(),v1.end());
    cout << *iter << endl;

    copy(v1.begin(), v1.end(), back_inserter(v2));
    reverse(v2.begin(), v2.end());
    sort(v2.begin(), v2.end());

    print(v2);

    return 1;
}


