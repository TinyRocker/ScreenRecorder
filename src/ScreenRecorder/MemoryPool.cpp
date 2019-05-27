#include "MemoryPool.h"
#include "MediaFormat.h"
#include "glog/logging.h"

MemoryPool::MemoryPool(int poolsize, uint64_t datasize)
{
    std::lock_guard<std::mutex> lck(m_lock);

    LOG(INFO) << "new " << poolsize << " * " << datasize << " bytes memory!";

    for (int i = 0; i < poolsize; ++i)
    {
        FrameData *frame = new FrameData;
        frame->data = new char[datasize];
        if (!frame->data)
        {
            delete frame;
            LOG(ERROR) << "new " << datasize << " bytes failed!";
            continue;
        }

        m_pool[frame] = false;     // 记录内存空间及使用状态
    }
    LOG(INFO) << "real pool size " << m_pool.size();
}

MemoryPool::~MemoryPool()
{
    std::lock_guard<std::mutex> lck(m_lock);

    for (auto iter = m_pool.begin(); iter != m_pool.end();)
    {
        delete iter->first->data;
        delete iter->first;
        iter = m_pool.erase(iter);
    }
}

FrameData * MemoryPool::allocMemory()
{
    std::lock_guard<std::mutex> lck(m_lock);

    for (auto iter = m_pool.begin(); iter != m_pool.end(); ++iter)
    {
        if (false == iter->second)  // 遍历寻找未使用的内存块
        {
            iter->second = true;    // 记录使用
            return iter->first;
        }
    }
    LOG(DETAIL) << "memory pool not has unused memory! pool size:" << m_pool.size();

    return nullptr;
}

void MemoryPool::freeMemory(FrameData * p)
{
    std::lock_guard<std::mutex> lck(m_lock);

    auto iter = m_pool.find(p);
    if (iter != m_pool.end())
    {
        iter->second = false;   // 消除使用记录
    }
}

void MemoryPool::clean()
{
    std::lock_guard<std::mutex> lck(m_lock);

    for (auto iter = m_pool.begin(); iter != m_pool.end(); iter++)
    {
        iter->second = false;
    }
}