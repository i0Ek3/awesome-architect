#include <iostream>
using namespace std;

template <typename T>
class Operation {
public:
    Operation(T a, T b) : x(a), y(b) {}
    T add (); 
    // 类内定义
    T sub () { return x - y; }
    T mul () { return x * y; }
    T div () { return x / y; }
private:
    T x;
    T y;
};

// 类外定义
template<class T>
T Operation<T>::add() {
    return x + y;
}

int main()
{
    Operation<int> o1(2, 3);
    cout << o1.add() << endl << o1.sub() << endl <<  o1.mul() << endl <<  o1.div() << endl;
    Operation<float> o2(1.1, 3.3);
    cout << o2.add() << endl << o2.sub() << endl << o2.mul() << endl <<  o2.div() << endl;
    return 0;
}

