#include <iostream>
#include <string>
#include <vector>

#include "HoudiniBifrost.h"

#include <GU/GU_Detail.h>

#include <bifrostapi/bifrost_om.h>
#include <bifrostapi/bifrost_stateserver.h>
#include <bifrostapi/bifrost_channel.h>
#include <bifrostapi/bifrost_component.h>
#include <bifrostapi/bifrost_layout.h>
#include <bifrostapi/bifrost_fileio.h>
#include <bifrostapi/bifrost_status.h>
#include <bifrostapi/bifrost_array.h>
#include <bifrostapi/bifrost_string.h>
#include <bifrostapi/bifrost_ref.h>

using namespace Bifrost::API;

using std::string;
using std::vector;
using Args = vector<string>;

Args parseArguments(int argc, char* argv[])
{
	Args res;
	for(int i=1;i<argc; ++i)
	{
		res.emplace_back(argv[i]);
	}

	return res;
}

int main(int argc, char* argv[])
{
	Args args = parseArguments(argc, argv);

	if(args.size() != 2)
	{
		std::cout << "invalid number of arguments: geo.bif geo.bgeo" << std::endl;
		return 0;
	}

	GU_Detail geo;

	ObjectModel om;
	FileIO fio = om.createFileIO(args[0].c_str());
	StateServer ss = fio.load();

	if(!ss.valid())
	{
		std::cout << "Error reading bif file." << std::endl;
		return 0;
	}

	// bif has only one component
	Component component = ss.components()[0];

	// convert
	convertBifrostPointCloud(component,geo);

	// save geo
	if(!geo.save(args[1].c_str(), nullptr).success())
	{
		std::cout << "Failed to save the file." << std::endl;
	}

	return 0;
}
