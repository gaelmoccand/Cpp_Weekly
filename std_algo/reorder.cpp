#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

template<typename T>
void display(T cont) {
    for_each(cont.cbegin(), cont.cend(), [&cont](const auto &elem) {
        const auto&[name, age] = elem;
        std::cout << "[ " << name << ":" << age << " ]" << "\n";
    });
}


template<typename K>
auto slide(K f, K l ,K p) {
    if (p < f) return std::make_pair(p, std::rotate(p,f,l));
    if (l > p) return std::make_pair(std::rotate(f,l,p), p);
    return std::make_pair(f,l);
}

struct People {
    std::string name;
    int age;
};

bool operator<(const People& lhs, const People& rhs) {
    return lhs.age < rhs.age;
}

int main() {

    std::vector<People> record = {{"Cathy", 59},{"Nelly", 96},{"Elon", 53},{"Leon", 13},{"Rene", 96},{"Anna", 43}};
    display(record);

    // 2 Reordering Element
    // 2.1 stable_partition: gather all element older than 65 and move them to the front
    auto over65 = [record](auto& elem){const auto&[name, age] = elem; return age > 65;};
    std::stable_partition(record.begin(), record.end(), over65);
    std::cout << "take over 65 and place in the front\n";
    display(record);
    
    // 2.2 rotate: take 1 block and move it 
    auto block_start = record.begin();
    auto block_end =record.begin()+2;
    auto move_to = record.end();
    rotate(block_start, block_end, move_to);
    std::cout << "move them back to the end: \n";
    display(record);

    // 2.3 nth_element is a partial sorting where nth is changed to whatever element would be in this pos if sorted
    // all element before nth are < nth; element after are > nth
    //std::vector<int> vec{5, 10, 6, 4, 3, 2, 6, 7, 9, 3};
    auto middle = record.size()/2;
    std::nth_element(record.begin(), record.begin()+middle, record.end());
    std::cout << "\n median is " << record[middle].age << "\n";


    // 2.4 stable_sort the order of equivalent element order is preserved
    // works also with compare function object
    std::stable_sort(record.begin(), record.end());
    std::cout << "sort but equivalent element order is preserved" << record[middle].age << "\n";
    display(record);

}