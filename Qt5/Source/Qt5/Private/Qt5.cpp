// Copyright Epic Games, Inc. All Rights Reserved.

#include "Qt5.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "Windows/UEWinApp.h"

#define LOCTEXT_NAMESPACE "FQt5Module"

DEFINE_LOG_CATEGORY(Qt5Log);

void FQt5Module::StartupModule()
{
#if PLATFORM_WINDOWS
	FString BaseDir = IPluginManager::Get().FindPlugin("Qt5")->GetBaseDir();
	FString BinDir= FPaths::Combine(*BaseDir, TEXT("ThirdParty/bin"));
	FString PluginsDir = FPaths::Combine(*BaseDir, TEXT("ThirdParty/plugins"));
	FPlatformProcess::AddDllDirectory(*BinDir);
	FPlatformProcess::AddDllDirectory(*PluginsDir);

	UEWinApp::pluginInstance();
	UE_LOG(Qt5Log,Display,TEXT("Qt5Module StartupModule!!!"));
#endif
}

void FQt5Module::ShutdownModule()
{
#if PLATFORM_WINDOWS
	UEWinApp::pluginExit();
	UE_LOG(Qt5Log,Display,TEXT("Qt5Module ShutdownModule!!!"));
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQt5Module, Qt5)