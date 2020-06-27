# fork 系列讲解

> 以下内容整理自网络，侵删。

即进程复制的三种机制，fork()，vfork() 和 clone()。

| 系统调用 | 描述                                                                                         |
| -------- | -------------------------------------------------------------------------------------------- |
| fork     | fork 创建的子进程是父进程的完整副本，复制了父进程的资源，包括内存的内容和 task_struct 的内容 |
| vfork    | vfork 创建的子进程与父进程共享数据段，而且由 vfork 创建的子进程将先于父进程运行,添加了COW机制|
| clone    | clone 是 Linux 上提供的创建线程的系统调用                                                    |

这三个系统调用的底层都是通过 do_fork() 内核函数来实现的，只是对 do_fork() 传递不同的参数来实现不同的功能的。

关于 fork() 的题目可以参考陈皓的文章：[https://coolshell.cn/articles/7965.html](https://coolshell.cn/articles/7965.html)。

## do_fork()

do_fork 中的字段：

- clone_flags：与clone()函数的flag参数相同
- stack_start：与clone()函数的child_stack参数相同
- regs：指向通用寄存器值的指针，通用寄存器的值是在从用户态切换到内核态时被保存到内核态堆栈中的
- stack_size：未使用，总是被设置为0

## fork 与 vfork 的区别

- fork 子进程拷贝父进程的数据段和代码段，vfork 子进程与父进程共享数据段
- fork 父子进程的执行顺序不确定，vfork 则保证子进程先运行

## 写时复制

Copy On Write，也就是说等一个进程写入数据的时候系统再进行复制资源。


## Ref

- [https://www.ibm.com/developerworks/cn/linux/kernel/l-thread/](https://www.ibm.com/developerworks/cn/linux/kernel/l-thread/)
- [http://guojing.me/linux-kernel-architecture/posts/clone-fork-and-vfork/](http://guojing.me/linux-kernel-architecture/posts/clone-fork-and-vfork/)
