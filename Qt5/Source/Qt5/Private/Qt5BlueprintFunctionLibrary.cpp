// Fill out your copyright notice in the Description page of Project Settings.


#include "Qt5BlueprintFunctionLibrary.h"

FString UQt5BlueprintFunctionLibrary::QStringToFString(QString String)
{
	return FString(reinterpret_cast<wchar_t*>(String.data()));
}

QString UQt5BlueprintFunctionLibrary::FStringToQString(FString String)
{
	return QString::fromStdWString(std::wstring(*String));
}
