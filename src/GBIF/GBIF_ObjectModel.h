#ifndef GBIF_FILEIO_H
#define GBIF_FILEIO_H

#include "GBIF_Types.h"

#include <string>

#include <bifrostapi/bifrost_om.h>
#include <bifrostapi/bifrost_fileio.h>
#include <bifrostapi/bifrost_stateserver.h>

/// Caches Bifrost ObjectModel
class GBIF_ObjectModel
{
public:
	/// opens a file and/or returns cached handle
	static GBIF_ObjectModelPtr open(const std::string& path);

	~GBIF_ObjectModel();

	inline bool valid() const
	{
		return myStateServer.valid();
	}

	inline std::string getPath() const
	{
		return myPath;
	}

	inline Bifrost::API::StateServer& getStateServer()
	{
		return myStateServer;
	}

private:
	/// This constructor is private, create instances through open method.
	GBIF_ObjectModel(const std::string& path);

	/// Cached values to read components.
	Bifrost::API::ObjectModel myObjectModel;
	Bifrost::API::FileIO myFileIO;
	Bifrost::API::StateServer myStateServer;

	/// BIF location.
	std::string myPath;
};

#endif // GBIF_FILEIO_H
