#include <iostream>
using namespace std;

// ref: https://www.cnblogs.com/gtarcoder/p/4803293.html
// 匿名函数，即一个没有名字的函数
// 用来解决只调用一次的函数的效率问题
// 定义如下：
// [外部变量访问方式说明符] (参数列表) -> return_type { //code; }
// [=](int x, int y) -> int { return a + b; }
// [&](int x, int y) -> int { return a + b; }
// =表示不允许，&表示允许。
// “外部变量访问方式说明符”还可以有更加复杂和灵活的用法。例如：
// [=, &x, &y]表示外部变量 x、y 的值可以被修改，其余外部变量不能被修改；
// [&, x, y]表示除 x、y 以外的外部变量，值都可以被修改。

int main()
{
    int a;
    auto f = [a](int a) -> int { return a*2; };
    cout << f(2) << endl;
    return 0;
}

