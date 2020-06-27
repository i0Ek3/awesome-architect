# Linux 命令

简单的就不列出来了，大家自行探索，这里列出一些我自己还很模糊的。

## netstat

```Shell
netstat -at  // list all tcp ports
netstat -au  // list all udp ports
netstat -an
```

## tcpdump

```Shell
tcpdump -i eth1 // 监听指定网络接口的数据包
sudo tcpdump -i any port 80 -A  // 抓去 80 端口 HTTP 报文，以文本形式显示
```

## ipcs/ipcrm

```Shell
ipcs -a
ipcrm -m xxx  // 删除 id 为 xxx 的共享内存
```

## IO 状态查看

```Shell
iostat
```

## awk/gawk

``` Shell
// Usage: awk action filename
awk '{print $0}' test.txt 
awk '{print $1 "\t" $3}' test.txt
awk -F ':' '{print $1}' /etc/passwd | tail -n 10 // 打印 /etc/passwd 中第一个字段的倒数 10 行
```


## 查看 CPU

```
cat /proc/cpuinfo
```

## 查看系统负载均衡

```
w
uptime
htop or top
    - VIRT 虚拟内存用量
    - RES 物理内存用量
    - SHR 共享内存用量
    - %MEM 内存用量
```

## 虚拟机状态？

```
vmstat
```

## 查看当前系统进程

```
ps -aux
ps -elf
    - S 表示正在休眠
    - s表示主进程
ps -elf | grep 'keyword'
```

## 查看系统开启的端口

```
netstat -lnp
```

## 查看网络连接情况

```
netstat -an
```

## 查看最近登陆情况

```
last
```

## 查看内核日志信息

```
dmesg
```

## 查看当前服务器的运行级别

```
who -r
runlevel
```

## 查看 PCI 总线和附加设备的信息

```
lspci -r
lspci -v
```

## rsync 的两种同步方式有什么不同

```
rsync -av /test/ ip:/test/    // 使用 ssh 方式同步
rsync -av /test/ ip::test     // 使用 rsync 服务的方式同步
```

## 某个账号登陆linux后，系统会在哪些日志文件中记录相关信息

```
/var/log/secure
/var/log/wtmp
```


## Ref

- [https://segmentfault.com/a/1190000002547332](https://segmentfault.com/a/1190000002547332)
- [https://zhuanlan.zhihu.com/p/32250942](https://zhuanlan.zhihu.com/p/32250942)
