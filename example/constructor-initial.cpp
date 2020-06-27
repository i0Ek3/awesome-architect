#include <iostream>
using namespace std;

class Foo {
    int value = 0;

public:
    Foo(int val) : value(val) {}
};

class Bar {
    Foo foo;
};

class Test {
    int age;
    string name;
    int month;

public:
    Test(const string &n, int a) : name(n), age(a), month(a) {}
    void print() {
        cout << "name = " << name << " age = " << age << " month = " << month << endl;
    }
};

int main()
{
    Test test("linux", 12);
    test.print();
    return 0;
}

