# TopK

> 以下内容均整理自网络，并非原创，侵删。

面试中会经常问到TopK的问题，今天我们就来学习一下。

TopK通常的问法是找出海量数据里前n条数据(即热点数据)，通常解法为：

第一步对query进行统计：

- 直接排序
- Hash Table

第二步对统计好的query进行排序，找出前n条数据：

- 普通排序 
    
时间复杂度：O(n\*lgn)，将n个数排序后，取出最大的K个。

- 局部排序 

时间复杂度：O(n\*k)，即只对最大的K个排序。

- 堆 

时间复杂度：O(n\*lgK)，只找到TopK，而不排序TopK。

- 随机选择 

时间复杂度：O(n)
    
> 分治法 

把一个大的问题，转化为若干个子问题，每个子问题“都”解决，大的问题便随之解决，如快速排序，时间复杂度：O(n\*lgn)。
    
> 减治法（分治法的特例）

把一个大的问题，转化为若干个子问题，这些子问题中“只”解决一个，大的问题便随之解决，如二分查找，时间复杂度：O(lgn)；随机选择，时间复杂度：O(n)。

- 随机选择 + partition







## Ref

- [https://mp.weixin.qq.com/s/FFsvWXiaZK96PtUg-mmtEw](https://mp.weixin.qq.com/s/FFsvWXiaZK96PtUg-mmtEw)
- [https://my.oschina.net/7001/blog/1633536](https://my.oschina.net/7001/blog/1633536)
- [https://cloud.tencent.com/developer/article/1071227](https://cloud.tencent.com/developer/article/1071227)


