#include <iostream>
#include <cctype>
using namespace std;

int main()
{
    string str = "12abc aUUdAK2!";
    cout << endl;
    cout << "Original str is: " << "\"" << str << "\"" << endl;
    cout << "Ok, after we use cctype, the str divided into follow cases: " << endl;
    cout << endl;
    for (int i = 0; i < str.size(); ++i) {
        if (isalpha(str[i])) cout << "str[" << i << "] = " << str[i] << " is a alpha." << endl;
        if (isblank(str[i])) cout << "str[" << i << "] = " << str[i] << " is a blank." << endl;
        if (isdigit(str[i])) cout << "str[" << i << "] = " << str[i] << " is a digit." << endl;
        if (ispunct(str[i])) cout << "str[" << i << "] = " << str[i] << " is a punct." << endl;
        if (isspace(str[i])) cout << "str[" << i << "] = " << str[i] << " is a space." << endl;
        if (isupper(str[i])) cout << "str[" << i << "] = " << str[i] << " is a upper." << endl;
        if (islower(str[i])) cout << "str[" << i << "] = " << str[i] << " is a lower." << endl;
    }
    return 0;
}

