#include <iostream>
using namespace std;

// 强制类型转换
// static_cast dynamic_cast reinterpret_cast const_cast
// static_cast 用于进行比较“自然”和低风险的转换，如整型和浮点型、字符型之间的互相转换。
// reinterpret_cast 用于进行各种不同类型的指针之间、不同类型的引用之间以及指针和能容纳指针的整数类型之间的转换。转换时，执行的是逐个比特复制的操作。
// const_cast 运算符仅用于进行去除 const 属性的转换。
// dynamic_cast专门用于将多态基类的指针或引用强制转换为派生类的指针或引用，而且能够检查转换的安全性。

int main()
{
    int n;
    n = static_cast<int>(3.14); // n = 3
    n = static_cast<double>(12); // n = 12.00

    const string str = "test";
    string &p = const_cast<string&>(str);
    string *q = const_cast<string*>(&str); // &s 的类型为const string*

    return 0;
}

