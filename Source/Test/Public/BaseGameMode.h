// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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
};

