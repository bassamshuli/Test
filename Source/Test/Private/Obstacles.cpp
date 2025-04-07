// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacles.h"
#include "UObject/ConstructorHelpers.h"

AObstacles::AObstacles()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
    SpriteComponent->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
    SpriteComponent->SetupAttachment(RootComponent);
    SpriteComponent->SetCollisionProfileName(TEXT("BlockAll"));
}

void AObstacles::BeginPlay()
{
    Super::BeginPlay();
}

void AObstacles::InitWithSprite(const FString& SpritePath)
{
    ConstructorHelpers::FObjectFinder<UPaperSprite> SpriteAsset(*SpritePath);
    if (SpriteAsset.Succeeded())
    {
        SpriteComponent->SetSprite(SpriteAsset.Object);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Sprite asset not found at: %s"), *SpritePath);
    }
}
