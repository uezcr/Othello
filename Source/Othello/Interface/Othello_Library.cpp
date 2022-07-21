// Fill out your copyright notice in the Description page of Project Settings.


#include "Othello_Library.h"

FCoordinate UOthello_Library::OffsetPlus(const FCoordinate A, const FCoordinate B)
{
	int32 Row = A.Row+B.Row;
	int32 Column= A.Column+B.Column;
	FCoordinate Value{Row,Column};
	return Value;
}

FCoordinate UOthello_Library::OffsetClamp(const FCoordinate InCoordinate, int32 Min, int32 Max)
{
	int32 Row = FMath::Clamp(InCoordinate.Row,Min,Max);
	int32 Column = FMath::Clamp(InCoordinate.Column,Min,Max);
	FCoordinate Value{Row,Column};
	return Value;
}
