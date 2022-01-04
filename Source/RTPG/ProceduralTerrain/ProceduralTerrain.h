
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

	UPROPERTY(EditAnywhere) bool generateWhileMoving = true;
	UPROPERTY(EditAnywhere) int frequency = 1;
	UPROPERTY(EditAnywhere) int chunkSize = 1000;
	UPROPERTY(EditAnywhere) int octaves = 5;
	UPROPERTY(EditAnywhere) int resolution = 25;
	UPROPERTY(EditAnywhere) int seed = 42;
	UPROPERTY(EditAnywhere) UMaterialInterface *material;

	UPROPERTY(VisibleAnywhere) UProceduralTerrainChunk *mesh;
	UPROPERTY(VisibleAnywhere) TMap<FIntPoint, UProceduralTerrainChunk *> meshes;

private:
	AProceduralTerrain();

	void spawnChunk(FIntPoint location);
	void despawnChunk(FIntPoint location);

	virtual void PostRegisterAllComponents();

	virtual void PostEditChangeProperty(FPropertyChangedEvent &event);
	virtual void PostEditMove(bool bFinished);
};
