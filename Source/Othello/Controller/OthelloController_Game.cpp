// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloController_Game.h"
#include "Net/UnrealNetwork.h"

void AOthelloController_Game::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOthelloController_Game, Auto);
}
