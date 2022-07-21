// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloActor_Board.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/HUD.h"
#include "Othello/Interface/OthelloInterface_HUD.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "OthelloActor_Selector.h"
#include "OthelloActor_Chess.h"


AOthelloActor_Board::AOthelloActor_Board(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	BoardMesh = CreateDefaultSubobject<UStaticMeshComponent>("BoardMesh");
	SetRootComponent(BoardMesh);
	SpringArmComponent=CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bDoCollisionTest=false;
	CameraComponent= CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	OnTurnIndexChanged.AddDynamic(this,&ThisClass::OnTurnIndexChangedFunction);

	Init();
	
}

void AOthelloActor_Board::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOthelloActor_Board, TurnIndex);
	DOREPLIFETIME(AOthelloActor_Board, Offsets);
	DOREPLIFETIME(AOthelloActor_Board, ChessBoard);
	DOREPLIFETIME(AOthelloActor_Board, Chess);
	DOREPLIFETIME(AOthelloActor_Board, Selector);
	DOREPLIFETIME(AOthelloActor_Board, LastCoordinate);
	DOREPLIFETIME(AOthelloActor_Board, TargetPosition);
	DOREPLIFETIME(AOthelloActor_Board, Testint32);
	
}

void AOthelloActor_Board::OnTurnIndexChangedFunction()
{
	
}

void AOthelloActor_Board::InitCoords()
{
	IndexCoord.Empty();
	for(int i= 0;i<=Size.Row;++i)
	{
		for(int j=0;j<=Size.Column;++j)
		{
			FCoordinate AddedCoordinate{i,j};
			IndexCoord.Add(IndexCoord.Num(),AddedCoordinate);
		}
	}
}

void AOthelloActor_Board::InitOffsets()
{
	Offsets.Empty();
	for (int i = 1; i <= Size.Row + 1; ++i)
	{
		for (int j = 1; j <= Size.Column + 1; ++j)
		{
			float X = j * 5.0f + j * 0.1f - 2.55f;
			float Y = i * 5.0f + i * 0.1f - 2.55f;
			FVector A = FVector(X, Y, 2.5f);
			FVector B = A - FVector(20.4f, 20.4f, 0.0f);
			Offsets.Add(FVector(-B.X, -B.Y, B.Z));
		}
	}
}

void AOthelloActor_Board::InitChessBoard()
{
	ChessBoard.Empty();
	ChessBoard.Init(-1, 64);
}

void AOthelloActor_Board::InitChess()
{
	for (auto i : Chess)
	{
		if (IsValid(i)) i->Destroy();
	}
	Chess.Empty();
}

void AOthelloActor_Board::InitSelector()
{
	if (IsValid(Selector)) Selector->Destroy();
}

void AOthelloActor_Board::InitHistory()
{
}

void AOthelloActor_Board::InitTurn()
{
	TurnIndex = -1;
	SetOwner(nullptr);
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

int32 AOthelloActor_Board::Convert2D(const FCoordinate InCoordinate)
{
	int32 Value = (Size.Column+1) * InCoordinate.Row + InCoordinate.Column;
	return Value;
}

void AOthelloActor_Board::SetChess1D(int32 Index, int32 InChess)
{
	ChessBoard[Index] = InChess;
}

void AOthelloActor_Board::SetChess2D(const FCoordinate InCoordonate, int32 InChess)
{
	SetChess1D(Convert2D(InCoordonate), InChess);
}

void AOthelloActor_Board::SetLastCoordinate(const FCoordinate InCoordinate)
{
	LastCoordinate = InCoordinate;
}

FVector AOthelloActor_Board::GetOffset(const FCoordinate InOffset)
{
	int32 Index = (Size.Column+1) * InOffset.Row +InOffset.Column;
	return Offsets[Index];
}

void AOthelloActor_Board::SpawnSelector()
{
	InitSelector();
	FVector SpawnLocation = GetOffset(LastCoordinate);
	FRotator SpawnRotaton = FRotator::ZeroRotator;
	FTransform SpawnTransform(SpawnRotaton,SpawnLocation);
	Selector = GetWorld()->SpawnActor<AOthelloActor_Selector>(SelectorClass,SpawnTransform);
	Selector->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);
}

void AOthelloActor_Board::RemoveSelector()
{
	if(IsValid(Selector))
	{
		Selector->Destroy();
		Selector = nullptr;
	}
}

void AOthelloActor_Board::ShowSelector(const bool NewHidden)
{
	if(IsValid(Selector)) Selector->SetHidden(NewHidden);
}

void AOthelloActor_Board::MoveSelector(const FCoordinate offset)
{
	if(IsValid(Selector))
	{
		LastCoordinate = UOthello_Library::OffsetClamp(UOthello_Library::OffsetPlus(LastCoordinate,offset),0,7);
		TargetPosition=GetOffset(LastCoordinate);
		Selector->SetActorRelativeLocation(TargetPosition);
	}
}

// Called when the game starts or when spawned
void AOthelloActor_Board::BeginPlay()
{
	Super::BeginPlay();
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

