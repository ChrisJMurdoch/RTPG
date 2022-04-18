
#pragma once

#include "CoreMinimal.h"

#include <string>
#include <map>

class RTPG_API Profiler
{
private:
	std::map<std::string, float> taskCount, taskMilli;
	FDateTime currentTaskStart;
	std::string currentTaskName;

public:
	Profiler(std::string startingTaskName);
	~Profiler();
	void switchTask(std::string taskName);
	void print() const;
};
