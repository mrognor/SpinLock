#include <thread>
#include <iostream>
#include <unistd.h>

#include "SpinLock.h"

// SpinLock
/*
int Count = 0;
SpinLock lk;

void ThreadFunc()
{
    for (int i = 0; i < 100000; ++i)
    {
        lk.Lock();
        ++Count;
        lk.Unlock();
    }
}

int main()
{
    std::thread th1(ThreadFunc);
    std::thread th2(ThreadFunc);
    std::thread th3(ThreadFunc);
    std::thread th4(ThreadFunc);

    th1.join();
    th2.join();
    th3.join();
    th4.join();

    std::cout << "Count: " << Count << std::endl;
}
*/

// Priority Lock
/*
PriorityLock Lk;

void PriorityTask1()
{
    Lk.Lock();
    std::cout << "Priority 1" << std::endl;
    Lk.Unlock();
}

void PriorityTask2()
{
    Lk.Lock(2);
    std::cout << "Priority 2" << std::endl;
    Lk.Unlock();
}

void PriorityTask3()
{
    Lk.Lock(7);
    std::cout << "Priority 7" << std::endl;
    Lk.Unlock();
}

int main()
{
    Lk.Lock();
    std::thread th1(PriorityTask1);
    std::thread th2(PriorityTask1);
    std::thread th3(PriorityTask2);
    std::thread th4(PriorityTask3);
    std::thread th5(PriorityTask2);

    usleep(50);
    
    Lk.Unlock();

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
}
*/

DelayedLock Lk;

void ThreadTask()
{
    Lk.Lock();
    std::cout << "Start task in thread: " << std::this_thread::get_id() << std::endl;
    sleep(5);
    std::cout << "End task in thread: " << std::this_thread::get_id() << std::endl;
    Lk.Unlock();

    Lk.Lock();
    std::cout << "Start task 2 in thread: " << std::this_thread::get_id() << std::endl;
    sleep(5);
    std::cout << "End task 2 in thread: " << std::this_thread::get_id() << std::endl;
    Lk.Unlock();
}

void DelayedTask()
{
    sleep(2);
    Lk.LockDelayed();
    std::cout << "Delayed lock in thread: " << std::this_thread::get_id() << std::endl;
    while(!Lk.GetIsDelayedLock());
    std::cout << "Delayed task in thread: " << std::this_thread::get_id() << std::endl;
    Lk.UnlockDelayed();
}

int main()
{
    std::thread th1(ThreadTask);
    std::thread th2(ThreadTask);
    std::thread th3(DelayedTask);

    th1.join();
    th2.join();
    th3.join();
}