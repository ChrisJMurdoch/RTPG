
#pragma once

#include "CoreMinimal.h"

#include <string>
#include <map>

class RTPG_API Profiler
{
private:
	std::map<std::string, int> taskCount, taskNano;
	FDateTime currentTaskStart;
	std::string currentTaskName;

public:
	Profiler(std::string startingTaskName);
	~Profiler();
	void switchTask(std::string taskName);
	void print() const;
};
