
#include "ProceduralTerrain.h"

#include "ProceduralTerrainChunk.h"

#include "../PerlinNoise/PerlinNoise.h"

#include <cmath>

AProceduralTerrain::AProceduralTerrain()
{

}

void AProceduralTerrain::spawnChunk(FIntPoint location)
{
	UProceduralTerrainChunk *chunk = NewObject<UProceduralTerrainChunk>(this);
	chunk->SetRelativeLocation( FVector(location.X, location.Y, 0) * chunkSize );
	chunk->generate();
	chunk->RegisterComponent();
	meshes.Add(location, chunk);
}

void AProceduralTerrain::despawnChunk(FIntPoint location)
{

}

// Initially generate chunk meshes
void AProceduralTerrain::PostRegisterAllComponents()
{
	FVector2D player(0, 0);

	float const spawnRadius = 3;
	int upperSpawnDiameter = std::round(spawnRadius)*2 + 1;
	int lowerSpawnRadius = upperSpawnDiameter / 2;

	FIntPoint offset( std::floor(player.X)-lowerSpawnRadius, std::floor(player.Y)-lowerSpawnRadius );

	for (int x=0; x<upperSpawnDiameter; x++)
	{
		for (int y=0; y<upperSpawnDiameter; y++)
		{
			FVector2D chunkCentre(offset.X+0.5+x, offset.Y+0.5+y);
			if ((chunkCentre-player).Size()<=spawnRadius)
			{
				spawnChunk(FIntPoint(offset.X+x, offset.Y+y));
			}
		}
	}
}

// Trigger mesh regeneration when parameters change
void AProceduralTerrain::PostEditChangeProperty(FPropertyChangedEvent &event)
{
	Super::PostEditChangeProperty(event);
	// mesh->generate();
}
void AProceduralTerrain::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);
	//if (bFinished || generateWhileMoving)
		//mesh->generate(FString("PostEditMove"));
}
