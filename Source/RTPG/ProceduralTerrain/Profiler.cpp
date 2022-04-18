
#include "Profiler.h"
#include <iostream>

Profiler::Profiler(std::string startingTaskName) : taskCount(), taskMilli()
{
	currentTaskName = startingTaskName;
	currentTaskStart = FDateTime::Now();
}

Profiler::~Profiler()
{

}

void Profiler::switchTask(std::string taskName)
{
	// End last task
	if (taskCount.count(currentTaskName) == 0)
		taskCount.emplace(currentTaskName, 1);
	else
		taskCount.at(currentTaskName)++;

	FDateTime currentTime = FDateTime::Now();
	float duration = (currentTime - currentTaskStart).GetFractionNano() / 1000000.0f;
	if (taskMilli.count(currentTaskName) == 0.0f)
		taskMilli.emplace(currentTaskName, duration);
	else
		taskMilli.at(currentTaskName) += duration;

	// Start new task
	currentTaskName = taskName;
	currentTaskStart = currentTime;
}

void Profiler::print() const
{
	// UE_LOG(LogTemp, Warning, TEXT("[PROFILER]:"));
	for (std::pair<std::string const, float> const &pair : taskCount)
	{
		FString task(pair.first.c_str());
		int count = pair.second;
		float milli = taskMilli.at(pair.first);
		UE_LOG(LogTemp, Warning, TEXT(" - Task '%s'(%d): avg. ms: %.2f"), *task, count, milli / count);

		char const *name = "Chris";

		// Printing a name in standard C++
		std::cout << "Hello, " << name << std::endl;

		// Printing a name in Unreal Engine
		FString unrealString{name};
		UE_LOG(LogTemp, Log, TEXT("Hello, %s"), *unrealString);
	}
}
