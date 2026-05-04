// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "BluePrint/UserWidget.h"

void AMainGameMode::SetGameOver()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true); 
	
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PC != nullptr)
	{
		UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(PC, GameOverWidgetClass);

		if (GameOverWidget != nullptr)
		{
			GameOverWidget->AddToViewport();
		}
	}
}
