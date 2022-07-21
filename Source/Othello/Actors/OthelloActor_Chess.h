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
	void Reverse(const int32 ToChess);
	void Remove();
	int32 Chess;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SpawnTimeLine(bool Reverse);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	USceneComponent* Scene;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* ChessMesh;
	void Init();

	

};
