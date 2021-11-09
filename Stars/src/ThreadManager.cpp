
#include "ThreadManager.h"

ThreadManager::ThreadManager(int LoadingThreads)
{
    stopped = false;
    terminate = false;
    maxLoadWorkers = LoadingThreads;

    for (int i = 0; i < maxLoadWorkers; i++)
    {
        LoadWorkers.push_back(std::thread(&ThreadManager::InitLoad, this));
    }

}
ThreadManager::~ThreadManager()
{
    if (!stopped)
        Shutdown();
}
void ThreadManager::QueueLoad(std::function<void()> newJob)
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        jobsLoad.push(newJob);
    }
    condition.notify_one();
}

void ThreadManager::InitLoad()
{
    std::function<void()> Job;
    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(queueMutex);

            condition.wait(lock, [&] {return !jobsLoad.empty() || terminate; });
            Job = jobsLoad.front();
            jobsLoad.pop();
        }
        Job();
    }
};

void ThreadManager::Shutdown() 
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        terminate = true; 
    }

    condition.notify_all();

    for (std::thread& everyThread : LoadWorkers)
    {
        everyThread.join();
    }

    LoadWorkers.clear();
    stopped = true; 
}
