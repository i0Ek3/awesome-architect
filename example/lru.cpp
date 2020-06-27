#include <list>
#include <map>
#include <iterator>
using namespace std;

Class LRUCache {
    int cap;
    list<pair<int, int>> lp;
    unordered_map<int, list<pair<int, int>>::iterator> m;

public:

    LRUCache(int capacity) {
        cap = capacity;
    }

    int get(int key) {
        auto it = m.find(key);
        if (it == m.end()) return -1;
        lp.splice(lp.beign(), lp, it->second);
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = m.find(key);
        if (it != m.end()) lp.erase(it->second);
        lp.push_front(make_pair(key, value));
        m[key] = lp.begin();
        if (m.size() > cap) {
            int tmp = lp.rbegin()->first;
            lp.pop_back();
            m.erase(tmp);
        }
    }
};
