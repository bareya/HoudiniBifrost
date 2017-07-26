#include <UT/UT_DSOVersion.h>
#include <GU/GU_Detail.h>

#include "GEO_BifrostIOTranslator.h"

void newGeometryIO(void*)
{
	GEO_BifrostIOTranslator::install();
}
