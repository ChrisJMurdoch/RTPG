
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralTerrainGenerator.h"
#include "ProceduralTerrain.generated.h"

class UProceduralTerrainChunk;

UCLASS()
class RTPG_API AProceduralTerrain : public AActor
{
	GENERATED_BODY()

private:
	friend class UProceduralTerrainChunk;
	friend class ProceduralTerrainGenerator;

	UPROPERTY(EditAnywhere) int maxBlocksRegisteredPerTick = 1;
	UPROPERTY(EditAnywhere) float renderDistance = 10;
	UPROPERTY(EditAnywhere) float frequency = 0.5;
	UPROPERTY(EditAnywhere) int chunkSize = 500;
	UPROPERTY(EditAnywhere) int octaves = 10;
	UPROPERTY(EditAnywhere) int resolution = 75;
	UPROPERTY(EditAnywhere) UMaterialInterface *material;

	UPROPERTY(VisibleAnywhere) FString debug;
	UPROPERTY(VisibleAnywhere) float playerX = 0;
	UPROPERTY(VisibleAnywhere) int chunksSpawned = 0;
	UPROPERTY(VisibleAnywhere) int chunksDespawned = 0;
	
	TMap<FIntPoint, UProceduralTerrainChunk *> meshes{};
	ProceduralTerrainGenerator generator;

	int blocksRegisteredThisTick = 0;

private:
	AProceduralTerrain();

	virtual void PostEditChangeProperty(FPropertyChangedEvent &event);
	virtual void Tick(float deltaSeconds);

	void syncChunks(FVector2D playerPosition);
	void clearChunks();

	void spawnChunk(FIntPoint location);
	void despawnChunk(FIntPoint location);
};
