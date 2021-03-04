// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Windows/WindowsHWrapper.h"

#if PLATFORM_WINDOWS
#include "QEventLoop"
#include "QApplication"
#include "QAbstractEventDispatcher"
#include "QWidget"

class UEWinApp : public QApplication
{
public:
	UEWinApp(int argc = 0, char **argv = nullptr);
	~UEWinApp();
public:
	 static bool pluginInstance();

	  static void pluginExit();
};
#endif
