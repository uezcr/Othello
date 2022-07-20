// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Othello/Interface/OthelloInterface_HUD.h"
#include "OthelloHUD_Game.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO_API AOthelloHUD_Game : public AHUD,public IOthelloInterface_HUD
{
	GENERATED_BODY()
public:
	void RefreshInputHints_Implementation() override;
	
};
