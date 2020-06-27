# Linux 进程

> 本文整理自网络，并非原创，侵删。

进程是指正在执行的程序,是程序正在运行的一个实例。它由程序指令，和从文件、其它程序中读取的数据或系统用户的输入组成。

Linux 系统下我们可以通过 `ps` 命令来查看系统中正在运行进程。

## 进程的类型

在 Linux 中主要有两种类型的进程：

- 前台进程（也称为交互式进程）：这些进程由终端会话初始化和控制。换句话说，需要有一个连接到系统中的用户来启动这样的进程；它们不是作为系统功能/服务的一部分自动启动。

- 后台进程（也称为非交互式/自动进程）：这些进程没有连接到终端；它们不需要任何用户输入。
    - 守护进程 后台进程的特殊类型，它们在系统启动时启动，并作为服务一直运行；它们不会死亡。它们自发地作为系统任务启动（作为服务运行）。但是，它们能被用户通过 init 进程控制。

## 进程的表示

在 Linux 内核内，进程是由相当大的一个称为 task_struct 的结构表示的。此结构包含所有表示此进程所必需的数据，此外，还包含了大量的其他数据用来统计（accounting）和维护与其他进程的关系（父和子）。

```C
// task_struct 结构体的部分代码
struct task_struct {
    volatile long state;
    void *stack;
    unsigned int flags;
    int prio, static_prio;
    struct list_head tasks;
    struct mm_struct *mm, *active_mm;
    pid_t pid;
    pid_t tgid;
    struct task_struct *real_parent;
    char comm[TASK_COMM_LEN];
    struct thread_struct thread;
    struct files_struct *files;
    ...
};
```

## 进程的创建

在 Linux 系统中有两种常规的方式来创建进程：

- 使用 system() 函数，这个方法相对简单，但是比较低效而且具有明显的安全隐患。
- 使用 fork() 和 exec() 函数，这个技巧比较高级但提供更好的灵活性、速度以及安全性。

## 进程的销毁

进程销毁可以通过几个事件驱动：通过正常的进程结束、通过信号或是通过对 exit 函数的调用。不管进程如何退出，进程的结束都要借助对内核函数 do_exit（在 ./linux/kernel/exit.c 内）的调用。

do_exit() 的目的是将所有对当前进程的引用从操作系统删除（针对所有没有共享的资源）。销毁的过程先要通过设置 PF_EXITING 标志来表明进程正在退出。内核的其他方面会利用它来避免在进程被删除时还试图处理此进程。将进程从它在其生命期间获得的各种资源分离开来是通过一系列调用实现的，比如 exit_mm（删除内存页）和 exit_keys（释放线程会话和进程安全键）。do_exit 函数执行释放进程所需的各种统计，这之后，通过调用 exit_notify 执行一系列通知（比如，告知父进程其子进程正在退出）。最后，进程状态被更改为 PF_DEAD，并且还会调用 schedule 函数来选择一个将要执行的新进程。请注意，如果对父进程的通知是必需的（或进程正在被跟踪），那么任务将不会彻底消失。如果无需任何通知，就可以调用 release_task 来实际收回由进程使用的那部分内存。

## 进程的识别

因为 Linux 是一个多用户系统，意味着不同的用户可以在系统上运行各种各样的程序，内核必须唯一标识程序运行的每个实例。

程序由它的进程 ID（PID）和它父进程的进程 ID（PPID）识别，因此进程可以被分类为：

- 父进程: 这些是在运行时创建其它进程的进程。
- 子进程: 这些是在运行时由其它进程创建的进程。

## init 进程

init 进程是系统中所有进程的父进程，它是启动 Linux 系统后第一个运行的程序；它管理着系统上的所有其它进程。它由内核自身启动，因此理论上说它没有父进程。

init 进程的进程 ID 总是为 1。它是所有孤儿进程的收养父母（它会收养所有孤儿进程）。


## 面试中可能会问到的问题

### 孤儿进程 & 僵尸进程

- 孤儿进程：父进程结束了，而它的一个或多个子进程还在运行，那么这些子进程就成为孤儿进程(father died)。子进程的资源由init进程(进程号PID = 1)回收。
- 僵尸进程：子进程退出了，但是父进程没有用wait或waitpid去获取子进程的状态信息，那么子进程的进程描述符仍然保存在系统中，这种进程称为僵尸进程。

#### 问题的危害

- 孤儿进程：孤儿进程是没有父进程的进程，它由init进程循环的wait()回收资源，init进程充当父进程。因此孤儿进程并没有什么危害。
- 僵尸进程：如果父进程不调用wait或waitpid的话，那么保留的信息就不会被释放，其进程号就会被一直占用，但是系统所能使用的进程号是有限的，如果大量产生僵尸进程，将因没有可用的进程号而导致系统无法产生新的进程。

#### 解决办法

- 使用 `kill` 命令杀死父进程，`kill -9 parent-pid`

严格的说，僵尸进程并不是问题的根源，罪魁祸首是产生大量僵死进程的父进程。因此，我们可以直接除掉元凶，通过kill发送SIGTERM或者SIGKILL信号。元凶死后，僵尸进程进程变成孤儿进程，由init充当父进程，并回收资源。

- 父进程用 wait 或 waitpid 去回收资源

父进程通过 wait 或 waitpid 等函数去等待子进程结束，但是不好，会导致父进程一直等待被挂起，相当于一个进程在干活，没有起到多进程的作用。

- 通过信号机制，在处理函数中调用 wait，回收资源

通过信号机制，子进程退出时向父进程发送 SIGCHLD 信号，父进程调用signal(SIGCHLD,sig_child) 去处理 SIGCHLD 信号，在信号处理函数 sig_child() 中调用 wait 进行处理僵尸进程。什么时候得到子进程信号，什么时候进行信号处理，父进程可以继续干其他活，不用去阻塞等待。

## 进程、线程和协程

### 概念

- 进程：进程是具有一定独立功能的程序，关于某个数据集合上的一次运行活动，进程是系统进行资源分配和调度的一个独立单位。
- 线程：线程是进程的一个实体，是 CPU 调度和分派的基本单位，它是比进程更小的能独立运行的基本单位。
- 协程：是一种比线程更加轻量级的存在，在线程中实现调度，避免了陷入内核级别的上下文切换造成的性能损失，进而突破了线程在 IO 上的性能瓶颈。

### 区别

主要差别在于它们是操作系统不同的资源管理方式。

- 进程是资源分配的最小单位，线程是程序执行的最小单位。
- 进程有自己的独立地址空间，而线程是共享进程中的数据。
- 线程之间的通信更方便，同一进程下的线程共享全局变量、静态变量等数据，而进程之间的通信需要以通信的方式（IPC)进行。
- 多进程程序更健壮，多线程程序只要有一个线程死掉，整个进程也就死掉了，而一个进程死掉并不会对另外一个进程造成影响，因为进程有自己独立的地址空间。
- 协程不是被操作系统内核所管理的，而完全是由程序所控制的，即在用户态执行。

### 进程同步

- 互斥锁
- 信号量
- 自旋锁
- 条件变量
- 读写锁: 一种特殊的自旋锁，将共享资源的访问者划分为读者和写者，读者只进行资源的读访问，写者只进行对资源的写操作
- 屏障
- 原子操作
- 各类IPC机制

### 线程具有的优势

- 在进程内创建、终止线程比创建、终止进程要快。
- 同一进程内的线程间切换比进程间的切换要快，尤其是用户级线程间的切换。



## Ref

- 《UNIX 环境高级编程》
- [https://www.ibm.com/developerworks/cn/linux/l-linux-process-management/](https://www.ibm.com/developerworks/cn/linux/l-linux-process-management/)
- [https://linux.cn/article-8451-1.html](https://linux.cn/article-8451-1.html)
- [http://www.sohu.com/a/120145657_220533](http://www.sohu.com/a/120145657_220533)
- [https://my.oschina.net/sallency/blog/1863423](https://my.oschina.net/sallency/blog/1863423)
- [https://yq.aliyun.com/articles/61894](https://yq.aliyun.com/articles/61894)
- [http://www.ruanyifeng.com/blog/2013/04/processes_and_threads.html](http://www.ruanyifeng.com/blog/2013/04/processes_and_threads.html)
- [https://foofish.net/thread-and-process.html](https://foofish.net/thread-and-process.html)
- [https://juejin.im/entry/59cf00646fb9a00a5c3c7715](https://juejin.im/entry/59cf00646fb9a00a5c3c7715)
- [https://www.jianshu.com/p/eca71b7fda2c](https://www.jianshu.com/p/eca71b7fda2c)
- [http://blog.2baxb.me/archives/407](http://blog.2baxb.me/archives/407)
