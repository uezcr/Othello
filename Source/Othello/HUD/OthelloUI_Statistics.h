// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OthelloUI_Statistics.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO_API UOthelloUI_Statistics : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Variables",meta=(AllowPrivateAccess=true))
	int32 WhiteChessNum;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Variables",meta=(AllowPrivateAccess=true))
	int32 BlackChessNum;
};
