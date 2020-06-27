# STL 源码学习

> 以下内容均整理自网络，侵删。

STL 为 C++ 中的标准模版库。

## 六大组件

- Memory Allocation
- Iterator
- Container
- Algorithm
- Function Object
- Adaptor

## 部分源码

### string

> 以下代码来自陈硕，保证正确和实现简单，但效率可能会低。

通过写时复制 COW 实现的？自己实现的 string 类中必须要包含构造、析构、拷贝构造、赋值函数。

```C++
#include <utility>
#include <string.h>
 
class String
{
 public:
  String()
    : data_(new char[1])
  {
    *data_ = '\0';
  }
 
  String(const char* str)
    : data_(new char[strlen(str) + 1])
  {
    strcpy(data_, str);
  }
 
  String(const String& rhs)
    : data_(new char[rhs.size() + 1])
  {
    strcpy(data_, rhs.c_str());
  }
  /* Delegate constructor in C++11
  String(const String& rhs)
    : String(rhs.data_)
  {
  }
  */
 
  ~String()
  {
    delete[] data_;
  }
 
  /* Traditional:
  String& operator=(const String& rhs)
  {
    String tmp(rhs);
    swap(tmp);
    return *this;
  }
  */
  String& operator=(String rhs) // yes, pass-by-value
  {
    swap(rhs);
    return *this;
  }
 
  // C++ 11
  String(String&& rhs)
    : data_(rhs.data_)
  {
    rhs.data_ = nullptr;
  }
 
  String& operator=(String&& rhs)
  {
    swap(rhs);
    return *this;
  }
 
  // Accessors
 
  size_t size() const
  {
    return strlen(data_);
  }
 
  const char* c_str() const
  {
    return data_;
  }
 
  void swap(String& rhs)
  {
    std::swap(data_, rhs.data_);
  }
 
 private:
  char* data_;
};
```

### map

和 set 类似，也采用了 rb_tree 作为底层容器，但每个节点是一个 pair 对象，它关联了键值和实值，键值不允许修改，但实值是可以修改的。

```C++
template <class Key, class T, class Compare = less<Key>, class Alloc = alloc>     // 默认采用递增顺序
class map {
public:
  typedef Key key_type; // 键值类型
  typedef T data_type;  // 实值类型
  typedef T mapped_type;
  typedef pair<const Key, T> value_type;  // 注意const Key，表示无法修改键值
  typedef Compare key_compare;
  ....
 
private:
  // select1st抽取出value_type（pair）内的first元素
  typedef rb_tree<key_type, value_type,      // rb_tree的一个节点存储一个pair
                  select1st<value_type>, key_compare, Alloc> rep_type;
  rep_type t;  // 底层容器——红黑树
public:
  ....
  typedef typename rep_type::iterator iterator;         // 实值可以修改
  ....
 
  map() : t(Compare()) {}
  explicit map(const Compare& comp) : t(comp) {}
 
  ....
 
  key_compare key_comp() const { return t.key_comp(); }
  value_compare value_comp() const { return value_compare(t.key_comp()); }
  iterator begin() { return t.begin(); }
  const_iterator begin() const { return t.begin(); }
  iterator end() { return t.end(); }
  ....
 
  // 下标操作，存在相同键值的节点则返回，否则插入再返回
  // insert返回pair<iterator, bool>，iterator指向红黑树的某个节点
  // 下标操作返回迭代器的第二个元素，即实值
  T& operator[](const key_type& k) {
    return (*((insert(value_type(k, T()))).first)).second;
  }
 
  // 插入/删除
  pair<iterator,bool> insert(const value_type& x) { return t.insert_unique(x); }
  iterator insert(iterator position, const value_type& x) {
    return t.insert_unique(position, x);
  }
  ....

  // map相关操作
  iterator find(const key_type& x) { return t.find(x); }
  size_type count(const key_type& x) const { return t.count(x); }
  iterator upper_bound(const key_type& x) {return t.upper_bound(x); }
  pair<iterator,iterator> equal_range(const key_type& x) {
    return t.equal_range(x);
  }
 
  friend bool operator== __STL_NULL_TMPL_ARGS (const map&, const map&);
  friend bool operator< __STL_NULL_TMPL_ARGS (const map&, const map&);
};
```

### set

set 中的 key 就是 value，value 就是 key，并且 key 是唯一的，利用红黑树有序地存储。

```C++
template <class Key, class Compare = less<Key>, class Alloc = alloc>  // 默认采用递增排序
class set {
public:
  // typedefs:
  // set的key既是key_type也是value_type
  typedef Key key_type;
  typedef Key value_type;
  typedef Compare key_compare;
  typedef Compare value_compare;
private:
  // 这里使用仿函数identity作为rb_tree的KeyOfValue类型实参
  // identity直接将传入的参数返回
  typedef rb_tree<key_type, value_type,      // 注意，红黑树节点实际存储的是value_type，也就是key_type
                  identity<value_type>, key_compare, Alloc> rep_type;
  rep_type t;  // 底层容器——红黑树
public:
  ....
  typedef typename rep_type::const_iterator iterator;           // const_iterator，迭代器无法写入
  typedef typename rep_type::const_iterator const_iterator;
  ....
 
  set() : t(Compare()) {}
  explicit set(const Compare& comp) : t(comp) {}
  ....
 
  // 转调用rb_tree的接口
  key_compare key_comp() const { return t.key_comp(); }
  value_compare value_comp() const { return t.key_comp(); }
  iterator begin() const { return t.begin(); }
  iterator end() const { return t.end(); }
....
 
  // 插入/删除
  typedef  pair<iterator, bool> pair_iterator_bool; 
  pair<iterator,bool> insert(const value_type& x) { 
    pair<typename rep_type::iterator, bool> p = t.insert_unique(x); 
    return pair<iterator, bool>(p.first, p.second);
  }
  iterator insert(iterator position, const value_type& x) {
    typedef typename rep_type::iterator rep_iterator;
    return t.insert_unique((rep_iterator&)position, x);
  }
  ....
 
// set相关操作
  iterator find(const key_type& x) const { return t.find(x); }
  size_type count(const key_type& x) const { return t.count(x); }
  iterator lower_bound(const key_type& x) const {
    return t.lower_bound(x);
  }
  iterator upper_bound(const key_type& x) const {
    return t.upper_bound(x); 
  }
  pair<iterator,iterator> equal_range(const key_type& x) const {
    return t.equal_range(x);
  }

  // set之间是可以比较的
  friend bool operator== __STL_NULL_TMPL_ARGS (const set&, const set&);
  friend bool operator< __STL_NULL_TMPL_ARGS (const set&, const set&);
};
```


### vector

不同于静态数组 array，vector 是一种动态的数组，空间的运用更加灵活。主要体现在数据移动的效率上，因为重新申请内存、拷贝数据和释放 vector 是非常消耗时间的。

```C++
// vector 的结构如下，有三个指针
template <typename T>
class Vector 
{
    ......
    typedef allocator<T> dataAlloc;
    private:
        T* _start;
        T* _finish;
        T* _end_of_storage;
}

// 迭代器，原生的指针可以充当迭代器
typedef T* iterator;
typedef const T *const_iterator

```

- push_back(const T &value)
    - 有空间则直接用 construct 构造元素
    - 没有空间则重新分配新的内存，然后把以前的元素拷贝回来，再构造元素

```C++
if (size() == capacity())
    reallocateAndCopy();
dataAlloc::construct(_finish++, value);
```

- insert(iterator postition, const size_type n, const T &value)
    - 备用空间充足
        - 插入点后现有元素个数 >= 新增元素个数
        - 插入点后现有元素个数 <= 新增元素个数
    - 备用空间不足
        - 将原 vector [ _start, position ) 拷贝至新的 vector
        - 构造 n 个 value
        - 将 [ position, _finish ) 元素也拷贝至新的 vector
        - 析构并释放原 vector

```C++
/* 备用空间足够 */
if (_end_of_storage - _finish >= n) {
    size_type elems_after = _finish - position;
    iterator old_finish = _finish;
    if (elems_after > n) {
        /* 插入点后现有元素个数 >= x新增元素个数 */
        uninitialized_copy(position, old_finish - n, old_finish);
        _finish += n;
    } else {
        /* 插入点后现有元素个数 <= x新增元素个数 */
        uninitialied_fill_n(_finish, n - elems_after, value);
        _finish += n - elems_after;
        uninitialized_copy(position, old_finish, _finish);
        _finish += elems_after;
        fill(position, old_finish, value);
    }
}
/* 备用空间不够 */
else {
    size_type old_size = size();
    size_type len = old_size + max(old_size, n); // 新长度为旧长度的2倍，或者旧长度+新元素个数

    iterator new_start = dataAlloc::allocate(len); 
    iterator new_finish = new_start;

    try{
        /* 将原 vector[_start, position) 拷贝至新的vec */
        new_finish = uninitialized_copy(_start, position, new_start);
        /* 构造 n 个value */
        new_finish = uninitialied_fill_n(new_finish, n, value);
        /* 将 [position, _finish) 元素也拷贝过来 */
        new_finish = uninitialized_copy(position, _finish, new_finish);
    } catch(...) {
        destory(new_start, new_finish);
        dataAlloc::deallocate(new_start, len);
        throw;
    }
    /* 析构并释放原vector */
    destroyAndFree();

    _start = new_start;
    _finish = new_finish;
    _end_of_storage = _start + len;
}

```

- resize
    - n > capacity() 需要重新分配内存空间，复制原 size() 元素，释放原资源，增加 n - size() 个元素
    - size() < n <= capacity 不需要重新分配内存空间，增加 n - capacaity() 个元素
    - n == size() 什么都不做
    - n < size() 需要析构 size() - n 个元素

```C++
void resize(size_type n, const T &value) {
    if (n > capacity()) {
        size_type addElementLength = n - size();
        /* 重新分配内存 */
        T *newStart = dataAlloc::allocate(n);
        T *newFinish = uninitialized_copy(begin(), end(), newStart);
        /* 拷贝元素 */
        newFinish = uninitialied_fill_n(newFinish, addElementLength, value);
        /* 销毁原内存 */
        destroyAndFree();
        /* 更新相关指针 */
        _start = newStart;
        _finish = newFinish;
        _end_of_storage = _start + n;
    } else if (n <= capacity() && n > size()) {
        size_type addElementLength = n - size();
        _finish = uninitialied_fill_n(end(), addElementLength, value);
    } else if (n < size()) {
        dataAlloc::destroy(begin() + n, end());
        _finish = _start + n;
    }
}
```

- reserve
    - n > capacity() 重新分配内存空间，复制原 size() 元素，释放原资源

```C++
void reserve(size_type n) {
    if (n > capacity()) {
        T *new_start = dataAlloc::allocate(n);
        T *new_finish = uninitialized_copy(begin(), end(), new_start);

        destroyAndFree();

        _start = new_start;
        _finish = new_finish;
        _end_of_storage = _start + n;
     }
}
```

- void pop_back()

```C++
void Vector<T>::pop_back() {
    dataAlloc::destroy(--_finish);
}
```

- iterator erase(iterator first, iterator last)

```C++
size_type remove_element_counts = last - first;     // 要删除的个数

if (remove_element_counts > 0) {
    auto it = first;
    for (; last != _finish; last++, it++)
        *it = *last;
    dataAlloc::destroy(it, _finish);
    _finish = _finish - remove_element_counts;
}

return first;
```

- iterator erase(iterator position)

```C++
return erase(position, position + 1);
```


### deque

vector 是单向开口的连续空间，而 deque 是双向开口的连续空间，可以在两头分别进行插入和删除。vector 和 deque 可以简单理解为 stack 和 queue。

另外，deque 可以在常数时间内对头端进行插入和删除操作，且没有容量的概念。

deque 由两部分组成：

- 缓冲区：一段连续的内存空间
- map：指向缓冲区的指针数组

```C++
template <typename T>
class Deque {
    ......
    private:
        T **_map;                      // 动态数组
        iterator _begin;
        iterator _end;
        size_t _mapSize = 1;          // map的数量
        size_t _pageSize = 3;         // 缓冲区的大小
}
```
deque 是分段的连续空间，维持其"整体连续"假象的任务，就落在了迭代器身上。为此迭代器必须能够做到：

- 能够指向缓冲区的元素
- 能够指向 map 槽位，方便能够跳跃至上面或下面的一个缓冲区

```C++
// 迭代器
class DequeIterator {
    private:
        Deque<T> *_containerPtr;      // 保存对容器的连接，重点是能访问到map。
        T *_cur;                      // 指向缓冲区的元素
        size_t _mapIndex;             // map数组的索引，向前跳跃或者向后跳跃
};

// DequeIterator &operator++()
_cur = isPageTail() ? _containerPtr->_map[++_mapIndex] : ++_cur;    // 判断是否是缓冲区末尾
return *this;

// DequeIterator &operator--()
_cur = isPageHead() ? _containerPtr->_map[--_mapIndex] + (getPageSize()-1) : --_cur;  // 判断是否是缓冲区头部
return *this;

// 添加元素
// void push_back(const T &value)
    - 如果没有足够空间
        - 申请新的 map
        - 拷贝原 deque 缓冲区中的元素
        - 构造元素

if (isLastPageTail())    // 是否是Deque缓冲区的最后一个位置
    reallocateMapForTail();
dataAlloc::construct(_end._cur, value);
++_end;

// void push_front(const T &value)
if (isFirstPageHead())
    reallocateMapForHead();
--_begin;
dataAlloc::construct(_begin._cur, value);

// 删除元素
// void Deque<T>::pop_back()
if (empty())
    throw std::out_of_range("pop_back() on empty Deque");
--_end;
dataAlloc::destroy(_end._cur);

// void pop_front()
if (empty())
    throw std::out_of_range("pop_front() on empty Deque");
dataAlloc::destroy(_begin._cur);
++_begin;
```


### list

list 的结构是双向循环链表，它有以下两个特点：

- list 对空间不造成浪费
- 对任意位置插入或者删除元素的时间复杂度都是常数级的

```C++
// 结点信息
struct ListNode {
      ListNode *_prev;
      ListNode *_next;
      T _data;
  };

// 构造结点
template<typename T>
typename List<T>::nodePtr List<T>::createListNode(const T &value) { // 构造有数据域的结点
    nodePtr ptrTmp = nodeAlloc::allocate();
    ptrTmp->_prev = nullptr;
    ptrTmp->_next = nullptr;
    allocator<T>::construct(&(ptrTmp->_data), value);

    return ptrTmp;
}

template<typename T>
typename List<T>::nodePtr List<T>::createListNode() { // 构造无数据域的结点
    nodePtr ptrTmp = nodeAlloc::allocate();
    ptrTmp->_prev = nullptr;
    ptrTmp->_next = nullptr;

    return ptrTmp;
}

// 销毁结点
template<typename T>
    void List<T>::deleteNode(nodePtr p) {
    allocator<T>::destroy(&(p->_data));
    nodeAlloc::deallocate(p);
}

// 迭代器，list 提供一个双向迭代器，另外链表在插入和删除结点后不会造成原有的 list 迭代器失效
struct ListIterator : public iterator<bidirectional_iterator_tag, T> {
    ......
    typedef ListNode<T> *nodePtr;

    nodePtr p;
}

// 插入元素
// void push_back(const T &value)
void List<T>::push_back(const T &value){
    if (empty()) {
        nodePtr ptrTmp = createListNode(value);
        ptrTmp->_prev = nullptr;
        _end.p->_prev = ptrTmp;
        ptrTmp->_next = _end.p;
        _begin.p = ptrTmp;
    } else {
        nodePtr ptrTmp = createListNode(value);
        nodePtr oldEndPrevNode = _end.p->_prev;

        oldEndPrevNode->_next = ptrTmp;
        ptrTmp->_prev = oldEndPrevNode;
        ptrTmp->_next = _end.p;
        _end.p->_prev = ptrTmp;
    }
}

// void push_front(const T& value)
void List<T>::push_front(const T &value) {
    auto ptrTmp = createListNode(value);
    auto oldStartNode = _begin.p;
    oldStartNode->_prev = ptrTmp;

    ptrTmp->_prev = nullptr;
    ptrTmp->_next = oldStartNode;

    _begin.p = ptrTmp;
}

// iterator insert(iterator position, const T& val)
    - position == _begin push_front(val);
    - position == _end push_back(val);
    - position == 中间节点

if (position == begin()) {
    push_front(val);
    return begin();
}
else if (position == end()) {
    auto ret = position;
    push_back(val);
    return ret;
}
auto node = createListNode(val);
(position.p->_prev)->_next = node;
node->_next = position.p;
node->_prev = (position.p)->_prev;
(position.p)->_prev = node;

return iterator(node);

// void insert(iterator position, size_type n, const T& value)
void List<T>::insert(iterator position, size_type n, const T& value)
{
    for (auto i = n; i != 0; --i)
        position = insert(position, value);
}

// void insert(iterator position, InputIterator first, InputIterator last)
void List<T>::insert(iterator position, InputIterator first, InputIterator last) 
{
    for (; first != last; first++)
        position = insert(position, *first);
}

// void pop_front()
if (empty())
    throw std::out_of_range("pop_front() on empty List");
auto oldNode = _begin.p;
_begin.p = oldNode->_next;
_begin.p->_prev = nullptr;
deleteNode(oldNode);

// void pop_back()
if (empty())
    throw std::out_of_range("pop_back() on empty List");

auto new_end_p = _end.p->_prev;
new_end_p->_next = nullptr;
allocator<T>::destroy(&(new_end_p->_data));     // 析构元素

nodeAlloc::deallocate(_end.p);  // 释放原_end.p内存
_end.p = new_end_p;

// iterator erase(iterator position)
    - delete _begin
    - delete _end
    - delete middle node

if (position == _begin) {
    pop_front();
    return _begin;
}
else if (position == _end){
    pop_back();
    return _end;
}
else {
    auto prevNode = position.p->_prev;
    auto nextNode = position.p->_next;

    prevNode->_next = nextNode;
    nextNode->_prev = prevNode;
    deleteNode(position.p);

    return iterator(nextNode);
}

// void reverse()
// void unique()
```


### hash_table

STL 中的 hash_map 和 hash_set 都是用 hash_table 实现的，hash_table 是采用开链法实现哈希表的。

```C++
// hash_table
template <typename Key>
class hash_table {
    ......
        typedef HashtableSetNode<Key> node;
    priavte:
        Vector<node *> buckets;    // 使用 vector 保存每一个 list
        size_type num_elements;    // 元素总个数，用于size()
    .....
}

// hash_table 节点
template <typename Key>
struct HashtableSetNode {
    Key key;
    HashtableSetNode* next;
};

// hash_table 迭代器，前向迭代器
template <typename Key>
struct HashTableSetIterator {
    ......
    typedef HashtableSetNode<Key> node;

    node* cur;                  // 迭代器目前所指向的结点
    HashTableSet<Key>* ht;      // 保持对容器的连接能力，因为需要从一个桶跳到另一个桶
}

HashTableSetIterator<Key>& HashTableSetIterator<Key>::operator++() {
    if (cur->next != nullptr)
        cur = cur->next;
    // 迭代器达到了一个list的末尾
    else {
        size_type bucket = ht->bkt_num_key(cur->key);
        /* 定位下一个非空桶或者bucket计数到末尾 */
        ++bucket;
        while(bucket < ht->bucketCount() && ht->buckets[bucket] == nullptr)
            bucket++;
        cur = bucket == ht->bucketCount() ? nullptr : ht->buckets[bucket];
    }
    return *this;
}
```



### Allocator 空间配置器

空间不一定是内存，空间也可以是磁盘或其它辅助储存媒体。

#### 一级配置器

- 使用 malloc 开辟大块空间
- 使用 free 释放掉大块空间
- 模拟 C++ operator new 中的 new_hanlder 机制处理内存不足的情况

#### 二级配置器

- 维护 16 个自由链表，自由链表中每个节点的内存空间的大小都是 1～16 的整数倍，相同大小的内存空间挂在 free_list 数组的相同下标下，如果内存池不足则调用一级空间配置器给内存池分配内存
- 如果需要大块 (大于 128 bytes) 空间，则直接调用一级空间配置器

二级配置器可以避免许多小额区块造成的内存碎片。其核心思想是将大于 128 bytes 的内存块交给一级配置器来处理，而将小于 128 bytes 的内存块交给内存池来管理。

内存池就是从堆中申请的大块的内存，负责填充 free_list。内存池直接由 malloc 分配，用 2 个指针表示，start_free 指针表示当前内存池剩余内存的起始地址，end_free 指针表示当前内存池剩余内存的结束地址。

- free_list

free_list 是指针数组。每一个数组槽内指向的是一个由指定大小的内存块连接起来的 list。客户申请内存，就将其中相应的内存从 list "拔下"，客户归还内存则就内存插入相应的 list 中。

```C++
// free_list
union obj
{
    union obj *free_list_link;
    char client_data[1];    // 没什么用
};

enum {  _ALIGN = 8  };    // 小型区块下限
enum {  _MAX_BYTES = 128  };    // 小型区块上限
enum {  _NFREELISTS = _MAX_BYTES / _ALIGN  };  // free_list 个数


// allocator
class alloc 
{
    private:
        static obj* free_list[_NFREELISTS];      // obj数组
    private:
        static char *start_free;    // 内存池起始位置
        static char *end_free;      // 内存池结束位置
        static size_t heap_size;
    private:
        /*  将bytes上调至8的倍数  */
        static size_t ROUND_UP(size_t bytes) 
        {  
            return (((bytes) + _ALIGN - 1) & ~(_ALIGN - 1)); 
        }

        /*  根据区块大小使用第n号free_list  */
        static size_t FREELIST_INDEX(size_t bytes) 
        {  
            return (((bytes) + _ALIGN - 1) / _ALIGN - 1);  
        }

        /*  为free_list某个大小的list增加节点  */
        static void *refill(size_t n);

        static char *chunk_alloc(size_t size, int &nobjs);
    public:
        static void *allocate(size_t n);
        static void deallocate(void *p, size_t n);
        static void *reallocate(void *p, size_t old_size, size_t new_size);
};

// 空间配置函数
void* alloc::allocate(size_t n)
{
    obj **my_free_list, *list;
    if (n > (size_t) _MAX_BYTES)    // 当需要的内存大于某个节点，就调用malloc
        return malloc(n);

    my_free_list = free_list + FREELIST_INDEX(n);    // 指向第n号list
    list = *my_free_list;

    if (list) {
        *my_free_list = list->free_list_link;    // 从list头部"拔出一个内存"，返回给客户
        return list;
    } else
        return refill(ROUND_UP(n));    // 如果当前list为空，则重新申请内存填充该list
}

// 空间释放函数
void alloc::deallocate(void *p, size_t n)
{
    obj *q = (obj *) p;
    obj **my_free_list;

    if (n > (size_t)_MAX_BYTES)    // 当需要的内存大于某个节点，就调用free
        free(p);
    else {            // 当需要的内存小于某个节点，就将其保存至某个list下
        my_free_list = free_list + FREELIST_INDEX(n);    // my_free_list指向第n号list
        q->free_list_link = *my_free_list;
        *my_free_list = q;
    }
}

// 从内存池取出内存供给free_list使用
// 从内存池取出空间给free_list使用(第n号)(假定size已经适当调整为8的倍数)
char *alloc::chunk_alloc(size_t size, int &n_objs) 
{    
    char *result;
    size_t total_bytes = size * n_objs;              // 需要的内存总量
    size_t bytes_left = end_free - start_free;       // 内存池剩余空间

    if (bytes_left >= total_bytes) {
        // 内存池剩余空间空间完全满足需要
        result = start_free;
        /* 更新start_free指针 */
        start_free += total_bytes;

        return result;
    } else if (bytes_left >= size) {
        // 内存池剩余空间不能完全满足需求量,但足够供应一个(含)以上的区块
        n_objs = bytes_left / size;    // 计算可以供应几个区块
        /* 更新start_free指针 */
        total_bytes = n_objs * size;
        result = start_free;
        start_free += total_bytes;

        return result;
    } else {
        // 内存池剩余空间连一个区块的大小都不满足
        size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
        // 试着让内存池中残余零头还有利用价值
        if ( bytes_left > 0 ) {
            // 将内存池残余内存配给适当的free_list(更小的size的list);
            obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
            ((obj *) start_free)->free_list_link = *my_free_list;
            *my_free_list = (obj *) start_free;
        }
        // 配置heap空间, 用来补充内存池
        start_free = (char *) malloc(bytes_to_get);
        if (nullptr == start_free) {
            obj *p, **my_free_list;
            // heap空间不足,搜寻适当的free_list("未用区块,且区块够大")
            for (int i = size; i <= _MAX_BYTES; i += _ALIGN) {
                my_free_list = free_list + FREELIST_INDEX(i);
                p = *my_free_list;
                if (0 != p) {
                    *my_free_list = p->free_list_link;
                     start_free = (char *)p;
                     end_free = start_free + i;
                     return chunk_alloc(size, n_objs);
                }
           }
           end_free = 0;
        }
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        return chunk_alloc(size, n_objs);
   }
}

```


### 迭代器

迭代器是 STL 的关键所在：将数据容器和算法分开，彼此独立设计，最后再用桥梁将它们联系在一起，这个桥梁就是迭代器。

迭代器型别：

- value_type：value_type 是指对象的型别，每一个打算与 STL 算法完美搭配的 class，都应该定义自己的 value_type 内嵌型别
- difference_type：difference_type 用来表示 2 个迭代器的距离，因此它可以用来表示容器的最大容量。通常用于 STL 中泛型算法提供的计数功能，例如 STL 中的 count()
- reference_type：表示迭代器所指的对象
    - 允许改变所指对象的内容
    - 不允许改变所指对象的内容
- pointer type：表示迭代器所指对象的地址
- Iterator_category
    - Input Iterator 只读
    - Output Iterator 只写
    - Forward Iterator
    - Bidirectional Iterator
    - Random Access Iterator

任何一个迭代器，其类型应该落在 "该迭代器所隶属的各种类型中，最强化的那个"，例如，int\* 既是 RandomAccess Iterator，又是 Bidirectional Iterator，同时也是 Forward Iterator，而且也是 Input Iterator，那么其类型应该归属为 RandomAccess iterator。



## Ref

- [http://ibillxia.github.io/blog/2014/11/26/insight-into-stl-archive/](http://ibillxia.github.io/blog/2014/11/26/insight-into-stl-archive/)
- [https://www.jianshu.com/p/0f010f76bfa3](https://www.jianshu.com/p/0f010f76bfa3)
- [https://www.cnblogs.com/Solstice/archive/2013/01/20/2868455.html](https://www.cnblogs.com/Solstice/archive/2013/01/20/2868455.html)
