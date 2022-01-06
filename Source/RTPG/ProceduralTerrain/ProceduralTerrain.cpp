
#include "ProceduralTerrain.h"

#include "ProceduralTerrainChunk.h"

#include "../PerlinNoise/PerlinNoise.h"

#include <cmath>


AProceduralTerrain::AProceduralTerrain() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}


// ===== ===== ===== EVENTS ===== ===== =====


// Initial generation (Editor and Game)
void AProceduralTerrain::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();

	chunksSpawned = 0;
	chunksDespawned = 0;

	syncChunks( FVector2D(playerX, 0) );
}

// Generation properties change (Editor)
void AProceduralTerrain::PostEditChangeProperty(FPropertyChangedEvent &event)
{
	Super::PostEditChangeProperty(event);

	clearChunks();
	syncChunks( FVector2D(playerX, 0) );
}

// Gameloop tick (Game)
void AProceduralTerrain::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	syncChunks( FVector2D(playerX, 0) );
}


// ===== ===== ===== HIGH-LEVEL OPERATIONS ===== ===== =====


// Ensure correct chunks are loaded
void AProceduralTerrain::syncChunks(FVector2D playerPosition)
{
	// Remove chunks out of range
	for (TPair<FIntPoint, UProceduralTerrainChunk *> entry : meshes)
	{
		FVector2D chunkCentre(entry.Key.X+0.5f, entry.Key.Y+0.5f);
		if ((chunkCentre-playerPosition).Size() > renderDistance)
		{
			despawnChunk(entry.Key);
		}
	}

	// Add chunks in range
	int spawnDiameter=std::round(renderDistance)*2+1, spawnRadius=spawnDiameter/2;
	int xOffset=std::floor(playerPosition.X)-spawnRadius, yOffset=std::floor(playerPosition.Y)-spawnRadius;
	for (int x=xOffset; x<xOffset+spawnDiameter; x++)
	{
		for (int y=yOffset; y<yOffset+spawnDiameter; y++)
		{
			FVector2D chunkCentre(x+0.5, y+0.5);
			if ( (chunkCentre-playerPosition).Size()<=renderDistance )
			{
				spawnChunk(FIntPoint(x, y));
			}
		}
	}
}

// Despawn all chunks
void AProceduralTerrain::clearChunks()
{
	for (TPair<FIntPoint, UProceduralTerrainChunk *> entry : meshes)
		despawnChunk(entry.Key);
}


// ===== ===== ===== LOW-LEVEL OPERATIONS ===== ===== =====


void AProceduralTerrain::spawnChunk(FIntPoint location)
{
	if (meshes.Contains(location))
		return;
	UProceduralTerrainChunk *chunk = NewObject<UProceduralTerrainChunk>(this);
	chunk->SetRelativeLocation( FVector(location.X, location.Y, 0) * chunkSize );
	chunk->generate();
	chunk->RegisterComponent();
	meshes.Add(location, chunk);
	chunksSpawned++;
}

void AProceduralTerrain::despawnChunk(FIntPoint location)
{
	if (!meshes.Contains(location))
		return;
	meshes[location]->DestroyComponent();
	meshes.Remove(location);
	chunksDespawned++;
}
