// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "WBP_Game.generated.h"

UCLASS()
class TEST_API UWBP_Game : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual bool Initialize() override;

    UFUNCTION(BlueprintCallable, Category = "UI") void UpdateStatusMessage(const FText& NewMessage);
    UFUNCTION() void StartGameButtonClicked();

    UPROPERTY(meta = (BindWidget)) UTextBlock* StatusText;
    UPROPERTY(meta = (BindWidget)) UButton* StartButton;
    UPROPERTY(meta = (BindWidget)) UButton* ButtonChooseBrawler;
    UPROPERTY(meta = (BindWidget)) UButton* ButtonChooseSniper;
 

    UFUNCTION(BlueprintCallable, Category = "UI") void ShowWelcomeMessage();
    UFUNCTION(BlueprintCallable, Category = "UI") void ShowPlacementMessage(bool bIsPlayerTurn, int32 CurrentUnitIndex);
    UFUNCTION(BlueprintCallable, Category = "UI") void ShowChooseUnitTypeUI();
    void HideChooseButtons();

    UFUNCTION() void OnBrawlerChosen();
    UFUNCTION() void OnSniperChosen();

    class ABaseGameMode* GameModeRef;
};