
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralTerrainChunk.generated.h"

class AProceduralTerrain;
class ProceduralTerrainGenerator;

struct MeshData
{
	bool ready = false;
	TArray<FVector> vertices{};
	TArray<int32> indices{};
	TArray<FVector> normals{};
	TArray<FVector2D> uvs{};
	TArray<FLinearColor> colours{};
	TArray<FProcMeshTangent> tangents{};

	void clearData() {
		vertices.Empty();
		indices.Empty();
		normals.Empty();
		uvs.Empty();
		colours.Empty();
		tangents.Empty();
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTPG_API UProceduralTerrainChunk : public UProceduralMeshComponent
{
	GENERATED_BODY()

private:
	AProceduralTerrain *actor{0};
	MeshData meshData;

public:
	void init(ProceduralTerrainGenerator &generator);
	void generate();
	void tick();

private:
	UProceduralTerrainChunk(FObjectInitializer const &ObjectIn);

	void generateVertices(FVector componentLocation, int resolution, float chunkSize, float frequency, int octaves);
	void generateIndices(int resolution);
	void generateNormals();
	void generateUVs(int resolution);
	void generateColours(FVector componentLocation, int resolution, float chunkSize, float frequency);
};
