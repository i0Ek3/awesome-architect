# MapReduce

> 以下内容均整理自网络，并非原创，侵删。


> Wiki: MapReduce是Google提出的一个软件架构，用于大规模数据集（大于1TB）的并行运算。当前的软件实现是指定一个Map（映射）函数，用来把一组键值对映射成一组新的键值对，指定并发的Reduce（归纳）函数，用来保证所有映射的键值对中的每一个共享相同的键组。

通常来说，每当你听到“大数据”，那也许意味着Hadoop被用来存储数据，也通常意味着数据的抽取和检索是用的MapReduce。

## 基本原理

MapReduce合并了两种经典函数：

- 映射（Mapping）对集合里的每个目标应用同一个操作。即，如果你想把表单里每个单元格乘以二，那么把这个函数单独地应用在每个单元格上的操作就属于mapping。
- 聚合（或者归纳）（Reducing ）遍历集合中的元素来返回一个综合的结果。即，输出表单里一列数字的和这个任务属于reducing。

请参考下图：

![1](https://github.com/i0Ek3/awesome-architect/blob/master/pics/mapreduce1.png)

![2](https://github.com/i0Ek3/awesome-architect/blob/master/pics/mapreduce2.png)


MapReduce算法的机制要远比这复杂得多，但是主体思想是一致的，即通过分散计算来分析大量数据。其实际工作流程为：Map -> Shuffle -> Reduce。


网络上流传的更为简单的解释：

> 我们要数图书馆中的所有书。你数1号书架，我数2号书架。这就是“Map”。我们人越多，数书就更快。现在我们到一起，把所有人的统计数加在一起。这就是“Reduce”。

## Ref

- [http://blog.jobbole.com/1321/](http://blog.jobbole.com/1321/)
- [https://zh.wikipedia.org/zh-cn/MapReduce](https://zh.wikipedia.org/zh-cn/MapReduce)
- [https://en.cppreference.com/w/cpp/algorithm/reduce](https://en.cppreference.com/w/cpp/algorithm/reduce)
- [http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3446.pdf](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3446.pdf)
- [https://github.com/google/mr4c](https://github.com/google/mr4c)
- [https://hadoop.apache.org/docs/r1.2.1/mapred_tutorial.html](https://hadoop.apache.org/docs/r1.2.1/mapred_tutorial.html)
- [http://blog.jobbole.com/79255/](http://blog.jobbole.com/79255/)

