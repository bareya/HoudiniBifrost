#ifndef GBIF_PACKED_IMPL_H
#define GBIF_PACKED_IMPL_H

#include "GU/GU_PackedImpl.h"
#include "GA/GA_PrimitiveTypeId.h"

class GA_PrimitiveFactory;

class GBIF_PackedImpl : public GU_PackedImpl
{
public:
	static void install(GA_PrimitiveFactory* factory);
	static bool isInstalled();
	static GA_PrimitiveTypeId typeId();

	GBIF_PackedImpl();
	GBIF_PackedImpl(const GBIF_PackedImpl& src);
	virtual ~GBIF_PackedImpl();

	static GU_PrimPacked* build(GU_Detail& gdp, const UT_StringHolder& filename);

	// factory, copy
	virtual GU_PackedFactory* getFactory() const;
	virtual GU_PackedImpl* copy() const;

	// memroy
	virtual int64 getMemoryUsage(bool inclusive) const;
	virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

	virtual bool isValid() const;
	virtual void clearData();

	// json
	virtual bool load(const UT_Options &options, const GA_LoadMap &map);
	virtual bool supportsJSONLoad() const;
	virtual bool loadFromJSON(const UT_JSONValueMap &options, const GA_LoadMap &map);

	virtual void update(const UT_Options &options);
	virtual bool save(UT_Options &options, const GA_SaveMap &map) const;

	// bounds
	virtual bool getBounds(UT_BoundingBox &box) const;
	virtual bool getRenderingBounds(UT_BoundingBox &box) const;
	virtual void getVelocityRange(UT_Vector3 &min, UT_Vector3 &max) const;
	virtual void getWidthRange(fpreal &min, fpreal &max) const;

	// unpack
	virtual bool unpack(GU_Detail &destgdp) const;

private:
	UT_StringHolder myFilename;

	static GA_PrimitiveTypeId theTypeId;
};

#endif//GBIF_PACKED_IMPL_H
