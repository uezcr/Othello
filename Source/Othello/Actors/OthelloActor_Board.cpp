// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloActor_Board.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/HUD.h"
#include "Othello/Interface/OthelloInterface_HUD.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "OthelloActor_Selector.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetArrayLibrary.h"

AOthelloActor_Board::AOthelloActor_Board(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	BoardMesh = CreateDefaultSubobject<UStaticMeshComponent>("BoardMesh");
	SetRootComponent(BoardMesh);
	SpringArmComponent=CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bDoCollisionTest=false;
	CameraComponent= CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	Init();
	
}

void AOthelloActor_Board::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOthelloActor_Board,TurnIndex);
	DOREPLIFETIME(AOthelloActor_Board,Selector);
	DOREPLIFETIME(AOthelloActor_Board,LastOffset);
}

void AOthelloActor_Board::InitCoords()
{
	IndexCoord.Empty();
	int Row = 0;
	int Column = 0;
	for(int i= 0;i<=Size.Row;++i)
	{
		Row = i;
		for(int j=0;j<=Size.Column;++j)
		{
			FCoordinate AddedCoordinate{i,j};
			IndexCoord.Add(IndexCoord.Num(),AddedCoordinate);
		}
	}
}

void AOthelloActor_Board::InitOffsets()
{
}

void AOthelloActor_Board::InitChessBoard()
{
}

void AOthelloActor_Board::InitChess()
{
}

void AOthelloActor_Board::InitSelector()
{
}

void AOthelloActor_Board::InitHistory()
{
}

void AOthelloActor_Board::InitTurn()
{
}

void AOthelloActor_Board::InitDifficulty()
{
}

void AOthelloActor_Board::InitDebug()
{
}

void AOthelloActor_Board::Init()
{
	InitCoords();
}

// Called when the game starts or when spawned
void AOthelloActor_Board::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,FString::Printf(TEXT("%d"),IndexCoord.Num()));
	
}

// Called every frame
void AOthelloActor_Board::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const APlayerController* AOthelloActor_Board::GetPlayerByTurn()
{
	if (Players.Num()>TurnIndex) return Players[TurnIndex];
	return nullptr;
}

bool AOthelloActor_Board::InTurn(const APlayerController* PlayerController)
{
	if(GetPlayerByTurn()==PlayerController) return true;
	return false;
}

void AOthelloActor_Board::OnRep_TurnIndex(int32 LastTurnIndex)
{
	OnTurnIndexChanged.Broadcast();
}

void AOthelloActor_Board::MoveSelector(const FCoordinate offset)
{
	if(IsValid(Selector))
	{
		LastOffset = UOthello_Library::OffsetClamp(UOthello_Library::OffsetPlus(LastOffset,offset),0,7);
		TargetPosition=GetOffset(LastOffset);
		Selector->SetActorRelativeLocation(TargetPosition);
	}
}

FVector AOthelloActor_Board::GetOffset(const FCoordinate InOffset)
{
	int32 index = Size.Column * InOffset.Row +InOffset.Column;
	return Offsets[index];
}

void AOthelloActor_Board::Move_Implementation(const APlayerController* PlayerController, const FCoordinate offset)
{
	if(InTurn(PlayerController))
	{
		MoveSelector(offset);
	}
}

void AOthelloActor_Board::RefreshInputHints_Implementation(const APlayerController* PlayerController)
{
	if (PlayerController)
	{
		AHUD* HUD = PlayerController->GetHUD();
		if(HUD->Implements<UOthelloInterface_HUD>())
		{
			IOthelloInterface_HUD::Execute_RefreshInputHints(HUD);
		}
	}
}

