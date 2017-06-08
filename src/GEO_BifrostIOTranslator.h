#include "GEO/GEO_IOTranslator.h"

class GEO_BifrostIOTranslator : public GEO_IOTranslator
{
public:
	GEO_BifrostIOTranslator(){}
	virtual ~GEO_BifrostIOTranslator(){}

	virtual GA_Detail::IOStatus fileLoad (GEO_Detail *gdp, UT_IStream &is, bool ate_magic){}
};
