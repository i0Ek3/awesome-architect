/*
    这是一个示例性质的libevent的程序，监听在TCP的9995端口。
    当连接建立成功后，它将会给Client回应一个消息"Hello, World!\n"
    发送完毕后就将连接关闭。

    程序也处理了SIGINT (ctrl-c)信号，收到这个信号后优雅退出程序。

    这个程序也用到了一些libevent比较高级的API：“bufferevent”
    这套API将buffer的“水位线”也抽象成了event来处理，灵感应该是来自
    Windows平台的IOCP。
*/

// 引入常用Linux系统头文件
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// 引入libevent 2.x相关的头文件
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

// 定义字符串常量，将会回应给Client用
static const char MESSAGE[] = "Hello, World!\n";

// server监听的端口
static const int PORT = 9995;

// 定义几个event callback的prototype（原型）
static void listener_cb(struct evconnlistener * , evutil_socket_t,
    struct sockaddr * , int socklen, void * );
static void conn_writecb(struct bufferevent * , void * );
static void conn_eventcb(struct bufferevent * , short, void * );
static void signal_cb(evutil_socket_t, short, void * );

// 定义标准的main函数
int main(int argc, char ** argv)
{
    // event_base是整个event循环必要的结构体
    struct event_base * base;
    // libevent的高级API专为监听的FD使用
    struct evconnlistener * listener;
    // 信号处理event指针
    struct event * signal_event;
    // 保存监听地址和端口的结构体
    struct sockaddr_in sin;

    // 分配并初始化event_base
    base = event_base_new();
    if (!base) {
        // 如果发生任何错误，向stderr（标准错误输出）打一条日志，退出
        // 在C语言里，很多返回指针的API都以返回null为出错的返回值
        // if (!base) 等价于 if (base == null)
        fprintf(stderr, "Could not initialize libevent!\n");
        return 1;
    }

    // 初始化sockaddr_in结构体，监听在0.0.0.0:9995
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);

    // bind在上面制定的IP和端口，同时初始化listen的事件循环和callback：listener_cb
    // 并把listener的事件循环注册在event_base：base上
    listener = evconnlistener_new_bind(base, listener_cb, (void * )base,
        LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
        (struct sockaddr*)&sin,
        sizeof(sin));

    if (!listener) {
        // 如果发生任何错误，向stderr（标准错误输出）打一条日志，退出
        fprintf(stderr, "Could not create a listener!\n");
        return 1;
    }

    // 初始化信号处理event
    signal_event = evsignal_new(base, SIGINT, signal_cb, (void * )base);

    // 把这个callback放入base中
    if (!signal_event || event_add(signal_event, NULL)<0) {
        fprintf(stderr, "Could not create/add a signal event!\n");
        return 1;
    }

    // 程序将在下面这一行内启动event循环，只有在调用event_base_loopexit后
    // 才会从下面这个函数返回，并向下执行各种清理函数，导致整个程序退出
    event_base_dispatch(base);

    // 各种清理free
    evconnlistener_free(listener);
    event_free(signal_event);
    event_base_free(base);

    printf("done\n");
    return 0;
}

// 监听端口的event callback
static void
listener_cb(struct evconnlistener * listener, evutil_socket_t fd,
    struct sockaddr * sa, int socklen, void * user_data)
{
    struct event_base * base = user_data;
    struct bufferevent * bev;

    // 新建一个bufferevent，设定BEV_OPT_CLOSE_ON_FREE，
    // 保证bufferevent被free的时候fd也会被关闭
    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        fprintf(stderr, "Error constructing bufferevent!");
        event_base_loopbreak(base);
        return;
    }
    // 设定写buffer的event和其它event
    bufferevent_setcb(bev, NULL, conn_writecb, conn_eventcb, NULL);
    // 开启向fd中写的event
    bufferevent_enable(bev, EV_WRITE);
    // 关闭从fd中读写入buffer的event
    bufferevent_disable(bev, EV_READ);
    // 向buffer中写入"Hello, World!\n"
    // 上面的操作保证在fd可写时，将buffer中的内容写出去
    bufferevent_write(bev, MESSAGE, strlen(MESSAGE));
}


// 每次fd可写，数据非阻塞写入后，会雕也难怪conn_writecb
// 这个函数每次检查eventbuffer的剩余大小，如果为0
// 表示数据已经全部写完，将eventbuffer free掉
// 由于在上面设定了BEV_OPT_CLOSE_ON_FREE，所以fd也会被关闭
static void
conn_writecb(struct bufferevent * bev, void * user_data)
{
    struct evbuffer * output = bufferevent_get_output(bev);
    if (evbuffer_get_length(output) == 0) {
        printf("flushed answer\n");
        bufferevent_free(bev);
    }
}

// 处理读、写event之外的event的callback
static void
conn_eventcb(struct bufferevent * bev, short events, void * user_data)
{
    if (events & BEV_EVENT_EOF) {
        // Client端关闭连接
        printf("Connection closed.\n");
    } else if (events & BEV_EVENT_ERROR) {
        // 连接出错
        printf("Got an error on the connection: %s\n",
            strerror(errno));
    }
    // 如果还有其它的event没有处理，那就关闭这个bufferevent
    bufferevent_free(bev);
}

// 信号处理event，收到SIGINT (ctrl-c)信号后，延迟2s退出event循环
static void
signal_cb(evutil_socket_t sig, short events, void * user_data)
{
    struct event_base * base = user_data;
    struct timeval delay = { 2, 0 };

    printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

    event_base_loopexit(base, &delay);
}
