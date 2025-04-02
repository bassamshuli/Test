// Fill out your copyright notice in the Description page of Project Settings.


#include "AISoldierController.h"
#include "Tile.h"
#include "BaseGameMode.h"               
#include "Kismet/GameplayStatics.h"     
#include "Soldier.h"
#include "WBP_Game.h"
#include "TimerManager.h"
#include "Engine/World.h"

AAISoldierController::AAISoldierController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAISoldierController::BeginPlay()
{
    Super::BeginPlay();
}

void AAISoldierController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAISoldierController::PlaceAIUnitDelayed(const TArray<ATile*>& Tiles, TArray<TSubclassOf<ASoldier>>& SpawnQueue, int32& CurrentUnitIndex, bool& bIsPlayerTurn, UWBP_Game* GameUIInstance)
{
    CachedTiles = Tiles;
    CachedSpawnQueue = &SpawnQueue;
    CachedCurrentUnitIndex = &CurrentUnitIndex;
    CachedIsPlayerTurn = &bIsPlayerTurn;
    CachedGameUI = GameUIInstance;

    GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &AAISoldierController::PlaceAIUnit, 4.0f, false);
}

void AAISoldierController::PlaceAIUnit()
{
    if (!CachedSpawnQueue || !CachedCurrentUnitIndex || !CachedIsPlayerTurn || !CachedGameUI) return;
    if (*CachedCurrentUnitIndex >= CachedSpawnQueue->Num()) return;

    TArray<ATile*> FreeTiles;
    for (ATile* Tile : CachedTiles)
    {
        if (Tile && Tile->IsTileFree() && !Tile->bHasObstacle)
        {
            FreeTiles.Add(Tile);
        }
    }

    if (FreeTiles.Num() > 0)
    {
        ATile* SelectedTile = FreeTiles[FMath::RandRange(0, FreeTiles.Num() - 1)];
        FVector SpawnLocation = SelectedTile->GetActorLocation() + FVector(0, 0, 50);
        ASoldier* AIUnit = GetWorld()->SpawnActor<ASoldier>((*CachedSpawnQueue)[*CachedCurrentUnitIndex], SpawnLocation, FRotator::ZeroRotator);

        if (AIUnit)
        {
            SelectedTile->SetTileOccupied(true);
            (*CachedCurrentUnitIndex)++;
            *CachedIsPlayerTurn = true;

            if (CachedGameUI)
            {
                CachedGameUI->ShowPlacementMessage(*CachedIsPlayerTurn, *CachedCurrentUnitIndex);
            }

            // ✅ Chiama il GameMode per continuare il turno
            if (GetWorld())
            {
                ABaseGameMode* GameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
                if (GameMode)
                {
                    GameMode->NextTurn();
                }
            }
        }
    }
}