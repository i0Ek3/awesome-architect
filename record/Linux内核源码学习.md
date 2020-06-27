# Linux 内核源码学习

天下无难事，只怕有心人。所以，Linux 内核源码没什么好难的，你敢来学一学吗？

## 学习方法

- 资料搜集
- 源码定位
- 简单注释
- 详细注释
- 模块内部标识符依赖关系
- 模块间相互依赖关系
- 模块架构图

## 部分源码

### CFS 调度器

> CFS 调度器 (Completely Fair Scheduler) 为完全公平调度器。

#### 基本原理

调度器是操作系统的核心，可以比作是 CPU 时间的管理员。其主要负责选择某些就绪的进程来执行，不同的调度器根据不同的方法挑选出最合适运行的进程。

目前，Linux 中支持的调度器有 RT Scheduler，Deadline Scheduler，CFS Scheduler 和 Idle Scheduler等。

在 Linux 2.6.23 开始引入调度类，目的是将调度器模块化，可以提高其扩展性。

Linux 中有如下几种调度类：

- dl_sched_class deadline 调度器，调度策略为 SCHED_DEADLINE
- rt_sched_class 实时调度器，调度策略为 SCHED_FIFO 和 SCHED_RR
- fair_sched_class 完全公平调度器，调度策略为 SCHED_NORMAL 和 SCHED_BATCH
- idle_sched_class 空闲任务，调度策略为 SCHED_IDLE

为了保证每个进程运行的时间一样长，CFS 引入了虚拟时间的概念。

#### 源码分析

请参考[这里](http://www.wowotech.net/process_management/448.html)。

### socket

### 内存管理



## 相关面试题

- Linux 中内核空间及用户空间的区别？用户空间与内核通信方式有哪些？
    - 32 为 CPU 架构中，物理内存限制在 4G 大小。其中 0-1G 为内核使用，剩下的为用户使用
    - 使用 API，proc 文件系统，netlink，mmap 系统调用，信号
- Linux 中内存划分及如何使用？虚拟地址及物理地址的概念及彼此之间的转化，高端内存概念？
    - 高端内存的作用就是用于建立临时地址映射，用于 kernel 申请用户空间内存
- Linux 中中断的实现机制，tasklet 与 workqueue 的区别及底层实现区别？为什么要区分上半部和下半部？
    - tasklet 运行于中断上下文，不允许阻塞和休眠，而 workqueue 运行与进程上下文，可以休眠和阻塞
    - 为了避免被中断的代码延迟太长的时间。上半部就是中断处理程序，它需要完成的工作越少越好，执行得越快越好；下半部分延后执行且执行期间可以相应所有中断，这样可使系统处于中断屏蔽状态的时间尽可能的短，提高了系统的响应能力
- Linux 中断的响应执行流程？中断的申请及何时执行(何时执行中断处理函数)？
    - cpu 接受终端 -> 保存中断上下文跳转到中断处理历程 -> 执行中断上半部 -> 执行中断下半部 -> 恢复中断上下文
    - 在第一次打开 、硬件被告知终端之前
- Linux 中的同步机制？spinlock 与信号量的区别？
    - 自旋锁/信号量/读写锁/循环缓冲区
    - spinlock 在得不到锁的时候，程序会循环访问锁，性能下降；信号量在得不到锁的时候会休眠，等到可以获得锁的时候，继续执行
- Linux 中 RCU 原理？
    - RCU, Read-Copy Update，读，拷贝更新。是 Linux 内核实现的一种针对“读多写少”的共享数据的同步机制，是一种改进的读写锁。该机制可理解为：随意读，但更新数据的时候需要先拷贝一份副本，在副本上完成修改，再替换就数据。
    - 不同于其他的同步机制，它允许多个读者同时访问共享数据，而且读者的性能不会受影响（“随意读”），读者与写者之间也不需要同步机制（但需要“复制后再写”），但如果存在多个写者时，在写者把更新后的“副本”覆盖到原数据时，写者与写者之间需要利用其他同步机制保证同步。
    - RCU 的一个典型的应用场景是链表，在 Linux kernel 中还专门提供了一个头文件（include/linux/rculist.h），提供了利用 RCU 机制对链表进行增删查改操作的接口。
    - 源码解析参考这里：[https://lwn.net/Articles/262464/](https://lwn.net/Articles/262464/)
- Linux 中软中断的实现原理？
    - 参考：[https://www.ibm.com/developerworks/cn/linux/l-cn-linuxkernelint/](https://www.ibm.com/developerworks/cn/linux/l-cn-linuxkernelint/)
- Linux 系统实现原子操作有哪些方法？
    - 锁机制？循环 CAS？我也不清楚
- Linux 中 netfilter 的实现机制？是如何实现对特定数据包进行处理(如过滤，NAT之类的)及 HOOK 点的注册？
    - netfilter 是 Linux 内核中的一种防火墙机制，是 Linux 系统的子系统。其主要是通过表，链实现规则。netfilter 是表的容器，表是链的容器，链是规则的容器，最终形成对数据报处理规则的实现
- Linux 中系统调用过程？如应用程序中 read() 在 linux 中执行过程即从用户空间到内核空间？
    - 系统调用是通过中断方式实现的，Linux 下有三种系统调用方式：int 0x80 ，sysenter 和 syscall
    - 用户程序 -> C 库(API) -> system_call -> 系统调用服务例程 -> 内核程序
    - 参考: [https://www.binss.me/blog/the-analysis-of-linux-system-call/](https://www.binss.me/blog/the-analysis-of-linux-system-call/)
- Linux 内核的启动过程(源代码级)？
    - 操作系统的启动过程：通电 -> BIOS -> 主引导记录 -> 操作系统
    - Linux 内核启动过程：加载内核 -> 启动初始化进程 -> 确定运行级别 -> 加载开机启动程序 -> 用户登录 -> 进入 login shell -> 打开 non-login shell
    - 具体请参考这里：[http://www.ruanyifeng.com/blog/2013/08/linux_boot_process.html](http://www.ruanyifeng.com/blog/2013/08/linux_boot_process.html)
- Linux 调度原理？
    - Linux 内核中有三种调度方式，分别是：SCHED_OTHER 分时调度策略, SCHED_FIFO 实时调度策略，先到先服务和 SCHED_RR 实时调度策略，时间片轮转
    - CFS（完全公平调度器）是 Linux 内核 2.6.23 版本开始采用的进程调度器，它的基本原理是这样的：设定一个调度周期（sched_latency_ns），目标是让每个进程在这个周期内至少有机会运行一次，换一种说法就是每个进程等待 CPU 的时间最长不超过这个调度周期；然后根据进程的数量，大家平分这个调度周期内的 CPU 使用权，由于进程的优先级即 nice 值不同，分割调度周期的时候要加权；每个进程的累计运行时间保存在自己的 vruntime 字段里，哪个进程的 vruntime 最小就获得本轮运行的权利
    - 参考：[http://linuxperf.com/?p=42](http://linuxperf.com/?p=42) 和 [https://www.ibm.com/developerworks/cn/linux/kernel/l-kn26sch/](https://www.ibm.com/developerworks/cn/linux/kernel/l-kn26sch/)
- 对 Linux 网络子系统的认识？
    - 其目的就是为了使 Linux 成为一个可拓展的网络操作系统
    - 参考：[https://yq.aliyun.com/album/84](https://yq.aliyun.com/album/84)


## Ref

- [https://www.cnblogs.com/alantu2018/p/8994674.html](https://www.cnblogs.com/alantu2018/p/8994674.html)
- [https://github.com/0xAX/linux-insides](https://github.com/0xAX/linux-insides)
- [https://github.com/MintCN/linux-insides-zh](https://github.com/MintCN/linux-insides-zh)
- [https://cloud.tencent.com/developer/article/1006204](https://cloud.tencent.com/developer/article/1006204)
- [https://www.kernel.org/doc/Documentation/](https://www.kernel.org/doc/Documentation/)
- [https://www.ibm.com/developerworks/cn/linux/l-rcu/](https://www.ibm.com/developerworks/cn/linux/l-rcu/)
- [http://www.wowotech.net/sort/linux_kenrel](http://www.wowotech.net/sort/linux_kenrel)
- [https://www.ibm.com/developerworks/cn/linux/kernel/syscall/part1/](https://www.ibm.com/developerworks/cn/linux/kernel/syscall/part1/)
