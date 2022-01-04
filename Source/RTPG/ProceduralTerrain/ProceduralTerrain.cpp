
#include "ProceduralTerrain.h"

#include "../PerlinNoise/PerlinNoise.h"

AProceduralTerrain::AProceduralTerrain()
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	RootComponent = mesh;
}

// Generate mesh when created, loaded or edited
void AProceduralTerrain::PostActorCreated()
{
	Super::PostActorCreated();
	generateMesh();
}
void AProceduralTerrain::PostLoad()
{
	Super::PostLoad();
	generateMesh();
}
void AProceduralTerrain::PostEditChangeProperty(FPropertyChangedEvent &event)
{
	Super::PostEditChangeProperty(event);
	generateMesh();
}
void AProceduralTerrain::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);
	if (bFinished || generateWhileMoving)
		generateMesh();
}

void AProceduralTerrain::generateMesh()
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
	mesh->CreateMeshSection_LinearColor(0, vertices, indices, normals, UVs, colours, tangents, true);
	if (material)
		mesh->SetMaterial(0, material);
}

// Generate vertices for a mesh of a given size
TArray<FVector> AProceduralTerrain::generateVertices()
{
	// Offset in terms of number of chunks
	FVector worldOffset = mesh->GetComponentLocation();
	FVector2D chunkOffset = FVector2D(worldOffset.X, worldOffset.Y) / chunkSize;

	// Create vertex array
	TArray<FVector> vertices;
	vertices.Reserve(resolution*resolution);

	// Calculate height for each vertex
	for (int x=0; x<resolution; x++)
	{
		float xPos = float(x) / (resolution-1);
		for (int y=0; y<resolution; y++)
		{
			float yPos = float(y) / (resolution-1);
			FVector2D samplePos = (FVector2D(xPos, yPos) + chunkOffset) * frequency;
			float zPos = PerlinNoise::fBm(samplePos.X, samplePos.Y, octaves, seed) / frequency;
			vertices.Add(FVector(xPos, yPos, zPos) * chunkSize);
		}
	}

	return vertices;
}

// Generate indices for a mesh of a given size
TArray<int32> AProceduralTerrain::generateIndices()
{
	// Create index array
	TArray<int32> indices;
	int quadDimension = resolution-1;
	indices.Reserve(quadDimension*quadDimension*2*3);

	// Iterate each vertex, excluding last row
	int n = resolution * (resolution-1);
	for (int i=0; i<n; i++)
	{
		// If vertex is left corner of a quad (i.e. has vertex to right)
		if ((i+1)%resolution!=0)
		{
			// Triangle 1
			indices.Add(i);
			indices.Add(i+1);
			indices.Add(i+resolution+1);

			// Triangle 2
			indices.Add(i+resolution+1);
			indices.Add(i+resolution);
			indices.Add(i);
		}
	}

	return indices;
}

// Generate normals for a given mesh
TArray<FVector> AProceduralTerrain::generateNormals(TArray<FVector> const &vertices, TArray<int32> const &indices)
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
TArray<FVector2D> AProceduralTerrain::generateUVs()
{
	// Create vertex array
	TArray<FVector2D> uvs;
	uvs.Reserve(resolution*resolution);

	// Calculate height for each vertex
	for (int x=0; x<resolution; x++)
	{
		float xPos = float(x) / (resolution-1);
		for (int y=0; y<resolution; y++)
		{
			float yPos = float(y) / (resolution-1);
			uvs.Add(FVector2D(xPos, yPos));
		}
	}

	return uvs;
}
