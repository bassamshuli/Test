﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "WBP_Game.h"
#include "Obstacles.h"
#include "GameFeild.generated.h"

UCLASS()
class TEST_API AGameFeild : public AActor
{
    GENERATED_BODY()

public:
    AGameFeild();

    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

    void GenerateGrid();
    void GenerateObstacles();

    // === GRID CONFIGURATION ===
    UPROPERTY(EditAnywhere, Category = "Grid") int32 Rows = 25;
    UPROPERTY(EditAnywhere, Category = "Grid") int32 Columns = 25;
    UPROPERTY(EditAnywhere, Category = "Grid") float CellSize = 37.0f;
    UPROPERTY(VisibleAnywhere, Category = "Grid") TArray<ATile*> Tiles;

    // === OBSTACLES ===
    UPROPERTY() TSubclassOf<AObstacles> MountainBlueprint;
    UPROPERTY() TSubclassOf<AObstacles> TreeBlueprint;
    UPROPERTY(EditAnywhere, Category = "Spawn") TSubclassOf<AObstacles> ObstacleToSpawn;

    // === UI ===
    UPROPERTY(EditAnywhere, Category = "UI") TSubclassOf<UUserWidget> GameWidgetClass;
    UPROPERTY() UWBP_Game* GameUIInstance;

    // === API ===
    UFUNCTION() ATile* GetTileAt(int32 X, int32 Y) const;
};