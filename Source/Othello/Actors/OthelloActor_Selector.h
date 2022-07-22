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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DefaultVariables")
	UMaterialInterface* MaterialClass;
private:
	//Components
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components",meta=(AllowPrivateAccess=true))
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components",meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* SelectorMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnRep_Color(FColor LastColor);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Variables
	UPROPERTY(ReplicatedUsing=OnRep_Color)
	FColor Color;
	//SelectorFunctions
	void SetColor(const FColor& InColor);

	
};
