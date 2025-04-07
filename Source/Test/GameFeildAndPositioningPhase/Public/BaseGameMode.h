// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Soldier.h"
#include "Tile.h"
#include "BaseGameMode.generated.h"

UCLASS()
class TEST_API ABaseGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ABaseGameMode();
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void StartGame();

    UFUNCTION()
    void PlayerChoseStartingUnit(bool bBrawlerFirst);

    UFUNCTION()
    void NextTurn();

    UFUNCTION()
    void HandleTileClicked(ATile* ClickedTile);

    UFUNCTION()
    void HandleSoldierSelected(ASoldier* Soldier);

    UFUNCTION()
    void OnPlacementPhaseComplete();

    UFUNCTION(BlueprintCallable)
    void ResetGame();

    void SetupAISpawnQueue();

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentUnitIndex = 0;

    UPROPERTY(BlueprintReadOnly)
    bool bIsPlayerTurn = true;

    UPROPERTY()
    TArray<TSubclassOf<class ASoldier>> SpawnQueue;

    UPROPERTY()
    class UWBP_Game* GameUIInstance;

    UPROPERTY(BlueprintReadWrite)
    TArray<ATile*> Tiles;

    UPROPERTY()
    ASoldier* SelectedSoldier = nullptr;

    UPROPERTY()
    ASoldier* SelectedSoldier_Current = nullptr;

    bool bActionPhaseStarted = false;
    ETeam CurrentTurnTeam;
    ETeam StartingTeam;
};
