
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralTerrainChunk.generated.h"

class AProceduralTerrain;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTPG_API UProceduralTerrainChunk : public UProceduralMeshComponent
{
	GENERATED_BODY()

private:
	AProceduralTerrain *actor;

public:
	void generate();

private:
	UProceduralTerrainChunk(FObjectInitializer const &ObjectIn);

	TArray<FVector> generateVertices();
	TArray<int32> generateIndices();
	TArray<FVector> generateNormals(TArray<FVector> const &vertices, TArray<int32> const &indices);
	TArray<FVector2D> generateUVs();
};
