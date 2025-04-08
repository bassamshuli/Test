// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGameMode.h"
#include "Camera/CameraActor.h"
#include "GameFeild.h"
#include "WBP_Game.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "SniperSoldier.h"
#include "BrawlerSoldier.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"

ABaseGameMode::ABaseGameMode()
{
    PlayerSpawnQueue.Empty();
    AISpawnQueue.Empty();
}

void ABaseGameMode::BeginPlay()
{
    Super::BeginPlay();

    for (TActorIterator<AGameFeild> It(GetWorld()); It; ++It)
    {
        GameUIInstance = It->GameUIInstance;
        Tiles = It->Tiles;
        break;
    }

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
        {
            if (It->GetName().Contains("BP_Camera"))
            {
                PlayerController->SetViewTargetWithBlend(*It, 0.0f);
                break;
            }
        }
    }
}

void ABaseGameMode::StartGame()
{
    PlayerUnitIndex = 0;
    AIUnitIndex = 0;
    bIsPlayerTurn = FMath::RandBool();
    StartingTeam = bIsPlayerTurn ? ETeam::Player : ETeam::AI;

    if (bIsPlayerTurn)
    {
        if (GameUIInstance)
        {
            GameUIInstance->ShowChooseUnitTypeUI();
        }
    }
    else
    {
        SetupAISpawnQueue();
        if (GameUIInstance)
        {
            GameUIInstance->SetSpawnQueue(AISpawnQueue);
            GameUIInstance->ShowPlacementMessage(false, AIUnitIndex);
        }
        GetWorldTimerManager().SetTimerForNextTick(this, &ABaseGameMode::NextTurn);
    }
}

void ABaseGameMode::SetupAISpawnQueue()
{
    bool bBrawlerFirst = FMath::RandBool();
    TSubclassOf<ASoldier> BrawlerClass = ABrawlerSoldier::StaticClass();
    TSubclassOf<ASoldier> SniperClass = ASniperSoldier::StaticClass();

    if (bBrawlerFirst)
    {
        AISpawnQueue = { BrawlerClass, SniperClass };
    }
    else
    {
        AISpawnQueue = { SniperClass, BrawlerClass };
    }
}

void ABaseGameMode::PlayerChoseStartingUnit(bool bBrawlerFirst)
{
    TSubclassOf<ASoldier> BrawlerClass = ABrawlerSoldier::StaticClass();
    TSubclassOf<ASoldier> SniperClass = ASniperSoldier::StaticClass();

    if (bBrawlerFirst)
    {
        PlayerSpawnQueue = { BrawlerClass, SniperClass };
        AISpawnQueue = { SniperClass, BrawlerClass };
    }
    else
    {
        PlayerSpawnQueue = { SniperClass, BrawlerClass };
        AISpawnQueue = { BrawlerClass, SniperClass };
    }

    PlayerUnitIndex = 0;
    AIUnitIndex = 0;

    if (GameUIInstance)
    {
        // ✅ ADESSO è popolato correttamente
        GameUIInstance->SetSpawnQueue(PlayerSpawnQueue);
        GameUIInstance->ShowPlacementMessage(true, PlayerUnitIndex);
    }

    NextTurn();
}
void ABaseGameMode::NextTurn()
{
    UE_LOG(LogTemp, Warning, TEXT("🔄 NextTurn - PlayerIndex: %d / %d, AIIndex: %d / %d, Turno: %s"),
        PlayerUnitIndex, PlayerSpawnQueue.Num(),
        AIUnitIndex, AISpawnQueue.Num(),
        bIsPlayerTurn ? TEXT("PLAYER") : TEXT("AI"));

    if (PlayerUnitIndex >= PlayerSpawnQueue.Num() && AIUnitIndex >= AISpawnQueue.Num())
    {
        OnPlacementPhaseComplete();
        return;
    }

    if (bIsPlayerTurn && PlayerUnitIndex < PlayerSpawnQueue.Num())
    {
        if (GameUIInstance)
        {
            GameUIInstance->SetSpawnQueue(PlayerSpawnQueue);
            GameUIInstance->ShowPlacementMessage(true, PlayerUnitIndex);
        }
    }
    else if (!bIsPlayerTurn && AIUnitIndex < AISpawnQueue.Num())
    {
        if (GameUIInstance)
        {
            GameUIInstance->SetSpawnQueue(AISpawnQueue);
            GameUIInstance->ShowPlacementMessage(false, AIUnitIndex);
        }

        GetWorldTimerManager().SetTimerForNextTick([this]() {
            PlaceAIUnit();
            });
    }
}

void ABaseGameMode::HandleTileClicked(ATile* ClickedTile)
{
    if (bIsPlayerTurn && PlayerUnitIndex < PlayerSpawnQueue.Num() && ClickedTile->IsTileFree())
    {
        FVector SpawnLocation = ClickedTile->GetActorLocation() + FVector(0, 0, 50);
        SpawnPlayerSoldier(PlayerSpawnQueue[PlayerUnitIndex], SpawnLocation, ClickedTile);
        PlayerUnitIndex++;

        if (PlayerUnitIndex >= PlayerSpawnQueue.Num() && AIUnitIndex >= AISpawnQueue.Num())
        {
            OnPlacementPhaseComplete();
        }
        else
        {
            bIsPlayerTurn = false;
            NextTurn();
        }
    }
}

void ABaseGameMode::SpawnPlayerSoldier(TSubclassOf<ASoldier> SoldierClass, const FVector& Location, ATile* Tile)
{
    ASoldier* NewSoldier = GetWorld()->SpawnActor<ASoldier>(SoldierClass, Location, FRotator::ZeroRotator);
    if (NewSoldier)
    {
        NewSoldier->Team = ETeam::Player;
        Tile->SetTileOccupied(true);
        NewSoldier->TryAssignOwningTile(Tiles);

        FString Path;
        if (SoldierClass->IsChildOf(ABrawlerSoldier::StaticClass()))
        {
            Path = "/Game/Sprites/Soldier1_Green_Sprite.Soldier1_Green_Sprite";
        }
        else
        {
            Path = "/Game/Sprites/Soldier2_Green_Sprite.Soldier2_Green_Sprite";
        }

        UPaperSprite* SpriteAsset = LoadObject<UPaperSprite>(nullptr, *Path);
        if (SpriteAsset)
        {
            NewSoldier->SpriteComponent->SetSprite(SpriteAsset);
        }

        UE_LOG(LogTemp, Warning, TEXT("🧍 Player ha piazzato un %s (Team: Player)"), *NewSoldier->GetName());
    }
}

void ABaseGameMode::HandleSoldierSelected(ASoldier* Soldier)
{
    if (!bIsPlayerTurn || !Soldier) return;

    if (SelectedSoldier && SelectedSoldier->OwningTile)
    {
        SelectedSoldier->OwningTile->SetSelected(false);
    }

    for (ATile* Tile : Tiles)
    {
        if (Tile)
        {
            Tile->SetSelected(false);
        }
    }

    SelectedSoldier = Soldier;

    if (SelectedSoldier->OwningTile)
    {
        SelectedSoldier->OwningTile->SetSelected(true);
    }

    if (SelectedSoldier->Team == ETeam::Player)
    {
        SelectedSoldier->ShowMovableTiles(Tiles);
    }
}

void ABaseGameMode::OnPlacementPhaseComplete()
{
    bActionPhaseStarted = true;
    CurrentTurnTeam = StartingTeam;

    if (GameUIInstance)
    {
        FString Message = CurrentTurnTeam == ETeam::Player ? TEXT("🎯 Player turn") : TEXT("🤖 AI turn");
        GameUIInstance->UpdateStatusMessage(FText::FromString(Message));
    }
}

void ABaseGameMode::ResetGame()
{
    for (TActorIterator<ASoldier> It(GetWorld()); It; ++It)
    {
        It->Destroy();
    }

    for (TActorIterator<AObstacles> It(GetWorld()); It; ++It)
    {
        It->Destroy();
    }

    for (ATile* Tile : Tiles)
    {
        if (Tile)
        {
            Tile->ResetTile();
        }
    }

    for (TActorIterator<AGameFeild> It(GetWorld()); It; ++It)
    {
        It->GenerateObstacles();
        break;
    }

    bIsPlayerTurn = true;
    PlayerUnitIndex = 0;
    AIUnitIndex = 0;
    bActionPhaseStarted = false;
    SelectedSoldier = nullptr;
    SelectedSoldier_Current = nullptr;
    PlayerSpawnQueue.Empty();
    AISpawnQueue.Empty();

    if (GameUIInstance)
    {
        GameUIInstance->ShowWelcomeMessage();
        GameUIInstance->SetSpawnQueue(PlayerSpawnQueue);
    }

    UE_LOG(LogTemp, Warning, TEXT("♻️ Reset completato!"));

}
void ABaseGameMode::SpawnAISoldier(TSubclassOf<ASoldier> SoldierClass, const FVector& Location, ATile* Tile)
{
    ASoldier* AIUnit = GetWorld()->SpawnActor<ASoldier>(SoldierClass, Location, FRotator::ZeroRotator);
    if (AIUnit)
    {
        AIUnit->Team = ETeam::AI;
        Tile->SetTileOccupied(true);
        AIUnit->TryAssignOwningTile(Tiles);

        FString Path;
        if (SoldierClass->IsChildOf(ABrawlerSoldier::StaticClass()))
        {
            Path = "/Game/Sprites/Soldier1_Red_Sprite.Soldier1_Red_Sprite";
        }
        else
        {
            Path = "/Game/Sprites/Soldier2_Red_Sprite.Soldier2_Red_Sprite";
        }

        UPaperSprite* SpriteAsset = LoadObject<UPaperSprite>(nullptr, *Path);
        if (SpriteAsset)
        {
            AIUnit->SpriteComponent->SetSprite(SpriteAsset);
        }

        UE_LOG(LogTemp, Warning, TEXT("🤖 AI ha piazzato un %s (Team: AI)"), *AIUnit->GetName());
    }
}

void ABaseGameMode::PlaceAIUnit()
{
    if (AIUnitIndex >= AISpawnQueue.Num()) return;

    TArray<ATile*> FreeTiles;
    for (ATile* Tile : Tiles)
    {
        if (Tile && Tile->IsTileFree())
        {
            FreeTiles.Add(Tile);
        }
    }

    if (FreeTiles.Num() == 0) return;

    ATile* SelectedTile = FreeTiles[FMath::RandRange(0, FreeTiles.Num() - 1)];
    FVector SpawnLocation = SelectedTile->GetActorLocation() + FVector(0, 0, 50);

    TSubclassOf<ASoldier> SoldierClass = AISpawnQueue[AIUnitIndex];
    SpawnAISoldier(SoldierClass, SpawnLocation, SelectedTile);

    AIUnitIndex++;

    if (GameUIInstance)
    {
        GameUIInstance->ShowPlacementMessage(false, AIUnitIndex);
    }

    if (AIUnitIndex >= AISpawnQueue.Num() && PlayerUnitIndex >= PlayerSpawnQueue.Num())
    {
        OnPlacementPhaseComplete();
    }
    else
    {
        bIsPlayerTurn = true;
        NextTurn();
    }
}