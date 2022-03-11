#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>


template<typename T>
void display(T cont){
    std::cout << "{";
    for (const auto& elem : cont) {
        std::cout << elem << ",";
    }
    std::cout << "}\n";
}
struct People {
    std::string name;
    int age;
};

std::ostream& operator<<(std::ostream& out, const People& people)
{
   return out << people.name << ':' << people.age ;
}

bool operator<(const People& lhs, const People& rhs) {
    return lhs.age < rhs.age;
}

bool operator==(const People& lhs, const People& rhs) {
    return lhs.age == rhs.age;
}

int main() {

    auto v1 = std::vector{0,1,2,3,4,5,6,7,8};
    display(v1);

    // 1. Non-Modifying Sequence Operation
    // 1.1 use find_if to find the 1st odd element
    auto firstOdd = std::find_if(v1.cbegin(), v1.cend(), [&v1](const auto &elem) {
        return ((elem % 2) == 1);
    });

    if (firstOdd != v1.cend()) {
        std::cout << "1st Odd element found is: " << *firstOdd << "\n";
    }

    // 1.2 minmax find the minimum element inside the container
    auto [min, max] = std::minmax_element(v1.cbegin(), v1.cend());
    std::cout << "the min is: " << *min << " the max is: " << *max << "\n";

    // 1.3 all of  and any_of
    auto inRange = [&v1](const auto &el){return (el >= 0) && (el <= 10);};
    auto is_in_range = std::all_of(v1.begin(), v1.end(), inRange);
    std::cout << "all elements have values between 0 and 10: " << is_in_range << "\n";

    auto predIsVal7 = [&v1](const auto &el){return el == 7;};
    auto has_val_7 = std::any_of(v1.cbegin(), v1.cend(), predIsVal7); // see  none_of
    std::cout << "contains value of 7: " << std::boolalpha << has_val_7 << "\n";

    // 1.4 check that both ranges are equal return a bool
    auto v2{v1};
    auto is_equal = std::equal(v1.begin() + 1, v1.end(), v2.begin() + 1);
    std::cout << "v1 and v2 are equal: " << std::boolalpha << is_equal << "\n";

    // 1.5 std::mismatch. mismatch return point to the first element in both sequences that did not compare equal to each other.
    // also a version exist with a predicate as 4th argument
    std::cout << "modify value at [3] \n";
    v2[3] += 10;
    display(v2);

    auto [mismatch_v1, mismatch_v2] = std::mismatch(v1.begin(), v1.end(), v2.begin());
    std::cout << "v1 element " << *mismatch_v1 << " mismatch v2 element " << *mismatch_v2 <<"\n";

    // 1.6 std::equal check if 2 sequences are equal and return a boolean. Possible to provide a predicate as last argument
    std::vector<People> group1 {{"Jack", 55},{"Joe", 40},{"John", 45}};
    std::vector<People> group2 {{"Jack", 55},{"Juan", 40},{"John", 45}};
    display(group1);
    display(group2);


    auto identical = std::equal(group1.begin(), group1.end(), group2.begin(), group2.end());
    std::cout << "using the == operator, both group equal : "<< std::boolalpha << identical << "\n";
    identical = std::equal(group1.begin(), group1.end(), group2.begin(), group2.end(), [](const auto &el1, const auto &el2){
        return el1.name == el2.name && el1.age == el2.age;
    });
    std::cout << "using a predicate, both group equal : "<< std::boolalpha << identical << "\n";

    // 1.7 std::search search a sequence and return the 1st occurence as result
    auto seq = std::vector{4,5,6};
    if (auto found_seq = search(v1.begin(), v1.end(), seq.begin(), seq.end()); found_seq != v1.end()) {
        std::cout << "found seq [4,5,6] inside container: " << *found_seq <<"\n";
    }

    std::string testString = "Hello Super World !!!";
    std::string needle = "Super";
    const auto it = search(testString.begin(), testString.end(),
                       std::boyer_moore_searcher(needle.begin(), needle.end()));

    if (it == cend(testString))
        std::cout << "The string " << needle << " not found\n";

}