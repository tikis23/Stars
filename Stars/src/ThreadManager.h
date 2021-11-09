#pragma once
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadManager
{
public:
	ThreadManager(int LoadingThreads);
	~ThreadManager(); 

	void QueueLoad(std::function<void()> NewJob);

private:
	void InitLoad();

	void Shutdown();

	int maxLoadWorkers;
	std::queue<std::function<void()>>jobsLoad;

	std::vector<std::thread> LoadWorkers;

	std::mutex queueMutex;
	std::condition_variable condition;
	bool terminate;
	bool stopped;
};


