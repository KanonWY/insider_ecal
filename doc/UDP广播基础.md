## UDP multicast
#### 1. 组播特点

支持主机间一对多的模式，允许将报文发送到特定的组播地址，然后让与一组主机接收。需要使用`setsockopt`进行设置。

#### 2. 多播地址管理：

| 地址范围                    | 作用                                                         |
| --------------------------- | ------------------------------------------------------------ |
| 224.0.0.0～224.0.0.255      | 为预留的组播地址，只能在局域网中，路由器是不会进行转发的     |
| 224.0.1.0～238.255.255.255  | 用户可用的组播地址，可以用于 Internet 上                     |
| 239.0.0.0 ~ 239.255.255.255 | 本地管理组播地址，可供内部网在内部使用，仅在特定的本地范<br/>围内有效。 |

#### 3. 简单编码

##### 发送端

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    // 1. 创建通信的套接字
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(0);
    }
    
    // 2. 设置组播属性
    struct in_addr opt;
    // 将组播地址初始化到这个结构体成员中即可
    inet_pton(AF_INET, "239.0.1.10", &opt.s_addr);
    setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &opt, sizeof(opt));

    char buf[1024];
    struct sockaddr_in cliaddr;
    int len = sizeof(cliaddr);
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(9999); // 接收端需要绑定9999端口
    // 发送组播消息, 需要使用组播地址, 和设置组播属性使用的组播地址一致就可以
    inet_pton(AF_INET, "239.0.1.10", &cliaddr.sin_addr.s_addr);
    // 3. 通信
    int num = 0;
    while(1)
    {
        sprintf(buf, "hello, client...%d\n", num++);
        // 数据广播
        sendto(fd, buf, strlen(buf)+1, 0, (struct sockaddr*)&cliaddr, len);
        printf("发送的组播的数据: %s\n", buf);
        sleep(1);
    }

    close(fd);

    return 0;
}
```

##### 接收端

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>

int main()
{
    // 1. 创建通信的套接字
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(0);
    }

    // 2. 通信的套接字和本地的IP与端口绑定
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);    // 大端
    addr.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1)
    {
        perror("bind");
        exit(0);
    }

    // 3. 加入到多播组
    struct ip_mreqn opt;
    // 要加入到哪个多播组, 通过组播地址来区分
    inet_pton(AF_INET, "239.0.1.10", &opt.imr_multiaddr.s_addr);
    opt.imr_address.s_addr = INADDR_ANY;
    opt.imr_ifindex = if_nametoindex("ens33");
    setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &opt, sizeof(opt));

    char buf[1024];
    // 3. 通信
    while(1)
    {
        // 接收广播消息
        memset(buf, 0, sizeof(buf));
        // 阻塞等待数据达到
        recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
        printf("接收到的组播消息: %s\n", buf);
    }

    close(fd);

    return 0;
}

```

#### 4. `UDP`一次能够发送的数据大小

- `UDP`本身协议的限制

<img src="http://kanon2020.top/imgs/socket/UDP%E5%A4%B4%E6%96%87%E4%BB%B6.jpg" style="zoom: 25%;" />

包长度是`2^16-1`，去除`UDP头部8字节`，`IP头部20字节`，最大实际可发的数据大小为`65507字节`，大约是`64k`少一点。

```cpp
#define MSG_BUFFER_SIZE   (64*1024 - 20 /* IP header */ - 8 /* UDP header */ - 1
```

- 链路层的限制

无论对于`TCP`还是`UDP`而言，在以太网数据链路层数据帧大小限制是固定的(`1500字节`)。

```bash
物理层的最大传输单元为:Maximum Transmission Unit 1500字节
网络层IP头占据20字节，传输层UDP头占用8字节。
```

`UDP`包中的数据多于`MTU(1472)`时，发送方的`IP层`需要分片进行传输，而在接收方`IP层`则需要进行数据报重组，由于`UDP`是不可靠的传输协议，如果分片丢失导致重组失败，将导致`UDP`数据包被丢弃。

- `UDP`有发送缓冲区和接收缓冲区吗？默认大小是多少？

[发送缓冲区问题](https://www.zhihu.com/question/59628363)





#### 5. UDP丢包情况

##### 5.1 `UDP`的接收缓冲区满

如果socket的接收缓冲区满了，应用层没有及时调用recv函数处理UDP包，后续到达的`UDP`包会被内核丢弃。

##### 5.2 `UDP`接收缓冲区小

如果Client端发送的UDP报文很大，`socket`接收缓冲区无法容纳该UDP报文也会丢失该报文。

##### 5.3 接收端处理过慢

应用层调用`recvfrom`速度过慢，再次调用`recvfrom`系统调用的时候，两次调用之间的UDP包填充满接收缓冲区，导致期间的包丢失。

#####  5.4 发送端发包频率太快

多个包连续发送中间不sleep也可能会导致丢包。可以增大接收缓冲区或者使用`sleep`控制发送频率。

[UDP Drop package](https://zhuanlan.zhihu.com/p/567785046)

[how to get MTU](https://blog.csdn.net/LearnLHC/article/details/115228649)

[UDP解决丢包问题总结](https://blog.csdn.net/c_base_jin/article/details/103229620?spm=1001.2101.3001.6661.1&utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-1-103229620-blog-119478595.pc_relevant_default&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-1-103229620-blog-119478595.pc_relevant_default&utm_relevant_index=1)

[UDP主要丢包原因及具体问题分析](https://www.2cto.com/net/201311/254835.html)

[How to use UDP](http://www.52im.net/thread-1024-1-1.html)

#### 6. ecal的UDP相关参数

1、收发缓冲区大小的调整似乎没有生效。

2、



#### Ref

[UDP多播原理](https://subingwen.cn/linux/multicast/)  
[UDP](https://www.cnblogs.com/jingliming/p/4477264.html)

[UDP分片与丢包](https://zhuanlan.zhihu.com/p/567785046)
