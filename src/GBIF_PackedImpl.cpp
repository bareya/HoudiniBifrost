#include "GBIF_PackedImpl.h"
#include "HoudiniBifrost.h"

#include "GU/GU_PackedFactory.h"
#include "GU/GU_PrimPacked.h"
#include "GA/GA_Primitive.h"

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


//
// Bifrost Cache
//
BifrostCache::BifrostCache()
{
	myPointCloudPackedDetail.allocateAndSet(new GU_Detail());
}


bool BifrostCache::load(const UT_StringHolder& filename)
{
	ObjectModel om;
	FileIO fio = om.createFileIO(filename.c_str());
	StateServer ss = fio.load();

	if(ss.valid())
	{
		return false;
	}

	// components
	RefArray components = ss.components();

	// component iterator
	for (size_t index = 0; index < components.count(); ++index)
	{
		Component component = ss.components()[index];
		std::cout << component.name() << std::endl;
		convertBifrostPointCloud(component, *myPointCloudPackedDetail.writeLock(), true);
	}
	
	return true;
}


BifrostCache::~BifrostCache()
{
}


//
// Bifrost Factory
//
class BifrostFactory : public GU_PackedFactory
{
public:
	BifrostFactory()
		: GU_PackedFactory("PackedBifrost", "Packed Bifrost")
	{
		registerIntrinsic("biffilename",
			StringHolderGetterCast(&GBIF_PackedImpl::intrinsicFilename)
		);
		registerIntrinsic("bifcomponentname",
			StringHolderGetterCast(&GBIF_PackedImpl::intrinsicComponentname)
		);
		registerIntrinsic("bifelementcount",
			IntGetterCast(&GBIF_PackedImpl::intrinsicElementcount)
		);
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

	if (theFactory->isRegistered())
	{
		theTypeId = theFactory->typeDef().getId();
		// TODO register GT primitive ?
	}
	else
	{
		std::cout << "Unable to register Bifrost Packed primitive" << std::endl;
	}
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
	// create BifrostPacked and wire it to vertex
	GA_Primitive* prim = gdp.appendPrimitive(theFactory->typeDef().getId());
	GU_PrimPacked* packed = dynamic_cast<GU_PrimPacked*>(prim);
	packed->setVertexPoint(gdp.appendPointOffset());

	// set BifrostPacked data
	GBIF_PackedImpl* bif = dynamic_cast<GBIF_PackedImpl*>(packed->implementation());
	bif->myFilename = filename;
	bif->myCache.load(filename);

	return packed;
}


GU_PrimPacked* GBIF_PackedImpl::build(GU_Detail& gdp, const UT_StringHolder& filename, UT_StringHolder& componentname)
{
	// create BifrostPacked and wire it to vertex
	GA_Primitive* prim = gdp.appendPrimitive(theFactory->typeDef().getId());
	GU_PrimPacked* packed = dynamic_cast<GU_PrimPacked*>(prim);
	packed->setVertexPoint(gdp.appendPointOffset());

	// set BifrostPacked data
	GBIF_PackedImpl* bif = dynamic_cast<GBIF_PackedImpl*>(packed->implementation());
	bif->myFilename = filename;
	bif->myComponentname = componentname;

	//bif->myCache.load(filename);

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
	if (!import(options, "filename", myFilename))
	{
		myFilename = "";
		return false;
	}

	if (!import(options, "componentname", myComponentname))
	{
		myComponentname = "";
		return false;
	}

	return true;
}


bool GBIF_PackedImpl::supportsJSONLoad() const
{
	return false;
}


bool GBIF_PackedImpl::loadFromJSON(const UT_JSONValueMap& options, const GA_LoadMap& map)
{
	return false;
}


void GBIF_PackedImpl::update(const UT_Options& options)
{
}


bool GBIF_PackedImpl::save(UT_Options& options, const GA_SaveMap& map) const
{
	options.setOptionS("filename", myFilename);
	options.setOptionS("componentname", myComponentname);

	return true;
}


bool GBIF_PackedImpl::getBounds(UT_BoundingBox& box) const
{
	box.setBounds(-1, -1, -1, 1, 1, 1);
	return true;
}


bool GBIF_PackedImpl::getRenderingBounds(UT_BoundingBox& box) const
{
	box.setBounds(-1, -1, -1, 1, 1, 1);
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
	gdp.merge(*myCache.getPointCloudPackedDetail().readLock());
	return true;
}


GU_ConstDetailHandle GBIF_PackedImpl::getPackedDetail(GU_PackedContext *context) const
{
	return myCache.getPointCloudPackedDetail();
}
