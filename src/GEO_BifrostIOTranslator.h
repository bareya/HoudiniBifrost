#include "GEO/GEO_IOTranslator.h"

class GEO_BifrostIOTranslator : public GEO_IOTranslator
{
public:
	GEO_BifrostIOTranslator();
	GEO_BifrostIOTranslator(const GEO_BifrostIOTranslator& other);
	virtual ~GEO_BifrostIOTranslator();

	virtual GEO_IOTranslator* duplicate() const;
	virtual const char*	formatName() const;
	virtual int	checkExtension(const char *name);

	virtual int	checkMagicNumber(unsigned magic);

	virtual GA_Detail::IOStatus fileLoad(GEO_Detail *gdp, UT_IStream &is, bool ate_magic);
	virtual GA_Detail::IOStatus fileSave(const GEO_Detail *gdp, std::ostream &os);
};

