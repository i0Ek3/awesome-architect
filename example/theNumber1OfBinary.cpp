#include <iostream>
using namespace std;

int main()
{
    int cnt = 0;
    int x = 981;
    while (x) {
        cnt++;
        x = x & (x-1);
    }
    cout << cnt << endl;
    return 0;
}

