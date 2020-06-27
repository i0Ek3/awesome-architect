#include <iostream>
#include <string>
#include <memory>
using namespace std;

int main()
{
    string str("thisisatest");
    char ch[] = "1234566787";
    string a;
    string b(str);
    string c(str, 2, 5);
    string d(ch, 6);
    string e(4, 'x');
    string f(str.begin(), str.end());
    
    cout << "str = " << str << endl;
    cout << "ch = " << ch << endl;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;
    cout << "d = " << d << endl;
    cout << "e = " << e << endl;
    cout << "f = " << f << endl;

    int size = str.size();
    int len = str.length();
    int maxsize = str.max_size();
    unsigned long cap = 0;
    cap = str.capacity();
    cout << "size = " << size << endl 
         << "len = " << len << endl 
         << "maxsize = " << maxsize << endl 
         << "cap = " << cap << endl;
    
    // 获取字符串
    char cc = str[2]; // []获取字符串，不会检查索引的有效性
    char c1 = str.at(2); // at()函数会检查下标是否有效
    cout << " cc = " << cc << endl << "c1 = " << c1 << endl;

    // 字符串比较
    if (str == b) cout << "str == b" << endl;
    if (str >= c) cout << "str >= c" << endl;
    if (str <= f) cout << "str <= f" << endl;

    // 字符串的修改与替换
    // assign(), operator=, erase(), swap(), insert()
    cout << "str = " << str << endl;
    str.assign(ch);
    cout << "str = " << str << endl;
    str.erase(1, 4);
    cout << "str = " << str << endl;
    str.swap(e);
    cout << "str = " << str << endl;
    str.insert(1, "oooo");
    cout << "str = " << str << endl;
    str.append("------this is append-------");
    cout << "str = " << str << endl;
    str.replace(2, 4, ch);
    cout << "str = " << str << endl;

    // 输入与输出
    string s1, s2;
    cout << "Pleacia inputo:";
    getline(cin, s1);
    cout << "s1 = " << s1 << endl;

    // 字符查找
    string::size_type find = str.find('t', 0);
    cout << "find = " << find << endl;
    string::size_type find2 = str.rfind('t', 0);
    cout << "find2 = " << find2 << endl;
    string::size_type f1 = str.find_first_of('t', 0);
    string::size_type f2 = str.find_last_of('t', 0);
    cout << "f1 = " << f1 << endl << "f2 = " << f2 << endl;

    // string 还支持迭代器和配置器
    string ss("test");
    cout << "ss = " << ss << endl;
    basic_string <char> ss1(ss.get_allocator());
    cout << "ss1 = " << ss1 << endl;
    basic_string <char>::allocator_type ss2 = ss1.get_allocator();
    string::size_type ss3 = ss1.size();
    cout << "ss3 = " << ss3 << endl;

    return 0;
}

