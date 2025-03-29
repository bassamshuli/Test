// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class TEST_API ATile : public AActor
{
    GENERATED_BODY()

public:
    ATile();

    void SetGridPosition(FVector2D NewPosition);

    UFUNCTION(BlueprintCallable)
    bool IsTileFree() const;

    UFUNCTION(BlueprintCallable)
    void SetTileOccupied(bool bOccupied);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FVector2D GridPosition;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
    bool bIsOccupied;

    UFUNCTION()
    void OnTileClicked(AActor* TouchedActor, FKey ButtonPressed);

public:
    virtual void Tick(float DeltaTime) override;
    virtual void NotifyActorOnClicked(FKey ButtonPressed) override;
};
