// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PaperSpriteComponent.h"
#include "Soldier.generated.h"

class ATile;

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
class TEST_API ASoldier : public APawn
{
    GENERATED_BODY()

public:
    ASoldier();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UPaperSpriteComponent* SpriteComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats") int32 MaxMovement;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats") EAttackType AttackType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats") int32 AttackRange;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats") int32 MinDamage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats") int32 MaxDamage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats") int32 Health;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    int32 GetRandomDamage() const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETeam Team;

    UPROPERTY()
    ATile* OwningTile;

    UFUNCTION()
    void OnSoldierClicked(AActor* TouchedActor, FKey ButtonPressed);
};