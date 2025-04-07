// Fill out your copyright notice in the Description page of Project Settings.


#include "SniperSoldier.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"

ASniperSoldier::ASniperSoldier()
{
    static ConstructorHelpers::FObjectFinder<UPaperSprite> DefaultSprite(TEXT("/Game/Sprites/Soldier2_Green_Sprite.Soldier2_Green_Sprite"));
    if (DefaultSprite.Succeeded())
    {
        SpriteComponent->SetSprite(DefaultSprite.Object);
    }

    SpriteComponent->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));

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
        UPaperSprite* SpriteAsset = LoadObject<UPaperSprite>(nullptr, TEXT("/Game/Sprites/Soldier2_Green_Sprite.Soldier2_Green_Sprite"));
        if (SpriteAsset)
        {
            SpriteComponent->SetSprite(SpriteAsset);
        }
    }
    else
    {
        UPaperSprite* SpriteAsset = LoadObject<UPaperSprite>(nullptr, TEXT("/Game/Sprites/Soldier2_Red_Sprite.Soldier2_Red_Sprite"));
        if (SpriteAsset)
        {
            SpriteComponent->SetSprite(SpriteAsset);
        }
    }
}
