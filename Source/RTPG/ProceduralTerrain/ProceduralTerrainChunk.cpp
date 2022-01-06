
#include "ProceduralTerrainChunk.h"

#include "ProceduralTerrain.h"

#include "../PerlinNoise/PerlinNoise.h"

UProceduralTerrainChunk::UProceduralTerrainChunk(FObjectInitializer const &ObjectIn) : UProceduralMeshComponent(ObjectIn)
{
	actor = static_cast<AProceduralTerrain *>(GetOwner());
}

// Procedurally generate mesh
void UProceduralTerrainChunk::generate()
{
	// Generate mesh data
	TArray<FVector> vertices = generateVertices();
	TArray<int32> indices = generateIndices();
	TArray<FVector> normals = generateNormals(vertices, indices);
	TArray<FVector2D> UVs = generateUVs();

	// Optional parameters
	TArray<FLinearColor> colours;
	TArray<FProcMeshTangent> tangents;

	// Compile mesh component
	CreateMeshSection_LinearColor(0, vertices, indices, normals, UVs, colours, tangents, true);
	if (actor->material)
		SetMaterial(0, actor->material);
}

// Generate vertices for a mesh of a given size
TArray<FVector> UProceduralTerrainChunk::generateVertices()
{
	// Offset in terms of number of chunks
	FVector worldOffset = GetComponentLocation();
	FVector2D chunkOffset = FVector2D(worldOffset.X, worldOffset.Y) / actor->chunkSize;

	// Create vertex array
	TArray<FVector> vertices;
	vertices.Reserve(actor->resolution*actor->resolution);

	// Calculate height for each vertex
	for (int x=0; x<actor->resolution; x++)
	{
		float xPos = float(x) / (actor->resolution-1);
		for (int y=0; y<actor->resolution; y++)
		{
			float yPos = float(y) / (actor->resolution-1);
			FVector2D samplePos = (FVector2D(xPos, yPos) + chunkOffset) * actor->frequency;
			float zPos = PerlinNoise::fBm(samplePos.X, samplePos.Y, actor->octaves) / actor->frequency;
			vertices.Add(FVector(xPos, yPos, zPos) * actor->chunkSize);
		}
	}

	return vertices;
}

// Generate indices for a mesh of a given size
TArray<int32> UProceduralTerrainChunk::generateIndices()
{
	// Create index array
	TArray<int32> indices;
	int quadDimension = actor->resolution-1;
	indices.Reserve(quadDimension*quadDimension*2*3);

	// Iterate each vertex, excluding last row
	int n = actor->resolution * (actor->resolution-1);
	for (int i=0; i<n; i++)
	{
		// If vertex is left corner of a quad (i.e. has vertex to right)
		if ((i+1)%actor->resolution!=0)
		{
			// Triangle 1
			indices.Add(i);
			indices.Add(i+1);
			indices.Add(i+actor->resolution+1);

			// Triangle 2
			indices.Add(i+actor->resolution+1);
			indices.Add(i+actor->resolution);
			indices.Add(i);
		}
	}

	return indices;
}

// Generate normals for a given mesh
TArray<FVector> UProceduralTerrainChunk::generateNormals(TArray<FVector> const &vertices, TArray<int32> const &indices)
{
	// Accumulate relevant triangle normals for each vertex
	TArray<FVector> accumulatedNormals;
	accumulatedNormals.Init(FVector(0, 0, 0), vertices.Num());

	// For each triangle
	for (int i=0; i<indices.Num(); i+=3)
	{
		int aI=indices[i+0], bI=indices[i+1], cI=indices[i+2];

		// Calculate triangle normal
		FVector a = vertices[aI], b=vertices[bI], c=vertices[cI];
		FVector normal = FVector::CrossProduct(c-a, b-a);

		// Accumulate vertex normals
		accumulatedNormals[aI] += normal;
		accumulatedNormals[bI] += normal;
		accumulatedNormals[cI] += normal;
	}

	// Normalise vectors
	for (int i=0; i<accumulatedNormals.Num(); i++)
		accumulatedNormals[i] /= accumulatedNormals[i].Size();

	return accumulatedNormals;
}

// Generate UVs for a mesh of a given size
TArray<FVector2D> UProceduralTerrainChunk::generateUVs()
{
	// Create vertex array
	TArray<FVector2D> uvs;
	uvs.Reserve(actor->resolution*actor->resolution);

	// Calculate height for each vertex
	for (int x=0; x<actor->resolution; x++)
	{
		float xPos = float(x) / (actor->resolution-1);
		for (int y=0; y<actor->resolution; y++)
		{
			float yPos = float(y) / (actor->resolution-1);
			uvs.Add(FVector2D(xPos, yPos));
		}
	}

	return uvs;
}
