#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>

template<typename T>
void display(T cont) {
    for_each(cont.cbegin(), cont.cend(), [&cont](const auto &elem) {
        const auto&[name, age] = elem; 
        std::cout << "[ " << name << ":" << age << " ]" << "\n";
    });
}


template<typename FwdIt>
auto slide(FwdIt f, FwdIt l ,FwdIt p) {
    if (p < f) return std::make_pair(p, std::rotate(p, f, l)); // down -> up 
    if (l < p) return std::make_pair(std::rotate(f, l, p), p); // up -> down
    return std::make_pair(f, l);
}

template<typename FwdIt, typename Compare = std::less<>>
void quickSort(FwdIt first, FwdIt last, Compare cmp = Compare{}) {
    auto const N = std::distance(first, last);
    if (N <= 1) return; 
    auto const pivot = std::next(first, N / 2);
    std::nth_element(first, pivot, last, cmp);
    quickSort(first, pivot, cmp); 
    quickSort(pivot, last, cmp); 
}


struct People {
    std::string name;
    int age;
};

bool operator<(const People& lhs, const People& rhs) {
    return lhs.age < rhs.age;
}

int main() {

    std::vector<People> record = {{"Cathy", 59},{"Rene", 99},{"Elon", 53},{"Leon", 13},{"Arthur", 99},{"Anna", 43}};
    display(record);

    // 2 Reordering Element
    // 2.1 stable_partition: gather all element older than 65 and move them to the front
    auto over65 = [record](auto& elem){const auto&[name, age] = elem; return age > 65;};
    std::stable_partition(record.begin(), record.end(), over65);
    std::cout << "\n stable_partition(start,end,over65): take over 65 and place in the front\n";
    display(record);
    
    // 2.2 rotate: take 1 block and move it 
    auto block_start = record.begin();
    auto block_end =record.begin()+2;
    auto move_to = record.end();
    rotate(block_start, block_end, move_to);
    std::cout << "\n rotate(start,start+2,move_to_end): move first block elem. back to the end: \n";
    display(record);

    // 2.3 nth_element is a partial sorting where nth is changed to whatever element would be in this pos if sorted
    // all element before nth are < nth; element after are > nth
    //std::vector<int> vec{5, 10, 6, 4, 3, 2, 6, 7, 9, 3};
    auto middle = record.size()/2;
    std::nth_element(record.begin(), record.begin()+middle, record.end());
    std::cout << "\n nth_element(start,middle,end): partial sorting, all element before nth are < nth; element after are > nth \n";
    std::cout << "median is " << record[middle].age << "\n";
    display(record);


    // 2.4 stable_sort the order of equivalent element order is preserved
    // works also with compare function object
    std::stable_sort(record.begin(), record.end());
    std::cout << "\n stable_sort(start,end): sort but equivalent while preserving order between equal elements\n";
    display(record);


    // 2.5 Quick_sort using nth_element
    quickSort(record.begin(), record.end(), [](const auto& lhs, const auto& rhs){return lhs.name < rhs.name;});
    std::cout << "\n quick sort using name \n";
    display(record);

    // 2.6 slide 
    slide(record.begin()+3, record.begin()+5, record.begin());
    std::cout << "\n slide to the begin [][][][*][*][] -> [*][*][][][][]  \n";
    display(record);

    slide(record.begin()+3, record.begin()+5, record.end());
    std::cout << "\n slide to the end [][][][*][*][] -> [][][][][*][*] \n";
    display(record);

}