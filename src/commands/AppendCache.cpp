#include <src/Skyrim/TES5File.h>
#include <src/Collection.h>
#include <src/ModFile.h>

#include <commands/AppendCache.h>

#include "stdafx.h"
#undef max
#undef min



#include <core/hkxcmd.h>
#include <core/hkxutils.h>
#include <core/AnimationCache.h>
#include <core/HKXWrangler.h>

#include <core/MathHelper.h>

#include <hkbBehaviorReferenceGenerator_0.h>
#include <hkbClipGenerator_2.h>
#include <hkbProjectData_2.h>
#include <hkbCharacterData_7.h>
#include <hkbBehaviorGraph_1.h>
#include <hkbStateMachineStateInfo_4.h>
#include <hkbStateMachine_4.h>
#include <hkbExpressionDataArray_0.h>
#include <hkbExpressionDataArray_0.h>
#include <Animation/Animation/hkaAnimationContainer.h>
#include <Animation/Animation/Animation/SplineCompressed/hkaSplineCompressedAnimation.h>
#include <BSSynchronizedClipGenerator_1.h>

#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>


using namespace std;

AppendCacheCMD::AppendCacheCMD()
{
}

AppendCacheCMD::~AppendCacheCMD()
{
}

string AppendCacheCMD::GetName() const
{
	return "AppendCache";
}

string AppendCacheCMD::GetHelp() const
{
	string name = GetName();
	transform(name.begin(), name.end(), name.begin(), ::tolower);

	// Usage: ck-cmd test <infile> [-o <outfile>] [-d <level>] [-f <flags> ...]
	string usage = "Usage: " + ExeCommandList::GetExeName() + " " + name + " <animation_folder> <new_animDataPath> <new_animSetDataPath>\r\n";

	const char help[] =
		R"(Test

Arguments:
    <infile>    Input File or directory

Options:
    -o <outfile>    Output File - Defaults to input file with '-out' appended
    -d <level>      Debug Level: ERROR, WARN, INFO, DEBUG, VERBOSE 
                    [default: INFO]
    -f <flags>      Havok saving flags: SAVE_DEFAULT, SAVE_TEXT_FORMAT, SAVE_SERIALIZE_IGNORED_MEMBERS, SAVE_WRITE_ATTRIBUTES, SAVE_CONCISE, SAVE_TEXT_NUMBERS 
                    [default: SAVE_TEXT_FORMAT SAVE_TEXT_NUMBERS]

Havok saving flags:
    SAVE_DEFAULT                    All flags default to OFF, enable whichever are needed
    SAVE_TEXT_FORMAT                Use text (usually XML) format, default is binary format if available
    SAVE_SERIALIZE_IGNORED_MEMBERS  Write members which are usually ignored
    SAVE_WRITE_ATTRIBUTES           Include extended attributes in metadata, default is to write minimum metadata
    SAVE_CONCISE                    Doesn't provide any extra information which would make the file easier to interpret. E.g. additionally write hex floats as text comments
    SAVE_TEXT_NUMBERS               Floating point numbers output as text, not as binary. Makes them easily readable/editable, but values may not be exact)";

	return usage + help;
}

string AppendCacheCMD::GetHelpShort() const
{
	return "AppendCache";
}

bool AppendCacheCMD::InternalRunCommand(map<string, docopt::value> parsedArgs)
{
	std:string animation_folder = "", new_animDataPath = "", new_animSetDataPath = "";

	if (parsedArgs["<animation_folder>"].isString())
		animation_folder = parsedArgs["<animation_folder>"].asString();
	if (parsedArgs["<new_animDataPath>"].isString())
		new_animDataPath = parsedArgs["<new_animDataPath>"].asString();
	if (parsedArgs["<new_animSetDataPath>"].isString())
		new_animSetDataPath = parsedArgs["<new_animSetDataPath>"].asString();
	

	fs::path source_havok_project_cache = fs::path(animation_folder);

	fs::path animDataPath = fs::path(source_havok_project_cache) / "animationdatasinglefile.txt";
	fs::path animDataSetPath = fs::path(source_havok_project_cache) / "animationsetdatasinglefile.txt";
	Log::Info("Loading cache, loading %s and %s", animDataPath.string().c_str(), animDataSetPath.string().c_str());
	AnimationCache cache(animDataPath, animDataSetPath);

	CreatureStaticCacheEntry staticEntry;
	AnimationCache::get_entries(staticEntry,new_animDataPath,new_animSetDataPath);
	CreatureCacheEntry entry(staticEntry.name, staticEntry.block, staticEntry.movements,staticEntry.sets);
	Log::Info("Get new project name %s", entry.name.c_str());

	auto oldEntry = cache.find(entry.name);
	if (oldEntry)
	{
		auto creatureEntry = dynamic_cast<CreatureCacheEntry*>(oldEntry);
		if (creatureEntry) {
			creatureEntry->block = entry.block;
			creatureEntry->movements = entry.movements;
			creatureEntry->sets = entry.sets;
		}
	}
	else
	{
		cache.animationData.putProject(entry.name + ".txt",entry.block, entry.movements);
		cache.animationSetData.putProjectAttackBlock(entry.name + "Data\\" + entry.name + ".txt", entry.sets);
		cache.creature_entries.push_back(entry);
		cache.rebuildIndex();
	}

	cache.save_creature(entry.name, &entry, animDataPath, animDataSetPath);

	return true;
}


