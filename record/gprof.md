# gprof

> GNU profiler

通过在编译和链接程序的时候（使用 -pg 编译和链接选项），gcc 在你应用程序的每个函数中都加入了一个名为mcount ( or  “_mcount”  , or  “__mcount” , 依赖于编译器或操作系统)的函数，也就是说你的应用程序里的每一个函数都会调用mcount, 而mcount 会在内存中保存一张函数调用图，并通过函数调用堆栈的形式查找子函数和父函数的地址。这张调用图也保存了所有与函数相关的调用时间，调用次数等等的所有信息。

## 使用流程

-在编译和链接时 加上-pg选项。一般我们可以加在 makefile 中
- 执行编译的二进制程序
- 在程序运行目录下 生成 gmon.out 文件。如果原来有gmon.out 文件，将会被重写
- 结束进程。这时 gmon.out 会再次被刷新
- 用 gprof 工具分析 gmon.out 文件


## Ref

- [https://ftp.gnu.org/old-gnu/Manuals/gprof-2.9.1/html_mono/gprof.html](https://ftp.gnu.org/old-gnu/Manuals/gprof-2.9.1/html_mono/gprof.html)
- [https://www.thegeekstuff.com/2012/08/gprof-tutorial](https://www.thegeekstuff.com/2012/08/gprof-tutorial)

