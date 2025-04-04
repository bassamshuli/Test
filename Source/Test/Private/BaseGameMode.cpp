// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"
#include "Camera/CameraActor.h"
#include "GameFeild.h"
#include "WBP_Game.h"
#include "AISoldierController.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

ABaseGameMode::ABaseGameMode() {}

void ABaseGameMode::BeginPlay()
{
    Super::BeginPlay();

    bool bAIControllerFound = false;
    for (TActorIterator<AAISoldierController> It(GetWorld()); It; ++It)
    {
        bAIControllerFound = true;
        break;
    }

    if (!bAIControllerFound)
    {
        FActorSpawnParameters SpawnParams;
        GetWorld()->SpawnActor<AAISoldierController>(AAISoldierController::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
    }

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
    CurrentUnitIndex = 0;
    bIsPlayerTurn = FMath::RandBool();

    if (bIsPlayerTurn)
    {
        if (GameUIInstance)
        {
            GameUIInstance->ShowChooseUnitTypeUI();
        }
    }
    else
    {
        SpawnQueue = { BP_Brawler_Red, BP_Brawler_Green, BP_Sniper_Red, BP_Sniper_Green };
        GetWorldTimerManager().SetTimerForNextTick(this, &ABaseGameMode::NextTurn);
    }
}

void ABaseGameMode::PlayerChoseStartingUnit(bool bBrawlerFirst)
{
    if (bBrawlerFirst)
    {
        SpawnQueue = { BP_Brawler_Green, BP_Brawler_Red, BP_Sniper_Green, BP_Sniper_Red };
    }
    else
    {
        SpawnQueue = { BP_Sniper_Green, BP_Sniper_Red, BP_Brawler_Green, BP_Brawler_Red };
    }

    CurrentUnitIndex = 0;

    if (GameUIInstance)
    {
        GameUIInstance->SetSpawnQueue(SpawnQueue);
        GameUIInstance->ShowPlacementMessage(true, CurrentUnitIndex);
    }

    // 🛠️ AGGIUNGI QUESTO:
    NextTurn(); // ⚠️ Abilita le tile per il primo posizionamento del player
}

void ABaseGameMode::NextTurn()
{
    if (CurrentUnitIndex >= SpawnQueue.Num()) return;

    if (bIsPlayerTurn)
    {
        for (ATile* Tile : Tiles)
        {
            if (Tile)
            {
                Tile->PlayerSoldierToSpawn = SpawnQueue[CurrentUnitIndex];
            }
        }
    }

    if (GameUIInstance)
    {
        GameUIInstance->SetSpawnQueue(SpawnQueue);
        GameUIInstance->ShowPlacementMessage(bIsPlayerTurn, CurrentUnitIndex);
    }

    if (!bIsPlayerTurn)
    {
        GetWorldTimerManager().SetTimerForNextTick([this]() {
            for (TActorIterator<AAISoldierController> It(GetWorld()); It; ++It)
            {
                It->PlaceAIUnitDelayed(Tiles, SpawnQueue, CurrentUnitIndex, bIsPlayerTurn, GameUIInstance);
                break;
            }
            });
    }
}

void ABaseGameMode::HandleTileClicked(ATile* ClickedTile)
{
    // 🧩 Se siamo nella fase di posizionamento
    if (CurrentUnitIndex < SpawnQueue.Num() && bIsPlayerTurn && ClickedTile->IsTileFree())
    {
        FVector SpawnLocation = ClickedTile->GetActorLocation() + FVector(0, 0, 50);
        ASoldier* NewSoldier = GetWorld()->SpawnActor<ASoldier>(SpawnQueue[CurrentUnitIndex], SpawnLocation, FRotator::ZeroRotator);

        if (NewSoldier)
        {
            ClickedTile->SetTileOccupied(true);
            NewSoldier->Team = ETeam::Player;
            NewSoldier->TryAssignOwningTile(Tiles);
            CurrentUnitIndex++;

            if (CurrentUnitIndex < SpawnQueue.Num())
            {
                bIsPlayerTurn = false;
                NextTurn();
            }
            else
            {
                // Tutti i soldati sono stati posizionati
                bActionPhaseStarted = true;
                CurrentTurnTeam = bIsPlayerTurn ? ETeam::Player : ETeam::AI;

                if (GameUIInstance)
                {
                    FString Message = CurrentTurnTeam == ETeam::Player
                        ? TEXT("🎯 Player turn")
                        : TEXT("🤖 AI turn");
                    GameUIInstance->UpdateStatusMessage(FText::FromString(Message));
                }
            }
            return;
        }
    }

    // 🧩 Se siamo nella fase di selezione
    if (!SelectedSoldier || SelectedSoldier->Team != ETeam::Player)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠️ Soldier selezionato non valido o non del team Player"));
        return;
    }

    // Deseleziona precedente
    if (SelectedSoldier_Current && SelectedSoldier_Current->OwningTile)
    {
        SelectedSoldier_Current->OwningTile->SetSelected(false);
    }

    SelectedSoldier_Current = SelectedSoldier;

    if (SelectedSoldier_Current && SelectedSoldier_Current->OwningTile)
    {
        SelectedSoldier_Current->OwningTile->SetSelected(true);
        UE_LOG(LogTemp, Warning, TEXT("🟩 Soldier selezionato: %s"), *SelectedSoldier_Current->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ SelectedSoldier o la sua tile è nullptr"));
    }

    // 🔁 Turno alternato cliccando qualsiasi tile (TEMPORANEO)
    if (bActionPhaseStarted)
    {
        CurrentTurnTeam = (CurrentTurnTeam == ETeam::Player) ? ETeam::AI : ETeam::Player;

        if (GameUIInstance)
        {
            FString Message = CurrentTurnTeam == ETeam::Player
                ? TEXT("🎯 Player turn")
                : TEXT("🤖 AI turn");
            GameUIInstance->UpdateStatusMessage(FText::FromString(Message));
        }
    }
}

void ABaseGameMode::HandleSoldierSelected(ASoldier* Soldier)
{
    if (!bIsPlayerTurn || !Soldier) return;

    // Deseleziona precedente
    if (SelectedSoldier && SelectedSoldier->OwningTile)
    {
        SelectedSoldier->OwningTile->SetSelected(false);
    }

    // Deseleziona tutte le tile precedenti
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

    // Mostra le celle raggiungibili
    if (SelectedSoldier->Team == ETeam::Player)
    {
        SelectedSoldier->ShowMovableTiles(Tiles);
    }
}
