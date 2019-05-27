#pragma once
#include <map>
#include <mutex>

class FrameData;

class MemoryPool
{
public:
    MemoryPool(int poolsize, uint64_t datasize);
    virtual ~MemoryPool();

    FrameData* allocMemory();
    void freeMemory(FrameData *p);
    void clean();
private:
    std::mutex m_lock;
    std::map<FrameData*, bool> m_pool;    // 内存索引,已使用标记为true
};