
#include "ProceduralTerrainGenerator.h"

#include "ProceduralTerrainChunk.h"

#include <string>

ProceduralTerrainGenerator::ProceduralTerrainGenerator() : mainThreadProfiler(std::string("idle")), workerThreadProfiler(std::string("idle"))
{
	// UE_LOG(LogTemp, Warning, TEXT("[GAME] Thread starting..."));
	thread = FRunnableThread::Create(this, TEXT("ProceduralTerrainGeneratorThread"));
}

ProceduralTerrainGenerator::~ProceduralTerrainGenerator()
{
	if (thread)
	{
		thread->Kill();
		delete thread;
	}
}

void ProceduralTerrainGenerator::asyncGenerate(UProceduralTerrainChunk *chunk)
{
	// UE_LOG(LogTemp, Warning, TEXT("[GAME] Chunk queued."));
	chunkQueue.Enqueue(chunk);
}

bool ProceduralTerrainGenerator::Init()
{
	// UE_LOG(LogTemp, Warning, TEXT("[WORKER] Thread started."));
	return true;
}

uint32 ProceduralTerrainGenerator::Run()
{
	while (running)
	{

		if (chunkQueue.IsEmpty())
		{
			FPlatformProcess::Sleep(0.001);
			continue;
		}

		FDateTime start = FDateTime::Now();

		UProceduralTerrainChunk *chunk;
		chunkQueue.Dequeue(chunk);
		chunk->generate();

		FDateTime end = FDateTime::Now();
		// UE_LOG(LogTemp, Warning, TEXT("[WORKER] Chunk dequeued. (%d ms)"), (end-start).GetFractionMilli());
	}

	// UE_LOG(LogTemp, Warning, TEXT("[WORKER] Thread stopped."));
	return 0;
}

void ProceduralTerrainGenerator::Stop()
{
	// UE_LOG(LogTemp, Warning, TEXT("[GAME] Thread stopping..."));
	running = false;
}
