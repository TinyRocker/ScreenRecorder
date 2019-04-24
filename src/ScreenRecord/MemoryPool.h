#pragma once
#include <map>
#include <queue>
#include <mutex>

class MemoryPool
{
public:
    MemoryPool(uint64_t poolsize, uint64_t datasize);
    virtual ~MemoryPool();

    void* allocMemory();
    void freeMemory(void *p);
    void clean();
private:
    std::mutex m_lock;
    std::queue<void*> m_pool;        // 未使用内存池队列
    std::map<void*, bool> m_used;    // 已使用内存索引
    uint64_t m_poolsize = 0;
};