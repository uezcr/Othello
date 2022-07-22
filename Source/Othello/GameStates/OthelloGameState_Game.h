// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "OthelloGameState_Game.generated.h"

class AOthelloActor_Board;
/**
 * 
 */
UCLASS()
class OTHELLO_API AOthelloGameState_Game : public AGameState
{
	GENERATED_BODY()

public:
	//UtilitiesFunction
	AOthelloActor_Board* GetOthello();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	//InitFunction
	void InitOthello();
	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DefaultVariables", meta = (AllowPrivateAccess = true))
	TSubclassOf<AOthelloActor_Board> OthelloClass;
	UPROPERTY(Replicated)
	AOthelloActor_Board* Othello;
protected:
	virtual void BeginPlay() override;
};
