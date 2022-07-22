// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "OthelloGameMode_Game.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMode : uint8
{
	AI		UMETA(DisplayName = "AI"),
	Friend		UMETA(DisplayName = "Friend"),
	Online		UMETA(DisplayName = "Online")
};

UCLASS()
class OTHELLO_API AOthelloGameMode_Game : public AGameMode
{
	GENERATED_BODY()
public:
	EMode GameplayMode = EMode::AI;
	
};
