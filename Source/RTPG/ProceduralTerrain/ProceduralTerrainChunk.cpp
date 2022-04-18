
#include "ProceduralTerrainChunk.h"

#include "ProceduralTerrain.h"
#include "ProceduralTerrainGenerator.h"
#include "Profiler.h"
#include "Biomes.h"

UProceduralTerrainChunk::UProceduralTerrainChunk(FObjectInitializer const &ObjectIn) : UProceduralMeshComponent(ObjectIn), meshData()
{
	actor = static_cast<AProceduralTerrain *>(GetOwner());
	bUseAsyncCooking = false;
	bUseComplexAsSimpleCollision = true;
}

// Register with async generator
void UProceduralTerrainChunk::init(ProceduralTerrainGenerator &generator)
{
	generator.asyncGenerate(this);
}

// Worker thread - Generate mesh data
void UProceduralTerrainChunk::generate()
{
	// Profile task
	static Profiler workerProfiler(std::string("idle"));

	// Generate mesh data
	workerProfiler.switchTask(std::string("generating"));
	generateVertices(GetComponentLocation(), actor->resolution, actor->chunkSize, actor->frequency, actor->octaves);
	generateIndices(actor->resolution);
	generateNormals();
	generateUVs(actor->resolution);
	generateColours(GetComponentLocation(), actor->resolution, actor->chunkSize, actor->frequency);
	workerProfiler.switchTask(std::string("idle"));

	// Set ready flag
	meshData.ready = true;
	UE_LOG(LogTemp, Warning, TEXT("[WORKER]: Profile:"));
	workerProfiler.print();
}

// Game thread - Create and register mesh from generated data
void UProceduralTerrainChunk::tick()
{
	// Profile task
	static Profiler mainProfiler(std::string("starting"));

	// Create mesh from data
	if (meshData.ready && actor->blocksRegisteredThisTick < actor->maxBlocksRegisteredPerTick)
	{
		meshData.ready = false;
		actor->blocksRegisteredThisTick++;

		FDateTime start = FDateTime::Now();

		// Create mesh
		mainProfiler.switchTask(std::string("creating mesh"));
		CreateMeshSection_LinearColor(0, meshData.vertices, meshData.indices, meshData.normals, meshData.uvs, meshData.colours, meshData.tangents, true);
		if (actor->material)
			SetMaterial(0, actor->material);

		// Free memory
		meshData.clearData();

		// Register mesh
		mainProfiler.switchTask(std::string("registering component"));
		RegisterComponent();

		FDateTime end = FDateTime::Now();
		// UE_LOG(LogTemp, Warning, TEXT("[GAME] Chunk registered. (%d ms)"), (end-start).GetFractionMilli());

		UE_LOG(LogTemp, Warning, TEXT("[MAIN]: Profile:"));
		mainProfiler.print();
	}
	
	mainProfiler.switchTask(std::string("busy"));
}

// Generate vertices for a mesh of a given size
void UProceduralTerrainChunk::generateVertices(FVector componentLocation, int resolution, float chunkSize, float frequency, int octaves)
{
	// Init vertex array
	meshData.vertices.Empty();
	meshData.vertices.Reserve(resolution*resolution);

	// Offset in terms of number of chunks
	FVector2D chunkOffset = FVector2D(componentLocation.X, componentLocation.Y) / chunkSize;

	// Calculate height for each vertex
	for (int x=0; x<resolution; x++)
	{
		float xPos = float(x) / (resolution-1);
		for (int y=0; y<resolution; y++)
		{
			float yPos = float(y) / (resolution-1);
			FVector2D samplePos = (FVector2D(xPos, yPos) + chunkOffset) * frequency;
			float zPos = Biomes::height(samplePos.X, samplePos.Y, octaves) / frequency;
			meshData.vertices.Add(FVector(xPos, yPos, zPos) * chunkSize);
		}
	}
}

// Generate indices for a mesh of a given size
void UProceduralTerrainChunk::generateIndices(int resolution)
{
	// Init index array
	int quadDimension = resolution-1;
	meshData.indices.Empty();
	meshData.indices.Reserve(quadDimension*quadDimension*2*3);

	// Iterate each vertex, excluding last row
	int n = resolution * (resolution-1);
	for (int i=0; i<n; i++)
	{
		// If vertex is left corner of a quad (i.e. has vertex to right)
		if ((i+1)%resolution!=0)
		{
			// Triangle 1
			meshData.indices.Add(i);
			meshData.indices.Add(i+1);
			meshData.indices.Add(i+resolution+1);

			// Triangle 2
			meshData.indices.Add(i+resolution+1);
			meshData.indices.Add(i+resolution);
			meshData.indices.Add(i);
		}
	}
}

// Generate normals for a given mesh
void UProceduralTerrainChunk::generateNormals()
{
	// Init normals array
	meshData.normals.Empty();
	meshData.normals.Init(FVector(0, 0, 0), meshData.vertices.Num());

	// For each triangle
	for (int i=0; i<meshData.indices.Num(); i+=3)
	{
		int aI=meshData.indices[i+0], bI=meshData.indices[i+1], cI=meshData.indices[i+2];

		// Calculate triangle normal
		FVector a = meshData.vertices[aI], b=meshData.vertices[bI], c=meshData.vertices[cI];
		FVector normal = FVector::CrossProduct(c-a, b-a);

		// Accumulate vertex normals
		meshData.normals[aI] += normal;
		meshData.normals[bI] += normal;
		meshData.normals[cI] += normal;
	}

	// Normalise vectors
	for (int i=0; i<meshData.normals.Num(); i++)
		meshData.normals[i] /= meshData.normals[i].Size();
}

// Generate UVs for a mesh of a given size
void UProceduralTerrainChunk::generateUVs(int resolution)
{
	// Init UV array
	meshData.uvs.Empty();
	meshData.uvs.Reserve(resolution*resolution);

	for (int x=0; x<resolution; x++)
	{
		float xPos = float(x) / (resolution-1);
		for (int y=0; y<resolution; y++)
		{
			float yPos = float(y) / (resolution-1);
			meshData.uvs.Add(FVector2D(xPos, yPos));
		}
	}
}

// Generate colours for a mesh of a given size
void UProceduralTerrainChunk::generateColours(FVector componentLocation, int resolution, float chunkSize, float frequency)
{
	// Init colours array
	meshData.colours.Empty();
	meshData.colours.Reserve(resolution*resolution);

	// Offset in terms of number of chunks
	FVector2D chunkOffset = FVector2D(componentLocation.X, componentLocation.Y) / chunkSize;

	// Calculate colour for each vertex
	for (int x=0; x<resolution; x++)
	{
		float xPos = float(x) / (resolution-1);
		for (int y=0; y<resolution; y++)
		{
			float yPos = float(y) / (resolution-1);
			FVector2D samplePos = (FVector2D(xPos, yPos) + chunkOffset) * frequency;
			meshData.colours.Add(Biomes::colour(samplePos.X, samplePos.Y));
		}
	}
}
