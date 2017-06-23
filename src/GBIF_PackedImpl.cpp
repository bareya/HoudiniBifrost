#include "GBIF_PackedImpl.h"

#include "GU/GU_PackedFactory.h"
#include "GU/GU_PrimPacked.h"


//
// Factory
//
class BifrostFactory : public GU_PackedFactory
{
public:
	BifrostFactory()
		: GU_PackedFactory("PackedBifrost", "Packed Bifrost")
	{
	}

	virtual ~BifrostFactory()
	{
	}

	virtual GU_PackedImpl* create() const
	{
		return new GBIF_PackedImpl();
	}
};


static BifrostFactory* theFactory = nullptr;


//
// GBIF_PackedImpl
//
GA_PrimitiveTypeId GBIF_PackedImpl::theTypeId(-1);


void GBIF_PackedImpl::install(GA_PrimitiveFactory* factory)
{
	if(theFactory)
	{
		return;
	}

	theFactory = new BifrostFactory();
	GU_PrimPacked::registerPacked(factory, theFactory);
	theTypeId = theFactory->typeDef().getId();

	// TODO register GT primitive
}


bool GBIF_PackedImpl::isInstalled()
{
	return theFactory != nullptr;
}


GA_PrimitiveTypeId GBIF_PackedImpl::typeId()
{
	return theTypeId;
}


GBIF_PackedImpl::GBIF_PackedImpl()
	: GU_PackedImpl()
{
}


GBIF_PackedImpl::GBIF_PackedImpl(const GBIF_PackedImpl& src)
	: GU_PackedImpl(src)
{
}


GBIF_PackedImpl::~GBIF_PackedImpl()
{
}


GU_PrimPacked* GBIF_PackedImpl::build(GU_Detail& gdp, const UT_StringHolder& filename)
{
	GA_Primitive* prim = gdp.appendPrimitive(theFactory->typeDef().getId());
	GU_PrimPacked* packed = dynamic_cast<GU_PrimPacked*>(prim);
	GBIF_PackedImpl* bif = dynamic_cast<GBIF_PackedImpl*>(packed->implementation());
	bif->myFilename = filename;
	return packed;
}


GU_PackedFactory* GBIF_PackedImpl::getFactory() const
{
	return theFactory;
}


GU_PackedImpl* GBIF_PackedImpl::copy() const
{
	return new GBIF_PackedImpl(*this);
}


int64 GBIF_PackedImpl::getMemoryUsage(bool inclusive) const
{
	return 0;
}


void GBIF_PackedImpl::countMemory(UT_MemoryCounter &counter, bool inclusive) const
{
}


bool GBIF_PackedImpl::isValid() const
{
	return true;
}


void GBIF_PackedImpl::clearData()
{
}


bool GBIF_PackedImpl::load(const UT_Options &options, const GA_LoadMap &map)
{
	return true;
}


bool GBIF_PackedImpl::supportsJSONLoad() const
{
	return true;
}


bool GBIF_PackedImpl::loadFromJSON(const UT_JSONValueMap& options, const GA_LoadMap& map)
{
	return true;
}


void GBIF_PackedImpl::update(const UT_Options& options)
{
}


bool GBIF_PackedImpl::save(UT_Options& options, const GA_SaveMap& map) const
{
}


bool GBIF_PackedImpl::getBounds(UT_BoundingBox& box) const
{
}


bool GBIF_PackedImpl::getRenderingBounds(UT_BoundingBox& box) const
{
	return true;
}


void GBIF_PackedImpl::getVelocityRange(UT_Vector3& min, UT_Vector3& max) const
{
}


void GBIF_PackedImpl::getWidthRange(fpreal& min, fpreal& max) const
{
}


bool GBIF_PackedImpl::unpack(GU_Detail& gdp) const
{
	return true;
}
