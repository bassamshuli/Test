// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AISoldierController.generated.h"

class ATile;
class ASoldier;
class UWBP_Game;

UCLASS()
class TEST_API AAISoldierController : public AActor
{
    GENERATED_BODY()

public:
    AAISoldierController();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void PlaceAIUnitDelayed(const TArray<ATile*>& Tiles, TArray<TSubclassOf<ASoldier>>& SpawnQueue, int32& CurrentUnitIndex, bool& bIsPlayerTurn, UWBP_Game* GameUIInstance);

private:
    void PlaceAIUnit();

    TArray<ATile*> CachedTiles;
    TArray<TSubclassOf<ASoldier>>* CachedSpawnQueue = nullptr;
    int32* CachedCurrentUnitIndex = nullptr;
    bool* CachedIsPlayerTurn = nullptr;
    UWBP_Game* CachedGameUI = nullptr;

    FTimerHandle DelayHandle;
};
