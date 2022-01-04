
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralTerrain.generated.h"

UCLASS()
class RTPG_API AProceduralTerrain : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		bool generateWhileMoving = true; // Generate terrain every tick while moving
	UPROPERTY(EditAnywhere)
		int frequency = 1; // Perlin Noise frequency
	UPROPERTY(EditAnywhere)
		int chunkSize = 1000; // Size of each chunk in centimeters
	UPROPERTY(EditAnywhere)
		int octaves = 5; // Number of layers used in fBm
	UPROPERTY(EditAnywhere)
		int resolution = 25; // Dimension of vertices for each chunk
	UPROPERTY(EditAnywhere)
		int seed = 42; // Pseudorandom seed for terrain generation
	UPROPERTY(EditAnywhere)
		UMaterialInterface *material; // Visible mesh material
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent *mesh; // Mesh object

private:
	AProceduralTerrain();

	virtual void PostActorCreated();
	virtual void PostLoad();
	virtual void PostEditChangeProperty(FPropertyChangedEvent &event);
	virtual void PostEditMove(bool bFinished);

	void generateMesh();

	TArray<FVector> generateVertices();
	TArray<int32> generateIndices();
	TArray<FVector> generateNormals(TArray<FVector> const &vertices, TArray<int32> const &indices);
	TArray<FVector2D> generateUVs();
};
