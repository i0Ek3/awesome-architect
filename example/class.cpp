#include <iostream>
using namespace std;

// 通常，我们将类的定义放到头文件中，而将函数的定义放到源文件中

// C++ 中，public private protected 只能修饰类的成员，而不能修饰类
// 类的内部，成员之间可以互相访问，无论声明了那种访问限定
// 但是在类外，只能通过对象进行访问 public 属性的成员

class Test { // 类本身是一种模板，不占用内存空间，所以没办法堆类内的成员变量进行赋值，因为变量的值需要内存来存储
public:
    int a;
    char c;
    void test();
    /*
    void test() { // 该函数默认是内联的
        cout << "hello test" << endl;
    }*/
};

// 若成员函数test()是在类外定义的，则我们需要通过类名进行限定
// 而在类中，你需要对该函数进行声明。
// 通常我们建议在类外对函数进行定义，如果你想让函数是内联的，那么建议将函数定义放在类中
void Test::test() { // 而该函数不是内联的
    cout << "Test::test()" << endl; 
}


// 根据C++软件设计规范，实际项目开发中的成员变量以及只在类内部使用的成员函数（只被成员函数调用的成员函数）都建议声明为 private，而只将允许通过对象调用的成员函数声明为 public
class Access { // class 默认为private，struct 默认为public
    int m_age;
    char *m_name;
    float m_score;
public:
    /*
    void setname(char *name);
    void setage(int age);
    void setscore(float score);
    */
    Access(int age, char *name, float score);
    void show();
};

/*
void Access::setname(char *name) {
    m_name = name;
}

void Access::setage(int age) {
    m_age = age;
}

void Access::setscore(float score) {
    m_score = score;
}
*/

Access::Access(int age, char *name, float score) {
    // 有点注意的地方：
    // 1. 构造函数不能有返回值，void也不行
    // 2. 函数体中不能有return语句
    m_age = age;
    m_name = name;
    m_score = score;

}

void Access::show() {
    cout << m_name << "'s age is " << m_age << ", and his score is " << m_score << endl;
}

// 在有了构造函数后，我们可以大大简化上述代码


int main()
{
    /*
    Test t;
    // Test *p = &t; // 在栈上创建，有名字
    Test *p = new Test; // 在堆上创建，没有名字，只有指针
    // 有了对象指针后，我们可以用->来访问对象的成员变量和成员函数了
    p->a=6;
    p->c='w';
    cout << p->a << endl << p->c << endl;
    delete p;
    t.a = 5;
    t.c = 'x';
    cout << t.a << endl << t.c << endl;
    t.test();
    return 0;
    */

    /*
    Access a;
    a.setname("kkk");
    a.setage(18);
    a.setscore(87.8);
    a.show();

    Access *p = new Access;
    p->setname("gofer");
    p->setage(18);
    p->setscore(90.9);
    p->show();
    delete p;
    */

    Access a(18, "xxx", 99.0);
    a.show();
    Access *p = new Access(19, "ooo", 100);
    p->show();
    delete p;


    return 0;

}

