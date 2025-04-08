// Fill out your copyright notice in the Description page of Project Settings.


#include "SniperSoldier.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"

ASniperSoldier::ASniperSoldier()
{
    SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SniperSprite"));
    RootComponent = SpriteComponent;

    SpriteComponent->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
    SpriteComponent->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));

    // Stats specifici
    MaxMovement = 3;
    AttackType = EAttackType::Ranged;
    AttackRange = 10;
    MinDamage = 4;
    MaxDamage = 8;
    Health = 20;
}

void ASniperSoldier::BeginPlay()
{
    Super::BeginPlay();

    if (Team == ETeam::Player)
    {
        UPaperSprite* SpriteAsset = LoadObject<UPaperSprite>(nullptr, TEXT("/Game/Sprites/Soldier1_Green_Sprite.Soldier1_Green_Sprite"));
        if (SpriteAsset) SpriteComponent->SetSprite(SpriteAsset);
    }
    else
    {
        UPaperSprite* SpriteAsset = LoadObject<UPaperSprite>(nullptr, TEXT("/Game/Sprites/Soldier1_Red_Sprite.Soldier1_Red_Sprite"));
        if (SpriteAsset) SpriteComponent->SetSprite(SpriteAsset);
    }
}
