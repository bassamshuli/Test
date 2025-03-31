// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacles.generated.h"

UCLASS()
class TEST_API AObstacles : public AActor
{
    GENERATED_BODY()

public:
    AObstacles();
    virtual void BeginPlay() override;
};
