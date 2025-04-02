// Fill out your copyright notice in the Description page of Project Settings.


#include "Soldier.h"
#include "Tile.h"
#include "BaseGameMode.h"
#include "Kismet/GameplayStatics.h"

ASoldier::ASoldier()
{
    PrimaryActorTick.bCanEverTick = true;

    PrimaryActorTick.bCanEverTick = true;

    // Crea lo sprite e rendilo Root
    SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
    RootComponent = SpriteComponent;

    // Imposta collisione corretta per il click
    SpriteComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SpriteComponent->SetCollisionProfileName(TEXT("BlockAll"));
    SpriteComponent->SetGenerateOverlapEvents(true);
    SpriteComponent->SetNotifyRigidBodyCollision(true);

    
}

void ASoldier::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        EnableInput(PC);
        UE_LOG(LogTemp, Warning, TEXT("🟢 Input abilitato su %s"), *GetName());
    }

    OnClicked.AddDynamic(this, &ASoldier::OnSoldierClicked);
    UE_LOG(LogTemp, Warning, TEXT("✅ Click binding completato su %s"), *GetName());
}

void ASoldier::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

int32 ASoldier::GetRandomDamage() const
{
    return FMath::RandRange(MinDamage, MaxDamage);
}

void ASoldier::OnSoldierClicked(AActor* TouchedActor, FKey ButtonPressed)
{
    ABaseGameMode* GameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode && Team == ETeam::Player)
    {
        UE_LOG(LogTemp, Warning, TEXT("🎯 Soldier cliccato: %s"), *GetName());
        GameMode->HandleSoldierSelected(this);
    }
}




