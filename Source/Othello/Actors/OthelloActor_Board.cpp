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
#include "Kismet/KismetMathLibrary.h"
#include "Othello/Controller/OthelloController_Game.h"



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
	DOREPLIFETIME(AOthelloActor_Board, TurnIndexed);
	DOREPLIFETIME(AOthelloActor_Board, Offsets);
	DOREPLIFETIME(AOthelloActor_Board, ChessBoard);
	DOREPLIFETIME(AOthelloActor_Board, Chess);
	DOREPLIFETIME(AOthelloActor_Board, Selector);
	DOREPLIFETIME(AOthelloActor_Board, LastCoordinate);
	DOREPLIFETIME(AOthelloActor_Board, TargetPosition);
	DOREPLIFETIME(AOthelloActor_Board, Histories);
	DOREPLIFETIME(AOthelloActor_Board, AIController);
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
		// for (int j = 1; j <= Size.Column + 1; ++j)
		// {
		// 	float X = j * 5.0f + j * 0.1f - 2.55f;
		// 	float Y = i * 5.0f + i * 0.1f - 2.55f;
		// 	FVector A = FVector(X, Y, 2.5f);
		// 	FVector B = A - FVector(20.4f, 20.4f, 0.0f);
		// 	Offsets.Add(FVector(-B.X, -B.Y, B.Z));
		// }
		for (int j = 1; j <= Size.Column + 1; ++j)
		{
			float X = j * 6.0625f - 3.03125f;
			float Y = i * 6.0625f - 3.03125f;
			FVector A = FVector(X, Y, 2.0f);
			FVector B = A - FVector(24.25f, 24.25f, 0.0f);
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

const bool AOthelloActor_Board::ValidGrid(TArray<int32>& Reverse,const TArray<int32>& InChessBoard, const FCoordinate& InCoordinate,
	const int32& InChess)
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

const bool AOthelloActor_Board::ValidGrid(const TArray<int32>& InChessBoard, const FCoordinate& InCoordinate, const int32& InChess)
{
	int32 Grid = GetChessByCoordinate(InChessBoard, InCoordinate);
	if (Grid == 0 || Grid == 1)
	{
		return false;
	}
	else if (InChess == -1)
	{
		return false;
	}
	else
	{
		int32 DirIndex = 0;
		TArray<int32> ReverseCur;
		TArray<int32> ReverseAll;
		FCoordinate NextCoordinate;
		for (int i = 0; i <= 7; ++i)
		{
			DirIndex = i;
			ReverseCur.Empty();
			NextCoordinate = UOthello_Library::CoordinatePlus(InCoordinate, DirOffsets[DirIndex]);
			NextCoordinated(NextCoordinate, InChessBoard, DirIndex, InChess, ReverseCur, ReverseAll);
		}
		return ReverseAll.Num() > 0;
	}
}

const FColor AOthelloActor_Board::GetColor()
{
	return PlayerColors[TurnIndex];
}

const bool AOthelloActor_Board::ValidTurn(TArray<FCoordinate>& InCoords, const TArray<int32>& InChessBoard, const int32& InChess)
{
	TArray<FCoordinate> Coords;
	FCoordinate Coord;
	bool Flag = false;
	for (auto& Tmp : IndexCoord)
	{
		Coord = Tmp.Value;
		if (ValidGrid(InChessBoard, Coord, InChess))
		{
			Flag = true;
			Coords.Add(Coord);
		}
	}
	InCoords = Coords;
	return Flag;
}

const bool AOthelloActor_Board::ValidTurn(const TArray<int32>& InChessBoard, const int32& InChess)
{
	FCoordinate Coord;
	bool Flag = false;
	for (auto& Tmp : IndexCoord)
	{
		Coord = Tmp.Value;
		if (ValidGrid(InChessBoard, Coord, InChess))
		{
			Flag = true;
			break;
		}
	}
	return Flag;
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

const bool AOthelloActor_Board::InCorner(const FCoordinate& InCoord)
{
	bool A = InCoord.Row==0&&InCoord.Column==0;
	bool B = InCoord.Row==7&&InCoord.Column==0;
	bool C = InCoord.Row==0&&InCoord.Column==7;
	bool D = InCoord.Row==7&&InCoord.Column==7;
	return A||B||C||D;
}

const bool AOthelloActor_Board::InSquare(const FCoordinate& InCoord)
{
	bool A = InCoord.Row<2&&InCoord.Column<2;
	bool B =((7-InCoord.Row)<2)&&InCoord.Column<2;
	bool C = InCoord.Row<2&&((7-InCoord.Column)<2);
	bool D = ((7-InCoord.Row)<2)&&((7-InCoord.Column)<2);
	return A||B||C||D;
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

const FCoordinate AOthelloActor_Board::GetLastCoordinate()
{
	return LastCoordinate;
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
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("TurnIndex = %d"), TurnIndex));
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
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("InChess = %d"), InChess));
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
			GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Red,TEXT(">>不能下在这里 ！"));
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
	OnChessSpawn.Broadcast(BlackChessNum,WhiteChessNum);
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
	SetLastCoordinate(FCoordinate{4,3});
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

void AOthelloActor_Board::SetHiddenSelector(const bool& NewHidden)
{
	if(IsValid(Selector)) Selector->SetHidden(NewHidden);
}

const EMode AOthelloActor_Board::GetMode()
{
	if (AOthelloGameMode_Game* _GameMode = CastChecked<AOthelloGameMode_Game>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		return _GameMode->GameplayMode;
	}
	return EMode::AI;
}

const bool AOthelloActor_Board::CanJoin()
{
	return Players.Num() < 2;
}

void AOthelloActor_Board::GameJoin(APlayerController* PlayerController)
{
	Players.Add(PlayerController);
	GameStart();
}

void AOthelloActor_Board::GameLeave(APlayerController* PlayerController)
{
	Players.Remove(PlayerController);
}

void AOthelloActor_Board::GameStart()
{
	EMode _Mode = GetMode();
	auto LamdaSpawn = [&]() {
		Init();
		SpawnChess(false, FCoordinate{ 3,3 }, 1);
		SpawnChess(false, FCoordinate{ 4,4 }, 1);
		SpawnChess(false, FCoordinate{ 4,3 }, 0);
		SpawnChess(false, FCoordinate{ 3,4 }, 0);
		SpawnSelector();
		GameTurn();
	};
	if (_Mode == EMode::AI)
	{
		AISpawn();
		LamdaSpawn();
	}
	else if (_Mode == EMode::Friend)
	{
		AIDestroy();
		if (Players.Num() <= 1)
		{
			Players.SetNum(2);
		}
		Players[1] = Players[0];
		LamdaSpawn();
	}
	else if (_Mode == EMode::Online)
	{
		AIDestroy();
		if (Players.Num() > 1)
		{
			Init();
		}
	}
}

void AOthelloActor_Board::GameRestart()
{
	GameStart();
}

void AOthelloActor_Board::GameEnd()
{

}

void AOthelloActor_Board::GameUndo()
{
	if (Histories.Num() <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString(" >> Error : Unable to undo."));
		return;
	}
	FHistory LastHistory = Histories.Last();
	Histories.RemoveAt(Histories.Num()-1);
	IndexCoord.Add(Convert2D(LastHistory.InCoord), LastHistory.InCoord);
	SetChess2D(LastHistory.InCoord, -1);
	Chess[Convert2D(LastHistory.InCoord)]->SpawnTimeLine(false);
	for (int32 &i : LastHistory.ReverseIndices)
	{
		int32 _InChess = (Chess[i]->Chess + 1) % 2;
		SetChess1D(i, _InChess);
		Chess[i]->Reverse(_InChess);
	}
}

void AOthelloActor_Board::GameTurn()
{
	TurnIndex = (TurnIndex + 1) % 2;
	Selector->SetColor(GetColor());
	if (ValidTurn(ChessBoard, TurnIndex))
	{
		EndCount = 0;
		if (AIController == GetPlayerByTurn())
		{
			SetHiddenSelector(true);
			EventTurnAI();
		}
		else if (GetPlayerAuto())
		{
			SetHiddenSelector(true);
			EventTurnAI();
		}
		else
		{
			EnableInput(GetPlayerByTurn());
			SetOwner(GetPlayerByTurn());
		}
	}
	else
	{
		++EndCount;
		if (EndCount > 1)
		{
			GameEnd();
		}
		else
		{
			GameTurn();
		}
	}
}

APlayerController* AOthelloActor_Board::GetAIController()
{
	return AIController;
}

void AOthelloActor_Board::AISpawn()
{
	if (!IsValid(AIController))
	{
		AIController = GetWorld()->SpawnActor<APlayerController>(APlayerController::StaticClass(),FTransform(FRotator::ZeroRotator,FVector::ZeroVector));
		if (AIController)
		{
			Players.Add(AIController);
		}
	}
}

void AOthelloActor_Board::AIDestroy()
{
	if (IsValid(AIController))
	{
		AIController->Destroy();
	}
}

void AOthelloActor_Board::AITurn(const int32& InChess)
{
	TArray<FCoordinate> _Coordinates;
	TArray<FCoordinate> BestCoordinates;
	FCoordinate _Coord;
	int32 Level = 2;
	if (ValidTurn(_Coordinates, ChessBoard, InChess))
	{
		if(Level == 0)
		{
			int32 AIChoose = UKismetMathLibrary::RandomIntegerInRange(0, _Coordinates.Num() - 1);
			_Coord = _Coordinates[AIChoose];
			EventConfirmAI(_Coord, InChess);
		}
		else if(Level == 1)
		{
			for(FCoordinate& Ref:_Coordinates)
			{
				int32 BestNum = -1;
				TArray<int32> _Reverse;
				ValidGrid(_Reverse,ChessBoard,Ref,InChess);
				if(_Reverse.Num()>BestNum)
				{
					BestCoordinates.Empty();
					BestCoordinates.Add(Ref);
					BestNum = _Reverse.Num();
				}
				else if(_Reverse.Num()==BestNum)
				{
					BestCoordinates.Add(Ref);
				}
			}
			_Coord = BestCoordinates[UKismetMathLibrary::RandomIntegerInRange(0,BestCoordinates.Num()-1)];
			EventConfirmAI(_Coord,InChess);
		}
		else if(Level == 2)
		{
			TArray<FCoordinate> HrdBestCoords;
			int32 HrdCost = 0;
			int32 HrdBestNum = -1024;
			for(FCoordinate& Ref:_Coordinates)
			{
				_Coord = Ref;
				HrdCost = AIDeepin(_Coord,ChessBoard);
				if(HrdCost>HrdBestNum)
				{
					HrdBestCoords.Empty();
					HrdBestCoords.Add(_Coord);
					HrdBestNum = HrdCost;
				}
				else if(HrdCost==HrdBestNum)
				{
					HrdBestCoords.Add(_Coord);
				}
			}		
			int32 AIChoose = UKismetMathLibrary::RandomIntegerInRange(0,HrdBestCoords.Num()-1);
			_Coord = HrdBestCoords[AIChoose];
			EventConfirmAI(_Coord,InChess);
		}
	}
}

int32 AOthelloActor_Board::AIDeepin(const FCoordinate& InCoordinate, const TArray<int32>& InChessBoard)
{
	//then 0
	int32 _Cost = -100005;
	TArray<int32> _ChessBoard1 = InChessBoard;
	TArray<int32> _Reverse;
	ValidGrid(_Reverse,_ChessBoard1,InCoordinate,TurnIndex);
	int32 _CostAI = AICost[Convert2D(InCoordinate)]+_Reverse.Num();
	AISimulate(_ChessBoard1,_Reverse,InCoordinate,TurnIndex);
	//then 1
	int32 NumCorner = 0;
	int32 NumSquare = 0;
	TArray<FCoordinate> _CoordsPlayer;
	ValidTurn(_CoordsPlayer,_ChessBoard1,(TurnIndex+1)%2);
	FCoordinate _Coord;
	for(FCoordinate& Ref:_CoordsPlayer)
	{
		_Coord = Ref;
		NumCorner++;
		if(InCorner(_Coord))
		{
			return -1800; 
		}
		else if(InSquare(_Coord))
		{
			NumSquare++;
		}
	}
	//then 2
	if((NumSquare*10)>(NumCorner*7))
	{
		return 1400;
	}
	//then 3
	TArray<int32> _ChessBoard2 = _ChessBoard1;
	int32 _CostPlayer;
	for(FCoordinate& Ref: _CoordsPlayer)
	{
		_Coord = Ref;
		ValidGrid(_Reverse,_ChessBoard2,_Coord,(TurnIndex+1)%2);
		_CostPlayer = AICost[Convert2D(_Coord)]+_Reverse.Num();
		AISimulate(_ChessBoard2,_Reverse,_Coord,(TurnIndex+1)%2);
		_Cost = _Cost>(_CostAI-_CostPlayer) ? _Cost:(_CostAI-_CostPlayer);
	}
	//then 4
	return _Cost;
}

void AOthelloActor_Board::AISimulate(TArray<int32>& InChessboard, TArray<int32>& InReverse,
	const FCoordinate& InCoordinate, const int32 InChess)
{
	for(int32& i: InReverse)
	{
		if(i>=InChessboard.Num())
		{
			InChessboard.SetNum(i+1);
		}
		InChessboard[i] = InChess;
	}
	if(Convert2D(InCoordinate)>=InChessboard.Num())
	{
		InChessboard.SetNum(Convert2D(InCoordinate)+1);
	}
	InChessboard[Convert2D(InCoordinate)] = InChess;
}

APlayerController* AOthelloActor_Board::GetPlayerByTurn()
{
	if (Players.Num() > TurnIndex) return Players[TurnIndex];
	return nullptr;
}

const bool AOthelloActor_Board::GetPlayerAuto()
{
	if (AOthelloController_Game* OthelloController = CastChecked<AOthelloController_Game>(GetPlayerByTurn()))
	{
		return OthelloController->Auto;
	}
	return false;
}

void AOthelloActor_Board::AIDelayTimerElapsed()
{
	AITurn(TurnIndex);
}

void AOthelloActor_Board::EventRestart_Implementation()
{
	GameRestart();
}

void AOthelloActor_Board::EventConfirm_Implementation(const APlayerController* PlayerController)
{
	if(InTurn(PlayerController))
	{
		SpawnChess(true,GetLastCoordinate(),GetChessByTurn());
	}
}

void AOthelloActor_Board::EventMove_Implementation(const APlayerController* PlayerController,
                                                   const FCoordinate& InOffset)
{
	if(InTurn(PlayerController))
	{
		MoveSelector(InOffset);
	}
}

void AOthelloActor_Board::EventUndo_Implementation(const APlayerController* PlayerController)
{
	if(InTurn(PlayerController))
	{
		GameUndo();
	}
}

void AOthelloActor_Board::EventTurnAI_Implementation()
{
	GetWorldTimerManager().SetTimer(AIDelayHandle,this,&ThisClass::AIDelayTimerElapsed,1.0f);
	//AITurn(TurnIndex);
}

void AOthelloActor_Board::EventConfirmAI_Implementation(const FCoordinate& InCoordinate, const int32& InChess)
{
	FCoordinate AIMove = UOthello_Library::CoordinateSubs(InCoordinate, LastCoordinate);
	MoveSelector(AIMove);
	SpawnChess(true,InCoordinate, InChess);
}

void AOthelloActor_Board::MoveSelector(const FCoordinate offset)
{
	if(IsValid(Selector))
	{
		LastCoordinate = UOthello_Library::OffsetClamp(UOthello_Library::CoordinatePlus(LastCoordinate,offset),0,7);
		TargetPosition=GetOffset(LastCoordinate);
		Selector->SetActorRelativeLocation(TargetPosition);
		SetHiddenSelector(false);
	}
}

// Called when the game starts or when spawned
void AOthelloActor_Board::BeginPlay()
{
	Super::BeginPlay();
	TurnIndexed = 0;
//	GameStart();
}

// Called every frame
void AOthelloActor_Board::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

