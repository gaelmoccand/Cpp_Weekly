#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

int main() {

    using namespace std;

    string s1{"Hello"}; 
    string s2{"13445notnumber"};
    const string s3{"123:456"};

    s1[0] = 'h';

    cout << s1 << endl;

    s1.insert(3,4,'-');
    cout << s1  << endl;
    s1.replace(3,4, "!!!!");
    cout << s1  << endl;
    s1.replace(3,4,4, '$');
    cout << s1  << endl;
    size_t afternum;
    auto numb = std::stoi(s2, &afternum, 10);
    cout << numb ;
    cout << afternum;
    
    if(isalnum(s2[0]))
    {
     //   cout <<"is al nium \n";
    }
    
    auto pos = s3.find(':');
    if(pos != string::npos)
    {
    //   cout << "found :";
    }
    
    std::istringstream strtosplit(s3);
    string substring;
    char delimiter = ':';
    while(getline(strtosplit, substring, delimiter))
    {
        cout << substring;
    }

    cout << to_string(42) << endl;

    return 0;
}


