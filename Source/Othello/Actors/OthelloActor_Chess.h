// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OthelloActor_Chess.generated.h"

UCLASS()
class OTHELLO_API AOthelloActor_Chess : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOthelloActor_Chess(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//NativeEventFunction
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void SpawnTimeLine(bool IsStart);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReverseTimeLine(bool IsStart);
	//ChessFunction
	void Reverse(const int32 ToChess);
	void Remove();
	void SetChess(const int32& InChess);
	//Variables
	UPROPERTY(Replicated)
	int32 Chess;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	USceneComponent* Scene;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* ChessMesh;
	//ChessFunction
	void Init();
};
