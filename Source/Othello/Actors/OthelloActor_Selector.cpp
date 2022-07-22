// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloActor_Selector.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AOthelloActor_Selector::AOthelloActor_Selector(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates=true;
	Scene=CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);
	SelectorMesh=CreateDefaultSubobject<UStaticMeshComponent>("SelectorMesh");
	SelectorMesh->SetupAttachment(RootComponent);
}

void AOthelloActor_Selector::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOthelloActor_Selector, Color);
}

// Called when the game starts or when spawned
void AOthelloActor_Selector::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOthelloActor_Selector::OnRep_Color(FColor LastColor)
{
	GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Blue,FString("SetColorOnRep"));
	if(IsValid(MaterialClass))
	{
		if(UMaterialInstanceDynamic* MaterialInstance = SelectorMesh->CreateDynamicMaterialInstance(0,MaterialClass,NAME_None))
		{
			MaterialInstance->SetVectorParameterValue("BaseColor",Color);
		}
	}
}

// Called every frame
void AOthelloActor_Selector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOthelloActor_Selector::SetColor(const FColor& InColor)
{
	//GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Blue,FString("SetColor"));
	Color = InColor;
	if(GetWorld()->GetFirstPlayerController()->IsLocalController())
	{
		if(IsValid(MaterialClass))
		{
			if(UMaterialInstanceDynamic* MaterialInstance = SelectorMesh->CreateDynamicMaterialInstance(0,MaterialClass,NAME_None))
			{
				MaterialInstance->SetVectorParameterValue("BaseColor",Color);
			}
		}
	}
}


