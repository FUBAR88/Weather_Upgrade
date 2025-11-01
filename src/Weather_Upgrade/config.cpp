class CfgPatches
{
	class Weather_Upgrade
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = 
		{
			"DZ_Data",
			"DZ_scripts",
			"JM_CF_Scripts"
		};
	};
};
class CfgMods
{
	class Weather_Upgrade
	{
		dir="Weather_Upgrade";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="Weather_Upgrade";
		credits="";
		author="";
		authorID="0";
		version="3.0";
		extra=0;
		type="mod";
		dependencies[]=
		{
			"Game",
			"World",
			"Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"Weather_Upgrade/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"Weather_Upgrade/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"Weather_Upgrade/scripts/5_Mission"
				};
			};
		};
	};
};
class CfgVehicles
{

};
