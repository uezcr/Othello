// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OthelloUI_Statistics.generated.h"

class AOthelloActor_Board;
/**
 * 
 */
UCLASS()
class OTHELLO_API UOthelloUI_Statistics : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Variables",meta=(AllowPrivateAccess=true))
	int32 WhiteChessNum = 2;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Variables",meta=(AllowPrivateAccess=true))
	int32 BlackChessNum = 2;
	AOthelloActor_Board* Othello;
private:
	UFUNCTION()
	void OnChessSpawnCallBack(const int32& Black,const int32& White);
};
