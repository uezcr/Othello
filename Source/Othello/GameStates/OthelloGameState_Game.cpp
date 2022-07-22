// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloGameState_Game.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Othello/Actors/OthelloActor_Board.h"

void AOthelloGameState_Game::BeginPlay()
{
	Super::BeginPlay();
	InitOthello();
	
}

AOthelloActor_Board* AOthelloGameState_Game::GetOthello()
{
	return Othello;
}

void AOthelloGameState_Game::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOthelloGameState_Game, Othello);
}

void AOthelloGameState_Game::InitOthello()
{
	if(!IsValid(OthelloClass))
	{
		return;
	}
	TArray<AActor*> Othellos;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),OthelloClass,Othellos);
	if(Othellos.Num()>0)
	{
		Othello = CastChecked<AOthelloActor_Board>(Othellos[0]);
	}
}
