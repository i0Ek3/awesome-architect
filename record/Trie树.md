# Trie 树

> Wiki: Trie 树，又称前缀树或字典树，是一种有序树，用于保存关联数组，其中的键通常为字符串。

直白的说，Trie 树就是一棵查找树，同样是一种树结构, 能做到高效查询与插入。与二叉查找树不同的是，键不是直接保存在节点中，而是由节点在树中的位置决定。一个节点的所有子孙都有相同的前缀，也就是这个节点对应的字符串，而根节点对应空字符串。一般情况下，不是所有的节点都有对应的值，只有叶子节点和部分内部节点所对应的键才有相关的值。Trie 树实际上是一个确定有限状态自动机，即 [DFA](https://zh.wikipedia.org/wiki/确定有限状态自动机)，通常用转移矩阵表示。

其**核心思想**就是通过最大限度地减少无谓的字符串比较，使得查询效率更高，即**用空间换时间**，再利用共同前缀来提高查询效率。


## 特点

- 根节点不包含字符，除根节点外每一个节点都只包含一个字符

- 从根节点到某一节点，路径上经过的字符连接起来，为该节点对应的字符串

- 每个节点的所有子节点包含的字符都不相同


## 具体操作

- 查找

    Trie 树的插入操作很简单，其实就是将单词的每个字母逐一插入 Trie 树。插入前先看字母对应的节点是否存在，存在则共享该节点，不存在则创建对应的节点。

- 插入

    将要查找的字符串分割成单个的字符，然后从 Trie 树的根节点开始匹配。

- 删除

    Trie 树的删除操作类似二叉树的删除操作，不过这里需要考虑 Trie 树中将要删除的节点的位置。


## 应用场景

Trie 树常用于搜索提示。如当输入一个网址，可以自动搜索出可能的选择。当没有完全匹配的搜索结果，可以返回前缀最相似的可能。

- 字符串的快速查找
- 串排序
- 前缀匹配
- 最长公共前缀问题


## Ref

- [https://zh.wikipedia.org/zh-hans/Trie](https://zh.wikipedia.org/zh-hans/Trie)
- [https://github.com/julycoding/The-Art-Of-Programming-By-July/blob/master/ebook/zh/06.09.md](https://github.com/julycoding/The-Art-Of-Programming-By-July/blob/master/ebook/zh/06.09.md)
- [https://www.jianshu.com/p/6f81da81bd02](https://www.jianshu.com/p/6f81da81bd02)
- [https://segmentfault.com/a/1190000008877595](https://segmentfault.com/a/1190000008877595)
- [https://juejin.im/post/5c2c096251882579717db3d2](https://juejin.im/post/5c2c096251882579717db3d2)
