#include "GEO_BifrostIOTranslator.h"
#include "HoudiniBifrost.h"

#include <GU/GU_Detail.h>
#include <UT/UT_IOTable.h>

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


GEO_BifrostIOTranslator::GEO_BifrostIOTranslator()
	: GEO_IOTranslator()
{
}


GEO_BifrostIOTranslator::GEO_BifrostIOTranslator(const GEO_BifrostIOTranslator &other)
{
}


GEO_BifrostIOTranslator::~GEO_BifrostIOTranslator()
{
}


GEO_IOTranslator* GEO_BifrostIOTranslator::duplicate() const
{
	return new GEO_BifrostIOTranslator(*this);
}


const char*	GEO_BifrostIOTranslator::formatName() const
{
	return "Bifrost file format";
}


int GEO_BifrostIOTranslator::checkExtension(const char *name)
{
	UT_String strName(name);
	return strName.matchFileExtension(".bif");
}


int GEO_BifrostIOTranslator::checkMagicNumber(unsigned magic)
{
	return 0;
}


GA_Detail::IOStatus GEO_BifrostIOTranslator::fileLoad(GEO_Detail *gdp, UT_IStream &is, bool ate_magic)
{
	ObjectModel om;
	FileIO fio = om.createFileIO(is.getFilename());
	StateServer ss = fio.load();

	// failed to read the file
	if(!ss.valid())
	{
		std::cout << "Error reading bif file" << std::endl;
		return 0;
	}

	// bif has only one component
	Component component = ss.components()[0];

	// check type
	if(component.type()!=PointComponentType)
	{
		std::cout << "Currently only point components can be read" << std::endl;
		return 0;
	}

	convertBifrostPointCloud(component, *dynamic_cast<GU_Detail*>(gdp));

	return 1;
}


GA_Detail::IOStatus GEO_BifrostIOTranslator::fileSave(const GEO_Detail*, std::ostream&)
{
	std::cout << "Save option is not supported" << std::endl;
	return 0;
}


void GEO_BifrostIOTranslator::install()
{
	GU_Detail::registerIOTranslator(new GEO_BifrostIOTranslator());

	UT_ExtensionList *geoextension = UTgetGeoExtensions();
	if ( !geoextension->findExtension( "bif" ) )
	{
		geoextension->addExtension( "bif" );
	}
}
