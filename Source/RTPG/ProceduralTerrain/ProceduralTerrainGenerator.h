
#pragma once

#include "CoreMinimal.h"
#include "Profiler.h"

class UProceduralTerrainChunk;

class RTPG_API ProceduralTerrainGenerator : public FRunnable
{
private:
	Profiler mainThreadProfiler, workerThreadProfiler;
	volatile bool running = true;
	FRunnableThread *thread{0};
	TQueue<UProceduralTerrainChunk *> chunkQueue{};

public:
	ProceduralTerrainGenerator();
	~ProceduralTerrainGenerator();

	void asyncGenerate(UProceduralTerrainChunk *chunk);

	bool Init() override;
	uint32 Run() override;
	void Stop() override;
};
