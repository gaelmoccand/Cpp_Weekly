#include <vector>
#include <string>
#include <iostream>
#include <algorithm>


int main() {
    using namespace std;


    vector<int> myvec{1,2,3,4,5,6,7};
    string mystr{"Hi Folks !"};

    cout << myvec[1] << "\n";
    cout << mystr << "\n";

    // 1. remove element at specific position
    myvec.erase(myvec.begin() + 1);
    mystr.erase(mystr.begin() + 1);
    cout << myvec[1] << "\n";
    cout << mystr << "\n";
    // for a subrange
    myvec.erase(myvec.begin(), myvec.begin()+2); // erase the first 2 el.
    mystr.erase(0, 2);
    cout << myvec[1] << "\n";
    cout << mystr << "\n";    

    // 2. remove element that match a specific value
    // using erase remove C++ idiom
    myvec.erase(std::remove(begin(myvec), end(myvec), 6), end(myvec));
    for(const auto& el : myvec )
    {
        cout << el;
    }
    cout << "\n";

    mystr.erase(std::remove(begin(mystr), end(mystr), '!'), end(mystr));
    cout << mystr << "\n";






}



