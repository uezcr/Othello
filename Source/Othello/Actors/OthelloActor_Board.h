// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Othello/Interface/Othello_Library.h"
#include "OthelloActor_Board.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AOthelloActor_Selector;
class UTimelineComponent;


//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOn)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnIndexChanged);
UCLASS()
class OTHELLO_API AOthelloActor_Board : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOthelloActor_Board(const FObjectInitializer& ObjectInitializer=FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	//Components
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Comoponents",meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* BoardMesh;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Comoponents",meta=(AllowPrivateAccess=true))
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Comoponents",meta=(AllowPrivateAccess=true))
	UCameraComponent* CameraComponent;
	//Delegate
	FOnTurnIndexChanged OnTurnIndexChanged;
	//Init
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


	//Variables
	TMap<int32,FCoordinate> IndexCoord;
	FCoordinate Size{7,7};
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
	
	UPROPERTY(ReplicatedUsing=OnRep_TurnIndex)
	int32 TurnIndex;
	UFUNCTION()
	void OnRep_TurnIndex(int32 LastTurnIndex);

	UPROPERTY(Replicated)
	AOthelloActor_Selector* Selector;
	UPROPERTY(Replicated)
	FCoordinate LastOffset;

	TArray<FVector> Offsets;
	FVector TargetPosition;
	FVector GetOffset(const FCoordinate InOffset);
	void MoveSelector(const FCoordinate offset);
};
