#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
using namespace std;

// 正向迭代器       container_name::iterator iterator_name
// 常量正向迭代器   container_name::const_iterator iterator_name
// 反向迭代器       container_name::reverse_iterator iterator_name
// 常量反向迭代器   container_name::const_reverse_iterator iterator_name

//
// 容器             迭代器功能
// vector           随机访问
// deque            随机访问
// list             双向
// set / multiset   双向
// map / multimap   双向
// stack            不支持迭代器
// queue            不支持迭代器
// priority_queue   不支持迭代器
//
//

int main()
{
    vector<int> vi;
    for (int i = 0; i < 10; ++i) {
        vi.push_back(i);
    }

    vector<int>::iterator it;
    for (it = vi.begin(); it != vi.end(); ++it) {
        vi.pop_back();
    }

    for (vector<int>::reverse_iterator j = vi.rbegin(); j != vi.rend(); ++j) {
        cout << "j = " << *j << " " << endl;
    }

    return 0;
}

