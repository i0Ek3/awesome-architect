#include <iostream> // 该头文件中定义了四个标准流对象：cin cout cerr clog
//
// cin 对应于标准输入流，用于从键盘读取数据，也可以被重定向为从文件中读取数据。
// cout 对应于标准输出流，用于向屏幕输出数据，也可以被重定向为向文件写入数据。
// cerr 对应于标准错误输出流，用于向屏幕输出出错信息，不能被重定向。
// clog 对应于标准错误输出流，用于向屏幕输出出错信息，不能被重定向。
//
// cerr 和 clog的区别在于：cerr 不使用缓冲区，直接向显示器输出信息；而输出到 clog 中的信息会先被存放到缓冲区，缓冲区满或者刷新时才输出到屏幕。
//
//
//              ios  抽象基类
//               /\
//       istream   ostream
//          /\        /\
//  ifstream  iostream  ofstream
//                |
//             fstream
//
// 为了避免多继承的二义性，从 ios 派生出 istream 和 ostream 时，均使用了 virtual 关键字（虚继承）。
// istream 是用于输入的流类，cin 就是该类的对象。
// ostream 是用于输出的流类，cout 就是该类的对象。
// ifstream 是用于从文件读取数据的类。
// ofstream 是用于向文件写入数据的类。
// iostream 是既能用于输入，又能用于输出的类。
// fstream 是既能从文件读取数据，又能向文件写入数据的类。
//

#include <string>
#include <iomanip>  // 该头文件中定义了各种流操作算子
using namespace std;


int main()
{
    string s1, s2;
    cin >> setw(4) >> s1 >> setw(3) >> s2; // setw 指定输出宽度为 w 个字符，或输人字符串时读入 w 个字符
    cout << s1 << " " << s2 << endl;

    string str = "https://www.google.com";
    for (int i = 0; i < str.size()-1; ++i) {
        cout.put(str[i]); // put() 用来输出单个字符，putchar() 也可以起到相同的作用
    }
    cout.put('\n');

    int c;
    while ((c = cin.get()) != EOF) { 
        // get() 用来从输入流中读取一个字符
        // getline() 用来从输入流中读取一行字符，有两个重载版本
        cout.put(c);
    }
    
    // ignore(int n, int delim) 函数的作用是跳过输入流中的 n 个字符，或跳过 delim 及其之前的所有字符，哪个条件先满足就按哪个执行。
    cin.ignore(); // 等价于 cin.ignore(1, EOF)，即跳过一个字符
    cin.ignore(5, 'a');

    // peek() 函数用于返回输入流中的下一个字符，但是并不将该字符从输入流中取走——相当于只是看了一眼下一个字符
    cin.peek(); // 查看输入流中的第一个字符
    
    // putback(char c) 用于将一个字符插入到输入流的最前面
    char ch;
    cin.putback(ch);

    // 输入输出错误处理
    int i = 0;
    cin >> i;
    if (!cin) {
        if (cin.bad()) { cout << "cin is bad"; }
        if (cin.eof()) { // todo }
        if (cin.fail()) { cin.clear(); }
        if (cin.good()) { cout << "cin is good"; }
    }

    // 刷新输出缓冲区
    cout << "test" << endl; // 输出test和一个换行，然后刷新缓冲区
    cout << "test" << flush; // 输出test，然后刷新缓冲区，不附加任何额外字符
    cout << "test" << ends; // 输出test和一个空字符，然后刷新缓冲区
    cout << unitbuf; // 所有输出操作后都会立即刷新缓冲区
    cout << nounitbuf; // 回到正常的缓冲方式

    // tie() 函数可以用来关联输出流，即绑定输出流到输入流
    cin.tie(&cout); // 标准库中已经将二者关联起来，这里只是为了展示

    return 0;
}

