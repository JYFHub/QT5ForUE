// Copyright Epic Games, Inc. All Rights Reserved.

#include "Qt5.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "Windows/UEWinApp.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/App.h"
#include "FileHelpers.h"
#include "UObject/Package.h"
#include "Misc/PackageName.h"


#if WITH_EDITOR

#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Editor/MainFrame/Public/Interfaces/IMainFrameModule.h"

#endif

#define LOCTEXT_NAMESPACE "FQt5Module"

DEFINE_LOG_CATEGORY(Qt5Log);

void FQt5Module::StartupModule()
{
#if PLATFORM_WINDOWS

	//���DLL����·��
	FString BaseDir = IPluginManager::Get().FindPlugin("Qt5")->GetBaseDir();
	FString BinDir= FPaths::Combine(*BaseDir, TEXT("ThirdParty/bin"));
	FString PluginsDir = FPaths::Combine(*BaseDir, TEXT("ThirdParty/plugins"));
	FPlatformProcess::AddDllDirectory(*BinDir);
	FPlatformProcess::AddDllDirectory(*PluginsDir);

#if WITH_EDITOR	

	//�����༭�����·�������氲װ·��
	FString EnginePath = FPaths::RootDir();//��ȡ����İ�װ·��
	FString EditorPath = FPaths::Combine(EnginePath,TEXT("Engine/Binaries/Win64"));
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	class FFileMatch : public IPlatformFile::FDirectoryVisitor
	{
	public:
		TArray<FString>& Result;
		FString WildCard;
		FFileMatch(TArray<FString>& InResult, const FString& InWildCard)
			: Result(InResult)
			, WildCard(InWildCard)
		{
		}
		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
		{
			if (!bIsDirectory)
			{
				new(Result) FString(FilenameOrDirectory);
			}
			return true;
		}
	};

	TArray<FString> PFFindedFiles;
	FFileMatch FileMatch( PFFindedFiles,TEXT("*"));
	FileManager.IterateDirectoryRecursively(*PluginsDir, FileMatch);

	FString FilePath;
	for (auto ItemFile : PFFindedFiles)
	{
		FilePath = FPaths::GetPath(ItemFile);
		if (!FileManager.DirectoryExists(*FilePath))
		{
			FileManager.CreateDirectory(*FilePath);//���Ŀ¼�������򴴽�Ŀ¼
		}
		
		//����ƽ̨�ļ�������Ŀ¼
		{
			FString TargetPath = EditorPath;
			FString TargetSubPath = ItemFile.Replace(*PluginsDir,TEXT(""));
			TargetPath.Append(TargetSubPath);

			//����������ԭ�е�ƽ̨�����ͣ���֤ƽ̨����ȷ
			//if (!FPaths::FileExists(TargetPath))
			{
				IFileManager::Get().Copy(*TargetPath, *ItemFile);
				UE_LOG(Qt5Log,Display,TEXT("Qt5Module Copy Plugins SrcDirectory is %s,TargetDirectory is %s!!!"),*ItemFile,*TargetPath);
			}
		}
	}

#endif

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