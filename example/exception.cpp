#include <iostream>
#include <stdexcept>
using namespace std;

/*
 * stdexcept contains following exception: 
 *
 * bad_alloc
 * bad_cast
 * bad_typeid
 * bad_exception
 * logic_error
 * runtime_error 
 *
 */

class FooException : public std::runtime_error {
public:
    FooException(void) : std::runtime_error("FooException") {}
};

class Foo {
public:
    Foo (void) { throw new FooException; }
};

int main(int argc, char **argv)
{
    Foo *foo = new Foo;

    double a, b;
    cin >> a >> b;
    try {
        cout << "-------" << endl;
        if (!b) throw -1;
        else cout << a/b;
        cout << "-------" << endl;
    } catch (double c) {
        cout << "catch(double)" << c << endl;
    } catch (int d) {
        cout << "catch(int)" << d << endl;
    }
    cout << "done" << endl;


    return 0;
}

