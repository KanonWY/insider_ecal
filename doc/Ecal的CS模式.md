## servcie
本质上是使用tcp实现的。  


ping_server 8 thread！

0. 主线程

1. 注册发送线程
```cpp
//循环执行函数
int CRegistrationProvider::RegisterSendThread()
{
    //....
}
```

2. 接收数据
```cpp
CSampleReceiver::Receive()
{
     // wait for any incoming message
    if(recv_len > 0)
    {
        return (Process(m_msg_buffer.data(), recv_len));
    }
}
```
3. 数据读取层线程池
对于service的cleint端口，没有开启UDP多播，仅仅开启了tcp读取层。
```cpp
  void CTCPReaderLayer::Initialize()
  {
    tcp_pubsub::logger::logger_t tcp_pubsub_logger = std::bind(TcpPubsubLogger, std::placeholders::_1, std::placeholders::_2);
    m_executor = std::make_shared<tcp_pubsub::Executor>(Config::GetTcpPubsubReaderThreadpoolSize(), tcp_pubsub_logger);
  }
会创建多个线程执行io_service->Run()
```
3. TCP读取层初始化线程池 4
```cpp
  void CTCPReaderLayer::Initialize()
  {
    tcp_pubsub::logger::logger_t tcp_pubsub_logger = std::bind(TcpPubsubLogger, std::placeholders::_1, std::placeholders::_2);
    m_executor = std::make_shared<tcp_pubsub::Executor>(Config::GetTcpPubsubReaderThreadpoolSize(), tcp_pubsub_logger);
  }

## cgdb
(gdb) p thread_count
4
```

4. 启动超时线程
```cpp
  int CSubGate::CheckTimeouts()
  {
    if (!m_created) return(0);

    // check subscriber timeouts
    std::shared_lock<std::shared_timed_mutex> lock(m_topic_name_datareader_sync);
    for (auto iter = m_topic_name_datareader_map.begin(); iter != m_topic_name_datareader_map.end(); ++iter)
    {
      iter->second->CheckReceiveTimeout();
    }

    // signal shutdown if eCAL is not okay
    bool ecal_is_ok = (g_globals_ctx != nullptr) && !gWaitForEvent(ShutdownProcEvent(), 0);
    if (!ecal_is_ok)
    {
      g_shutdown = 1;
    }

    return(0);
  }
```
### clientservice没有绑定任何端口，也不知道服务器信息，它是如何与目标进程建立连接的？


### 编译选项的问题
默认不使用冰狮的共享内存，如果想开启，可以在cmake中编译添加。


### service启动的线程
- registration_provider_instance
- registration_receiver_instance
- CSubGate   ----> TCP Executor/UDP Executor 本质是两个线程池. 默认是4个。 线程池的数量从Config::GetTcpPubsubReaderThreadpoolSize()中读取。


### TODO:改进没有设置线程的名字，调试的时候不方便。

1. 基于策略
[Policy](http://wiki.ros.org/message_filters#Policy-Based_Synchronizer_.5BROS_1.1.2B-.5D)

2. 学习ASIO,UDP部分


