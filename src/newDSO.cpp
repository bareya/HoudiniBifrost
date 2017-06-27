#include <UT/UT_DSOVersion.h>
#include <GU/GU_Detail.h>

#include <GBIF/GBIF_PackedImpl.h>

#include "GEO_BifrostIOTranslator.h"


void newGeometryIO(void*)
{
	GEO_BifrostIOTranslator::install();
}


void newGeometryPrim(GA_PrimitiveFactory* primFactory)
{
	GBIF_PackedImpl::install(primFactory);
}
