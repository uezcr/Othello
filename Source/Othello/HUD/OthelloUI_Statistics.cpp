// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloUI_Statistics.h"

#include "Othello/Actors/OthelloActor_Board.h"
#include "Othello/GameStates/OthelloGameState_Game.h"

void UOthelloUI_Statistics::NativeConstruct()
{
	Super::NativeConstruct();
	if(AOthelloGameState_Game* OthelloGameState = CastChecked<AOthelloGameState_Game>(GetWorld()->GetGameState()))
	{
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,FString("OthelloState"));
		Othello = OthelloGameState->GetOthello();
		if(IsValid(Othello))
		{
			GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,FString("Othello"));
			Othello->OnChessSpawn.AddDynamic(this,&ThisClass::OnChessSpawnCallBack);
		}
	}
}

void UOthelloUI_Statistics::OnChessSpawnCallBack(const int32& Black, const int32& White)
{
	BlackChessNum = Black;
	WhiteChessNum = White;
}


