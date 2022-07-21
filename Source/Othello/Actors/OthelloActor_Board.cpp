// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloActor_Board.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/HUD.h"
#include "Othello/Interface/OthelloInterface_HUD.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "OthelloActor_Selector.h"
#include "OthelloActor_Chess.h"
#include "Kismet/GameplayStatics.h"



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
	DOREPLIFETIME(AOthelloActor_Board, Histories);
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
	Histories.Empty();
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
	InitOffsets();
	InitChessBoard();
	InitChess();
	InitSelector();
	InitHistory();
	InitTurn();
	InitDifficulty();
	InitDebug();
}

const bool AOthelloActor_Board::ValidGrid(TArray<int32>& Reverse,const TArray<int32> InChessBoard, const FCoordinate InCoordinate,
	const int32 InChess)
{
	int32 Grid = GetChessByCoordinate(InChessBoard,InCoordinate);
	if(Grid==0||Grid==1)
	{
		Reverse = TArray<int32>{};
		return false;
	}
	else if(InChess==-1)
	{
		Reverse = TArray<int32>{};
		return false;
	}
	else
	{
		int32 DirIndex=0;
		TArray<int32> ReverseCur;
		TArray<int32> ReverseAll;
		FCoordinate NextCoordinate;
		for(int i =0;i<=7;++i)
		{
			DirIndex=i;
			ReverseCur.Empty();
			NextCoordinate = UOthello_Library::CoordinatePlus(InCoordinate,DirOffsets[DirIndex]);
			NextCoordinated(NextCoordinate,InChessBoard,DirIndex,InChess,ReverseCur,ReverseAll);
		}
		Reverse = ReverseAll;
		return ReverseAll.Num()>0;
	}
}

void AOthelloActor_Board::NextCoordinated(FCoordinate& InNextCoord, const TArray<int32> InChessBoard,const int32 InDirIndex,const int32 InChess, TArray<int32>& InCur,
	TArray<int32>& InAll)
{
	if(!InBound(InNextCoord))
	{
		return;
	}
	int32 NextGrid = GetChessByCoordinate(InChessBoard,InNextCoord);
	if(InChess==1)
	{
		if(NextGrid==1)
		{
			InAll.Append(InCur);
			return;
		}
		else if(NextGrid==0)
		{
			InCur.Add(Convert2D(InNextCoord));
			InNextCoord=UOthello_Library::CoordinatePlus(InNextCoord,DirOffsets[InDirIndex]);
			NextCoordinated(InNextCoord,InChessBoard,InDirIndex,InChess,InCur,InAll);
		}
	}
	else if(InChess==0)
	{
		if(NextGrid==0)
		{
			InAll.Append(InCur);
			return;
		}
		else if(NextGrid==1)
		{
			InCur.Add(Convert2D(InNextCoord));
			InNextCoord = UOthello_Library::CoordinatePlus(InNextCoord,DirOffsets[InDirIndex]);
			NextCoordinated(InNextCoord,InChessBoard,InDirIndex,InChess,InCur,InAll);
		}
	}
}

int32 AOthelloActor_Board::Convert2D(const FCoordinate InCoordinate)
{
	int32 Value = (Size.Column+1) * InCoordinate.Row + InCoordinate.Column;
	return Value;
}

void AOthelloActor_Board::SetChess1D(int32 Index, int32 InChess)
{
	if (Index>=ChessBoard.Num())
	{
		ChessBoard.SetNum(Index+1);
	}
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

const bool AOthelloActor_Board::RemoveCoordinate(const FCoordinate& InCoordinate)
{
	return IndexCoord.Remove(Convert2D(InCoordinate)) > 0;
}

FVector AOthelloActor_Board::GetOffset(const FCoordinate InOffset)
{
	int32 Index = (Size.Column+1) * InOffset.Row +InOffset.Column;
	return Offsets[Index];
}

const bool AOthelloActor_Board::InBound(const FCoordinate InCoordinate)
{
	if((InCoordinate.Row>=0&&InCoordinate.Row<=7)&&(InCoordinate.Column>=0&&InCoordinate.Column<=7))
	{
		return true;
	}
	return false;
}

int32 AOthelloActor_Board::GetChessByTurn()
{
	return TurnIndex;
}

int32 AOthelloActor_Board::GetChessByCoordinate(const TArray<int32> InChessBoard, const FCoordinate InCoordinate)
{
	return InChessBoard[Convert2D(InCoordinate)];
}

const bool AOthelloActor_Board::ValidChess(const TArray<int32> InChessBoard, const FCoordinate InCoordinate)
{
	if(GetChessByCoordinate(InChessBoard,InCoordinate)>=0) return true;
	return false;
}

void AOthelloActor_Board::CountChess(int32& White, int32& Black)
{
	int32 _White = 0;
	int32 _Black = 0;
	for (int32 i : ChessBoard)
	{
		if(i==0) _Black++;
		if(i==1) _White++;
	}
	White = _White;
	Black = _Black;
}

void AOthelloActor_Board::SpawnChess(const bool& CheckTurn, const FCoordinate& InCoordinate, const int32& InChess)
{
	if(CheckTurn)
	{
		TArray<int32> Reverse;
		if(ValidGrid(Reverse,ChessBoard,InCoordinate,InChess))
		{
			Histories.Add(FHistory{InCoordinate,Reverse});
			ReverseChess(Reverse,InChess);
			FTransform SpawnTransform(FRotator::ZeroRotator,GetOffset(InCoordinate));
			AOthelloActor_Chess * SpawnChess = GetWorld()->SpawnActorDeferred<AOthelloActor_Chess>(ChessClass,SpawnTransform);
			if(SpawnChess)
			{
				SpawnChess->Chess = InChess;
				UGameplayStatics::FinishSpawningActor(SpawnChess,SpawnTransform);
				int32 ChessIndex = Convert2D(InCoordinate);
				if(ChessIndex>=Chess.Num())
				{
					Chess.SetNum(ChessIndex+1);
				}
				Chess[ChessIndex]=SpawnChess;
				SpawnChess->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);
				SetChess2D(InCoordinate,InChess);
				if(CheckTurn)
				{
					GameTurn();
				}
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Red,TEXT(">> Error : You can not place a chess here."));
		}
	}
	else
	{
		FTransform SpawnTransform(FRotator::ZeroRotator,GetOffset(InCoordinate));
		AOthelloActor_Chess* SpawnChess = GetWorld()->SpawnActorDeferred<AOthelloActor_Chess>(ChessClass,SpawnTransform);
		if(SpawnChess)
		{
			SpawnChess->Chess = InChess;
			UGameplayStatics::FinishSpawningActor(SpawnChess,SpawnTransform);
			int32 ChessIndex = Convert2D(InCoordinate);
			//GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Red,FString::Printf(TEXT("%d"),ChessIndex));
			if(ChessIndex>=Chess.Num())
			{
				Chess.SetNum(ChessIndex+1);
			}
			Chess[ChessIndex] = SpawnChess;
			SpawnChess->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);
			SetChess2D(InCoordinate,InChess);
			if(CheckTurn)
			{
				GameTurn();
			}
		}
	}
}


void AOthelloActor_Board::ReverseChess(const TArray<int32>& Index, const int32& ToChess)
{
	for(int32 i : Index)
	{
		SetChess1D(i,ToChess);
		if(IsValid(Chess[i])) Chess[i]->Reverse(ToChess);
	}
}

void AOthelloActor_Board::RemoveChess()
{
	for(auto RefChess:Chess)
	{
		if(IsValid(RefChess))
		{
			RefChess->Remove();
		}
	}
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

void AOthelloActor_Board::GameTurn()
{
}

void AOthelloActor_Board::MoveSelector(const FCoordinate offset)
{
	if(IsValid(Selector))
	{
		LastCoordinate = UOthello_Library::OffsetClamp(UOthello_Library::CoordinatePlus(LastCoordinate,offset),0,7);
		TargetPosition=GetOffset(LastCoordinate);
		Selector->SetActorRelativeLocation(TargetPosition);
	}
}

// Called when the game starts or when spawned
void AOthelloActor_Board::BeginPlay()
{
	Super::BeginPlay();
	SpawnChess(false,FCoordinate{3,3},0);
	SpawnChess(false,FCoordinate{4,4},0);
	SpawnChess(false,FCoordinate{4,3},1);
	SpawnChess(false,FCoordinate{3,4},1);
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

