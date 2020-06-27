#include <iostream>

// i++ 的实现
int operator++(int) {
    int tmp = *this;
    ++*this;
    return tmp;
}

// ++i 的实现
int& operator++() {
    *this += 1;
    return *this;
}

int main()
{
    return 0;
}

