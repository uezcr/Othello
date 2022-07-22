// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OthelloController_Game.generated.h"

class AOthelloActor_Board;
class AOthelloGameState_Game;
/**
 * 
 */
UCLASS()
class OTHELLO_API AOthelloController_Game : public APlayerController
{
	GENERATED_BODY()	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupInputComponent() override;
	UPROPERTY(Replicated)
	bool Auto;
protected:
	virtual void BeginPlay() override;
	void JoinGame();
	//InputFunction
	void OthelloMoveUp();
	void OthelloMoveDown();
	void OthelloMoveLeft();
	void OthelloMoveRight();
	void OthelloConfirm();
private:
	//UtilitiesFunction
	AOthelloGameState_Game* GetOthelloGameState();
	AOthelloGameState_Game* OthelloGameState;
	

	//Events
	UFUNCTION(Server,Reliable)
	void EventOthelloRestart();
	UFUNCTION(Server,Reliable)
	void EventOthelloUndo();
};
