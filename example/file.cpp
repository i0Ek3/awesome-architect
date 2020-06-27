#include <iostream>
#include <fstream> // 文件操作头文件

// 建议总是以二进制的方式打开文件
// ios::in | ios::binary表示用二进制模式，以读取的方式打开文件。
// ios::out | ios::binary表示用二进制模式，以写入的方式打开文件。

int main()
{
    ifstream in; // 输入文件流
    in.open("./file.cpp", ios::in);
    if (in) in.close();
    else cout << "file exist" << endl;
    in.gcount(); // 可用来查看读了多少字节

    ofstream out; // 输出文件流
    out.open("./aoti.c", ios::out);
    if (!out) cout << "error" << endl;
    else out.close();

    out.open("./tmp.txt", ios::out | ios::in);
    if (out) out.close();
    else cout << "error" << endl;

    fstream io; // 输入输出文件流
    io.open("./test.txt". ios::out | ios::in | ios::trunc);
    if (!io) cout << "error" << endl;
    else io.close();

    // 用 ostream::write() 成员函数写文件
    // 用 istream::read() 成员函数读文件
    
    // 移动和获取文件的读写指针，有四个成员函数
    // seekg seekp tellg tellp

    return 0;
}

