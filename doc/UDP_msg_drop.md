## UDP_drop_msg

**[REF](https://cloud.tencent.com/developer/article/1638140)**

![UDP收到包](https://ask.qcloudimg.com/http-save/7256485/lvvrsffme9.jpeg?imageView2/2/w/2560/h/7000)

RX-->receive

TX-->transmit

#### 1、如何查看是否有UDP丢包

- ifconfig

```bash
ifconfig 网卡名
➜  netplan ifconfig wlp0s20f3
wlp0s20f3: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.92.240  netmask 255.255.252.0  broadcast 192.168.95.255
        inet6 fe80::5375:cfad:4514:96e  prefixlen 64  scopeid 0x20<link>
        ether f4:7b:09:d5:0c:32  txqueuelen 1000  (以太网)
        RX packets 289345  bytes 298239018 (298.2 MB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 136605  bytes 48270618 (48.2 MB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```

- netstat

```bash
netstat -s -u
IcmpMsg:
    InType3: 429
    InType8: 15
    OutType0: 15
    OutType3: 484
    OutType5: 56
Udp:
    71402 packets received
    484 packets to unknown port received  //表示系统接收到的 UDP 报文所在的目标端口没有应用在监听，一般是服务没有启动导致的
    7516 packet receive errors //不为空，并且在一直增长说明系统有 UDP 丢包
    72682 packets sent
    7516 receive buffer errors	//UDP 的接收缓存太小导致丢包的数量
    0 send buffer errors
    IgnoredMulti: 48
UdpLite:
IpExt:
    InNoRoutes: 51
    InMcastPkts: 560
    OutMcastPkts: 1048
    InBcastPkts: 9429
    OutBcastPkts: 9392
    InOctets: 398595696
    OutOctets: 154669389
    InMcastOctets: 110345
    OutMcastOctets: 202417
    InBcastOctets: 12154495
    OutBcastOctets: 12141347
    InNoECTPkts: 460694
```

#### 2、丢包的环节

- 网卡驱动
- Linux系统丢包
- 应用丢包

