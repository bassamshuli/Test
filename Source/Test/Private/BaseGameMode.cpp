// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h" // Per TActorIterator
#include "GameFramework/PlayerController.h"

ABaseGameMode::ABaseGameMode()
{


}




void ABaseGameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    if (World)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
        if (PlayerController)
        {
            for (TActorIterator<ACameraActor> It(World); It; ++It)
            {
                ACameraActor* Camera = *It;
                if (Camera && Camera->GetName().Contains("BP_Camera"))
                {
                    PlayerController->SetViewTargetWithBlend(Camera, 0.0f);
                    UE_LOG(LogTemp, Warning, TEXT("✅ Camera impostata con successo da BeginPlay!"));
                    break;
                }
            }
        }
    }
}

void ABaseGameMode::StartGame()
{
    // Lancia la moneta
    bool bPlayerStarts = FMath::RandBool();
    CurrentTeam = bPlayerStarts ? ETeam::Player : ETeam::AI;

    FString TurnMessage = bPlayerStarts ? TEXT("🎲 Turno del Player - Posiziona un'unità!") : TEXT("🎲 Turno dell'AI - Posiziona un'unità!");
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TurnMessage);

    // Qui potrai poi gestire il posizionamento a turni
}


