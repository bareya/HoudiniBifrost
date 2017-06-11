#include "GEO_BifrostIOTranslator.h"

#include "GU/GU_Detail.h"
#include "UT/UT_DSOVersion.h"
#include "UT/UT_IOTable.h"

void newGeometryIO(void*)
{
	GU_Detail::registerIOTranslator(new GEO_BifrostIOTranslator());

	UT_ExtensionList *geoextension = UTgetGeoExtensions();
	if ( !geoextension->findExtension( "bif" ) )
	{
		geoextension->addExtension( "bif" );
	}
}
