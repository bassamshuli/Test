// Fill out your copyright notice in the Description page of Project Settings.


#include "WBP_Game.h"
#include "Kismet/GameplayStatics.h"
#include "BaseGameMode.h"
#include "GameFeild.h"

bool UWBP_Game::Initialize()
{
    const bool Success = Super::Initialize();
    if (!Success) return false;

    if (StartButton)
    {
        StartButton->OnClicked.AddUniqueDynamic(this, &UWBP_Game::StartGameButtonClicked);
    }

    return true;
}

void UWBP_Game::StartGameButtonClicked()
{
    ABaseGameMode* GM = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this));
    if (GM)
    {
        GM->StartGame();
    }
}

void UWBP_Game::ShowWelcomeMessage()
{
    UpdateStatusMessage(FText::FromString(TEXT("👋 Benvenuto! Premi Start per iniziare")));
}

void UWBP_Game::ShowPlacementMessage(bool bIsPlayerTurn, int32 CurrentUnitIndex)
{
    FString Message;

    if (CurrentUnitIndex <= 1)
        Message = bIsPlayerTurn ? TEXT("🎯 Player turn - Posiziona il tuo BRAWLER") : TEXT("🤖 AI turn - Posiziona il suo BRAWLER");
    else
        Message = bIsPlayerTurn ? TEXT("🎯 Player turn - Posiziona il tuo SNIPER") : TEXT("🤖 AI turn - Posiziona il suo SNIPER");

    UpdateStatusMessage(FText::FromString(Message));
}

void UWBP_Game::UpdateStatusMessage(const FText& NewMessage)
{
    if (StatusText)
    {
        StatusText->SetText(NewMessage);
    }
}