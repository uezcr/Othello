// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Othello/Interface/Othello_Library.h"
#include "Othello/GameModes/OthelloGameMode_Game.h"
#include "OthelloActor_Board.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AOthelloActor_Selector;
class AOthelloActor_Chess;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnIndexChanged);
UCLASS()
class OTHELLO_API AOthelloActor_Board : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOthelloActor_Board(const FObjectInitializer& ObjectInitializer=FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//Delegate
	FOnTurnIndexChanged OnTurnIndexChanged;
	//Events
	UFUNCTION(Server, Reliable)
    void EventRestart();
	UFUNCTION(Server, Reliable)
	void EventUndo(const APlayerController* PlayerController);
	UFUNCTION(Server, Reliable)
	void EventMove(const APlayerController* PlayerController,const FCoordinate& InOffset);
	UFUNCTION(Server, Reliable)
	void EventConfirm(const APlayerController* PlayerController);
	//GameplayFunction
	const bool CanJoin();
	void GameJoin(APlayerController* PlayerController);
	void GameLeave(APlayerController* PlayerController);
	void GameStart();
private:
	//Components
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Comoponents",meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* BoardMesh;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Comoponents",meta=(AllowPrivateAccess=true))
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Comoponents",meta=(AllowPrivateAccess=true))
	UCameraComponent* CameraComponent;

	//DelegateFunctions
	UFUNCTION()
	void OnTurnIndexChangedFunction();
	//InitFunction
	void InitCoords();
	void InitOffsets();
	void InitChessBoard();
	void InitChess();
	void InitSelector();
	void InitHistory();
	void InitTurn();
	void InitDifficulty();
	void InitDebug();
	void Init();
	//UtilitiesFunction
	const bool ValidGrid(TArray<int32>& Reverse,const TArray<int32>& InChessBoard,const FCoordinate& InCoordinate,const int32& InChess);
	const bool ValidGrid(const TArray<int32>& InChessBoard, const FCoordinate& InCoordinate, const int32& InChess);
	const FColor GetColor();
	const bool ValidTurn(TArray<FCoordinate>& InCoords, const TArray<int32>& InChessBoard, const int32& InChess);
	const bool ValidTurn(const TArray<int32>& InChessBoard, const int32& InChess);
	void NextCoordinated(FCoordinate& InNextCoord,const TArray<int32> InChessBoard,const int32 InDirIndex,const int32 InChess,TArray<int32>&InCur,TArray<int32>&InAll);
	const bool InCorner(const FCoordinate& InCoord);
	const bool InSquare(const FCoordinate& InCoord);
	//ChessBoardAndCoordinateFunction
	int32 Convert2D(const FCoordinate InCoordinate);
	void SetChess1D(int32 Index, int32 Chess);
	void SetChess2D(const FCoordinate InCoordonate, int32 InChess);
	void SetLastCoordinate(const FCoordinate InCoordinate);
	const FCoordinate GetLastCoordinate();
	const bool RemoveCoordinate(const FCoordinate& InCoordinate);
	FVector GetOffset(const FCoordinate InOffset);
	const bool InBound(const FCoordinate InCoordinate);
	int32 GetChessByTurn();
	int32 GetChessByCoordinate(const TArray<int32>InChessBoard,const FCoordinate InCoordinate);
	const bool ValidChess(const TArray<int32>InChessBoard,const FCoordinate InCoordinate); //此函数有争议
	void CountChess(int32& White,int32& Black);
	void SpawnChess(const bool& CheckTurn,const FCoordinate& InCoordinate,const int32& InChess);
	void ReverseChess(const TArray<int32>&Index,const int32& ToChess);
	void RemoveChess();
	//SeletorFunction
	void SpawnSelector();
	void MoveSelector(const FCoordinate offset);
	void RemoveSelector();
	void SetHiddenSelector(const bool& NewHidden);
	//GameplayFunction
	const EMode GetMode();
	void GameRestart();
	void GameEnd(); //Undefine
	void GameUndo();
	void GameTurn();
	//AIFunction
	APlayerController* GetAIController();
	void AISpawn();
	void AIDestroy();
	void AITurn(const int32& InChess);
	int32 AIDeepin(const FCoordinate& InCoordinate,const TArray<int32>& InChessBoard);
	void AISimulate(TArray<int32>& InChessboard,TArray<int32>& InReverse,const FCoordinate& InCoordinate,const int32 InChess);
	//PlayerFunction
	APlayerController* GetPlayerByTurn();
	const bool GetPlayerAuto(); //UnDefine

	//EventFunction
	UFUNCTION(Server, Reliable)
	void EventTurnAI();
	UFUNCTION(Server, Reliable)
	void EventConfirmAI(const FCoordinate& InCoordinate,const int32& InChess);
	//CallBackFunction
	void AIDelayTimerElapsed();
	//Variables
	TMap<int32,FCoordinate> IndexCoord;
	FCoordinate Size{7,7};
	UPROPERTY(Replicated)
	TArray<FVector> Offsets;
	UPROPERTY(Replicated)
	TArray<int32> ChessBoard;
	UPROPERTY(Replicated)
	TArray<AOthelloActor_Chess*> Chess;
	UPROPERTY(Replicated)
	AOthelloActor_Selector* Selector;
	UPROPERTY(ReplicatedUsing = OnRep_TurnIndex)
	int32 TurnIndex = -1;
	UPROPERTY(Replicated)
	int32 TurnIndexed = -1;
	UPROPERTY(Replicated)
	int32 Testint32;
	UPROPERTY(Replicated)
	FCoordinate LastCoordinate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DefaultVariables", meta = (AllowPrivateAccess = true))
	TSubclassOf<AOthelloActor_Selector> SelectorClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DefaultVariables", meta = (AllowPrivateAccess = true))
	TSubclassOf<AOthelloActor_Chess> ChessClass;
	UPROPERTY(Replicated)
	FVector TargetPosition;
	const TArray<FCoordinate> DirOffsets
	{
		{-1,0},
		{-1,1},
		{0,1},
		{1,1},
		{1,0},
		{1,-1},
		{0,-1},
		{-1,-1}
	};
	UPROPERTY(Replicated)
	TArray<FHistory> Histories;
	UPROPERTY(Replicated)
	APlayerController* AIController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultVariables", meta = (AllowPrivateAccess = true))
	TArray<FColor> PlayerColors;
	int32 EndCount;
	FTimerHandle AIDelayHandle;
	TArray<int32> AICost
	{
		90,-60,10,10,10,10,-60,90,
		-60,-80,5,5,5,5,-80,-60,
		10,5,1,1,1,1,5,10,
		10,5,1,1,1,1,5,10,
		10,5,1,1,1,1,5,10,
		10,5,1,1,1,1,5,10,
		-60,-80,5,5,5,5,-80,-60,
		90,-60,10,10,10,10,-60,90
	};
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(Client,Reliable)
	void RefreshInputHints(const APlayerController* PlayerController);
	UFUNCTION(Server,Reliable)
	void Move(const APlayerController* PlayerController,const FCoordinate offset);

	bool InTurn(const APlayerController* PlayerController);
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Othello",meta=(AllowPrivateAccess=true))
	TArray<APlayerController*> Players;

	UFUNCTION()
	void OnRep_TurnIndex(int32 LastTurnIndex);
	
	
};

