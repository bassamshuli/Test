// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "Blueprint/UserWidget.h"
#include "Soldier.h"
#include "Obstacles.h"
#include "GameFeild.generated.h"

UCLASS()
class TEST_API AGameFeild : public AActor
{
	GENERATED_BODY()

public:
	AGameFeild();

	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	void HandleTileClicked(ATile* ClickedTile);
	void StartGame();
	void GenerateGrid();
	void GenerateObstacles();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void PlaceAIUnit();

	bool bGameStarted = false;
	bool bIsPlayerTurn = true;
	int32 CurrentUnitIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 Rows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 Columns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float CellSize;

	UPROPERTY(VisibleAnywhere, Category = "Grid")
	TArray<ATile*> Tiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TSubclassOf<AActor> TileBlueprint;

	UPROPERTY(EditAnywhere, Category = "Obstacles")
	TSubclassOf<AObstacles> MountainBlueprint;

	UPROPERTY(EditAnywhere, Category = "Obstacles")
	TSubclassOf<AObstacles> TreeBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AObstacles> ObstacleToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameWidgetClass;

	UUserWidget* GameWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<ASoldier> BP_Brawler_Green;

	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<ASoldier> BP_Brawler_Red;

	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<ASoldier> BP_Sniper_Green;

	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<ASoldier> BP_Sniper_Red;

	TArray<TSubclassOf<ASoldier>> SpawnQueue;
};