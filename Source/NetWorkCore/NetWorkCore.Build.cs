using System.IO;
using UnrealBuildTool;

public class NetWorkCore : ModuleRules
{
	public NetWorkCore(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		System.Console.WriteLine("AAA !!!!");
		System.Console.WriteLine(Path.Combine(ModuleDirectory, "lib", "win64", ConfigurationDir(Target.Configuration), "libprotobuf.lib"));
		System.Console.WriteLine("BBBB !!!!");

		if (ConfigurationDir(Target.Configuration) == "Debug")
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "win64", ConfigurationDir(Target.Configuration), "libprotobuf.lib"));
		else
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "win64", ConfigurationDir(Target.Configuration), "libprotobuf.lib"));

		PublicSystemIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));
	}

	public string ConfigurationDir(UnrealTargetConfiguration Configuration)
	{
		if (Configuration == UnrealTargetConfiguration.Debug || Configuration == UnrealTargetConfiguration.DebugGame)
		{
			return "Debug";
		}
		else
		{
			return "Release";
		}
	}

}
	