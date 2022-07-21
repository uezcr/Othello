// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Othello/Interface/Othello_Library.h"
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
	//ChessBoardFunction
	int32 Convert2D(const FCoordinate InCoordinate);
	void SetChess1D(int32 Index, int32 Chess);
	void SetChess2D(const FCoordinate InCoordonate, int32 InChess);
	void SetLastCoordinate(const FCoordinate InCoordinate);
	FVector GetOffset(const FCoordinate InOffset);
	//SeletorFunction
	void SpawnSelector();
	void MoveSelector(const FCoordinate offset);
	void RemoveSelector();
	void ShowSelector(const bool NewHidden);


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
	int32 TurnIndex;
	UPROPERTY(Replicated)
	int32 Testint32;
	UPROPERTY(Replicated)
	FCoordinate LastCoordinate;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="DefaultVariables",meta=(AllowPrivateAccess=true))
	TSubclassOf<AOthelloActor_Selector> SelectorClass;
	UPROPERTY(Replicated)
	FVector TargetPosition;

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

	
	const APlayerController* GetPlayerByTurn();
	bool InTurn(const APlayerController* PlayerController);
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Othello",meta=(AllowPrivateAccess=true))
	TArray<APlayerController*> Players;

	UFUNCTION()
	void OnRep_TurnIndex(int32 LastTurnIndex);
	

	

	
};
