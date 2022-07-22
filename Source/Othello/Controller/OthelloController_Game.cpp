// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloController_Game.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Othello/GameStates/OthelloGameState_Game.h"
#include "Othello/Actors/OthelloActor_Board.h"

void AOthelloController_Game::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOthelloController_Game, Auto);
}

void AOthelloController_Game::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Othello_MoveUp",IE_Pressed,this,&ThisClass::OthelloMoveUp);
	InputComponent->BindAction("Othello_MoveDown",IE_Pressed,this,&ThisClass::OthelloMoveDown);
	InputComponent->BindAction("Othello_MoveLeft",IE_Pressed,this,&ThisClass::OthelloMoveLeft);
	InputComponent->BindAction("Othello_MoveRight",IE_Pressed,this,&ThisClass::OthelloMoveRight);
	InputComponent->BindAction("Othello_Confirm",IE_Pressed,this,&ThisClass::OthelloConfirm);
}

void AOthelloController_Game::BeginPlay()
{
	Super::BeginPlay();
	OthelloGameState = CastChecked<AOthelloGameState_Game>(GetWorld()->GetGameState());
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle,this,&ThisClass::JoinGame,1.0f);
}

void AOthelloController_Game::JoinGame()
{
	if(OthelloGameState)
	{
		SetViewTargetWithBlend(OthelloGameState->GetOthello(),1.0f,EViewTargetBlendFunction::VTBlend_Linear,0,true);
		if(OthelloGameState->GetOthello()->CanJoin())
		{
			OthelloGameState->GetOthello()->GameJoin(this);
			OthelloGameState->GetOthello()->GameStart();
		}
	}
}

void AOthelloController_Game::OthelloMoveUp()
{
	if(OthelloGameState)
	{
		OthelloGameState->GetOthello()->EventMove(this,FCoordinate{-1,0});
	}
}

void AOthelloController_Game::OthelloMoveDown()
{
	if(OthelloGameState)
	{
		OthelloGameState->GetOthello()->EventMove(this,FCoordinate{1,0});
	}
}

void AOthelloController_Game::OthelloMoveLeft()
{
	if(OthelloGameState)
	{
		OthelloGameState->GetOthello()->EventMove(this,FCoordinate{0,-1});
	}
}

void AOthelloController_Game::OthelloMoveRight()
{
	if(OthelloGameState)
	{
		OthelloGameState->GetOthello()->EventMove(this,FCoordinate{0,1});
	}
}

void AOthelloController_Game::OthelloConfirm()
{
	GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,FString("Confirm"));
	if(IsValid(OthelloGameState))
	{
		OthelloGameState->GetOthello()->EventConfirm(this);
	}
}

AOthelloGameState_Game* AOthelloController_Game::GetOthelloGameState()
{
	AOthelloGameState_Game* GameState = CastChecked<AOthelloGameState_Game>(GetWorld()->GetGameState());
	if(GameState)
	{
		return GameState;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,FString("Failed to get GameState"));
		return nullptr;
	}
}


void AOthelloController_Game::EventOthelloRestart_Implementation()
{
	if(IsValid(OthelloGameState))
	{
		OthelloGameState->GetOthello()->EventRestart();
	}
}

void AOthelloController_Game::EventOthelloUndo_Implementation()
{
	if(IsValid(OthelloGameState))
	{
		OthelloGameState->GetOthello()->EventUndo(this);
	}
}

