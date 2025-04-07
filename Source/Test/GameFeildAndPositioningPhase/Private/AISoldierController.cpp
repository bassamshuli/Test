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

void AAISoldierController::PlaceAIUnitDelayed(
    const TArray<ATile*>& Tiles,
    const TArray<TSubclassOf<ASoldier>>& SpawnQueue,
    UWBP_Game* GameUI,
    ABaseGameMode* GameMode)
{
    CachedTiles = Tiles;
    CachedSpawnQueue = &SpawnQueue;
    CachedGameUI = GameUI;
    CachedGameMode = GameMode;

    GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &AAISoldierController::PlaceAIUnit, 2.0f, false);
}

void AAISoldierController::PlaceAIUnit()
{
    if (!CachedGameMode || !CachedSpawnQueue) return;
    if (CachedGameMode->CurrentUnitIndex >= CachedSpawnQueue->Num()) return;

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
        TSubclassOf<ASoldier> SoldierClass = (*CachedSpawnQueue)[CachedGameMode->CurrentUnitIndex];

        if (!SoldierClass) return;

        ASoldier* AIUnit = GetWorld()->SpawnActor<ASoldier>(SoldierClass, SpawnLocation, FRotator::ZeroRotator);

        if (AIUnit)
        {
            AIUnit->Team = ETeam::AI;

            SelectedTile->SetTileOccupied(true);
            AIUnit->TryAssignOwningTile(CachedTiles);

            UE_LOG(LogTemp, Warning, TEXT("🤖 AI ha piazzato un %s in posizione %s"),
                *AIUnit->GetName(), *SelectedTile->GetActorLocation().ToString());

            CachedGameMode->CurrentUnitIndex++;

            if (CachedGameUI)
            {
                CachedGameUI->ShowPlacementMessage(true, CachedGameMode->CurrentUnitIndex);
            }

            if (CachedGameMode->CurrentUnitIndex >= CachedSpawnQueue->Num())
            {
                CachedGameMode->OnPlacementPhaseComplete();
            }
            else
            {
                CachedGameMode->bIsPlayerTurn = true;
                CachedGameMode->NextTurn();
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ AI: Spawn fallito per unità %d"), CachedGameMode->CurrentUnitIndex);
        }
    }
}