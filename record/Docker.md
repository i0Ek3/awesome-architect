# Docker

Docker 是一个轻量级的虚拟化技术的容器引擎项目，基于 Go 语言开发。其主要通过运用内核虚拟化技术，如 namespace, cgroups 以及 UFS 来提供容器的资源隔离与安全保障。

Docker 的最初实现是基于 [LXC](https://linuxcontainers.org/lxc/introduction/) 的，后又自行开发了 libcontainer，然后又进一步使用了 runC 和 containerd。其中，runC 是一个 Linux 命令行工具，用于创建和运行容器。containerd 是一个守护程序，用来管理容器的生命周期。


## 整体架构

Docker 的整体架构大概可以分为 Deamon，Client 和 Engine 三部分。

## Docker 构成

### 镜像 Image

类似于我们说的系统盘，包括了必要的软件和资源。

### 容器 Container

而容器可以理解为为上述系统提供的系统硬件环境，容器之间彼此是独立的，容器的启动是基于镜像文件的，即 Dockerfile，该文件为分层结构，下一层是上一层的基础。

### 仓库 Repository

仓库用于存放镜像，类似于 Git，有公有和私有之分。








## References

- [https://docs.docker.com/](https://docs.docker.com/)
- [https://legacy.gitbook.com/book/yeasy/docker_practice/details](https://legacy.gitbook.com/book/yeasy/docker_practice/details)
- [https://www.infoq.cn/article/docker-source-code-analysis-part1](https://www.infoq.cn/article/docker-source-code-analysis-part1)
- [https://docs.docker.com/engine/reference/builder/?spm=5176.8351553.0.0.6eb21991rT7ejF](https://docs.docker.com/engine/reference/builder/?spm=5176.8351553.0.0.6eb21991rT7ejF)
