# Linux 内存管理

> 以下内容均整理自网络，并非原创，侵删。

## Linux 内存管理算法

在Linux中，伙伴系统（buddy system）是以页为单位管理和分配内存的。其中，伙伴算法是用来管理 Page 的经典算法。那什么是伙伴算法呢？

伙伴算法在理论上是非常简单的内存分配算法。它的用途主要是尽可能减少外部碎片，同时允许快速分配与回收物理页面。为了减少外部碎片，连续的空闲页面，根据空闲块（由连续的空闲页面组成）大小，组织成不同的链表（或者orders）。这样所有的2个页面大小的空闲块在一个链表中，4个页面大小的空闲块在另外一个链表中，以此类推。

简言之，就是为了避免在当前内存中没有连续的页框而到另一段内存中寻找新的连续的页框而造成的内存浪费而引入的一种算法。


那由伙伴算法造成的问题该如何解决呢？那就是 Slab 分配器。

Slab 分配器是为了解决伙伴算法中内存严重浪费问题而产生的、专为小内存分配的一种算法。slab分配器分配内存以Byte为单位。但是slab分配器并没有脱离伙伴系统，而是基于伙伴系统分配的大内存进一步细分成小内存分配。

## 虚拟内存

- 概念：虚拟内存是计算机系统内存管理的一种技术，它使得应用程序认为它拥有连续可用的内存。
- 作用
    - 把主存看作为一个存储在硬盘上的虚拟地址空间的高速缓存，并且只在主存中缓存活动区域（按需缓存）。
    - 为每个进程提供了一个一致的地址空间，从而降低了程序员对内存管理的复杂性。
    - 保护了每个进程的地址空间不会被其他进程破坏。
- 实现
    - 请求分页存储管理
    - 请求分段存储管理
    - 请求段页式存储管理

## 内存池

内存池(Memery Pool)技术是在真正使用内存之前，先申请分配一定数量的、大小相等(一般情况下)的内存块留作备用。当有新的内存需求时，就从内存池中分出一部分内存块，若内存块不够再继续申请新的内存。

- 作用
    - 避免了内存碎片，使得内存分配效率得到提升。
    - 速度远比 malloc/free 快，因为减少了系统调用的次数，特别是频繁申请/释放内存块的情况。
    - 节省空间
- 实现
    - STL 中内存池的实现
    - [https://github.com/cacay/MemoryPool](https://github.com/cacay/MemoryPool)


## Ref

- [https://zhuanlan.zhihu.com/p/36140017](https://zhuanlan.zhihu.com/p/36140017)
- [http://ilinuxkernel.com/?p=1029](http://ilinuxkernel.com/?p=1029)
- [https://coolshell.cn/articles/10427.html](https://coolshell.cn/articles/10427.html)
- [https://www.jianshu.com/p/eecbb1506eee](https://www.jianshu.com/p/eecbb1506eee)
- [https://www.ibm.com/developerworks/cn/linux/l-linux-slab-allocator/](https://www.ibm.com/developerworks/cn/linux/l-linux-slab-allocator/)
- [https://juejin.im/post/59f8691b51882534af254317](https://juejin.im/post/59f8691b51882534af254317)
- [https://sylvanassun.github.io/2017/10/29/2017-10-29-virtual_memory/](https://sylvanassun.github.io/2017/10/29/2017-10-29-virtual_memory/)
- [http://blog.jobbole.com/107345/](http://blog.jobbole.com/107345/)
- [https://juejin.im/post/5beb8b32f265da617464641c](https://juejin.im/post/5beb8b32f265da617464641c)


