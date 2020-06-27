# 其他知识点

- [git merge 和 git rebase 的区别](https://juejin.im/post/5af26c4d5188256728605809)
- [cin与scanf cout与printf效率问题](https://blog.csdn.net/l2580258/article/details/51319387)
- 动态链接和静态链接的区别
    - [https://www.zhihu.com/question/20484931](https://www.zhihu.com/question/20484931)
    - [https://www.jianshu.com/p/8743a0edb1ee](https://www.jianshu.com/p/8743a0edb1ee)
- 写一个 C/C++ 程序判断系统是多少位的？
    - 
    ```C
    #include <stdio.h>
    int main()
    {
        void* num = 0;
        printf("%d\n", sizeof(&num));
        // 输出 8 是 64 位的，4 则是 32 位的
    }
    ```
- i++ 是不是原子操作？
    - i++ 不是原子操作，因为 i++ 的三个阶段都可以被中断分离开，比如内存到寄存器，寄存器自增运算，再写回内存
    - 而 ++i 则不确定，需要根据编译器的具体情况来确定
- Linux 中的信号有那些？
    - SIGHUP 1：终端挂起或者控制进程终止
    - SIGINT 2：键盘中断
    - SIHQUIT 3：键盘退出键被按下
    - SIGFPE 8：发生致命的运算错误
    - SIGKILL 9：无条件终止进程
    - SIGALRM 14：定时器超时
    - SIGTERM 15：程序结束信号
    - [More...](https://www.jianshu.com/p/9c9b74f6a222)
- [RTT] Round-Trip Time，即往返时延。


