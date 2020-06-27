# Redis 源码学习

> 以下内容整理自网络，侵删。

## 如何阅读源码？

参考黄健宏老师的解答： [http://blog.huangz.me/diary/2014/how-to-read-redis-source-code.html](http://blog.huangz.me/diary/2014/how-to-read-redis-source-code.html)。

- 阅读数据结构实现
- 阅读内存编码数据结构实现
- 阅读数据类型实现
- 阅读数据库实现相关代码
- 阅读客户端和服务器的相关代码
- 阅读多机功能的实现

## 最新 Redis src 目录结构

```
.
├── Makefile
├── Makefile.dep
├── adlist.c
├── adlist.h
├── ae.c
├── ae.h
├── ae_epoll.c
├── ae_evport.c
├── ae_kqueue.c
├── ae_select.c
├── anet.c
├── anet.h
├── aof.c
├── asciilogo.h
├── atomicvar.h
├── bio.c
├── bio.h
├── bitops.c
├── blocked.c
├── childinfo.c
├── cluster.c
├── cluster.h
├── config.c
├── config.h
├── crc16.c
├── crc64.c
├── crc64.h
├── db.c
├── debug.c
├── debugmacro.h
├── defrag.c
├── dict.c
├── dict.h
├── endianconv.c
├── endianconv.h
├── evict.c
├── expire.c
├── fmacros.h
├── geo.c
├── geo.h
├── geohash.c
├── geohash.h
├── geohash_helper.c
├── geohash_helper.h
├── help.h
├── hyperloglog.c
├── intset.c
├── intset.h
├── latency.c
├── latency.h
├── lazyfree.c
├── listpack.c
├── listpack.h
├── listpack_malloc.h
├── localtime.c
├── lolwut.c
├── lolwut5.c
├── lzf.h
├── lzfP.h
├── lzf_c.c
├── lzf_d.c
├── memtest.c
├── mkreleasehdr.sh
├── module.c
├── modules
├── multi.c
├── networking.c
├── notify.c
├── object.c
├── pqsort.c
├── pqsort.h
├── pubsub.c
├── quicklist.c
├── quicklist.h
├── rand.c
├── rand.h
├── rax.c
├── rax.h
├── rax_malloc.h
├── rdb.c
├── rdb.h
├── redis-benchmark
├── redis-benchmark.c
├── redis-check-aof
├── redis-check-aof.c
├── redis-check-rdb
├── redis-check-rdb.c
├── redis-cli
├── redis-cli.c
├── redis-sentinel
├── redis-server
├── redis-trib.rb
├── redisassert.h
├── redismodule.h
├── release.c
├── release.h
├── replication.c
├── rio.c
├── rio.h
├── scripting.c
├── sds.c
├── sds.h
├── sdsalloc.h
├── sentinel.c
├── server.c
├── server.h
├── setproctitle.c
├── sha1.c
├── sha1.h
├── siphash.c
├── slowlog.c
├── slowlog.h
├── solarisfixes.h
├── sort.c
├── sparkline.c
├── sparkline.h
├── stream.h
├── syncio.c
├── t_hash.c
├── t_list.c
├── t_set.c
├── t_stream.c
├── t_string.c
├── t_zset.c
├── testhelp.h
├── util.c
├── util.h
├── valgrind.sup
├── version.h
├── ziplist.c
├── ziplist.h
├── zipmap.c
├── zipmap.h
├── zmalloc.c
└── zmalloc.h

```

## 部分源码

### dict

```C++
// dict.h

#include <stdint.h>

#ifndef __DICT_H
#define __DICT_H

// 定义了成功与错误的值  
#define DICT_OK 0
#define DICT_ERR 1

// Unused arguments generate annoying warnings...  
// dict没有用到时，用来提示警告的  
#define DICT_NOTUSED(V) ((void) V)

// 字典结构体，保存K-V值的结构体  
typedef struct dictEntry {
	//字典key函数指针
    void *key;
    union {
        void *val;
        //无符号整型值
        uint64_t u64;
        //有符号整型值
        int64_t s64;
        double d;
    } v;
    //下一字典结点
    struct dictEntry *next;
} dictEntry;

// 字典类型  
typedef struct dictType {
	//哈希计算方法，返回整形变量
    unsigned int (*hashFunction)(const void *key);
    //复制key方法
    void *(*keyDup)(void *privdata, const void *key);
    //复制val方法
    void *(*valDup)(void *privdata, const void *obj);
    //key值比较方法
    int (*keyCompare)(void *privdata, const void *key1, const void *key2);
    //key的析构函数
    void (*keyDestructor)(void *privdata, void *key);
    //val的析构函数
    void (*valDestructor)(void *privdata, void *obj);
} dictType;

// This is our hash table structure. Every dictionary has two of this as we
 * implement incremental rehashing, for the old to the new table.  
// 哈希表结构体  
typedef struct dictht {
	//字典实体
    dictEntry **table;
    //表格可容纳字典数量
    unsigned long size;
    unsigned long sizemask;
    //正在被使用的数量
    unsigned long used;
} dictht;

// 字典主操作类  
typedef struct dict {
	//字典类型
    dictType *type;
    //私有数据指针
    void *privdata;
    //字典哈希表，共2张，一张旧的，一张新的
    dictht ht[2];
    //重定位哈希时的下标
    long rehashidx; // rehashing not in progress if rehashidx == -1  
    //当前迭代器数量
    int iterators; // number of iterators currently running  
} dict;

// If safe is set to 1 this is a safe iterator, that means, you can call
 * dictAdd, dictFind, and other functions against the dictionary even while
 * iterating. Otherwise it is a non safe iterator, and only dictNext()
 * should be called while iterating.  
// 字典迭代器，如果是安全迭代器，这safe设置为1，可以调用dicAdd，dictFind  
// 如果是不安全的，则只能调用dicNext方法 
typedef struct dictIterator {
	//当前字典
    dict *d;
    //下标
    long index;
    //表格，和安全值的表格代表的是旧的表格，还是新的表格
    int table, safe;
    //字典实体
    dictEntry *entry, *nextEntry;
    // unsafe iterator fingerprint for misuse detection.  
    // 指纹标记，避免不安全的迭代器滥用现象  
    long long fingerprint;
} dictIterator;

// 字典扫描方法  
typedef void (dictScanFunction)(void *privdata, const dictEntry *de);

// This is the initial size of every hash table  
// 初始化哈希表的数目  
#define DICT_HT_INITIAL_SIZE     4

// ------------------------------- Macros ------------------------------------ 
// 字典释放val函数时候调用，如果dict中的dictType定义了这个函数指针，  
#define dictFreeVal(d, entry) \
    if ((d)->type->valDestructor) \
        (d)->type->valDestructor((d)->privdata, (entry)->v.val)

// 字典val函数复制时候调用，如果dict中的dictType定义了这个函数指针，  
#define dictSetVal(d, entry, _val_) do { \
    if ((d)->type->valDup) \
        entry->v.val = (d)->type->valDup((d)->privdata, _val_); \
    else \
        entry->v.val = (_val_); \
} while(0)

// 设置dictEntry中共用体v中有符号类型的值  
#define dictSetSignedIntegerVal(entry, _val_) \
    do { entry->v.s64 = _val_; } while(0)

// 设置dictEntry中共用体v中无符号类型的值  
#define dictSetUnsignedIntegerVal(entry, _val_) \
    do { entry->v.u64 = _val_; } while(0)

// 设置dictEntry中共用体v中double类型的值  
#define dictSetDoubleVal(entry, _val_) \
    do { entry->v.d = _val_; } while(0)

// 调用dictType定义的key析构函数  
#define dictFreeKey(d, entry) \
    if ((d)->type->keyDestructor) \
        (d)->type->keyDestructor((d)->privdata, (entry)->key)

// 调用dictType定义的key复制函数，没有定义直接赋值  
#define dictSetKey(d, entry, _key_) do { \
    if ((d)->type->keyDup) \
        entry->key = (d)->type->keyDup((d)->privdata, _key_); \
    else \
        entry->key = (_key_); \
} while(0)

// 调用dictType定义的key比较函数，没有定义直接key值直接比较  
#define dictCompareKeys(d, key1, key2) \
    (((d)->type->keyCompare) ? \
        (d)->type->keyCompare((d)->privdata, key1, key2) : \
        (key1) == (key2))

#define dictHashKey(d, key) (d)->type->hashFunction(key)   //哈希定位方法
#define dictGetKey(he) ((he)->key)    //获取dictEntry的key值
#define dictGetVal(he) ((he)->v.val)  //获取dicEntry中共用体v中定义的val值
#define dictGetSignedIntegerVal(he) ((he)->v.s64) //获取dicEntry中共用体v中定义的有符号值
#define dictGetUnsignedIntegerVal(he) ((he)->v.u64)  //获取dicEntry中共用体v中定义的无符号值
#define dictGetDoubleVal(he) ((he)->v.d)  //获取dicEntry中共用体v中定义的double类型值
#define dictSlots(d) ((d)->ht[0].size+(d)->ht[1].size)  //获取dict字典中总的表大小
#define dictSize(d) ((d)->ht[0].used+(d)->ht[1].used)   //获取dict字典中总的表的总正在被使用的数量
#define dictIsRehashing(d) ((d)->rehashidx != -1)   //字典有无被重定位过

// API  
dict *dictCreate(dictType *type, void *privDataPtr);   //创建dict字典总类
int dictExpand(dict *d, unsigned long size);    //字典扩增方法
int dictAdd(dict *d, void *key, void *val);    //字典根据key, val添加一个字典集
dictEntry *dictAddRaw(dict *d, void *key);     //字典添加一个只有key值的dicEntry
int dictReplace(dict *d, void *key, void *val); //替代dict中一个字典集
dictEntry *dictReplaceRaw(dict *d, void *key);  //替代dict中的一个字典，只提供一个key值
int dictDelete(dict *d, const void *key);    //根据key删除一个字典集
int dictDeleteNoFree(dict *d, const void *key);  //字典集删除无、不调用free方法
void dictRelease(dict *d);   //释放整个dict
dictEntry * dictFind(dict *d, const void *key);  //根据key寻找字典集
void *dictFetchValue(dict *d, const void *key);  //根据key值寻找相应的val值
int dictResize(dict *d);  //重新计算大小
dictIterator *dictGetIterator(dict *d); //获取字典迭代器
dictIterator *dictGetSafeIterator(dict *d);  //获取字典安全迭代器
dictEntry *dictNext(dictIterator *iter);   //根据字典迭代器获取字典集的下一字典集
void dictReleaseIterator(dictIterator *iter); //释放迭代器
dictEntry *dictGetRandomKey(dict *d);  //随机获取一个字典集
void dictPrintStats(dict *d);  //打印当前字典状态
unsigned int dictGenHashFunction(const void *key, int len); //输入的key值，目标长度，此方法帮你计算出索引值
unsigned int dictGenCaseHashFunction(const unsigned char *buf, int len); //这里提供了一种比较简单的哈希算法
void dictEmpty(dict *d, void(callback)(void*)); //清空字典
void dictEnableResize(void);  //启用调整方法
void dictDisableResize(void); //禁用调整方法
int dictRehash(dict *d, int n); //hash重定位，主要从旧的表映射到新表中,分n轮定位
int dictRehashMilliseconds(dict *d, int ms);  //在给定时间内，循环执行哈希重定位
void dictSetHashFunctionSeed(unsigned int initval); //设置哈希方法种子
unsigned int dictGetHashFunctionSeed(void);  //获取哈希种子
unsigned long dictScan(dict *d, unsigned long v, dictScanFunction *fn, void *privdata); //字典扫描方法

// Hash table types  
// 哈希表类型   
extern dictType dictTypeHeapStringCopyKey;
extern dictType dictTypeHeapStrings;
extern dictType dictTypeHeapStringCopyKeyValue;

#endif // __DICT_H  

// dict.c
#include "fmacros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <sys/time.h>
#include <ctype.h>

#include "dict.h"
#include "zmalloc.h"
#include "redisassert.h"

// Using dictEnableResize() / dictDisableResize() we make possible to
 * enable/disable resizing of the hash table as needed. This is very important
 * for Redis, as we use copy-on-write and don't want to move too much memory
 * around when there is a child performing saving operations.
 *
 * Note that even when dict_can_resize is set to 0, not all resizes are
 * prevented: a hash table is still allowed to grow if the ratio between
 * the number of elements and the buckets > dict_force_resize_ratio.  
// redis用了dictEnableResize() / dictDisableResize()方法可以重新调整哈希表的长度，
 *因为redis采用的是写时复制的算法，不会挪动太多的内存，只有当调整数量大于一定比例才可能有效  
static int dict_can_resize = 1;
static unsigned int dict_force_resize_ratio = 5;

// -------------------------- private prototypes ----------------------------  
// 私有方法  
static int _dictExpandIfNeeded(dict *ht);    //字典是否需要扩展
static unsigned long _dictNextPower(unsigned long size);
static int _dictKeyIndex(dict *ht, const void *key);
static int _dictInit(dict *ht, dictType *type, void *privDataPtr);  //字典初始化方法

// -------------------------- hash functions --------------------------------  
// 哈希索引计算的方法  

// Thomas Wang's 32 bit Mix Function  
// Thomas Wang's 32 bit Mix 的哈希算法直接输入key值，获取索引值，据说这种冲突的概率很低  
unsigned int dictIntHashFunction(unsigned int key)
{
    key += ~(key << 15);
    key ^=  (key >> 10);
    key +=  (key << 3);
    key ^=  (key >> 6);
    key += ~(key << 11);
    key ^=  (key >> 16);
    return key;
}

//哈希方法种子，跟产生随机数的种子作用应该是一样的
static uint32_t dict_hash_function_seed = 5381;

// 重设哈希种子  
void dictSetHashFunctionSeed(uint32_t seed) {
    dict_hash_function_seed = seed;
}

// 获取哈希种子  
uint32_t dictGetHashFunctionSeed(void) {
    return dict_hash_function_seed;
}

// MurmurHash2, by Austin Appleby
 * Note - This code makes a few assumptions about how your machine behaves -
 * 1. We can read a 4-byte value from any address without crashing
 * 2. sizeof(int) == 4
 *
 * And it has a few limitations -
 *
 * 1. It will not work incrementally.
 * 2. It will not produce the same results on little-endian and big-endian
 *    machines.
  
// 输入的key值，目标长度，此方法帮你计算出索引值，此方法特别表明，
 *	不会因为机器之间高低位存储的不同而产生相同的结果  
unsigned int dictGenHashFunction(const void *key, int len) {
    // 'm' and 'r' are mixing constants generated offline.
     They're not really 'magic', they just happen to work well.   
    //seed种子，m，r的值都将会参与到计算中
    uint32_t seed = dict_hash_function_seed;
    const uint32_t m = 0x5bd1e995;
    const int r = 24;

    // Initialize the hash to a 'random' value  
    uint32_t h = seed ^ len;

    // Mix 4 bytes at a time into the hash  
    const unsigned char *data = (const unsigned char *)key;

    while(len >= 4) {
        uint32_t k = *(uint32_t*)data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    // Handle the last few bytes of the input array   
    switch(len) {
    case 3: h ^= data[2] << 16;
    case 2: h ^= data[1] << 8;
    case 1: h ^= data[0]; h *= m;
    };

    // Do a few final mixes of the hash to ensure the last few
     * bytes are well-incorporated.  
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return (unsigned int)h;
}

// And a case insensitive hash function (based on djb hash)  
// 这里提供了一种比较简单的哈希算法  
unsigned int dictGenCaseHashFunction(const unsigned char *buf, int len) {
	//以djb hash为基础，俗称“times33”就是不断的乘33
	//几乎所有的流行的hash map都采用了DJB hash function
    unsigned int hash = (unsigned int)dict_hash_function_seed;

    while (len--)
        hash = ((hash << 5) + hash) + (tolower(*buf++)); // hash * 33 + c  
    return hash;
}

// ----------------------------- API implementation -------------------------  

// Reset a hash table already initialized with ht_init().
 * NOTE: This function should only be called by ht_destroy().  
// 重置哈希表方法，只在ht_destroy时使用  
static void _dictReset(dictht *ht)
{
	//清空相应的变量，ht->table的类型其实是dictEntry，叫table名字太有歧义了
    ht->table = NULL;
    ht->size = 0;
    ht->sizemask = 0;
    ht->used = 0;
}

// Create a new hash table  
// 创建dict操作类  
dict *dictCreate(dictType *type,
        void *privDataPtr)
{
    dict *d = zmalloc(sizeof(*d));

	//创建好空间之后调用初始化方法
    _dictInit(d,type,privDataPtr);
    return d;
}

// Initialize the hash table  
// 初始化dict类中的type，ht等变量  
int _dictInit(dict *d, dictType *type,
        void *privDataPtr)
{
	//重置2个ht哈希表
    _dictReset(&d->ht[0]);
    _dictReset(&d->ht[1]);
    //赋值dictType
    d->type = type;
    d->privdata = privDataPtr;
    //-1代表还没有rehash过，
    d->rehashidx = -1;
    //当前使用中的迭代器为0
    d->iterators = 0;

    //返回DICT_OK，代表初始化成功
    return DICT_OK;
}

// Resize the table to the minimal size that contains all the elements,
 * but with the invariant of a USED/BUCKETS ratio near to <= 1  
// 调整哈希表，用最少的值容纳所有的字典集合  
int dictResize(dict *d)
{
    int minimal;

	//如果系统默认调整值不大于0或已经调rehash过的就提示出错，拒绝操作
    if (!dict_can_resize || dictIsRehashing(d)) return DICT_ERR;

    //最少数等于哈希标准鸿正在使用的数
    minimal = d->ht[0].used;
    if (minimal < DICT_HT_INITIAL_SIZE)
        minimal = DICT_HT_INITIAL_SIZE;

    //调用expand扩容
    return dictExpand(d, minimal);
}

// Expand or create the hash table  
// 哈希表扩增方法  
int dictExpand(dict *d, unsigned long size)
{
    dictht n; // the new hash table  
    //获取调整值，以2的幂次向上取
    unsigned long realsize = _dictNextPower(size);

    // the size is invalid if it is smaller than the number of
     * elements already inside the hash table  
     //再次判断数量符合不符合
    if (dictIsRehashing(d) || d->ht[0].used > size)
        return DICT_ERR;

    // Allocate the new hash table and initialize all pointers to NULL  
    //初始化大小
    n.size = realsize;
    n.sizemask = realsize-1;
    //为表格申请realsize个字典集的大小
    n.table = zcalloc(realsize*sizeof(dictEntry*));
    n.used = 0;

    // Is this the first initialization? If so it's not really a rehashing
     * we just set the first hash table so that it can accept keys.  
    if (d->ht[0].table == NULL) {
        d->ht[0] = n;
        return DICT_OK;
    }

    // Prepare a second hash table for incremental rehashing  
   	//赋值给第二张表格
    d->ht[1] = n;
    d->rehashidx = 0;
    return DICT_OK;
}

// Performs N steps of incremental rehashing. Returns 1 if there are still
 * keys to move from the old to the new hash table, otherwise 0 is returned.
 * Note that a rehashing step consists in moving a bucket (that may have more
 * than one key as we use chaining) from the old to the new hash table.  
// hash重定位，主要从旧的表映射到新表中
 * 如果返回1说明旧的表中还存在key迁移到新表中，0代表没有  
int dictRehash(dict *d, int n) {
    if (!dictIsRehashing(d)) return 0;

	// 根据参数分n步多次循环操作  
    while(n--) {
        dictEntry *de, *nextde;

        // Check if we already rehashed the whole table...  
        if (d->ht[0].used == 0) {
            zfree(d->ht[0].table);
            d->ht[0] = d->ht[1];
            _dictReset(&d->ht[1]);
            d->rehashidx = -1;
            return 0;
        }

        // Note that rehashidx can't overflow as we are sure there are more
         * elements because ht[0].used != 0  
        assert(d->ht[0].size > (unsigned long)d->rehashidx);
        while(d->ht[0].table[d->rehashidx] == NULL) d->rehashidx++;
        de = d->ht[0].table[d->rehashidx];
        // Move all the keys in this bucket from the old to the new hash HT  
        // 移动的关键操作  
        while(de) {
            unsigned int h;

            nextde = de->next;
            // Get the index in the new hash table  
            h = dictHashKey(d, de->key) & d->ht[1].sizemask;
            de->next = d->ht[1].table[h];
            d->ht[1].table[h] = de;
            d->ht[0].used--;
            d->ht[1].used++;
            de = nextde;
        }
        d->ht[0].table[d->rehashidx] = NULL;
        d->rehashidx++;
    }
    return 1;
}

// 获取当前毫秒的时间  
long long timeInMilliseconds(void) {
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

// Rehash for an amount of time between ms milliseconds and ms+1 milliseconds  
// 在给定时间内，循环执行哈希重定位  
int dictRehashMilliseconds(dict *d, int ms) {
    long long start = timeInMilliseconds();
    int rehashes = 0;

    while(dictRehash(d,100)) {
    	//重定位的次数累加
        rehashes += 100;
        //时间超出给定时间范围，则终止
        if (timeInMilliseconds()-start > ms) break;
    }
    return rehashes;
}

// This function performs just a step of rehashing, and only if there are
 * no safe iterators bound to our hash table. When we have iterators in the
 * middle of a rehashing we can't mess with the two hash tables otherwise
 * some element can be missed or duplicated.
 *
 * This function is called by common lookup or update operations in the
 * dictionary so that the hash table automatically migrates from H1 to H2
 * while it is actively used.  
// 当没有迭代器时候，进行重定位算法  
static void _dictRehashStep(dict *d) {
    if (d->iterators == 0) dictRehash(d,1);
}

// Add an element to the target hash table  
// 添加一个dicEntry  
int dictAdd(dict *d, void *key, void *val)
{
    dictEntry *entry = dictAddRaw(d,key);

    if (!entry) return DICT_ERR;
    dictSetVal(d, entry, val);
    return DICT_OK;
}

// Low level add. This function adds the entry but instead of setting
 * a value returns the dictEntry structure to the user, that will make
 * sure to fill the value field as he wishes.
 *
 * This function is also directly exposed to user API to be called
 * mainly in order to store non-pointers inside the hash value, example:
 *
 * entry = dictAddRaw(dict,mykey);
 * if (entry != NULL) dictSetSignedIntegerVal(entry,1000);
 *
 * Return values:
 *
 * If key already exists NULL is returned.
 * If key was added, the hash entry is returned to be manipulated by the caller.
  
// 添加一个指定key值的Entry  
dictEntry *dictAddRaw(dict *d, void *key)
{
    int index;
    dictEntry *entry;
    dictht *ht;

    if (dictIsRehashing(d)) _dictRehashStep(d);

    // Get the index of the new element, or -1 if
     * the element already exists.  
    // 如果指定的key已经存在，则直接返回NULL说明添加失败  
    if ((index = _dictKeyIndex(d, key)) == -1)
        return NULL;

    // Allocate the memory and store the new entry  
    ht = dictIsRehashing(d) ? &d->ht[1] : &d->ht[0];
    entry = zmalloc(sizeof(*entry));
    entry->next = ht->table[index];
    ht->table[index] = entry;
    ht->used++;

    // Set the hash entry fields.  
    dictSetKey(d, entry, key);
    return entry;
}

// Add an element, discarding the old if the key already exists.
 * Return 1 if the key was added from scratch, 0 if there was already an
 * element with such key and dictReplace() just performed a value update
 * operation.  
// 替换一个子字典集，如果不存在直接添加，存在，覆盖val的值  
int dictReplace(dict *d, void *key, void *val)
{
    dictEntry *entry, auxentry;

    // Try to add the element. If the key
     * does not exists dictAdd will suceed.  
    //不存在，这个key直接添加
    if (dictAdd(d, key, val) == DICT_OK)
        return 1;
    // It already exists, get the entry  
    entry = dictFind(d, key);
    // Set the new value and free the old one. Note that it is important
     * to do that in this order, as the value may just be exactly the same
     * as the previous one. In this context, think to reference counting,
     * you want to increment (set), and then decrement (free), and not the
     * reverse.  
    //赋值方法
    auxentry = *entry;
    dictSetVal(d, entry, val);
    dictFreeVal(d, &auxentry);
    return 0;
}

// dictReplaceRaw() is simply a version of dictAddRaw() that always
 * returns the hash entry of the specified key, even if the key already
 * exists and can't be added (in that case the entry of the already
 * existing key is returned.)
 *
 * See dictAddRaw() for more information.  
// 添加字典，没有函数方法，如果存在，就不添加  
dictEntry *dictReplaceRaw(dict *d, void *key) {
    dictEntry *entry = dictFind(d,key);

    return entry ? entry : dictAddRaw(d,key);
}

// Search and remove an element  
// 删除给定key的结点，可控制是否调用释放方法  
static int dictGenericDelete(dict *d, const void *key, int nofree)
{
    unsigned int h, idx;
    dictEntry *he, *prevHe;
    int table;

    if (d->ht[0].size == 0) return DICT_ERR; // d->ht[0].table is NULL  
    if (dictIsRehashing(d)) _dictRehashStep(d);
    //计算key对应的哈希索引
    h = dictHashKey(d, key);

    for (table = 0; table <= 1; table++) {
        idx = h & d->ht[table].sizemask;
        //找到具体的索引对应的结点
        he = d->ht[table].table[idx];
        prevHe = NULL;
        while(he) {
            if (dictCompareKeys(d, key, he->key)) {
                // Unlink the element from the list  
                if (prevHe)
                    prevHe->next = he->next;
                else
                    d->ht[table].table[idx] = he->next;
                if (!nofree) {
                	//判断是否需要调用dict定义的free方法
                    dictFreeKey(d, he);
                    dictFreeVal(d, he);
                }
                zfree(he);
                d->ht[table].used--;
                return DICT_OK;
            }
            prevHe = he;
            he = he->next;
        }
        if (!dictIsRehashing(d)) break;
    }
    return DICT_ERR; // not found  
}

// 会调用free方法的删除方法  
int dictDelete(dict *ht, const void *key) {
    return dictGenericDelete(ht,key,0);
}

// 不会调用free方法的删除方法  
int dictDeleteNoFree(dict *ht, const void *key) {
    return dictGenericDelete(ht,key,1);
}

// Destroy an entire dictionary  
// 清空整个哈希表  
int _dictClear(dict *d, dictht *ht, void(callback)(void *)) {
    unsigned long i;

    // Free all the elements  
    for (i = 0; i < ht->size && ht->used > 0; i++) {
        dictEntry *he, *nextHe;

		//每次情况会调用回调方法
        if (callback && (i & 65535) == 0) callback(d->privdata);

        if ((he = ht->table[i]) == NULL) continue;
        while(he) {
        	//依次释放结点
            nextHe = he->next;
            dictFreeKey(d, he);
            dictFreeVal(d, he);
            zfree(he);
            ht->used--;
            he = nextHe;
        }
    }
    // Free the table and the allocated cache structure  
    zfree(ht->table);
    // Re-initialize the table  
    _dictReset(ht);
    return DICT_OK; // never fails  
}

// Clear & Release the hash table  
// 重置字典总类，清空2张表  
void dictRelease(dict *d)
{
    _dictClear(d,&d->ht[0],NULL);
    _dictClear(d,&d->ht[1],NULL);
    zfree(d);
}

// 根据key返回具体的字典集  
dictEntry *dictFind(dict *d, const void *key)
{
    dictEntry *he;
    unsigned int h, idx, table;

    if (d->ht[0].size == 0) return NULL; // We don't have a table at all  
    if (dictIsRehashing(d)) _dictRehashStep(d);
    h = dictHashKey(d, key);
    for (table = 0; table <= 1; table++) {
        idx = h & d->ht[table].sizemask;
        he = d->ht[table].table[idx];
        while(he) {
            if (dictCompareKeys(d, key, he->key))
                return he;
            he = he->next;
        }
        if (!dictIsRehashing(d)) return NULL;
    }
    return NULL;
}

// 获取目标字典集的方法  
void *dictFetchValue(dict *d, const void *key) {
    dictEntry *he;

    he = dictFind(d,key);
    // 获取字典集的方法  
    return he ? dictGetVal(he) : NULL;
}

// A fingerprint is a 64 bit number that represents the state of the dictionary
 * at a given time, it's just a few dict properties xored together.
 * When an unsafe iterator is initialized, we get the dict fingerprint, and check
 * the fingerprint again when the iterator is released.
 * If the two fingerprints are different it means that the user of the iterator
 * performed forbidden operations against the dictionary while iterating.  
// 通过指纹来禁止每个不安全的哈希迭代器的非法操作，每个不安全迭代器只能有一个指纹  
long long dictFingerprint(dict *d) {
    long long integers[6], hash = 0;
    int j;

    integers[0] = (long) d->ht[0].table;
    integers[1] = d->ht[0].size;
    integers[2] = d->ht[0].used;
    integers[3] = (long) d->ht[1].table;
    integers[4] = d->ht[1].size;
    integers[5] = d->ht[1].used;

    // We hash N integers by summing every successive integer with the integer
     * hashing of the previous sum. Basically:
     *
     * Result = hash(hash(hash(int1)+int2)+int3) ...
     *
     * This way the same set of integers in a different order will (likely) hash
     * to a different number.  
    for (j = 0; j < 6; j++) {
        hash += integers[j];
        // For the hashing step we use Tomas Wang's 64 bit integer hash.  
        hash = (~hash) + (hash << 21); // hash = (hash << 21) - hash - 1;
        hash = hash ^ (hash >> 24);
        hash = (hash + (hash << 3)) + (hash << 8); // hash * 265
        hash = hash ^ (hash >> 14);
        hash = (hash + (hash << 2)) + (hash << 4); // hash * 21
        hash = hash ^ (hash >> 28);
        hash = hash + (hash << 31);
    }
    return hash;
}

// 获取哈希迭代器，默认不安全的  
dictIterator *dictGetIterator(dict *d)
{
    dictIterator *iter = zmalloc(sizeof(*iter));

    iter->d = d;
    iter->table = 0;
    iter->index = -1;
    iter->safe = 0;
    iter->entry = NULL;
    iter->nextEntry = NULL;
    return iter;
}

// 获取安全哈希迭代器  
dictIterator *dictGetSafeIterator(dict *d) {
    dictIterator *i = dictGetIterator(d);

    i->safe = 1;
    return i;
}

// 迭代器获取下一个集合点  
dictEntry *dictNext(dictIterator *iter)
{
    while (1) {
        if (iter->entry == NULL) {
            dictht *ht = &iter->d->ht[iter->table];
            if (iter->index == -1 && iter->table == 0) {
            	//如果迭代器index下标为-1说明还没开始使用，设置迭代器的指纹或增加引用计数量
                if (iter->safe)
                    iter->d->iterators++;
                else
                    iter->fingerprint = dictFingerprint(iter->d);
            }
            //迭代器下标递增
            iter->index++;
            if (iter->index >= (long) ht->size) {
                if (dictIsRehashing(iter->d) && iter->table == 0) {
                    iter->table++;
                    iter->index = 0;
                    ht = &iter->d->ht[1];
                } else {
                    break;
                }
            }
            //根据下标选择集合点
            iter->entry = ht->table[iter->index];
        } else {
            iter->entry = iter->nextEntry;
        }
        if (iter->entry) {
            // We need to save the 'next' here, the iterator user
             * may delete the entry we are returning.  
            iter->nextEntry = iter->entry->next;
            return iter->entry;
        }
    }
    return NULL;
}

// 释放迭代器  
void dictReleaseIterator(dictIterator *iter)
{
    if (!(iter->index == -1 && iter->table == 0)) {
        if (iter->safe)
            iter->d->iterators--;
        else
        	//这时判断指纹是否还是之前定义的那个
            assert(iter->fingerprint == dictFingerprint(iter->d));
    }
    zfree(iter);
}

// Return a random entry from the hash table. Useful to
 * implement randomized algorithms  
// 随机获取一个集合点  
dictEntry *dictGetRandomKey(dict *d)
{
    dictEntry *he, *orighe;
    unsigned int h;
    int listlen, listele;

    if (dictSize(d) == 0) return NULL;
    if (dictIsRehashing(d)) _dictRehashStep(d);
    if (dictIsRehashing(d)) {
        do {
        	//随机数向2个表格的总数求余运算
            h = random() % (d->ht[0].size+d->ht[1].size);
            he = (h >= d->ht[0].size) ? d->ht[1].table[h - d->ht[0].size] :
                                      d->ht[0].table[h];
        } while(he == NULL);
    } else {
        do {
            h = random() & d->ht[0].sizemask;
            he = d->ht[0].table[h];
        } while(he == NULL);
    }

    // Now we found a non empty bucket, but it is a linked
     * list and we need to get a random element from the list.
     * The only sane way to do so is counting the elements and
     * select a random index.  
    listlen = 0;
    orighe = he;
    while(he) {
        he = he->next;
        listlen++;
    }
    listele = random() % listlen;
    he = orighe;
    while(listele--) he = he->next;
    return he;
}

// Function to reverse bits. Algorithm from:
 * http://graphics.stanford.edu/~seander/bithacks.html#ReverseParallel  
// 很神奇的翻转位  
static unsigned long rev(unsigned long v) {
    unsigned long s = 8 * sizeof(v); // bit size; must be power of 2
    unsigned long mask = ~0;
    while ((s >>= 1) > 0) {
        mask ^= (mask << s);
        v = ((v >> s) & mask) | ((v << s) & ~mask);
    }
    return v;
}

// dictScan() is used to iterate over the elements of a dictionary.
 *
 * Iterating works in the following way:
 *
 * 1) Initially you call the function using a cursor (v) value of 0.
 * 2) The function performs one step of the iteration, and returns the
 *    new cursor value that you must use in the next call.
 * 3) When the returned cursor is 0, the iteration is complete.
 *
 * The function guarantees that all the elements that are present in the
 * dictionary from the start to the end of the iteration are returned.
 * However it is possible that some element is returned multiple time.
 *
 * For every element returned, the callback 'fn' passed as argument is
 * called, with 'privdata' as first argument and the dictionar entry
 * 'de' as second argument.
 *
 * HOW IT WORKS.
 *
 * The algorithm used in the iteration was designed by Pieter Noordhuis.
 * The main idea is to increment a cursor starting from the higher order
 * bits, that is, instead of incrementing the cursor normally, the bits
 * of the cursor are reversed, then the cursor is incremented, and finally
 * the bits are reversed again.
 *
 * This strategy is needed because the hash table may be resized from one
 * call to the other call of the same iteration.
 *
 * dict.c hash tables are always power of two in size, and they
 * use chaining, so the position of an element in a given table is given
 * always by computing the bitwise AND between Hash(key) and SIZE-1
 * (where SIZE-1 is always the mask that is equivalent to taking the rest
 *  of the division between the Hash of the key and SIZE).
 *
 * For example if the current hash table size is 16, the mask is
 * (in binary) 1111. The position of a key in the hash table will be always
 * the last four bits of the hash output, and so forth.
 *
 * WHAT HAPPENS IF THE TABLE CHANGES IN SIZE?
 *
 * If the hash table grows, elements can go anyway in one multiple of
 * the old bucket: for example let's say that we already iterated with
 * a 4 bit cursor 1100, since the mask is 1111 (hash table size = 16).
 *
 * If the hash table will be resized to 64 elements, and the new mask will
 * be 111111, the new buckets that you obtain substituting in ??1100
 * either 0 or 1, can be targeted only by keys that we already visited
 * when scanning the bucket 1100 in the smaller hash table.
 *
 * By iterating the higher bits first, because of the inverted counter, the
 * cursor does not need to restart if the table size gets bigger, and will
 * just continue iterating with cursors that don't have '1100' at the end,
 * nor any other combination of final 4 bits already explored.
 *
 * Similarly when the table size shrinks over time, for example going from
 * 16 to 8, If a combination of the lower three bits (the mask for size 8
 * is 111) was already completely explored, it will not be visited again
 * as we are sure that, we tried for example, both 0111 and 1111 (all the
 * variations of the higher bit) so we don't need to test it again.
 *
 * WAIT... YOU HAVE *TWO* TABLES DURING REHASHING!
 *
 * Yes, this is true, but we always iterate the smaller one of the tables,
 * testing also all the expansions of the current cursor into the larger
 * table. So for example if the current cursor is 101 and we also have a
 * larger table of size 16, we also test (0)101 and (1)101 inside the larger
 * table. This reduces the problem back to having only one table, where
 * the larger one, if exists, is just an expansion of the smaller one.
 *
 * LIMITATIONS
 *
 * This iterator is completely stateless, and this is a huge advantage,
 * including no additional memory used.
 *
 * The disadvantages resulting from this design are:
 *
 * 1) It is possible that we return duplicated elements. However this is usually
 *    easy to deal with in the application level.
 * 2) The iterator must return multiple elements per call, as it needs to always
 *    return all the keys chained in a given bucket, and all the expansions, so
 *    we are sure we don't miss keys moving.
 * 3) The reverse cursor is somewhat hard to understand at first, but this
 *    comment is supposed to help.
  
// 扫描方法  
unsigned long dictScan(dict *d,
                       unsigned long v,
                       dictScanFunction *fn,
                       void *privdata)
{
    dictht *t0, *t1;
    const dictEntry *de;
    unsigned long m0, m1;

    if (dictSize(d) == 0) return 0;

    if (!dictIsRehashing(d)) {
        t0 = &(d->ht[0]);
        m0 = t0->sizemask;

        // Emit entries at cursor  
        de = t0->table[v & m0];
        while (de) {
            fn(privdata, de);
            de = de->next;
        }

    } else {
        t0 = &d->ht[0];
        t1 = &d->ht[1];

        // Make sure t0 is the smaller and t1 is the bigger table  
        if (t0->size > t1->size) {
            t0 = &d->ht[1];
            t1 = &d->ht[0];
        }

        m0 = t0->sizemask;
        m1 = t1->sizemask;

        // Emit entries at cursor  
        de = t0->table[v & m0];
        while (de) {
            fn(privdata, de);
            de = de->next;
        }

        // Iterate over indices in larger table that are the expansion
         * of the index pointed to by the cursor in the smaller table  
        do {
            // Emit entries at cursor  
            de = t1->table[v & m1];
            while (de) {
                fn(privdata, de);
                de = de->next;
            }

            // Increment bits not covered by the smaller mask  
            v = (((v | m0) + 1) & ~m0) | (v & m0);

            // Continue while bits covered by mask difference is non-zero  
        } while (v & (m0 ^ m1));
    }

    // Set unmasked bits so incrementing the reversed cursor
     * operates on the masked bits of the smaller table  
    v |= ~m0;

    // Increment the reverse cursor  
    v = rev(v);
    v++;
    v = rev(v);

    return v;
}

// ------------------------- private functions ------------------------------  

// Expand the hash table if needed  
// 判断是否需要扩容  
static int _dictExpandIfNeeded(dict *d)
{
    // Incremental rehashing already in progress. Return.  
    if (dictIsRehashing(d)) return DICT_OK;

    // If the hash table is empty expand it to the initial size.  
    if (d->ht[0].size == 0) return dictExpand(d, DICT_HT_INITIAL_SIZE);

    // If we reached the 1:1 ratio, and we are allowed to resize the hash
     * table (global setting) or we should avoid it but the ratio between
     * elements/buckets is over the "safe" threshold, we resize doubling
     * the number of buckets.  
    // 判断是否需要扩容  
    if (d->ht[0].used >= d->ht[0].size &&
        (dict_can_resize ||
         d->ht[0].used/d->ht[0].size > dict_force_resize_ratio))
    {
        return dictExpand(d, d->ht[0].used*2);
    }
    return DICT_OK;
}

// Our hash table capability is a power of two  
// 哈希表的容量以2的幂次方，所以数量以2的幂次向上取  
static unsigned long _dictNextPower(unsigned long size)
{
    unsigned long i = DICT_HT_INITIAL_SIZE;

    if (size >= LONG_MAX) return LONG_MAX;
    while(1) {
        if (i >= size)
            return i;
        i *= 2;
    }
}

/// Returns the index of a free slot that can be populated with
 * a hash entry for the given 'key'.
 * If the key already exists, -1 is returned.
 *
 * Note that if we are in the process of rehashing the hash table, the
 * index is always returned in the context of the second (new) hash table.  
// 获取key值对应的哈希索引值，如果已经存在此key则返回-1  
static int _dictKeyIndex(dict *d, const void *key)
{
    unsigned int h, idx, table;
    dictEntry *he;

    // Expand the hash table if needed  
    if (_dictExpandIfNeeded(d) == DICT_ERR)
        return -1;
    // Compute the key hash value  
    h = dictHashKey(d, key);
    for (table = 0; table <= 1; table++) {
        idx = h & d->ht[table].sizemask;
        // Search if this slot does not already contain the given key  
        he = d->ht[table].table[idx];
        while(he) {
            if (dictCompareKeys(d, key, he->key))
                return -1;
            he = he->next;
        }
        if (!dictIsRehashing(d)) break;
    }
    return idx;
}

// 清空整个字典，即清空里面的2张哈希表  
void dictEmpty(dict *d, void(callback)(void*)) {
    _dictClear(d,&d->ht[0],callback);
    _dictClear(d,&d->ht[1],callback);
    d->rehashidx = -1;
    d->iterators = 0;
}

//启用哈希表调整 
void dictEnableResize(void) {
    dict_can_resize = 1;
}

// 启用哈希表调整  
void dictDisableResize(void) {
    dict_can_resize = 0;
}

#if 0

// The following is code that we don't use for Redis currently, but that is part
of the library.  
// redis中还存着调试的代码  
// ----------------------- Debugging ------------------------ 

#define DICT_STATS_VECTLEN 50
static void _dictPrintStatsHt(dictht *ht) {
    unsigned long i, slots = 0, chainlen, maxchainlen = 0;
    unsigned long totchainlen = 0;
    unsigned long clvector[DICT_STATS_VECTLEN];

    if (ht->used == 0) {
        printf("No stats available for empty dictionaries\n");
        return;
    }

    for (i = 0; i < DICT_STATS_VECTLEN; i++) clvector[i] = 0;
    for (i = 0; i < ht->size; i++) {
        dictEntry *he;

        if (ht->table[i] == NULL) {
            clvector[0]++;
            continue;
        }
        slots++;
        // For each hash entry on this slot...  
        chainlen = 0;
        he = ht->table[i];
        while(he) {
            chainlen++;
            he = he->next;
        }
        clvector[(chainlen < DICT_STATS_VECTLEN) ? chainlen : (DICT_STATS_VECTLEN-1)]++;
        if (chainlen > maxchainlen) maxchainlen = chainlen;
        totchainlen += chainlen;
    }
    printf("Hash table stats:\n");
    printf(" table size: %ld\n", ht->size);
    printf(" number of elements: %ld\n", ht->used);
    printf(" different slots: %ld\n", slots);
    printf(" max chain length: %ld\n", maxchainlen);
    printf(" avg chain length (counted): %.02f\n", (float)totchainlen/slots);
    printf(" avg chain length (computed): %.02f\n", (float)ht->used/slots);
    printf(" Chain length distribution:\n");
    for (i = 0; i < DICT_STATS_VECTLEN-1; i++) {
        if (clvector[i] == 0) continue;
        printf("   %s%ld: %ld (%.02f%%)\n",(i == DICT_STATS_VECTLEN-1)?">= ":"", i, clvector[i], ((float)clvector[i]/ht->size)*100);
    }
}

void dictPrintStats(dict *d) {
    _dictPrintStatsHt(&d->ht[0]);
    if (dictIsRehashing(d)) {
        printf("-- Rehashing into ht[1]:\n");
        _dictPrintStatsHt(&d->ht[1]);
    }
}

// ----------------------- StringCopy Hash Table Type ------------------------

static unsigned int _dictStringCopyHTHashFunction(const void *key)
{
    return dictGenHashFunction(key, strlen(key));
}

static void *_dictStringDup(void *privdata, const void *key)
{
    int len = strlen(key);
    char *copy = zmalloc(len+1);
    DICT_NOTUSED(privdata);

    memcpy(copy, key, len);
    copy[len] = '\0';
    return copy;
}

static int _dictStringCopyHTKeyCompare(void *privdata, const void *key1,
        const void *key2)
{
    DICT_NOTUSED(privdata);

    return strcmp(key1, key2) == 0;
}

static void _dictStringDestructor(void *privdata, void *key)
{
    DICT_NOTUSED(privdata);

    zfree(key);
}

// 定义了3种类型的dictType，有些类型无val dup方法的定义  
dictType dictTypeHeapStringCopyKey = {
    _dictStringCopyHTHashFunction, // hash function  
    _dictStringDup,                // key dup  
    NULL,                          // val dup  
    _dictStringCopyHTKeyCompare,   // key compare  
    _dictStringDestructor,         // key destructor  
    NULL                           // val destructor  
};

// This is like StringCopy but does not auto-duplicate the key.
 * It's used for intepreter's shared strings.  
dictType dictTypeHeapStrings = {
    _dictStringCopyHTHashFunction, // hash function  
    NULL,                          // key dup  
    NULL,                          // val dup  
    _dictStringCopyHTKeyCompare,   // key compare  
    _dictStringDestructor,         // key destructor  
    NULL                           // val destructor  
};

// This is like StringCopy but also automatically handle dynamic
 * allocated C strings as values.  
dictType dictTypeHeapStringCopyKeyValue = {
    _dictStringCopyHTHashFunction, // hash function  
    _dictStringDup,                // key dup  
    _dictStringDup,                // val dup  
    _dictStringCopyHTKeyCompare,   // key compare  
    _dictStringDestructor,         // key destructor  
    _dictStringDestructor,         // val destructor  
};
#endif
```

### sds

```C
// sds.h
#ifndef __SDS_H
#define __SDS_H

// 最大分配内存1M  
#define SDS_MAX_PREALLOC (1024*1024)

#include <sys/types.h>
#include <stdarg.h>

// 声明了sds的一种char类型  
typedef char *sds;

// 字符串结构体类型  
struct sdshdr {
	//字符长度
    unsigned int len;
    //当前可用空间
    unsigned int free;
    //具体存放字符的buf
    char buf[];
};

// 计算sds的长度，返回的size_t类型的数值  
// size_t,它是一个与机器相关的unsigned类型，其大小足以保证存储内存中对象的大小。  
static inline size_t sdslen(const sds s) {
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->len;
}

// 根据sdshdr中的free标记获取可用空间  
static inline size_t sdsavail(const sds s) {
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->free;
}

sds sdsnewlen(const void *init, size_t initlen);   //根据给定长度，新生出一个sds
sds sdsnew(const char *init);    //根据给定的值，生出sds
sds sdsempty(void);    //清空sds操作
size_t sdslen(const sds s);   //获取sds的长度
sds sdsdup(const sds s);   //sds的复制方法
void sdsfree(sds s);   //sds的free释放方法
size_t sdsavail(const sds s);   //判断sds获取可用空间
sds sdsgrowzero(sds s, size_t len); // 扩展字符串到指定的长度
sds sdscatlen(sds s, const void *t, size_t len);
sds sdscat(sds s, const char *t);    //sds连接上char字符
sds sdscatsds(sds s, const sds t);  //sds连接上sds
sds sdscpylen(sds s, const char *t, size_t len);  //字符串复制相关
sds sdscpy(sds s, const char *t); //字符串复制相关

sds sdscatvprintf(sds s, const char *fmt, va_list ap);   //字符串格式化输出，依赖已有的方法sprintf，效率不及下面自己写的
#ifdef __GNUC__
sds sdscatprintf(sds s, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
#else
sds sdscatprintf(sds s, const char *fmt, ...);
#endif

sds sdscatfmt(sds s, char const *fmt, ...);   //字符串格式化输出
sds sdstrim(sds s, const char *cset);       //字符串缩减
void sdsrange(sds s, int start, int end);   //字符串截取函数
void sdsupdatelen(sds s);   //更新字符串最新的长度
void sdsclear(sds s);   //字符串清空操作
int sdscmp(const sds s1, const sds s2);   //sds比较函数
sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count);  //字符串分割子字符串
void sdsfreesplitres(sds *tokens, int count);  //释放子字符串数组
void sdstolower(sds s);    //sds字符转小写表示
void sdstoupper(sds s);    //sds字符统一转大写
sds sdsfromlonglong(long long value);   //生出数组字符串
sds sdscatrepr(sds s, const char *p, size_t len);
sds *sdssplitargs(const char *line, int *argc);   //参数拆分
sds sdsmapchars(sds s, const char *from, const char *to, size_t setlen); //字符映射,"ho", "01", h映射为0， o映射为1
sds sdsjoin(char **argv, int argc, char *sep);   //以分隔符连接字符串子数组构成新的字符串

// Low level functions exposed to the user API  
// 开放给使用者的API  
sds sdsMakeRoomFor(sds s, size_t addlen);
void sdsIncrLen(sds s, int incr);
sds sdsRemoveFreeSpace(sds s);
size_t sdsAllocSize(sds s);

#endif

// 清空字符串  
void sdsclear(sds s) {
    struct sdshdr *sh = (void*) (s-(sizeof(struct sdshdr)));
    //空闲的长度增多
    sh->free += sh->len;
    sh->len = 0;
    //字符串中的缓存其实没有被丢底，只是把第一个设成了结束标志，以便下次操作可以复用
    sh->buf[0] = '\0';
}


// 创建新字符串方法，传入目标长度，初始化方法  
sds sdsnewlen(const void *init, size_t initlen) {
    struct sdshdr *sh;
 
    if (init) {
        sh = zmalloc(sizeof(struct sdshdr)+initlen+1);
    } else {
    	//当init函数为NULL时候，又来了zcalloc的方法
        sh = zcalloc(sizeof(struct sdshdr)+initlen+1);
    }
    if (sh == NULL) return NULL;
    sh->len = initlen;
    sh->free = 0;
    if (initlen && init)
        memcpy(sh->buf, init, initlen);
   //最末端同样要加‘\0’结束符
    sh->buf[initlen] = '\0';
    //最后是通过返回字符串结构体中的buf代表新的字符串
    return (char*)sh->buf;
}

// 字符串格式化输出，输入原字符串，格式，参数  
sds sdscatfmt(sds s, char const *fmt, ...) {
    struct sdshdr *sh = (void*) (s-(sizeof(struct sdshdr)));
    size_t initlen = sdslen(s);
    const char *f = fmt;
    int i;
    va_list ap;
 
    va_start(ap,fmt);
    f = fmt;    // Next format specifier byte to process.  
    i = initlen; // Position of the next byte to write to dest str.  
    //关键地方，以此比较输入的格式类型
    while(*f) {
        char next, *str;
        unsigned int l;
        long long num;
        unsigned long long unum;
 
        // Make sure there is always space for at least 1 char.  
        if (sh->free == 0) {
            s = sdsMakeRoomFor(s,1);
            sh = (void*) (s-(sizeof(struct sdshdr)));
        }
 
        switch(*f) {
        case '%':
            //如果是%,记住百分号后面的类型操作值 
            next = *(f+1);
            f++;
            switch(next) {
            case 's':
            case 'S':
                str = va_arg(ap,char*);
            	//判断普通的str,还是sds类型，计算长度的方法不一样
                l = (next == 's') ? strlen(str) : sdslen(str);
                if (sh->free < l) {
                    s = sdsMakeRoomFor(s,l);
                    sh = (void*) (s-(sizeof(struct sdshdr)));
                }
                //如果是字符串，直接复制到后面
                memcpy(s+i,str,l);
                sh->len += l;
                sh->free -= l;
                i += l;
                break;
            case 'i':
            case 'I':
                if (next == 'i')
                    num = va_arg(ap,int);
                else
                    num = va_arg(ap,long long);
                {
                    char buf[SDS_LLSTR_SIZE];
                    //如果是数字，调用添加数值字符串方法
                    l = sdsll2str(buf,num);
                    if (sh->free < l) {
                        s = sdsMakeRoomFor(s,l);
                        sh = (void*) (s-(sizeof(struct sdshdr)));
                    }
                    memcpy(s+i,buf,l);
                    sh->len += l;
                    sh->free -= l;
                    i += l;
                }
                break;
            case 'u':
            case 'U':
            //无符号整型同上
                if (next == 'u')
                    unum = va_arg(ap,unsigned int);
                else
                    unum = va_arg(ap,unsigned long long);
                {
                    char buf[SDS_LLSTR_SIZE];
                    l = sdsull2str(buf,unum);
                    if (sh->free < l) {
                        s = sdsMakeRoomFor(s,l);
                        sh = (void*) (s-(sizeof(struct sdshdr)));
                    }
                    memcpy(s+i,buf,l);
                    sh->len += l;
                    sh->free -= l;
                    i += l;
                }
                break;
            default: // Handle %% and generally %<unknown>.  
                s[i++] = next;
                sh->len += 1;
                sh->free -= 1;
                break;
            }
            break;
        default:
        	//非操作类型，直接单字符添加
            s[i++] = *f;
            sh->len += 1;
            sh->free -= 1;
            break;
        }
        f++;
    }
    va_end(ap);
 
    // Add null-term  
    s[i] = '\0';
    return s;
}

// 字符串分割方法

sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count) {
    int elements = 0, slots = 5, start = 0, j;
    sds *tokens;

    if (seplen < 1 || len < 0) return NULL;

	//分割的子字符串初始值只有5组
    tokens = zmalloc(sizeof(sds)*slots);
    //如果内存溢出，直接返回NULL值
    if (tokens == NULL) return NULL;

    if (len == 0) {
        *count = 0;
        return tokens;
    }
    //从前往后扫描，到最后一个能匹配分隔符字符串的位置len-seplen
    for (j = 0; j < (len-(seplen-1)); j++) {
        // make sure there is room for the next element and the final one  
        //如果当前字符串数组数量少于当前已存在数组+2个的时候，动态添加
        if (slots < elements+2) {
            sds *newtokens;

            slots *= 2;
            newtokens = zrealloc(tokens,sizeof(sds)*slots);
            //如果内存此时溢出，goto语句free释放内存，终于看到了goto语句的派上用处了
            if (newtokens == NULL) goto cleanup;
            tokens = newtokens;
        }
        // search the separator  
        //分成单字符比较和多字符比较匹配
        if ((seplen == 1 && *(s+j) == sep[0]) || (memcmp(s+j,sep,seplen) == 0)) {
        	//赋值子字符串
            tokens[elements] = sdsnewlen(s+start,j-start);
            if (tokens[elements] == NULL) goto cleanup;
            elements++;
            start = j+seplen;
            j = j+seplen-1; // skip the separator  
        }
    }
    // Add the final element. We are sure there is room in the tokens array.  
    //最后一个字符串添加
    tokens[elements] = sdsnewlen(s+start,len-start);
    //如果内存溢出，再次清空，也直接直接返回NULL
    if (tokens[elements] == NULL) goto cleanup;
    elements++;
    *count = elements;
    return tokens;

cleanup:
    {
    	//清除空间
        int i;
        for (i = 0; i < elements; i++) sdsfree(tokens[i]);
        zfree(tokens);
        *count = 0;
        return NULL;
    }
}


// 字符串末尾添加数值字符串组成新的字符串  
#define SDS_LLSTR_SIZE 21
int sdsll2str(char *s, long long value) {
    char *p, aux;
    unsigned long long v;
    size_t l;
 
    // Generate the string representation, this method produces
     * an reversed string.  
    v = (value < 0) ? -value : value;
    p = s;
    //用最传统的逐位取商算出每一个位置上的数，注意现在的顺序其实是逆序的
    do {
        *p++ = '0'+(v%10);
        v /= 10;
    } while(v);
    //后面别忘了正负号的添加
    if (value < 0) *p++ = '-';
 
    // Compute length and add null term.  
    l = p-s;
    *p = '\0';
 
    // Reverse the string.  
    //将刚才的添加的逆序的数字字符串进行倒叙添加到本身的字符串s中
    p--;
    while(s < p) {
        aux = *s;
        *s = *p;
        *p = aux;
        s++;
        p--;
    }
    return l;
}


// 字符映射,"ho", "01", h映射为0， o映射为1  
sds sdsmapchars(sds s, const char *from, const char *to, size_t setlen) {
    size_t j, i, l = sdslen(s);

    for (j = 0; j < l; j++) {
        for (i = 0; i < setlen; i++) {
            if (s[j] == from[i]) {
                s[j] = to[i];
                break;
            }
        }
    }
    return s;
}

```

### ziplist

ziplist 是 Redis 中的压缩列表，主要是为了解决正常链表中内存空间的浪费，因为我们需要通过 pre 指针和 next 指针来找到当前结点的前继结点和后继结点，这需要消耗内存空间。而 ziplist 采用长度的表示方法，整个 ziplist 是一个很长的字符串，根据链表中各个结点的长度，上一个结点的长度等信息，通过快速定位实现相关的操作。并且在长度上也做了动态分配字节的方法，避免了内存的浪费，也就实现了长度压缩。ziplist 主要用于 rpush，lpush 等这些给链表添加数据的方法中。

```C
/* The ziplist is a specially encoded dually linked list that is designed
 * to be very memory efficient. It stores both strings and integer values,
 * where integers are encoded as actual integers instead of a series of
 * characters. It allows push and pop operations on either side of the list
 * in O(1) time. However, because every operation requires a reallocation of
 * the memory used by the ziplist, the actual complexity is related to the
 * amount of memory used by the ziplist.
 *
 * ziplist是一个编码后的列表，特殊的设计使得内存操作非常有效率，此列表可以同时存放
 * 字符串和整数类型，列表可以在头尾各边支持推加和弹出操作在O(1)常量时间,但是，因为每次
 * 操作涉及到内存的重新分配与释放，所以加大了操作的复杂性
 * ----------------------------------------------------------------------------
 *
 * ziplist的结构组成：
 * ZIPLIST OVERALL LAYOUT:
 * The general layout of the ziplist is as follows:
 * <zlbytes><zltail><zllen><entry><entry><zlend>
 *
 * <zlbytes> is an unsigned integer to hold the number of bytes that the
 * ziplist occupies. This value needs to be stored to be able to resize the
 * entire structure without the need to traverse it first.
 * <zipbytes>代表着ziplist占有的字节数，这方便当重新调整大小的时候不需要重新从头遍历
 * 
 * <zltail> is the offset to the last entry in the list. This allows a pop
 * operation on the far side of the list without the need for full traversal.
 * <zltail>记录了最后一个entry的位置在列表中，可以方便快速在列表末尾弹出操作
 *
 * <zllen> is the number of entries.When this value is larger than 2**16-2,
 * we need to traverse the entire list to know how many items it holds.
 * <zllen>记录的是ziplist里面entry数据结点的总数
 *
 * <zlend> is a single byte special value, equal to 255, which indicates the
 * end of the list.
 * <zlend>代表的是结束标识别，用单字节表示，值是255，就是11111111
 *
 * ZIPLIST ENTRIES:
 * Every entry in the ziplist is prefixed by a header that contains two pieces
 * of information. First, the length of the previous entry is stored to be
 * able to traverse the list from back to front. Second, the encoding with an
 * optional string length of the entry itself is stored.
 * 每个entry数据结点主要包含2部分信息，第一个，上一个结点的长度，主要就可以可以从任意结点从后往前遍历整个列表
 * 第二个，编码字符串的方式的类型保存
 *
 * The length of the previous entry is encoded in the following way:
 * If this length is smaller than 254 bytes, it will only consume a single
 * byte that takes the length as value. When the length is greater than or
 * equal to 254, it will consume 5 bytes. The first byte is set to 254 to
 * indicate a larger value is following. The remaining 4 bytes take the
 * length of the previous entry as value.
 * 之前的数据结点的字符串长度的长度少于254个字节，他将消耗单个字节，一个字节8位，最大可表示长度为2的8次方
 * 当字符串的长度大于254个字节，则用5个字节表示，第一个字节被设置成254,其余的4个字节占据的长度为之前的数据结点的长度
 *
 * The other header field of the entry itself depends on the contents of the
 * entry. When the entry is a string, the first 2 bits of this header will hold
 * the type of encoding used to store the length of the string, followed by the
 * actual length of the string. When the entry is an integer the first 2 bits
 * are both set to 1. The following 2 bits are used to specify what kind of
 * integer will be stored after this header. An overview of the different
 * types and encodings is as follows:
 * 头部信息中的另一个值记录着编码的方式，当编码的是字符串，头部的前2位为00,01,10共3种
 * 如果编码的是整型数字的时候，则头部的前2位为11,代表的是整数编码，后面2位代表什么类型整型值将会在头部后面被编码
 * 00-int16_t, 01-int32_t, 10-int64_t, 11-24 bit signed，还有比较特殊的2个，11111110-8 bit signed,
 * 1111 0000 - 1111 1101,代表的是整型值0-12,头尾都已经存在，都不能使用，与传统的通过固定的指针表示长度，这么做的好处实现
 * 可以更合理的分配内存
 *
 * String字符串编码的3种形式
 * |00pppppp| - 1 byte
 *      String value with length less than or equal to 63 bytes (6 bits).
 * |01pppppp|qqqqqqqq| - 2 bytes
 *      String value with length less than or equal to 16383 bytes (14 bits).
 * |10______|qqqqqqqq|rrrrrrrr|ssssssss|tttttttt| - 5 bytes
 *      String value with length greater than or equal to 16384 bytes.
 * |11000000| - 1 byte
 *      Integer encoded as int16_t (2 bytes).
 * |11010000| - 1 byte
 *      Integer encoded as int32_t (4 bytes).
 * |11100000| - 1 byte
 *      Integer encoded as int64_t (8 bytes).
 * |11110000| - 1 byte
 *      Integer encoded as 24 bit signed (3 bytes).
 * |11111110| - 1 byte
 *      Integer encoded as 8 bit signed (1 byte).
 * |1111xxxx| - (with xxxx between 0000 and 1101) immediate 4 bit integer.
 *      Unsigned integer from 0 to 12. The encoded value is actually from
 *      1 to 13 because 0000 and 1111 can not be used, so 1 should be
 *      subtracted from the encoded 4 bit value to obtain the right value.
 * |11111111| - End of ziplist.
 *
 * All the integers are represented in little endian byte order.
 *
 * ----------------------------------------------------------------------------

/* 实际存放数据的数据结点 */
typedef struct zlentry {
	//prevrawlen为上一个数据结点的长度，prevrawlensize为记录该长度数值所需要的字节数
    unsigned int prevrawlensize, prevrawlen;
    //len为当前数据结点的长度，lensize表示表示当前长度表示所需的字节数
    unsigned int lensize, len;
    //数据结点的头部信息长度的字节数
    unsigned int headersize;
    //编码的方式
    unsigned char encoding;
    //数据结点的数据(已包含头部等信息)，以字符串形式保存
    unsigned char *p;
} zlentry;
/* <zlentry>的结构图线表示 <pre_node_len>(上一结点的长度信息)<node_encode>(本结点的编码方式和编码数据的长度信息)<node>(本结点的编码数据) */

/* Insert item at "p". */
/* 插入操作的实现 */
static unsigned char *__ziplistInsert(unsigned char *zl, unsigned char *p, unsigned char *s, unsigned int slen) {
    size_t curlen = intrev32ifbe(ZIPLIST_BYTES(zl)), reqlen;
    unsigned int prevlensize, prevlen = 0;
    size_t offset;
    int nextdiff = 0;
    unsigned char encoding = 0;
    long long value = 123456789; /* initialized to avoid warning. Using a value
                                    that is easy to see if for some reason
                                    we use it uninitialized. */
    zlentry tail;

    /* Find out prevlen for the entry that is inserted. */
    //寻找插入的位置
    if (p[0] != ZIP_END) {
    	//定位到指定位置
        ZIP_DECODE_PREVLEN(p, prevlensize, prevlen);
    } else {
    	//如果插入的位置是尾结点，直接定位到尾结点，看第一个字节的就可以判断
        unsigned char *ptail = ZIPLIST_ENTRY_TAIL(zl);
        if (ptail[0] != ZIP_END) {
            prevlen = zipRawEntryLength(ptail);
        }
    }

    /* See if the entry can be encoded */
    if (zipTryEncoding(s,slen,&value,&encoding)) {
        /* 'encoding' is set to the appropriate integer encoding */
        reqlen = zipIntSize(encoding);
    } else {
        /* 'encoding' is untouched, however zipEncodeLength will use the
         * string length to figure out how to encode it. */
        reqlen = slen;
    }
    /* We need space for both the length of the previous entry and
     * the length of the payload. */
    reqlen += zipPrevEncodeLength(NULL,prevlen);
    reqlen += zipEncodeLength(NULL,encoding,slen);

    /* When the insert position is not equal to the tail, we need to
     * make sure that the next entry can hold this entry's length in
     * its prevlen field. */
    nextdiff = (p[0] != ZIP_END) ? zipPrevLenByteDiff(p,reqlen) : 0;

    /* Store offset because a realloc may change the address of zl. */
    //调整大小，为新结点的插入预留空间
    offset = p-zl;
    zl = ziplistResize(zl,curlen+reqlen+nextdiff);
    p = zl+offset;

    /* Apply memory move when necessary and update tail offset. */
    if (p[0] != ZIP_END) {
        /* Subtract one because of the ZIP_END bytes */
        //如果插入的位置不是尾结点，则挪动位置
        memmove(p+reqlen,p-nextdiff,curlen-offset-1+nextdiff);

        /* Encode this entry's raw length in the next entry. */
        zipPrevEncodeLength(p+reqlen,reqlen);

        /* Update offset for tail */
        ZIPLIST_TAIL_OFFSET(zl) =
            intrev32ifbe(intrev32ifbe(ZIPLIST_TAIL_OFFSET(zl))+reqlen);

        /* When the tail contains more than one entry, we need to take
         * "nextdiff" in account as well. Otherwise, a change in the
         * size of prevlen doesn't have an effect on the *tail* offset. */
        tail = zipEntry(p+reqlen);
        if (p[reqlen+tail.headersize+tail.len] != ZIP_END) {
            ZIPLIST_TAIL_OFFSET(zl) =
                intrev32ifbe(intrev32ifbe(ZIPLIST_TAIL_OFFSET(zl))+nextdiff);
        }
    } else {
    	//如果是尾结点，直接设置新尾结点
        /* This element will be the new tail. */
        ZIPLIST_TAIL_OFFSET(zl) = intrev32ifbe(p-zl);
    }

    /* When nextdiff != 0, the raw length of the next entry has changed, so
     * we need to cascade the update throughout the ziplist */
    if (nextdiff != 0) {
        offset = p-zl;
        zl = __ziplistCascadeUpdate(zl,p+reqlen);
        p = zl+offset;
    }

    /* Write the entry */
    //写入新的数据结点信息
    p += zipPrevEncodeLength(p,prevlen);
    p += zipEncodeLength(p,encoding,slen);
    if (ZIP_IS_STR(encoding)) {
        memcpy(p,s,slen);
    } else {
        zipSaveInteger(p,value,encoding);
    }

    //更新列表的长度加1
    ZIPLIST_INCR_LENGTH(zl,1);
    return zl;
}

/* Delete "num" entries, starting at "p". Returns pointer to the ziplist. */
/* 删除方法涉及p指针的滑动，后面的地址内容都需要滑动 */
static unsigned char *__ziplistDelete(unsigned char *zl, unsigned char *p, unsigned int num) {
    unsigned int i, totlen, deleted = 0;
    size_t offset;
    int nextdiff = 0;
    zlentry first, tail;

    first = zipEntry(p);
    for (i = 0; p[0] != ZIP_END && i < num; i++) {
        p += zipRawEntryLength(p);
        deleted++;
    }

    totlen = p-first.p;
    if (totlen > 0) {
        if (p[0] != ZIP_END) {
            /* Storing `prevrawlen` in this entry may increase or decrease the
             * number of bytes required compare to the current `prevrawlen`.
             * There always is room to store this, because it was previously
             * stored by an entry that is now being deleted. */
            nextdiff = zipPrevLenByteDiff(p,first.prevrawlen);
            p -= nextdiff;
            zipPrevEncodeLength(p,first.prevrawlen);

            /* Update offset for tail */
            ZIPLIST_TAIL_OFFSET(zl) =
                intrev32ifbe(intrev32ifbe(ZIPLIST_TAIL_OFFSET(zl))-totlen);

            /* When the tail contains more than one entry, we need to take
             * "nextdiff" in account as well. Otherwise, a change in the
             * size of prevlen doesn't have an effect on the *tail* offset. */
            tail = zipEntry(p);
            if (p[tail.headersize+tail.len] != ZIP_END) {
                ZIPLIST_TAIL_OFFSET(zl) =
                   intrev32ifbe(intrev32ifbe(ZIPLIST_TAIL_OFFSET(zl))+nextdiff);
            }

            /* Move tail to the front of the ziplist */
            memmove(first.p,p,
                intrev32ifbe(ZIPLIST_BYTES(zl))-(p-zl)-1);
        } else {
            /* The entire tail was deleted. No need to move memory. */
            ZIPLIST_TAIL_OFFSET(zl) =
                intrev32ifbe((first.p-zl)-first.prevrawlen);
        }

        /* Resize and update length */
        //调整列表大小
        offset = first.p-zl;
        zl = ziplistResize(zl, intrev32ifbe(ZIPLIST_BYTES(zl))-totlen+nextdiff);
        ZIPLIST_INCR_LENGTH(zl,-deleted);
        p = zl+offset;

        /* When nextdiff != 0, the raw length of the next entry has changed, so
         * we need to cascade the update throughout the ziplist */
        if (nextdiff != 0)
            zl = __ziplistCascadeUpdate(zl,p);
    }
    return zl;
}

/* zip列表的末尾值 */
#define ZIP_END 255
/* zip列表的最大长度 */
#define ZIP_BIGLEN 254

/* Different encoding/length possibilities */
/* 不同的编码 */
#define ZIP_STR_MASK 0xc0
#define ZIP_INT_MASK 0x30
#define ZIP_STR_06B (0 << 6)
#define ZIP_STR_14B (1 << 6)
#define ZIP_STR_32B (2 << 6)
#define ZIP_INT_16B (0xc0 | 0<<4)
#define ZIP_INT_32B (0xc0 | 1<<4)
#define ZIP_INT_64B (0xc0 | 2<<4)
#define ZIP_INT_24B (0xc0 | 3<<4)
#define ZIP_INT_8B 0xfe

/* 4 bit integer immediate encoding */
#define ZIP_INT_IMM_MASK 0x0f    //后续的好多运算都需要与掩码进行位运算
#define ZIP_INT_IMM_MIN 0xf1    /* 11110001 */
#define ZIP_INT_IMM_MAX 0xfd    /* 11111101 */   //最大值不能为11111111，这跟最末尾的结点重复了
#define ZIP_INT_IMM_VAL(v) (v & ZIP_INT_IMM_MASK)

#define INT24_MAX 0x7fffff
#define INT24_MIN (-INT24_MAX - 1)

/* Macro to determine type */
#define ZIP_IS_STR(enc) (((enc) & ZIP_STR_MASK) < ZIP_STR_MASK)

/* Utility macros */
/* 下面是一些用来到时能够直接定位的数值偏移量 */
#define ZIPLIST_BYTES(zl)       (*((uint32_t*)(zl)))
#define ZIPLIST_TAIL_OFFSET(zl) (*((uint32_t*)((zl)+sizeof(uint32_t))))
#define ZIPLIST_LENGTH(zl)      (*((uint16_t*)((zl)+sizeof(uint32_t)*2)))
#define ZIPLIST_HEADER_SIZE     (sizeof(uint32_t)*2+sizeof(uint16_t))
#define ZIPLIST_ENTRY_HEAD(zl)  ((zl)+ZIPLIST_HEADER_SIZE)
#define ZIPLIST_ENTRY_TAIL(zl)  ((zl)+intrev32ifbe(ZIPLIST_TAIL_OFFSET(zl)))
#define ZIPLIST_ENTRY_END(zl)   ((zl)+intrev32ifbe(ZIPLIST_BYTES(zl))-1)


/* 标记列表头节点和尾结点的标识 */
#define ZIPLIST_HEAD 0
#define ZIPLIST_TAIL 1

unsigned char *ziplistNew(void);    //创建新列表
unsigned char *ziplistPush(unsigned char *zl, unsigned char *s, unsigned int slen, int where);  //像列表中推入数据
unsigned char *ziplistIndex(unsigned char *zl, int index);   //索引定位到列表的某个位置
unsigned char *ziplistNext(unsigned char *zl, unsigned char *p);   //获取当前列表位置的下一个值
unsigned char *ziplistPrev(unsigned char *zl, unsigned char *p);   //获取当期列表位置的前一个值
unsigned int ziplistGet(unsigned char *p, unsigned char **sval, unsigned int *slen, long long *lval);   //获取列表的信息
unsigned char *ziplistInsert(unsigned char *zl, unsigned char *p, unsigned char *s, unsigned int slen); //向列表中插入数据
unsigned char *ziplistDelete(unsigned char *zl, unsigned char **p); //列表中删除某个结点
unsigned char *ziplistDeleteRange(unsigned char *zl, unsigned int index, unsigned int num);   //从index索引对应的结点开始算起，删除num个结点
unsigned int ziplistCompare(unsigned char *p, unsigned char *s, unsigned int slen);   //列表间的比较方法
unsigned char *ziplistFind(unsigned char *p, unsigned char *vstr, unsigned int vlen, unsigned int skip); //在列表中寻找某个结点
unsigned int ziplistLen(unsigned char *zl);   //返回列表的长度
size_t ziplistBlobLen(unsigned char *zl);   //返回列表的二进制长度，返回的是字节数
```

### sparkline

是 Redis 中的一种画图的数据结构，叫做微线图，类似于折线图，由一个一个信息结点构成。

sparkline 是一类信息体积小和数据密度高的图表。目前它被用作一些测量，相关的变化的信息呈现的方式，如平均温度，股市交投活跃。sparkline 常常以一组多条的形式出现在柱状图、折线图当中，可以理解为一个图线信息。

```C
#ifndef __SPARKLINE_H
#define __SPARKLINE_H

/* sparkline是一类信息体积小和数据密度高的图表。目前它被用作一些测量，
 *相关的变化的信息呈现的方式，如平均温度，股市交投活跃。sparkline常常以一组多条的形式出现在柱状图，折线图当中。
 *可以理解为一个图线信息 */
/* A sequence is represented of many "samples" */
/* 可以理解为图像上的一个信息点，有文字，有值的大小 */
struct sample {
    double value;
    char *label;
};

/* 图线信息结构体，包括n个元素点，可以据此描述出图，绘图的可不是直接按点和值直接绘制的 */
struct sequence {
	//当前元素点个数
    int length;
    //总共的文字个数，有些点没有label描述，为NULL
    int labels;
    //元素点列表
    struct sample *samples;
    //元素中的最大值，最小值
    double min, max;
};

/* 定义了一些渲染图时候一些属性操作设置 */
#define SPARKLINE_NO_FLAGS 0
#define SPARKLINE_FILL 1      /* Fill the area under the curve. */
#define SPARKLINE_LOG_SCALE 2 /* Use logarithmic scale. */

struct sequence *createSparklineSequence(void);  //创建图线序列结构体
void sparklineSequenceAddSample(struct sequence *seq, double value, char *label); //在图线序列中添加一个信息点
void freeSparklineSequence(struct sequence *seq);   //释放图线序列
sds sparklineRenderRange(sds output, struct sequence *seq, int rows, int offset, int len, int flags); //渲染图线序列为一个图，其实就是得到一个字符串组成的图
sds sparklineRender(sds output, struct sequence *seq, int columns, int rows, int flags); //方法同上，只是少可一个偏移量

#endif /* __SPARKLINE_H */

/* Render part of a sequence, so that render_sequence() call call this function
 * with differnent parts in order to create the full output without overflowing
 * the current terminal columns. */
/* 渲染出这个图线信息 */
sds sparklineRenderRange(sds output, struct sequence *seq, int rows, int offset, int len, int flags) {
    int j;
    double relmax = seq->max - seq->min;
    int steps = charset_len*rows;
    int row = 0;
    char *chars = zmalloc(len);
    int loop = 1;
    int opt_fill = flags & SPARKLINE_FILL;
    int opt_log = flags & SPARKLINE_LOG_SCALE;

    if (opt_log) {
        relmax = log(relmax+1);
    } else if (relmax == 0) {
        relmax = 1;
    }

    while(loop) {
        loop = 0;
        memset(chars,' ',len);
        for (j = 0; j < len; j++) {
            struct sample *s = &seq->samples[j+offset];
            //value派上用处了
            double relval = s->value - seq->min;
            int step;

            if (opt_log) relval = log(relval+1);
            //最后会算出相关的step
            step = (int) (relval*steps)/relmax;
            if (step < 0) step = 0;
            if (step >= steps) step = steps-1;

            if (row < rows) {
                /* Print the character needed to create the sparkline */
                /* step控制输出的字符是哪一个 */
                int charidx = step-((rows-row-1)*charset_len);
                loop = 1;
                if (charidx >= 0 && charidx < charset_len) {
                    chars[j] = opt_fill ? charset_fill[charidx] :
                                          charset[charidx];
                } else if(opt_fill && charidx >= charset_len) {
                	//用"|"填充内容，更加可视化
                    chars[j] = '|';
                }
            } else {
                /* Labels spacing */
                if (seq->labels && row-rows < label_margin_top) {
                    loop = 1;
                    break;
                }
                /* Print the label if needed. */
                if (s->label) {
                    int label_len = strlen(s->label);
                    int label_char = row - rows - label_margin_top;

                    if (label_len > label_char) {
                        loop = 1;
                        chars[j] = s->label[label_char];
                    }
                }
            }
        }
        if (loop) {
            row++;
            output = sdscatlen(output,chars,len);
            output = sdscatlen(output,"\n",1);
        }
    }
    zfree(chars);
    return output;
}
```

### zipmap

压缩图，ziplist 压缩列表和 zipmap 压缩图都采用了动态分配字节的做法表示长度，比如通过固定的字节表示节省了不少的空间。zipmap 其实也就是一个超级长的字符串，用来保存一个个的键值对，通过 key:value key:value 的形式连在一起。

```C
/* String -> String Map data structure optimized for size.
 * This file implements a data structure mapping strings to other strings
 * implementing an O(n) lookup data structure designed to be very memory
 * efficient.
 *
 * The Redis Hash type uses this data structure for hashes composed of a small
 * number of elements, to switch to a hash table once a given number of
 * elements is reached.
 *
 * Given that many times Redis Hashes are used to represent objects composed
 * of few fields, this is a very big win in terms of used memory.
 *
 * zipmap压缩表和ziplist十分类似，都做到了内存操作效率比较高的
 * --------------------------------------------------------------------------
 *
 * Copyright (c) 2009-2010, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* Memory layout of a zipmap, for the map "foo" => "bar", "hello" => "world":
 *
 * <zmlen><len>"foo"<len><free>"bar"<len>"hello"<len><free>"world"
 *
 * <zmlen> is 1 byte length that holds the current size of the zipmap.
 * When the zipmap length is greater than or equal to 254, this value
 * is not used and the zipmap needs to be traversed to find out the length.
 * <zmeln>占有着1个字节，所以他的最多可代表的数量是254,当zipmap中的元素记录超过这个数时，
 * 那只能从前往后后遍历算大小了，和ziplist是不一样的。
 *
 * <len> is the length of the following string (key or value).
 * <len> lengths are encoded in a single value or in a 5 bytes value.
 * If the first byte value (as an unsigned 8 bit value) is between 0 and
 * 252, it's a single-byte length. If it is 253 then a four bytes unsigned
 * integer follows (in the host byte ordering). A value of 255 is used to
 * signal the end of the hash. The special value 254 is used to mark
 * empty space that can be used to add new key/value pairs.
 * <len>代表了后面字符串key 或 value的值的长度，长度一般被编码1个字节或5个字节表示，这个和ziplist类似
 * 如果后面的字符串长度小于等于252个，可与用单字节表示，其他253,254等长度被用来表示其他作用了，当超过这个数时候
 * 则直接按5字节的方式存储长度。
 *
 * <free> is the number of free unused bytes after the string, resulting
 * from modification of values associated to a key. For instance if "foo"
 * is set to "bar", and later "foo" will be set to "hi", it will have a
 * free byte to use if the value will enlarge again later, or even in
 * order to add a key/value pair if it fits.
 * <free>一般来表示后面的value长度的空闲值，当key：value=“foo”:"bar",后来被改为“foo”:"hi"，空闲长度就为1了
 *
 * <free> is always an unsigned 8 bit number, because if after an
 * update operation there are more than a few free bytes, the zipmap will be
 * reallocated to make sure it is as small as possible.
 * <free>的数字一般比较小，如果空闲太大，zipmap会进行调整大小使map整体变得尽可能小
 *
 * The most compact representation of the above two elements hash is actually:
 * 这是一个例子：
 * <zmlen><len>"foo"<len><free>"bar"<len>"hello"<len><free>"world" 
 * <总键值对数><第一个key的长度>key字符<第一个value的长度><空闲长度开始都为0>后面同前
 * "\x02\x03foo\x03\x00bar\x05hello\x05\x00world\xff"
 *
 * Note that because keys and values are prefixed length "objects",
 * the lookup will take O(N) where N is the number of elements
 * in the zipmap and *not* the number of bytes needed to represent the zipmap.
 * This lowers the constant times considerably.
 */

/* Set key to value, creating the key if it does not already exist.
 * If 'update' is not NULL, *update is set to 1 if the key was
 * already preset, otherwise to 0. */
unsigned char *zipmapSet(unsigned char *zm, unsigned char *key, unsigned int klen, unsigned char *val, unsigned int vlen, int *update) {
    unsigned int zmlen, offset;
    unsigned int freelen, reqlen = zipmapRequiredLength(klen,vlen);
    unsigned int empty, vempty;
    unsigned char *p;

    freelen = reqlen;
    if (update) *update = 0;
    //寻找key的位置
    p = zipmapLookupRaw(zm,key,klen,&zmlen);
    if (p == NULL) {
        /* Key not found: enlarge */
        //key的位置没有找到，调整zipmap的大小，准备添加操作
        zm = zipmapResize(zm, zmlen+reqlen);
        p = zm+zmlen-1;
        zmlen = zmlen+reqlen;

        /* Increase zipmap length (this is an insert) */
        //如果头字节还没有达到最大值，则递增
        if (zm[0] < ZIPMAP_BIGLEN) zm[0]++;
    } else {
        /* Key found. Is there enough space for the new value? */
        /* Compute the total length: */
        if (update) *update = 1;
        //key的位置以及找到，判断是否有空间插入新的值
        freelen = zipmapRawEntryLength(p);
        if (freelen < reqlen) {
            /* Store the offset of this key within the current zipmap, so
             * it can be resized. Then, move the tail backwards so this
             * pair fits at the current position. */
             //如果没有空间插入新的值，则调整大小
            offset = p-zm;
            zm = zipmapResize(zm, zmlen-freelen+reqlen);
            p = zm+offset;

            /* The +1 in the number of bytes to be moved is caused by the
             * end-of-zipmap byte. Note: the *original* zmlen is used. */
            //移动空间以便增加新的值
            memmove(p+reqlen, p+freelen, zmlen-(offset+freelen+1));
            zmlen = zmlen-freelen+reqlen;
            freelen = reqlen;
        }
    }

    /* We now have a suitable block where the key/value entry can
     * be written. If there is too much free space, move the tail
     * of the zipmap a few bytes to the front and shrink the zipmap,
     * as we want zipmaps to be very space efficient. */
    empty = freelen-reqlen;
    if (empty >= ZIPMAP_VALUE_MAX_FREE) {
        /* First, move the tail <empty> bytes to the front, then resize
         * the zipmap to be <empty> bytes smaller. */
        offset = p-zm;
        memmove(p+reqlen, p+freelen, zmlen-(offset+freelen+1));
        zmlen -= empty;
        zm = zipmapResize(zm, zmlen);
        p = zm+offset;
        vempty = 0;
    } else {
        vempty = empty;
    }

    /* Just write the key + value and we are done. */
    /* Key: */
    //定位到插入的位置，首先写入key值
    p += zipmapEncodeLength(p,klen);
    memcpy(p,key,klen);
    p += klen;
    /* Value: */
    //key值后面是value值，再次写入
    p += zipmapEncodeLength(p,vlen);
    *p++ = vempty;
    memcpy(p,val,vlen);
    return zm;
}

/* String -> String Map data structure optimized for size.
 *
 * See zipmap.c for more info.
 *
 * --------------------------------------------------------------------------
 *
 * Copyright (c) 2009-2010, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _ZIPMAP_H
#define _ZIPMAP_H

unsigned char *zipmapNew(void);  //创建一个新的压缩图
unsigned char *zipmapSet(unsigned char *zm, unsigned char *key, unsigned int klen, unsigned char *val, unsigned int vlen, int *update); //设置压缩图中的某个键值对
unsigned char *zipmapDel(unsigned char *zm, unsigned char *key, unsigned int klen, int *deleted);  //删除压缩图上的某个键值对
unsigned char *zipmapRewind(unsigned char *zm);   //将在zipmapNext中被调用到
unsigned char *zipmapNext(unsigned char *zm, unsigned char **key, unsigned int *klen, unsigned char **value, unsigned int *vlen); //取得此键值对的下一个键值对
int zipmapGet(unsigned char *zm, unsigned char *key, unsigned int klen, unsigned char **value, unsigned int *vlen); //获取某个键值对
int zipmapExists(unsigned char *zm, unsigned char *key, unsigned int klen); //某个key值在zipmap中是否存在
unsigned int zipmapLen(unsigned char *zm); //zipmap压缩图的总键值对数
size_t zipmapBlobLen(unsigned char *zm); //压缩图的序列化到文件中所需大小
void zipmapRepr(unsigned char *p);  //输出的压缩图的具体信息，用于测试

#endif
```

## Ref

- [http://redisbook.com](http://redisbook.com)
- [https://www.kancloud.cn/digest/redis-code/199030](https://www.kancloud.cn/digest/redis-code/199030)
- [https://blog.csdn.net/androidlushangderen/column/info/redis-code/2](https://blog.csdn.net/androidlushangderen/column/info/redis-code/2)
