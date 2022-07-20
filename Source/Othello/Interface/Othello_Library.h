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
/**
 * 
 */
UCLASS()
class OTHELLO_API UOthello_Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static FCoordinate OffsetPlus(const FCoordinate A,const FCoordinate B);
	static FCoordinate OffsetClamp(const FCoordinate InOffset,int32 Min,int32 Max);
	
};