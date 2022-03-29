
#include "Profiler.h"

Profiler::Profiler(std::string startingTaskName) : taskCount(), taskNano()
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
	int duration = (currentTime - currentTaskStart).GetFractionNano();
	if (taskNano.count(currentTaskName) == 0)
		taskNano.emplace(currentTaskName, duration);
	else
		taskNano.at(currentTaskName) += duration;

	// Start new task
	currentTaskName = taskName;
	currentTaskStart = currentTime;
}

void Profiler::print() const
{
	for (auto const &p : taskCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("[PROFILER]: %s count: %d"), p.first.c_str(), p.second);
	}
}
