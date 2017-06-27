#include "GBIF_PackedGT.h"


GBIF_CollectPacked::GBIF_CollectPacked()
{
}


GBIF_CollectPacked::~GBIF_CollectPacked()
{
}


GT_GEOPrimCollectData* GBIF_CollectPacked::beginCollecting(
	const GT_GEODetailListHandle &geometry,
	const GT_RefineParms *parms) const
{
	return new GT_GEOPrimCollectOffsets();
}


GT_PrimitiveHandle GBIF_CollectPacked::collect(
	const GT_GEODetailListHandle &geo,
	const GEO_Primitive *const* prim_list,
	int nsegments,
	GT_GEOPrimCollectData *data) const
{
	GT_GEOPrimCollectOffsets *list = data->asPointer<GT_GEOPrimCollectOffsets>();
	list->append(prim_list[0]);

	return GT_PrimitiveHandle();
}


GT_PrimitiveHandle GBIF_CollectPacked::endCollecting(
	const GT_GEODetailListHandle &geometry,
	GT_GEOPrimCollectData *data) const
{
	return GT_PrimitiveHandle();
}


void GBIF_CollectPacked::install(const GA_PrimitiveTypeId &id)
{
	GBIF_CollectPacked* gt = new GBIF_CollectPacked();
	gt->bind(id);
}