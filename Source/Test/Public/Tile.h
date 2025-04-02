// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class ASoldier; // ✅ Forward declaration, evita includere Soldier.h qui

UCLASS()
class TEST_API ATile : public AActor
{
    GENERATED_BODY()

public:
    ATile();

    UFUNCTION(BlueprintCallable) bool IsTileFree() const;
    UFUNCTION(BlueprintCallable) void SetTileOccupied(bool bOccupied);
    UFUNCTION() void OnTileClicked(AActor* TouchedActor, FKey ButtonPressed);

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FVector2D GridPosition;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
    bool bIsOccupied;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<ASoldier> PlayerSoldierToSpawn;

    UPROPERTY(EditDefaultsOnly, Category = "Spawning")
    TSubclassOf<ASoldier> SoldierClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bHasObstacle = false;
};