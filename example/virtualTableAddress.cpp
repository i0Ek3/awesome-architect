#include <iostream>
using namespace std;

class Base {
public:
    virtual void f() { cout <<  "Base::f()" << endl; }
    virtual void g() { cout <<  "Base::g()" << endl; }
    virtual void h() { cout <<  "Base::h()" << endl; }
};

typedef void(*Fun)(void);

int main()
{
    Base b;
    Fun pFun = NULL;

    cout << "The address of virtual table is: " << (int*)(&b) << endl;
    cout << "The first function address of virtual table is: " << (int*)*(int*)(&b) << endl;

    pFun = (Fun)*((int*)*(int*)(&b));
    pFun();
}
