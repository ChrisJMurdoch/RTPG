
#include "ProceduralTerrain.h"

#include "ProceduralTerrainChunk.h"
#include "../PerlinNoise/PerlinNoise.h"

#include <cmath>


AProceduralTerrain::AProceduralTerrain() : Super(), generator()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	chunksSpawned = 0;
	chunksDespawned = 0;
}


// ===== ===== ===== EVENTS ===== ===== =====


// Generation properties change (Editor)
void AProceduralTerrain::PostEditChangeProperty(FPropertyChangedEvent &event)
{
	Super::PostEditChangeProperty(event);

	clearChunks();
}

// Gameloop tick (Game)
void AProceduralTerrain::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	blocksRegisteredThisTick = 0;

	// FVector playerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	UWorld *world = GetWorld();
	APlayerController *controller = world->GetFirstPlayerController();
	APawn *pawn = controller->GetPawn();
	FVector playerPosition = pawn ? pawn->GetActorLocation()/chunkSize : FVector(0.5, 0.5, 0);

	try {
		syncChunks(FVector2D(playerPosition.X, playerPosition.Y));

		for (TPair<FIntPoint, UProceduralTerrainChunk *> entry : meshes)
			entry.Value->tick();
	}
	catch (std::exception const &e)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tick Exception: %s"), e.what());
	}
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

	// Calculate range
	int spawnDiameter=std::round(renderDistance)*2+1, spawnRadius=spawnDiameter/2;
	int xOffset=std::floor(playerPosition.X)-spawnRadius, yOffset=std::floor(playerPosition.Y)-spawnRadius;

	// Prioritise spawning chunk player is on
	spawnChunk(FIntPoint(xOffset+spawnRadius, yOffset+spawnRadius));

	// Queue the rest
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
	meshes.Add(location, chunk);
	chunk->init(generator);
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
