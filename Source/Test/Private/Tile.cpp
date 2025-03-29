// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFeild.h"

ATile::ATile()
{
    PrimaryActorTick.bCanEverTick = false;

    bIsOccupied = false;

    SetActorEnableCollision(true);
    SetActorTickEnabled(true);
    AutoReceiveInput = EAutoReceiveInput::Player0;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToAllChannels(ECR_Block);
    Mesh->SetGenerateOverlapEvents(true);
    Mesh->SetNotifyRigidBodyCollision(true);
    Mesh->SetVisibility(true);
    Mesh->SetMobility(EComponentMobility::Static);
}

void ATile::BeginPlay()
{
    Super::BeginPlay();
    OnClicked.AddDynamic(this, &ATile::OnTileClicked);
}

void ATile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATile::SetGridPosition(FVector2D NewPosition)
{
    GridPosition = NewPosition;
}

bool ATile::IsTileFree() const
{
    return !bIsOccupied;
}

void ATile::SetTileOccupied(bool bOccupied)
{
    bIsOccupied = bOccupied;
}

void ATile::NotifyActorOnClicked(FKey ButtonPressed)
{
    Super::NotifyActorOnClicked(ButtonPressed);
    UE_LOG(LogTemp, Warning, TEXT("🟩 NotifyActorOnClicked chiamato!"));

    AGameFeild* Game = Cast<AGameFeild>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameFeild::StaticClass()));
    if (Game)
    {
        Game->HandleTileClicked(this);
    }
}

void ATile::OnTileClicked(AActor* TouchedActor, FKey ButtonPressed)
{
    NotifyActorOnClicked(ButtonPressed);
}

