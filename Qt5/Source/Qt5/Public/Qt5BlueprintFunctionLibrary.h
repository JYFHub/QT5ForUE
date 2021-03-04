// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QtCore/QString"
#include "Qt5BlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class QT5_API UQt5BlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static FString QStringToFString(QString String);

	static QString FStringToQString(FString String);
};
