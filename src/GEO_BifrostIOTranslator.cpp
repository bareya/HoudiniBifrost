#include "GEO_BifrostIOTranslator.h"

#include <GU/GU_Detail.h>
#include <UT/UT_IOTable.h>

#include <GBIF/HoudiniBifrost.h>
#include <GBIF/GBIF_PackedImpl.h>


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
	GU_PrimPacked* packed = GBIF_PackedImpl::build(*dynamic_cast<GU_Detail*>(gdp), is.getFilename());

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
