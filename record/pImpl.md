# pImpl

> pointer to implementation, 指向实现的指针。

是一种 C++ 编程技巧，它将类的实现细节放到分离的、以不透明指针访问的类中，以从其对象表示中移除实现细节。


## 优点

- 降低耦合
- 信息隐藏
- 降低编译依赖，提高编译速度
- 接口与实现分离

## Code

```C++
// foo.h - header file
#include <memory>
class foo
{
public:
    foo();
    ~foo();
    foo(foo&&);
    foo& operator=(foo&&);
private:
    class impl;
    std::unique_ptr<impl> pimpl;
};

// foo.cpp - implementation file
class foo::impl
{
public:
    void do_internal_work()
    {
        internal_data = 5;
    }
private:
    int internal_data = 0;
};

foo::foo()
  : pimpl{std::make_unique<impl>()}
{
    pimpl->do_internal_work();
}
foo::~foo() = default;
foo::foo(foo&&) = default;
foo& foo::operator=(foo&&) = default;
```

## Ref

- [https://en.cppreference.com/w/cpp/language/pimpl](https://en.cppreference.com/w/cpp/language/pimpl)
- [https://cpppatterns.com/patterns/pimpl.html](https://cpppatterns.com/patterns/pimpl.html)

