// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Tile.h"
#include "BaseGameMode.generated.h"

UENUM(BlueprintType)
enum class ETeam : uint8
{
    Player,
    AI
};

UCLASS()
class TEST_API ABaseGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ABaseGameMode();
    virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadOnly) ETeam CurrentTeam;
    UPROPERTY(BlueprintReadOnly) int32 PlayerUnitsToPlace = 2;
    UPROPERTY(BlueprintReadOnly) int32 AIUnitsToPlace = 2;

    UFUNCTION(BlueprintCallable)
    void StartGame();

    UFUNCTION()
    void NextTurn();

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentUnitIndex = 0;

    UPROPERTY(BlueprintReadOnly)
    bool bIsPlayerTurn = true;

    UPROPERTY()
    TArray<TSubclassOf<class ASoldier>> SpawnQueue;

    UPROPERTY(EditAnywhere, Category = "Setup")
    TSubclassOf<ASoldier> BP_Brawler_Green;

    UPROPERTY(EditAnywhere, Category = "Setup")
    TSubclassOf<ASoldier> BP_Brawler_Red;

    UPROPERTY(EditAnywhere, Category = "Setup")
    TSubclassOf<ASoldier> BP_Sniper_Green;

    UPROPERTY(EditAnywhere, Category = "Setup")
    TSubclassOf<ASoldier> BP_Sniper_Red;

    UPROPERTY()
    class UWBP_Game* GameUIInstance;

    UPROPERTY(BlueprintReadWrite)
    TArray<ATile*> Tiles;

    UFUNCTION()
    void HandleTileClicked(ATile* ClickedTile);
};

