# RPC

> Remote Procedure Call，即远程过程调用，是一种进程间通信方式。由 Bruce Jay Nelson 在论文 《Implementing Remote Procedure Calls》中提出。

RPC 是指计算机 A 上的进程，调用另外一台计算机 B 上的进程，其中 A 上的调用进程被挂起，而 B 上的被调用进程开始执行，当值返回给 A 时，A 进程继续执行。

远程过程调用采用客户机/服务器(C/S)模式。请求程序就是一个客户机，而服务提供程序就是一台服务器。和常规或本地过程调用一样，远程过程调用是同步操作，在远程过程结果返回之前，需要暂时中止请求程序。使用相同地址空间的低权进程或低权线程允许同时运行多个远程过程调用。

## 调用方式

- 同步调用：客户端等待调用执行完成并获取到执行结果。
- 异步调用：客户端调用后不用等待执行结果返回，但依然可以通过回调通知等方式获取返回结果。若客户端不关心调用返回结果，则变成单向异步调用，单向调用不用返回结果。

## coming soon...


## Ref

- [https://waylau.com/remote-procedure-calls/](https://waylau.com/remote-procedure-calls/)

