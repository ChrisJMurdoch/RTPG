
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralTerrain.generated.h"

class UProceduralTerrainChunk;

UCLASS()
class RTPG_API AProceduralTerrain : public AActor
{
	GENERATED_BODY()

private:
	friend class UProceduralTerrainChunk;

	UPROPERTY(EditAnywhere) float renderDistance = 2.5;
	UPROPERTY(EditAnywhere) int frequency = 1;
	UPROPERTY(EditAnywhere) int chunkSize = 1000;
	UPROPERTY(EditAnywhere) int octaves = 5;
	UPROPERTY(EditAnywhere) int resolution = 25;
	UPROPERTY(EditAnywhere) UMaterialInterface *material;

	UPROPERTY(VisibleAnywhere) FString debug;
	UPROPERTY(VisibleAnywhere) float playerX = 0;
	UPROPERTY(VisibleAnywhere) int chunksSpawned = 0;
	UPROPERTY(VisibleAnywhere) int chunksDespawned = 0;
	UPROPERTY(VisibleAnywhere) UProceduralTerrainChunk *mesh;
	UPROPERTY(VisibleAnywhere) TMap<FIntPoint, UProceduralTerrainChunk *> meshes;

private:
	AProceduralTerrain();

	virtual void PostRegisterAllComponents();
	virtual void PostEditChangeProperty(FPropertyChangedEvent &event);
	virtual void Tick(float deltaSeconds);

	void syncChunks(FVector2D playerPosition);
	void clearChunks();

	void spawnChunk(FIntPoint location);
	void despawnChunk(FIntPoint location);
};
