##  Buffer and Cache

[Buffer_and_Cache](https://medium.com/geekculture/linux-memory-buffer-vs-cache-44d8a187f310)

`Buffer `是磁盘的临时存储，即写入磁盘的缓存数据，这样内核就可以集中分散的写入。例如：将多个写入合并为一个。

`Cache`是用于从磁盘读取文件的页面缓存，用于缓存从页面读取的数据。使用了`Cache`之后，下一次从页面中读取数据可以快速的从内存页中获取而无需从磁盘中读取。

`free`中的buffers/cache说明。

```bash
buffers  Memory used by kernel buffers (Buffers in /proc/meminfo)
cache  Memory used by the page cache and slabs (Cached and SReclaimable in /proc/meminfo)
```

`proc`文件系统中的说明。

```cpp
Buffers  Relatively temporary storage for raw disk blocks that shouldn't get tremendously large.
Cached   In-memory cache for files read from the disk (the page cache).  Doesn't include SwapCached.
```

强制内核丢弃部分缓存

```bash
echo 3 > /proc/sys/vm/drop_caches
```

