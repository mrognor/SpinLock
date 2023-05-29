#include "SpinLock.h"

SpinLock::SpinLock() : IsLocked(false) {}
    
void SpinLock::Lock()
{
    while (IsLocked.test_and_set());
}

void SpinLock::Unlock()
{
    IsLocked.clear();
}

bool SpinLock::TryLock()
{
    return IsLocked.test_and_set();
}


PriorityLock::PriorityLock()
{
    LockedLevel.store(0);
}
    
void PriorityLock::Lock(int priority)
{
    Mtx.lock();
    if (LockedLevel.load() == 0) 
        LockedLevel.store(priority);
    else
        Priorities.push(priority);
    Mtx.unlock();

    int Priority = priority;
    while (!LockedLevel.compare_exchange_weak(Priority, -1)) Priority = priority;
}

void PriorityLock::Unlock()
{
    Mtx.lock();
    if (Priorities.size() == 0)
        LockedLevel = 0;
    else
    {
        LockedLevel.store(Priorities.top());
        Priorities.pop();
    }
    Mtx.unlock();
}


DelayedLock::DelayedLock()
{
    LockedLevel.store(0);
}

void DelayedLock::Lock()
{
    int Zero = 0;
    while (!LockedLevel.compare_exchange_weak(Zero, 1)) Zero = 0;
}

void DelayedLock::Unlock()
{
    int One = 1, OneMinus = -1;
    LockedLevel.compare_exchange_strong(One, 0);
    LockedLevel.compare_exchange_strong(OneMinus, -2);
}

void DelayedLock::LockDelayed()
{
    int a = LockedLevel.exchange(-1);
    if (a == 0) LockedLevel.store(-2);
    IsDelayedLock = true;
}

void DelayedLock::UnlockDelayed()
{
    IsDelayedLock = false;
    LockedLevel.store(0);
}

bool DelayedLock::GetIsDelayedLock()
{
    return IsDelayedLock && (LockedLevel.load() == -2);
}