// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

public class Qt5 : ModuleRules
{
    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/")); }
    }
    private string ProjectBinariesPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../../Binaries/Win64/")); }
    }
    public Qt5(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);	
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
                "Projects",
				// ... add private dependencies that you statically link with here ...	
			}
			);

        WindowPlatformTargetRules(Target);
    }

	void WindowPlatformTargetRules(ReadOnlyTargetRules Target)
    {
        Console.WriteLine("Target.Platform is {0}", Target.Platform);
        bool IsWindows = false;
        if (Target.Platform == UnrealTargetPlatform.Win32 || Target.Platform == UnrealTargetPlatform.Win64)
        {
            IsWindows = true;
            Console.WriteLine("Target.Platform is Windows {0}",IsWindows);
        }
        if (!IsWindows) return;
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include","QtCore"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include","QtGui"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include","QtWidgets"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include","QtNetwork"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include","QtSql"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include","QtTextToSpeech"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include","QtXml"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include"));

        Console.WriteLine("ThirdPartyPath is {0}", ThirdPartyPath);
        PublicSystemLibraryPaths.Add(Path.Combine(ThirdPartyPath, "lib"));

        DirectoryInfo dir = new DirectoryInfo(Path.Combine(ThirdPartyPath, "lib"));
        foreach (FileInfo dChild in dir.GetFiles("*.lib"))
        {
            //Console.WriteLine(dChild.Name);//打印文件名
            //PublicDelayLoadDLLs.Add(dChild.Name);
            //Console.WriteLine(dChild.FullName);//打印路径和文件名
            PublicSystemLibraries.Add(dChild.Name);
        }
        if (Target.Type == TargetType.Editor)
        {
            Console.WriteLine("Editor CopyBin File To ProjectBinariesPath = {0}", ProjectBinariesPath);
            CopyDirectory(Path.Combine(ThirdPartyPath, "bin"),ProjectBinariesPath);
            CopyDirectory(Path.Combine(ThirdPartyPath, "plugins"), ProjectBinariesPath);
        }
        if(Target.Type == TargetType.Game)
        {
            dir = new DirectoryInfo(Path.Combine(ThirdPartyPath, "bin"));
            foreach (FileInfo dChild in dir.GetFiles("*.dll"))
            {
                //Console.WriteLine("$(TargetOutputDir)/" + dChild.Name);//打印文件名
                //PublicDelayLoadDLLs.Add(dChild.Name);
                //Console.WriteLine(dChild.FullName);//打印路径和文件名
                RuntimeDependencies.Add("$(TargetOutputDir)/"+ dChild.Name, "$(PluginDir)/ThirdParty/bin/" + dChild.Name);
            }
            string[] dirs = Directory.GetDirectories(Path.Combine(ThirdPartyPath, "plugins"));
            foreach (string dChildDir in dirs)
            {
                dir = new DirectoryInfo(dChildDir);
                Console.WriteLine(dir.Name);//打印文件名
                foreach (FileInfo dChild in dir.GetFiles("*.dll"))
                {
                    //Console.WriteLine("$(TargetOutputDir)/" + dir.Name + "/" + dChild.Name);//打印文件名
                    //RuntimeDependencies.Add("$(PluginDir)/ThirdParty/plugins/" + dir.Name + "/" + dChild.Name);
                    RuntimeDependencies.Add("$(TargetOutputDir)/" + dir.Name + "/" + dChild.Name, "$(PluginDir)/ThirdParty/plugins/" + dir.Name + "/" + dChild.Name);
                }
            }
        }
    }

    public void CopyDirectory(string sourceDirPath, string saveDirPath)
    {
        if (!Directory.Exists(saveDirPath))
        {
            Directory.CreateDirectory(saveDirPath);
        }
        string[] files = Directory.GetFiles(sourceDirPath);
        foreach (string file in files)
        {
            string pFilePath = saveDirPath + "\\" + Path.GetFileName(file);
            if (File.Exists(pFilePath))
                continue;
            File.Copy(file, pFilePath, true);
            //Console.WriteLine("Copy File SrcPath is {0},Target Path is{1}",file, pFilePath);
        }

        string[] dirs = Directory.GetDirectories(sourceDirPath);
        foreach (string dir in dirs)
        {
            CopyDirectory(dir, saveDirPath + "\\" + Path.GetFileName(dir));
        }
    }         
}
