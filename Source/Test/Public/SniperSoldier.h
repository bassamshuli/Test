// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Soldier.h"
#include "SniperSoldier.generated.h"

UCLASS()
class TEST_API ASniperSoldier : public ASoldier
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASniperSoldier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


    

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
