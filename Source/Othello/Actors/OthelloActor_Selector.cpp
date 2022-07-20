// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloActor_Selector.h"


// Sets default values
AOthelloActor_Selector::AOthelloActor_Selector(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	Scene=CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);
	SelectorMesh=CreateDefaultSubobject<UStaticMeshComponent>("SelectorMesh");
	SelectorMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AOthelloActor_Selector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOthelloActor_Selector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

