// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Soldier.generated.h"

class ATile;
class UPaperSpriteComponent;

UENUM(BlueprintType)
enum class EAttackType : uint8
{
    Melee,
    Ranged
};

UENUM(BlueprintType)
enum class ETeam : uint8
{
    Player,
    AI
};

UCLASS()
class TEST_API ASoldier : public AActor
{
    GENERATED_BODY()

public:
    ASoldier();
    virtual void Tick(float DeltaTime) override;

    virtual int32 GetRandomDamage() const;

    void TryAssignOwningTile(const TArray<ATile*>& AllTiles);
    void ShowMovableTiles(const TArray<ATile*>& AllTiles);

    UFUNCTION()
    void OnSoldierClicked(AActor* TouchedActor, FKey ButtonPressed);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxMovement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAttackType AttackType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETeam Team;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UPaperSpriteComponent* SpriteComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class ATile* OwningTile;
};