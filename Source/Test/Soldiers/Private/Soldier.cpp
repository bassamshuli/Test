// Fill out your copyright notice in the Description page of Project Settings.

#include "Soldier.h"
#include "Tile.h"
#include "BaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Containers/Queue.h"
#include "Containers/Set.h"

ASoldier::ASoldier()
{
    PrimaryActorTick.bCanEverTick = false;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
    CapsuleComponent->InitCapsuleSize(34.f, 88.f);
    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
    RootComponent = CapsuleComponent;

    SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
    SpriteComponent->SetupAttachment(RootComponent);
    SpriteComponent->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
}

void ASoldier::BeginPlay()
{
    Super::BeginPlay();

    if (Team == ETeam::Player)
    {
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
        {
            EnableInput(PC);
            UE_LOG(LogTemp, Warning, TEXT("🟢 Input abilitato su %s"), *GetName());
        }

        OnClicked.AddDynamic(this, &ASoldier::OnSoldierClicked);
    }
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

void ASoldier::TryAssignOwningTile(const TArray<ATile*>& AllTiles)
{
    for (ATile* Tile : AllTiles)
    {
        if (Tile && FVector::Dist2D(Tile->GetActorLocation(), GetActorLocation()) < 10.0f)
        {
            OwningTile = Tile;
            Tile->SetTileOccupied(true);
            UE_LOG(LogTemp, Warning, TEXT("🔁 OwningTile trovata automaticamente: (%d, %d) per %s"),
                Tile->GridPosition.X, Tile->GridPosition.Y, *GetName());
            break;
        }
    }

    if (!OwningTile)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Nessuna OwningTile trovata per %s"), *GetName());
    }
}

void ASoldier::ShowMovableTiles(const TArray<ATile*>& AllTiles)
{
    if (!OwningTile)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠️ OwningTile mancante, provo a ricalcolarla"));
        TryAssignOwningTile(AllTiles);
    }

    if (!OwningTile)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ OwningTile ancora nulla. Abbandono ShowMovableTiles."));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("👣 Tile di partenza: (%d, %d) per %s"),
        OwningTile->GridPosition.X, OwningTile->GridPosition.Y, *GetName());

    FIntPoint Start = OwningTile->GridPosition;

    TMap<FIntPoint, ATile*> TileMap;
    for (ATile* Tile : AllTiles)
    {
        if (Tile)
        {
            TileMap.Add(Tile->GridPosition, Tile);
            Tile->SetSelected(false);
            Tile->SetEnemyHighlighted(false);
        }
    }

    TQueue<TPair<FIntPoint, int32>> Queue;
    TSet<FIntPoint> Visited;

    Queue.Enqueue(TPair<FIntPoint, int32>(Start, 0));
    Visited.Add(Start);

    while (!Queue.IsEmpty())
    {
        TPair<FIntPoint, int32> Current;
        Queue.Dequeue(Current);

        FIntPoint Pos = Current.Key;
        int32 Distance = Current.Value;

        if (Distance > MaxMovement)
            continue;

        ATile* Tile = TileMap.FindRef(Pos);
        if (Tile && Tile != OwningTile && !Tile->bIsOccupied && !Tile->bHasObstacle)
        {
            Tile->SetSelected(true);
        }

        const TArray<FIntPoint> Directions = {
            FIntPoint(1, 0), FIntPoint(-1, 0),
            FIntPoint(0, 1), FIntPoint(0, -1)
        };

        for (const FIntPoint& Dir : Directions)
        {
            FIntPoint Next = Pos + Dir;
            if (!Visited.Contains(Next))
            {
                ATile* NextTile = TileMap.FindRef(Next);
                if (NextTile && !NextTile->bIsOccupied && !NextTile->bHasObstacle)
                {
                    Queue.Enqueue(TPair<FIntPoint, int32>(Next, Distance + 1));
                    Visited.Add(Next);
                }
            }
        }
    }

    // Evidenzia nemici
    for (ATile* Tile : AllTiles)
    {
        if (!Tile || !Tile->bIsOccupied || Tile->bHasObstacle)
            continue;

        int32 GridDistance = FMath::Abs(Tile->GridPosition.X - Start.X) + FMath::Abs(Tile->GridPosition.Y - Start.Y);
        if (GridDistance <= AttackRange)
        {
            for (TActorIterator<ASoldier> It(GetWorld()); It; ++It)
            {
                ASoldier* Enemy = *It;
                if (Enemy->Team != ETeam::Player && Enemy->OwningTile == Tile)
                {
                    Tile->SetEnemyHighlighted(true);
                    break;
                }
            }
        }
    }
}