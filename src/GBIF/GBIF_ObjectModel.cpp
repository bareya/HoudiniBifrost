#include "GBIF_ObjectModel.h"

#include <map>

using std::map;
using std::weak_ptr;
using std::string;
using ObjectModelCache = map<string, weak_ptr<GBIF_ObjectModel>>;

using namespace Bifrost::API;


static ObjectModelCache theCache;


// Custom deleter to remove:
//  * string path from the cache
//  * object from the heap
static void FileIOCacheDeleter(GBIF_ObjectModel* p)
{
	// requiures a lock
	theCache.erase(p->getPath());
	delete p;
}


GBIF_ObjectModel::GBIF_ObjectModel(const string& path)
	: myPath(path),
	  myObjectModel(),
	  myFileIO(myObjectModel.createFileIO(path.c_str())),
	  myStateServer(myFileIO.load())
{
}


GBIF_ObjectModelPtr GBIF_ObjectModel::open(const string& path)
{
	// requires a lock
	ObjectModelCache::iterator it = theCache.find(path);
	if (it==theCache.end())
	{
		GBIF_ObjectModelPtr p(new GBIF_ObjectModel(path));
		theCache.emplace(path, p);
		return p;
	}
	else
	{
		return it->second.lock();
	}
}


GBIF_ObjectModel::~GBIF_ObjectModel()
{
}
