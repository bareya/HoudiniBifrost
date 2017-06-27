
#include <GT/GT_GEOPrimCollectPacked.h>

class GBIF_CollectPacked : public GT_GEOPrimCollect
{
public:
	GBIF_CollectPacked();
	virtual ~GBIF_CollectPacked();

	virtual GT_GEOPrimCollectData* beginCollecting(
					const GT_GEODetailListHandle &geometry,
					const GT_RefineParms *parms) const;

	virtual GT_PrimitiveHandle collect(
					const GT_GEODetailListHandle &geo,
					const GEO_Primitive *const* prim_list,
					int nsegments,
					GT_GEOPrimCollectData *data) const;

	virtual GT_PrimitiveHandle endCollecting(
					const GT_GEODetailListHandle &geometry,
					GT_GEOPrimCollectData *data) const;

	static void install(const GA_PrimitiveTypeId &id);
};