#pragma once

#include <atomic>
#include <queue>
#include <mutex>
#include <iostream>

class SpinLock
{
private:
    std::atomic_flag IsLocked;
public:
    SpinLock();
    void Lock();
    void Unlock();

    bool TryLock();
};


class PriorityLock
{
private:
    std::mutex Mtx;
    std::priority_queue<int> Priorities;
    std::atomic_int LockedLevel;
public:
    PriorityLock();
    void Lock(int priority = 1);
    void Unlock();
};

class DelayedLock
{
private:
    std::atomic_int LockedLevel;
    bool IsDelayedLock = false;
public:
    DelayedLock();

    void Lock();
    void Unlock();

    // Non blocking call
    void LockDelayed();
    void UnlockDelayed();

    // Return true if this thread was blocked using LockDelayed.
    // It may be in 2 situations. 
    // 1 - LockDelayed was called without previous Lock in different thread
    // 2 - LockDelayed was called after Lock and Unlock in different thread
    // Use it only in same thread as LockDelayed.
    bool GetIsDelayedLock();
};