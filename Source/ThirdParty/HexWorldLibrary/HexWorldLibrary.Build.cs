using System;
using System.Collections.Generic;
using System.IO;

using UnrealBuildTool;

public class HexWorldLibrary : ModuleRules
{
	public IEnumerable<string> FindFiles(string Dir, string Ext)
	{
		return Directory.GetFiles(Dir, "*." + Ext);
	}

	public HexWorldLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleRules.ModuleType.External;
		

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			var libraries = FindFiles(Path.GetFullPath(Path.Combine(ModuleDirectory, "Win64/Release")), "lib");
			Console.WriteLine(string.Join("Adding library: ", libraries));
			PublicAdditionalLibraries.AddRange(libraries);	
		}
		
		if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			var libraries = FindFiles(Path.GetFullPath(Path.Combine(ModuleDirectory, "Mac")), "a");
			Console.WriteLine(string.Join("Adding library: ", libraries));
			PublicAdditionalLibraries.AddRange(libraries);	
		}
		
		AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenSSL");
		AddEngineThirdPartyPrivateStaticDependencies(Target, "zlib");
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });
		
		PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine" });
		
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));	
		
	}
}