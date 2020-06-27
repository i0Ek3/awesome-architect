# Randx


我们知道，rand() 是随机函数，常见的有 rand5(), rand7() 等等。其中，rand5() 用来随机生成1～5之间的自然数，而 rand7() 用来随机生成1～7之间的自然数。

通常我们用 rand_a() 来实现 rand_b() 是很容易的，其中，a > b。

即只要调用一次 rand_a()，在得到\[0, b)之间的数时，就返回这个数，否则继续调用 rand_a()。

但反过来呢？比如用 rand_a() 来实现 rand_b() 呢? 其中，a < b。

该类型的问题主要考查的是对概率的理解，其关键是要算出rand_b()。

如用 rand7() 来实现 rand10() 的参考代码如下：


```C++

int rand7() {
    return rand() % 7 + 1;
}

int rand10() {
    int a_1, a_2, a_3;
    do {
        a_1 = rand7() - 1;
        a_2 = rand7() - 1;
        a_3 = a_1 * 7 + a_2;
    } while(a_3 >= 40)

    return (a_1 * 7 + a_2) / 4 + 1;
}

```



## Ref

- [https://blog.csdn.net/u010025211/article/details/49668017](https://blog.csdn.net/u010025211/article/details/49668017)
- [http://www.interviewdruid.com/rand7-given-rand5/](http://www.interviewdruid.com/rand7-given-rand5/)
- [https://github.com/julycoding/The-Art-Of-Programming-By-July/blob/master/ebook/zh/08.01.md](https://github.com/julycoding/The-Art-Of-Programming-By-July/blob/master/ebook/zh/08.01.md)

