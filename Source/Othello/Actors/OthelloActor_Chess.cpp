// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloActor_Chess.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AOthelloActor_Chess::AOthelloActor_Chess(const FObjectInitializer& ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);
	ChessMesh = CreateDefaultSubobject<UStaticMeshComponent>("ChessMesh");
	ChessMesh->SetupAttachment(RootComponent);

	Init();
}
void AOthelloActor_Chess::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOthelloActor_Chess, ChessMesh);
}
// Called when the game starts or when spawned
void AOthelloActor_Chess::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AOthelloActor_Chess::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOthelloActor_Chess::Init()
{
	if (Chess == 0)
	{
		ChessMesh->SetRelativeRotation(FRotator(0.0f,0.0f,180.0f));
	}
	else
	{
		ChessMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
}



void AOthelloActor_Chess::Reverse(const int32 ToChess)
{
	
}

void AOthelloActor_Chess::Remove()
{
	
}

void AOthelloActor_Chess::SpawnTimeLine_Implementation(bool Reverse)
{
	if (Reverse)
	{
		this->Destroy();
	}
}

