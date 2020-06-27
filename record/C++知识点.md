# C++ 知识点

> 以下内容均整理自网络，侵删。

这里整理一些 C++ 语法知识中的难点。欢迎参考[C++官方文档](https://zh.cppreference.com/w/cpp/language)。

## 多态

多态是一种将消息以多种形式显示出来的能力，是以封装和继承为基础的。多态分为运行时多态和编译时多态。

编译时多态又称静态多态，如函数重载，运算符重载等。

运行时多态又称动态多态，如虚函数。

## 纯虚函数和抽象类

抽象类是用来实现 C++ 的接口的。当一个类定义中，至少有一个函数被声明为纯虚函数时，该类则为抽象类。而纯虚函数就是 `virtual void func() = 0` 这样的函数。

## 构造函数的执行步骤

通常有默认构造函数，复制（拷贝）构造函数，赋值构造函数和移动构造函数。

```C++
class A {
public:
    A() {} // 默认拷贝构造函数
    ~A() {} // 析构函数
    A(A&& a) {} // 移动构造函数
    A(const A& a) {} //复制（拷贝）构造函数
    A& operator=(const A& a) {} // 赋值构造函数
};
```

[代码参考](https://github.com/i0Ek3/awesome-architect/blob/master/example/constructor.cpp)

- 构造函数接收主函数传递的参数
- 根据成员变量定义的顺序给对象开辟空间
- 按顺序初始化成员变量
- 执行输入输出语句

## 构造函数可以调用虚函数吗？

不能，但在语法上是可行的，语义上有问题。

因为派生类对象内的基类对象会在派生类自身构造之前就已经构造完毕了，所以基类的构造函数中执行的虚函数是基类自己的版本，而不是派生类的。

## 为什么析构函数要是虚函数，而C++默认的析构函数不是虚函数？

- 析构函数不一定必须是虚函数，是否为虚函数取决于该类的使用，一般该类为基类产生继承和多态时，才会是虚函数，单独使用可以不是虚函数。之所以在继承和多态时设计为虚函数是因为当new派生类并且用基类指针指向这个派生类，在销毁基类指针时只会调用基类的析构函数，不会调用派生类的析构函数，因为基类无法操作派生类中非继承的成员，这样就造成派生类只new无法delete造成内存泄露。 
- 默认不是虚析构函数是因为如果析构函数为虚函数就需要编译器在类中增加虚函数表来实现虚函数机制，这样所需内存空间就更大了，因此没有必要默认为虚析构函数。

## 函数重载的实现原理

函数重载要求：

- 函数名要相同
- 参数个数、参数类型和参数顺序可以不同
- 返回值类型必须相同

在 C++ 中，使用 name mangling (倾轧) 技术来实现重载，通过命名倾轧来改名函数名，区分参数不同的同名函数。

关于命名倾轧，可以参考[这篇帖子](https://blog.csdn.net/u013220338/article/details/51472322)，总结的不错。

## 虚函数的实现原理

虚函数主要是由虚函数表实现的。在该表中，主要是一个类的虚函数的地址表，这张表解决了继承、覆盖等问题，保证其内容真实反应实际的函数。代码可参考[这里](#./../example/virtualTableAddress.cpp)。

编译器会为每个有虚函数的类创建一个虚函数表，该虚函数表将被该类的所有对象共享。类的虚函数表是一块连续的内存，每个内存单元中记录一个 JMP 指令的地址。类的每个虚函数占据虚函数表中的一块，如果类中有 N 个虚函数，那么其虚函数表将有 4N 字节的大小。

虚函数用来解决让基类指针能够访问派生类的成员函数，即其唯一用处就是构成多态。

有几点需要注意的是：

- 非类成员函数的普通函数不能是虚函数
- 静态函数不能是虚函数
- 构造函数不能是虚函数
- 内联函数不能是表现多态时的虚函数


参考：[https://coolshell.cn/articles/12165.html](https://coolshell.cn/articles/12165.html)

## 虚继承的实现原理

虚继承是解决 C++ 中多重继承问题的一种手段，具体来说就是为了解决多继承时的命名冲突和冗余数据的问题，使得在派生类中只保留一份间接基类的成员。参考：[https://zhuanlan.zhihu.com/p/41309205](https://zhuanlan.zhihu.com/p/41309205)

具体和 vftable 有关？参考：[http://www.uml.org.cn/c++/201611212.asp](http://www.uml.org.cn/c++/201611212.asp)

## 静态函数和虚函数

静态成员不属于对象，而属于类。不创建对象也可调用。从代码形式上来说没有任何区别。普通类成员函数与静态成员函数的区别在于普通成员函数有一个隐藏的调用参数（this）指针。所以就是说静态成员函数没有对象也可调用。但他们都是在编译期就进行地址绑定的。虚成员函数与他们的区别在于虚函数是对象生成时由对象的虚表决定的（所以是动态的）。

> virtual成员函数的关键是动态类型绑定的实例调用。然而，静态函数和任何类的实例都不相关，它是class的属性。

static function 是静态决议（编译的时候就绑定了），而virtual function 是动态决议的（运行时才绑定）。所以没办法将虚函数设置为静态的。

## Virtual 函数可以声明为内联的吗？

通常情况下是不可以的，因为 inline 是在编译期确定的，而 virtual 是在运行期确定的。

但有个特例，就是当编译阶段就已经知道调用虚函数的指针为多态指针。

## 重载、覆盖、隐藏

- 重载：是指同一可访问区内被声明的几个具有不同参数列（参数的类型，个数，顺序不同）的同名函数，根据参数列表确定调用哪个函数，重载不关心函数返回类型，与访问修饰符无关，构造方法也可以重载。
- 重写(覆盖)：是指派生类中存在重新定义的函数。其函数名，参数列表，返回值类型，所有都必须同基类中被重写的函数一致。只有函数体不同（花括号内），派生类调用时会调用派生类的重写函数，不会调用被重写函数。重写的基类中被重写的函数必须有virtual修饰。另外，构造函数不可以被重写，应为构造函数必须与类名相同。有private修饰的成员方法和有static\final修饰的方法不能被重写。
- 隐藏：是指派生类的函数屏蔽了与其同名的基类函数，注意只要同名函数，不管参数列表是否相同，基类函数都会被隐藏。

区别：

- 范围区别，即是否在同一函数中
- 参数区别，即参数列表是否相同
- virtual的区别，即是否可有无virtual关键字

## 继承权限

- public
- private
- protected

| 继承方式  | public 成员 | protected 成员 | private 成员 |
| --------- | ----------- | -------------- | ------------ |
| public    | public      | protected      | 不可见       |
| protected | protected   | protected      | 不可见       |
| private   | private     | private        | 不可见       |


我们可以使用 using 关键字来改变基类成员在派生类中的访问权限。

## 菱形继承

所谓菱形继承，就是说几个类的继承关系如菱形一般，是一种多重继承。

## C++ 对象模型

> 主要是 C++ 中的对象在内存中的布局。

我们一般从以下几种情况来讨论 C++ 对象的内存布局情况：

- 单一的一般继承
    - 虚函数表在最前面的位置
    - 成员变量根据其继承和声明顺序依次放在后面
    - 被覆盖的虚函数在虚函数表中靠前的位置
- 单一的虚拟继承
- 多重继承
    - 每个基类都有自己的虚表
    - 子类的成员函数被放到了第一个基类的表中
    - 内存布局中，其基类布局依次按声明顺序排序
    - 若继承关系中存在覆盖，则该基类中的虚函数会被覆盖为子类的虚函数
- 重复多重继承
- 钻石型虚拟多重继承

在 C++ 中有两种数据成员：static 和 nonstatic，以及三种类成员函数：static、nonstatic 和 virtual。

如下面的例子中，则包含了上述五种类型的数据或者函数：

```C++
class Base
{
public:
    Base(int i) :baseI(i){};
    int getI(){ return baseI; }
    static void countI(){};
    virtual ~Base(){}
    virtual void print(void){ cout << "Base::print()"; }
private:
    int baseI;
    static int baseS;
};
```

上述代码中 Base 类的内存分布如下，也就是说，在该对象模型下，对象由一系列的指针组成，且类中的每个数据成员和成员函数所占的大小是相同的，即一个指针的大小。

    slot -> Base::Base(int i)
    slot -> int Base::getI()
    slot -> static void countI()
    slot -> void Base::print(void)
    slot -> Base::~Base()
    slot -> int baseI()
    slot -> static in baseS

参考：

- [https://coolshell.cn/articles/12176.html](https://coolshell.cn/articles/12176.html)
- [https://www.jianshu.com/p/9fb37bb3094f](https://www.jianshu.com/p/9fb37bb3094f)
- [https://www.cnblogs.com/QG-whz/p/4909359.html](https://www.cnblogs.com/QG-whz/p/4909359.html)
- [https://github.com/zfengzhen/Blog/blob/master/article/《深入探索C%2B%2B对象模型》读书笔记.md](https://github.com/zfengzhen/Blog/blob/master/article/《深入探索C%2B%2B对象模型》读书笔记.md)

## 智能指针的实现原理

> `#include <memory>`

智能指针是指行为类似于指针的类对象，用于生存期控制、能够确保在离开指针所在作用域时，自动正确的销毁动态分配的对象，防止内存泄露。

常见的智能指针有如下几种：

- auto_ptr 

不支持复制和赋值，目前弃用。

- shared_ptr 

可随意赋值，并允许多个指针指向同一个对象。每一个shared_ptr的拷贝都指向相同的内存，在最后一个shared_ptr析构的时候，内存才会被释放。 

注意点：

- 不要用一个原始指针初始化多个 shared_ptr
- 不要在函数实参中创建 shared_ptr
- 通过shared_from_this() 返回this指针，不要将this指针作为shared_ptr返回出来，因为this指针本质上是一个裸指针，因此这样可能会导致重复析构。正确返回this的shared_ptr的做法是：让目标类通过派生 std::enable_shared_from_this类，然后使用基类的成员函数 shared_from_this来返回this的shared_ptr。
- 要避免循环引用。智能指针最大的一个陷阱是循环引用，循环引用会导致内存泄露。

- unique_ptr 

不支持复制和赋值，unique_ptr是一个独占型的智能指针，它不允许其他的智能指针共享器内部的指针，不允许通过赋值将一个unique_ptr赋值给另一个unique_ptr，但可以通过函数返回给其他的unique_ptr，还可以通过std::move来转移到其他的unique_ptr，这样他本身就不拥有原来指针的所有权。

- weak_ptr 

只引用，不计数。是从shared_ptr 生成的，可以避免 shared_ptr 带来的循环引用问题。

弱引用指针weak_ptr是用来监视shared_ptr的，不会使引用计数加1，它不会管理shared_ptr内部的指针，主要是为了监视shared_ptr的生命周期。 


智能指针是通过使用引用计数来实现的。即智能指针类将一个计数器与类指向的对象相关联，引用计数跟踪该类有多少个对象共享同一指针。

Ref:
    https://blog.csdn.net/ithiker/article/details/51532484 shared_ptr的gcc实现
    https://www.tamarous.com/shared_ptr/
    https://www.cnblogs.com/wxquare/p/4759020.html
    https://www.cnblogs.com/gtarcoder/p/4851729.html
    https://sustyuxiao.github.io/2018/02/04/2018-02-04/
    https://liam.page/2018/01/13/smart-pointer/

## C++ 中的强制类型转换

- static_cast
    用于基本类型间的转换
    不能用于基本类型指针间的转换
    用于有继承关系类对象间的转换和类指针间的转换
- dynamic_cast
    用于有继承关系的类指针间的转换
    用于有交叉关系的类指针间的转换
    具有类型检查的功能
    需要虚函数的支持
- reinterpret_cast
    用于指针间的类型转换
    用于整数和指针间的类型转换
- const_cast
    用于去掉变量的const属性
    转换的目标类型必须是指针或者引用

转载自[https://blog.csdn.net/kuweicai/article/details/82779648](https://blog.csdn.net/kuweicai/article/details/82779648)。

## C++中隐式类型转换

隐式转换，是指不需要用户干预，编译器私下进行的类型转换行为。

通过隐式转换，你可以直接将一个子类的对象使用父类的类型进行返回。

### 转换规则

- 基本数据类型的转换以取值范围的作为转换基础（保证精度不丢失）。隐式转换发生在从小->大的转换中。比如从char转换为int。
- 自定义对象的转换中，子类对象可以隐式的转换为父类对象。

### 发生条件

- 混合类型的算法运算
- 不同类型的赋值操作
- 函数参数传值
- 函数返回值


## 多态对象模型

多态，简单来说，是指在继承层次中，父类的指针可以具有多种形态——当它指向某个子类对象时，通过它能够调用到子类的函数，而非父类的函数，是一种泛型技术。

> 泛型技术，指的是试图使用不变的代码来实现可变的算法。比如模板技术，RTTI 技术，虚函数技术等。

另一个，多态主要是由虚函数实现的。在具有复杂继承关系的大中型程序中，多态可以增加其灵活性，让代码更具有表现力。

具体可以参考 [这里](https://sustyuxiao.github.io/2018/03/09/2018-03-09/)。

## 全特化/偏特化与类型萃取

通过全特化一个模板，可以对一个特定参数集合自定义当前模板，类模板和函数模板都可以全特化。 全特化的模板参数列表应当是空的，并且应当给出”模板实参”列表。

类似于全特化，偏特化也是为了给自定义一个参数集合的模板，但偏特化后的模板需要进一步的实例化才能形成确定的签名，需要注意的是函数模板不允许偏特化。

> 这里可能会发生重载决议。也就是说，函数调用时会在主模板之间进行决议，会选定主模版的重载模版，如果这个重载模版有全特化版本，则选用全特化版本，否则选用重载模版。

什么是主模板呢？就是非特化的模板。类模板可以全特化和偏特化，但是函数模板只能全特化，不能偏特化。

所谓类型萃取，就是要抽取类型的一些具体特征，比如它是哪种具体类型，它是引用类型，内建类型，还是类类型等。

## Traits 特性萃取

把功能相同而参数不同的函数抽象出来，通过traits将不同的参数的相同属性提取出来，在函数中利用这些用traits提取的属性，使得函数对不同的参数表现一致。

即提取被传进的对象对应的返回类型，让同一个接口实现对应的功能。

因为STL的算法和容器是分离的，两者通过迭代器链接。算法的实现并不知道自己被传进来什么。萃取器相当于在接口和实现之间加一层封装，来隐藏一些细节并协助调用合适的方法。

traits 的实现需要用到 enum，typedef，模板的偏特化。

enum 用于统一不同类型间变化的标识，或者理解为类中的 define。

typedef 用于定义模板类支持的特定形式，如果模板类不能以某种形式支持某一特性，则类型萃取器 traits 将无法正常工作。

通过type_traits可以实现在编译期计算、查询、判断、转换和选择，增强了泛型编程的能力，也增强了程序的弹性，使得我们在编译期就能做到优化改进甚至排错，能进一步提高代码质量。


参考：

- [https://sustyuxiao.github.io/2017/11/21/2017-11-21/](https://sustyuxiao.github.io/2017/11/21/2017-11-21/)
- [https://harttle.land/2015/10/03/cpp-template.html](https://harttle.land/2015/10/03/cpp-template.html)
- [https://zh.cppreference.com/w/cpp/language/template_specialization](https://zh.cppreference.com/w/cpp/language/template_specialization)

## 函数调用的过程

函数调用过程实际上也是一种中断的过程，其过程为：参数入栈->函数跳转->保护现场->恢复现场。

- EIP：指令指针，指向下一条即将执行的指令的地址
- EBP：基址指针，指向栈底
- ESP：栈指针，指向栈顶

具体可参考[这里](https://blog.csdn.net/dongtingzhizi/article/details/6680050)。

## 程序加载时的内存分布

- 代码区(.text)
- 只读数据段(.rodata)
- 可读写数据段(.data)
- BSS 段(.bss)
- 堆：通常存放 malloc 等库函数或者 new 来的对象
- 栈：通常存放普通变量
- 动态链接库映射区
- 保留区

## 内存管理

具体可参考[这里](https://chenqx.github.io/2014/09/25/Cpp-Memory-Management/)和[这里](https://www.ibm.com/developerworks/cn/linux/l-memory/)。

C++ 中的内存模型的组成：

- 自由存储区（栈）：局部非静态变量
- 动态区（堆）：用 new，malloc 分配的内存
- 静态区：全局变量，静态变量，字符串常量

C++ 中存储区域的划分：

- 栈区：由编译器自动分配释放，存放局部变量，函数参数，返回数据，返回地址等
- 堆区：由开发者手动分配释放， new 和 malloc 等分配的内存块
- 全局区（静态存储区）：存放全局变量，静态变量，常量
- 文字常量区：存储常量字符串
- 程序代码区：存储函数体代码的地方

> 全局变量、静态变量会初始化为零。

其中，堆和栈的区别如下：

- 管理方式不同：栈由编译器自动管理，堆需要我们人为控制
- 空间大小不同：在 32 系统下，堆内存可以有 4G 的空间，但对于栈来说，有固定的空间大小
- 是否可以产生碎片：对于堆来说，频繁的操作 new/delete 会使得内存空间不连续，从而产生大量碎片，但对于栈来说，则不会有这个问题，只因为它是先进后出的队列
- 生长方向不同：堆向上，栈向下
- 分配方式不同：堆是动态分配的，栈则可以是动态或者静态分配的
- 分配效率不同：栈是机器提供的数据结构，堆则是由函数库提供的

## 关于 new/delete 和 malloc/free

二者都可以用来动态申请内存和释放内存。二者申请的内存存储在不同的区域，如上所述。

new/delete 是 C++ 的运算符，而 malloc/free 是 C/C++ 的库函数。

new/delete 的底层是调用 malloc/free 实现的，而 malloc/free 的底层是调用系统的 API 实现的。

```
new      ---> operator new() ---> malloc() ---> constructor ---> ptr
delete   ---> destructor ---> operator delete() ---> free()

```

## realloc/malloc/calloc 的区别

```
#include <stdlib.h>

void* realloc(void* ptr, unsigned newsize); // 给一个已经分配了地址的指针重新分配空间

void* malloc(unsigned size); // 用于申请一段新的地址，只需指定需要的长度

void* calloc(size_t numElements, size_t sizeOfElement); // 同 malloc，但指定了申请地址的长度和元素数
```
## 野指针

野指针是指向被释放的或者访问受限的内存的一种指针，俗称指向“垃圾”地址的指针。

造成野指针的原因：

- 指针变量没有初始化
- 指针被 free 或者 delete 后，没有设置为 NULL
- 指针操作跨越了变量的作用范围

## 内存泄露

内存泄露产生的原因：

- 在类的构造函数和析构函数中没有匹配的调用new和delete函数
- 没有正确地清除嵌套的对象指针
- 在释放对象数组时在delete中没有使用方括号
- 指向对象的指针数组不等同于对象数组
- 缺少拷贝构造函数
- 缺少重载赋值运算符
- 没有将基类的析构函数定义为虚函数

如何检测：

- 软件检测
- 库函数
- 对象计数
- 重载 new/delete

## RTTI

即 Run Time Type Identification，运行时类型识别。

RTTI 是为多态而生成的信息，包括对象继承关系，对象本身的描述等，只有具有虚函数的对象才会生成。C++ 引入这个机制是为了让程序在运行时能根据基类的指针或引用来获得该指针或引用所指的对象的实际类型。

C++ 中通过两个操作提供 RTTI：

- typeid 运算符：该运算符返回其表达式或类型名的实际类型
- dynamic_cast 运算符：该运算符将基类的指针或引用安全地转换为派生类类型的指针或引用

简单来讲，其底层实现原理就是在一个类的虚函数表里添加了一个新条目。

参考：[https://blog.csdn.net/ljianhui/article/details/46487951](https://blog.csdn.net/ljianhui/article/details/46487951)

## RAII

即 Resource Acquisition Is Initialization，资源获取即初始化。

### 作用

- 申请资源
- 使用资源
- 释放资源

### 原理

- 将资源封装到一个类中
- 在构造函数中初始化资源
- 在析构函数中销毁资源
- 使用时声明一个该对象的类

其本质是用对象代表资源，把管理资源的任务转化为管理对象的任务，将资源的获取和释放与对象的构造和析构对应起来，从而确保在对象的生存期内资源始终有效，对象销毁时资源一定会被释放。

参考：

- [https://zhuanlan.zhihu.com/p/34660259](https://zhuanlan.zhihu.com/p/34660259)
- [https://zh.cppreference.com/w/cpp/language/raii](https://zh.cppreference.com/w/cpp/language/raii)

## 左/右值及其引用

左值是表达式结束之后仍然存在的持久化对象，而右值是指表达式结束时就不再存在的临时对象。 

### 右值

- 将亡值

将亡值是c++11新增的、与右值引用相关的表达式，比如，将要被移动的对象、T&&函数返回值、std::move返回值和转换为T&&的类型的转换函数的返回值

- 纯右值

非引用返回的临时变量、运算表达式产生的临时变量、原始字面量和lambda表达式等都是纯右值

### 右值引用

实现对一个右值进行引用，标记为T&&. 因为右值不具名，因此只能通过引用的方式找到它。

右值引用可以延长右值的生命周期，可以避免一些临时对象的构造和析构，从而提高性能。

### 左值引用

左值引用是对左值进行引用的类型，分为常量左值引用和非常量左值引用。其中，常量左值引用可以引用常量左值、非常量左值、常量右值、非常量右值；而非常量左值引用只能引用非常量左值。

参考：[https://www.cnblogs.com/gtarcoder/p/4805606.html](https://www.cnblogs.com/gtarcoder/p/4805606.html)

## 值语义 & 对象语义

值语义是指对象的拷贝与原对象无关，拷贝之后就与原对象脱离关系，彼此独立互不影响，如深拷贝。而 int，C++ 中的内置类型等都是值语义，标准库类型 string，vector，map 也都是值语义。

对象语义或者说是引用语义，指的是面向对象意义下的对象，对象拷贝是禁止的。

## 浅拷贝 & 深拷贝

浅拷贝只是对对象的指针进行拷贝，而深拷贝出了拷贝指针外，还会拷贝对象本身。这样会有两个不同的对象，而浅拷贝的指针还是指向的同一个地址。

浅拷贝会造成程序的崩溃。因为只是拷贝了指针，所以会有两个指针指向同一个地址，那么在对象结束的时候，同一份资源就会调用两次析构函数，即 delete 同一块内存 2 次，这会导致程序崩溃。

另一方面，两个指针指向的是同一块内存，任何一方的变动都会影响到另一放的变化。并且对同一空间来说，第二次释放失败，将会造成该空间无法操作，造成内存泄漏。

## 移动语义 & 完美转发

移动语义可以将资源 ( 堆，系统对象等 ) 从一个对象转移到另一个对象，这样能够减少不必要的临时对象的创建、拷贝以及销毁，能够大幅度提高 C++ 应用程序的性能，通过 std::move 实现。

完美转发即将一组参数原封不动的传递给另一个函数，通过 std::forward 实现。

## C++ 中的三五法则

三五法则：如果需要析构函数，则一定需要拷贝构造函数和赋值操作符。三五法则规定了什么时候需要拷贝构造函数、拷贝赋值函数、析构函数等。

- 需要析构函数的类也需要拷贝构造函数和拷贝赋值函数
- 需要拷贝操作的类也需要赋值操作，反之亦然
- 析构函数不能是删除的
- 如果一个类有删除的或不可访问的析构函数，那么其默认和拷贝构造函数会被定义为删除的
- 如果一个类有 const 或引用成员，则不能使用合成的拷贝赋值操作

参考：[https://blog.csdn.net/u013613341/article/details/50551513](https://blog.csdn.net/u013613341/article/details/50551513)

## 生命周期

- 全局对象在 main 函数开始前被创建，在 main 函数退出后被销毁
- 静态对象在第一次进行作用域时被创建，在 main 函数退出后被销毁
- 局部对象在进入作用域时被创建，在退出作用域时被销毁
- new 创建的对象会一直存在

```
CPerson p1;             // main执行前，构造函数被调用
void func(){
    static CPerson p2;  // func第一次执行前，构造函数被调用
    CPerson p3;         // p3的构造函数被调用
                        // func结束时，p3的析构函数被调用
}  
int main(){
    CPerson p4, *p5;    // 调用p4的构造函数
    func();
    p5 = new CPerson;   // 调用p5的构造函数
    delete p5;          // 调用p5的析构函数
                        // main结束时，p4的析构函数被调用
}
                        // 程序结束前，p1, p2的析构函数被调用

ref: https://www.tianmaying.com/tutorial/cpp-object-lifecycle

```

## class 和 struct

- 在不指明具体属性的情况下，struct 成员默认是 public 的，class 成员默认属性是 private的。

## 基于对象和面向对象

什么是“基于”对象呢？就是关注“对象之间”的关系，而不是关注对象和类的关系。

通常“基于对象”是使用对象，但是无法利用现有的对象模板产生新的对象类型，继而产生新的对象，也就是说“基于对象”没有继承的特点。

“面向对象”和“基于对象”都实现了“封装”的概念，但是面向对象还实现了“继承和多态”，而“基于对象”则没有。

另外，面向对象是一种思想。并且在基于对象的概念里，只有“对象”而没有“类”的概念，且函数（方法）也是变量。


## C++ 11 中的新特性

- auto/decltype
- lambda 表达式
- 智能指针
- delete 和 default 函数
- 委托构造函数
- STL 标准库
- 范围 for 循环
- 模板及其模板编程
- 函数式编程，如 for_each
- 时间工具 chrono
- 线程
- 正则表达式
- 文件操作
- 异常


参考：

- [图说 C++ 对象模型](https://www.cnblogs.com/QG-whz/p/4909359.html)
- [https://coolshell.cn/articles/5265.html](https://coolshell.cn/articles/5265.html)
- [https://www.kancloud.cn/wangshubo1989/new-characteristics/99704](https://www.kancloud.cn/wangshubo1989/new-characteristics/99704)
- [C++ 14 新特性](https://www.infoq.com/news/2014/08/cpp14-here-features)
- [C++ 17 新特性](http://www.masalaanews.com/top-19-new-features-of-c17-you-need-to-know-about-it/)
- [C++ 20 新特性](https://www.reddit.com/r/cpp/comments/6ngkgc/2017_toronto_iso_c_committee_discussion_thread/)
- [C++ 参考手册](https://zh.cppreference.com/w/)
- [C++ 的几个基本原理和技术](https://liam.page/2017/04/09/Foundations-of-Cpp/)
- [C++ 中虚函数、虚继承内存模型](https://juejin.im/entry/5b8634b0e51d4538a423db53)
- [C++ 中对象的生命周期](https://blog.csdn.net/bzhxuexi/article/details/17253227)
- [移动语义和完美转发](https://codinfox.github.io/dev/2014/06/03/move-semantic-perfect-forward/)
- [基于对象和面向对象](https://cloud.tencent.com/developer/article/1004486)
