// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OthelloActor_Selector.generated.h"

UCLASS()
class OTHELLO_API AOthelloActor_Selector : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOthelloActor_Selector(const FObjectInitializer& ObjectInitializer=FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components",meta=(AllowPrivateAccess=true))
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components",meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* SelectorMesh;
};
