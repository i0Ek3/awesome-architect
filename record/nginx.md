# Nginx

如果要做服务器编程的话，那么 nginx/libevent 等肯定是要了解的，所以今天我们来学一下。

那 Nginx 是啥呢？它是一个多进程模型的轻量级的 Web 服务器，同时也是反向代理服务器，支持负载均衡。

## 惊群现象

Nginx 的惊群现象已经在[这里](https://github.com/i0Ek3/HPS/doc)做了总结，这里不再赘述。

## master-worker 模式

master 进程主要用来管理 worker 进程的，其有四个功能：
 
- 接收来自外界的信号
- 向 worker 发送信号
- 监控 worker 进程的运行状态
- 当 worker 进程异常退出后，自动重新启动新的 worker 进程

而 worker 进程主要是用来处理网络事件的，不同 worker 之间是相互独立的，同等竞争来自客户端的请求。

## 热备份

我们知道 Nginx 通过热备份来升级服务器的，也就是不需要停止服务器去加载配置文件。那它是如何做的呢？

Nginx 的主要工作模式是 master-worker（如上文所述），所以，当我们修改了配置文件 nginx.conf 之后，nginx 会创建新的 worker 进程来处理新的请求，而老的 worker 进程将在执行完当前任务后被杀掉，这样就实现了热备份。



## Reference

- [https://zhuanlan.zhihu.com/p/34943332](https://zhuanlan.zhihu.com/p/34943332)

