# AC 自动机

> Aho–Corasick 算法是由 Alfred V. Aho 和 Margaret J.Corasick 发明的字符串搜索算法，用于在输入的一串字符串中匹配有限组“字典”中的子串。

AC 自动机主要是将 n 个模式串构建成一个确定性的树形有限状态机，然后将主串作为该有限状态机的输入，使该状态机进行状态的转换，当到达某些特定的状态时则说明发生模式匹配。

其中，自动机是计算理论的一个概念，其实是一张“图”，每个点是一个“状态”，而边则是状态之间的转移，根据条件能指导从一个状态走向另一个状态。

## AC 中的三个主要部分

- 建立 Trie 树
- 在 Trie 树上建立失配指针，成为 AC 自动机
- 在自动机上匹配字符串

## AC 的原理--三个函数

- goto() 用来指导状态转换
- failure() 用来指导匹配失败时的状态转换
- output() 描述哪些状态下发生了匹配，匹配的模式是什么

## 实现

一般情况下，都是用 Trie 树实现的。




## Ref

- [https://juejin.im/post/5b42ab4af265da0f8815eded](https://juejin.im/post/5b42ab4af265da0f8815eded)
- [https://segmentfault.com/a/1190000000484127](https://segmentfault.com/a/1190000000484127)


