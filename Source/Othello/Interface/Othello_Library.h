// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Othello_Library.generated.h"


USTRUCT(BlueprintType)
struct FCoordinate
{
	GENERATED_BODY()
	int32 Row;
	int32 Column;
};

USTRUCT(BlueprintType)
struct FHistory
{
	GENERATED_BODY()
	FCoordinate InCoord;
	TArray<int32> ReverseIndices;
};
/**
 * 
 */
UCLASS()
class OTHELLO_API UOthello_Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static FCoordinate CoordinatePlus(const FCoordinate A,const FCoordinate B);
	static FCoordinate CoordinateSubs(const FCoordinate& A, const FCoordinate& B);
	static FCoordinate OffsetClamp(const FCoordinate InOffset,int32 Min,int32 Max);
	
};
