# KMP

> 以下内容均整理自网络，侵删。

KMP: Knuth-Morris-Pratt algorithm。KMP 算法用于在一个文本串 s 内查找模式串 p 出现的位置。

## 算法流程

假设文本串 S 匹配到 i 位置，模式串 P 匹配到 j 位置，则有以下步骤：

-  j = -1，或者当前字符匹配成功（即 S[i] == P[j]），都令 i++，j++，继续匹配下一个字符；
- 如果 j != -1，且当前字符匹配失败（即 S[i] != P[j]），则令 i 不变，j = next[j]。此举意味着失配时，模式串 P 相对于文本串 S 向右移动了 j - next [j] 位；
> 是不是感觉有点二分查找和快排的感觉？

其中，next [j] = k 表示为 j 之前的字符串中有最大长度为 k 的相同前缀后缀

## 代码

```C
int KMP(char* s, char* p) {
    int i = 0;
    int j = 0;
    int len_s = strlen(s);
    int len_p = strlen(p);

    while (i < len_s && j < len_p) {
        if (j == -1 ||s[i] == p[j]) {
            i++;
            j++;
        } else {
            j = next[j];
        }
    }
    if (j = len_p) return i - j;
    else return -1;
}

void next(char* p, int* next) {
    int len_p = strlen(p);
    next[0] = -1;
    int k = -1;
    int j = 0;
    while (j < len_p - 1) {
        if (k == -1 || p[j] == p[k]) { // p[k]表示前缀，p[j]表示后缀
            ++j;
            ++k;
            if (p[j] != p[k]) next[j] = k;
            else next[j] = next[k];
        } else {
            k = next[k];
        }
    }
}
```
## 扩展算法

- BM
- Sunday


## Ref

- [http://wiki.jikexueyuan.com/index.php/project/kmp-algorithm/define.html](http://wiki.jikexueyuan.com/index.php/project/kmp-algorithm/define.html)

