#include "GEO_BifrostIOTranslator.h"

#include "GEO/GEO_Detail.h"

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


static ChannelAttributeAccessor createAttribute(GEO_Detail& geo, Ref& ch)
{
	Channel channel(ch);
	DataType type = channel.dataType();

	if(channel.name()=="position" && type==FloatV3Type)
	{
		GA_Attribute* a = geo.getP();
		return {ch, a, a->getAIFTuple()};
	}

	if(channel.name()=="velocity" && type==FloatV3Type)
	{
		GA_Attribute* a = geo.addFloatTuple(GA_ATTRIB_POINT,"v",3);
		return {ch, a, a->getAIFTuple()};
	}

	if(channel.name()=="id64" && type==UInt64Type)
	{
		GA_Attribute* a =  geo.addIntTuple(GA_ATTRIB_POINT, "id", 1);
		return {ch, a, a->getAIFTuple()};
	}

	switch(type)
	{
		case UInt64Type:
		case Int64Type:
		{
			GA_Attribute* a = geo.addIntTuple(GA_ATTRIB_POINT, channel.name().c_str(), 1);
			return {ch, a, a->getAIFTuple()};
		}
		case FloatType:
		{
			GA_Attribute* a = geo.addFloatTuple(GA_ATTRIB_POINT, channel.name().c_str(), 1);
			return {ch, a, a->getAIFTuple()};
		}
		case FloatV2Type:
		{
			GA_Attribute* a = geo.addFloatTuple(GA_ATTRIB_POINT, channel.name().c_str(), 2);
			return {ch, a, a->getAIFTuple()};
		}
		case FloatV3Type:
		{
			GA_Attribute* a = geo.addFloatTuple(GA_ATTRIB_POINT, channel.name().c_str(), 3);
			return {ch, a, a->getAIFTuple()};
		}
		default:
		{
			return {ch, nullptr, nullptr};
		}
	}

	return {ch, nullptr, nullptr};
}


GEO_BifrostIOTranslator::GEO_BifrostIOTranslator()
	: GEO_IOTranslator()
{
}


GEO_BifrostIOTranslator::GEO_BifrostIOTranslator(const GEO_BifrostIOTranslator &other)
{
}


GEO_BifrostIOTranslator::~GEO_BifrostIOTranslator()
{
}


GEO_IOTranslator* GEO_BifrostIOTranslator::duplicate() const
{
	return new GEO_BifrostIOTranslator(*this);
}


const char*	GEO_BifrostIOTranslator::formatName() const
{
	return "Bifrost file format";
}


int GEO_BifrostIOTranslator::checkExtension(const char *name)
{
	UT_String strName(name);
	return strName.matchFileExtension(".bif");
}


int GEO_BifrostIOTranslator::checkMagicNumber(unsigned magic)
{
	return 0;
}


GA_Detail::IOStatus GEO_BifrostIOTranslator::fileLoad(GEO_Detail *gdp, UT_IStream &is, bool ate_magic)
{
	ObjectModel om;
	FileIO fio = om.createFileIO(is.getFilename());
	StateServer ss = fio.load();

	// failed to read the file
	if(!ss.valid())
	{
		std::cout << "Error reading bif file" << std::endl;
		return 0;
	}

	// bif has only one component
	Component component = ss.components()[0];

	// check type
	if(component.type()!=PointComponentType)
	{
		std::cout << "Currently only point components can be read" << std::endl;
		return 0;
	}

	// component info
	Layout layout = component.layout();
	RefArray channels = component.channels();

	// create attributes
	std::vector<ChannelAttributeAccessor> channelAttributeMap;
	for(int ch=0;ch<channels.count();++ch)
	{
		channelAttributeMap.emplace_back(createAttribute(*gdp, channels[ch]));
	}

	// create points
	GA_Offset blockBegin = gdp->appendPointBlock(component.elementCount());
	const GA_IndexMap& pointMap = gdp->getPointMap();

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


GA_Detail::IOStatus GEO_BifrostIOTranslator::fileSave(const GEO_Detail *gdp, std::ostream &os)
{
	std::cout << "Save option is not supported" << std::endl;
	return 0;
}

