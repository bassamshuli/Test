// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Soldier.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Melee UMETA(DisplayName = "Melee"),
	Ranged UMETA(DisplayName = "Ranged")
};


UCLASS()
class TEST_API ASoldier : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASoldier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 MaxMovement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    EAttackType AttackType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 MinDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 MaxDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Health;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    int32 GetRandomDamage() const;
};
