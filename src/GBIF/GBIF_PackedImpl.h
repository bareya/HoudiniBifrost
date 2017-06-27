/*
MIT License

Copyright (c) 2017 Piotr Barejko

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef GBIF_PACKED_IMPL_H
#define GBIF_PACKED_IMPL_H

#include <memory>

#include <GU/GU_PackedImpl.h>
#include <GA/GA_PrimitiveTypeId.h>
#include <UT/UT_PtrArray.h>

class GA_PrimitiveFactory;

namespace Bifrost { namespace API { class ObjectModel; } }

class BifrostCache
{
public:
	BifrostCache();
	~BifrostCache();

	bool load(const UT_StringHolder& filename);

	GU_ConstDetailHandle getPointCloudPackedDetail() const
	{
		return myPointCloudPackedDetail;
	}

private:
	//std::unique_ptr<Bifrost::API::ObjectModel> myObjectModel;

	GU_DetailHandle myPackedDetail;

	// only points with position for display
	GU_DetailHandle myPointCloudPackedDetail;
};


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
	static GU_PrimPacked* build(GU_Detail& gdp, const UT_StringHolder& filename, UT_StringHolder& componentname);

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

	// packed
	virtual bool unpack(GU_Detail &destgdp) const;
	virtual GU_ConstDetailHandle getPackedDetail(GU_PackedContext *context = 0) const;

	// intrinsics
	UT_StringHolder intrinsicFilename() const
	{
		return myFilename;
	}
	UT_StringHolder intrinsicComponentname() const
	{
		return myComponentname;
	}
	int64 intrinsicElementcount() const
	{
		return myElementcount;
	}

private:
	UT_StringHolder myFilename;
	UT_StringHolder myComponentname;
	int64 myElementcount;

	BifrostCache myCache;

	static GA_PrimitiveTypeId theTypeId;
};

#endif//GBIF_PACKED_IMPL_H
