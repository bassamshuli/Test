// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AISoldierController.generated.h"

class ABaseGameMode;
class ATile;
class UWBP_Game;
class ASoldier;

UCLASS()
class TEST_API AAISoldierController : public AActor
{
    GENERATED_BODY()

public:
    AAISoldierController();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    void PlaceAIUnitDelayed(
        const TArray<ATile*>& Tiles,
        const TArray<TSubclassOf<ASoldier>>& SpawnQueue,
        UWBP_Game* GameUI,
        ABaseGameMode* GameMode);

private:
    void PlaceAIUnit();

    TArray<ATile*> CachedTiles;
    const TArray<TSubclassOf<ASoldier>>* CachedSpawnQueue;
    UWBP_Game* CachedGameUI;
    ABaseGameMode* CachedGameMode;

    FTimerHandle DelayHandle;
};
