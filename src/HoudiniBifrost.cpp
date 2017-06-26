#include "HoudiniBifrost.h"

#include "GU/GU_Detail.h"

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


struct ChannelAttributeAccessor
{
	Channel m_channel;
	GA_Attribute* m_attribute;
	const GA_AIFTuple* m_tuple;
};


static ChannelAttributeAccessor createAttribute(GEO_Detail& geo, Channel& channel)
{
	DataType type = channel.dataType();

	if(channel.name()=="position" && type==FloatV3Type)
	{
		GA_Attribute* a = geo.getP();
		return {channel, a, a->getAIFTuple()};
	}

	if(channel.name()=="velocity" && type==FloatV3Type)
	{
		GA_Attribute* a = geo.addFloatTuple(GA_ATTRIB_POINT,"v",3);
		return {channel, a, a->getAIFTuple()};
	}

	if(channel.name()=="id64" && type==UInt64Type)
	{
		GA_Attribute* a =  geo.addIntTuple(GA_ATTRIB_POINT, "id", 1);
		return {channel, a, a->getAIFTuple()};
	}

	switch(type)
	{
		case UInt64Type:
		case Int64Type:
		{
			GA_Attribute* a = geo.addIntTuple(GA_ATTRIB_POINT, channel.name().c_str(), 1);
			return {channel, a, a->getAIFTuple()};
		}
		case FloatType:
		{
			GA_Attribute* a = geo.addFloatTuple(GA_ATTRIB_POINT, channel.name().c_str(), 1);
			return {channel, a, a->getAIFTuple()};
		}
		case FloatV2Type:
		{
			GA_Attribute* a = geo.addFloatTuple(GA_ATTRIB_POINT, channel.name().c_str(), 2);
			return {channel, a, a->getAIFTuple()};
		}
		case FloatV3Type:
		{
			GA_Attribute* a = geo.addFloatTuple(GA_ATTRIB_POINT, channel.name().c_str(), 3);
			return {channel, a, a->getAIFTuple()};
		}
		default:
		{
			return {channel, nullptr, nullptr};
		}
	}
}


bool convertBifrostPointCloud(const Component& component, GU_Detail& geo, bool positionOnly)
{
	if(component.type()!=PointComponentType)
	{
		return 0;
	}

	// component info
	Layout layout = component.layout();
	RefArray channels = component.channels();

	// create attributes
	std::vector<ChannelAttributeAccessor> channelAttributeMap;
	if (positionOnly)
	{
		size_t index = channels.findFirstByName("position");
		Channel channel(channels[index]);
		channelAttributeMap.emplace_back(createAttribute(geo, channel));
	}
	else
	{
		for (int index = 0; index<channels.count(); ++index)
		{
			Channel channel(channels[index]);
			channelAttributeMap.emplace_back(createAttribute(geo, channel));
		}
	}
	

	// create points
	GA_Offset blockBegin = geo.appendPointBlock(component.elementCount());
	const GA_IndexMap& pointMap = geo.getPointMap();

	// tile iterator
	for(size_t d=0; d<layout.depthCount(); ++d)
	{
		for(size_t t=0; t<layout.tileCount(d); ++t)
		{
			TreeIndex tindex(t,d);

			size_t size{0};
			for(auto it=channelAttributeMap.begin(); it!=channelAttributeMap.end(); ++it)
			{
				Channel& channel = (*it).m_channel;
				DataType type = channel.dataType();

				GA_Attribute* attrib = (*it).m_attribute;
				const GA_AIFTuple* tuple = (*it).m_tuple;

				// data pointer
				const void* tileData = channel.tileDataPtr(tindex, size);
				if(!tileData)
				{
					continue;
				}

				// point range
				GA_Range range(pointMap, blockBegin, blockBegin+size);

				switch(type)
				{
					case UInt64Type:
					case Int64Type:
					{
						const int64* v = reinterpret_cast<const int64*>(tileData);
						tuple->setRange(attrib, range, v, 0, 1);
						break;
					}
					case FloatType:
					{
						const float* v = reinterpret_cast<const float*>(tileData);
						tuple->setRange(attrib, range, v, 0, 1);
						break;
					}
					case FloatV2Type:
					{
						const float* v = reinterpret_cast<const float*>(tileData);
						tuple->setRange(attrib, range, v, 0, 2);
						break;
					}
					case FloatV3Type:
					{
						const float* v = reinterpret_cast<const float*>(tileData);
						tuple->setRange(attrib, range, v, 0, 3);
						break;
					}
					default:
					{
						break;
					}
				}
			}
			blockBegin+=size;
		}
	}

	return 1;
}
