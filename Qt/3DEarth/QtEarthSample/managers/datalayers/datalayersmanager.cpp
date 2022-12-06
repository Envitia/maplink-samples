#ifdef WINNT
# define WIN32_LEAN_AND_MEAN
# define VC_EXTRALEAN
# include "windows.h"
#endif
#include "MapLink.h"

// Include the MapLink Earth SDK
// Note that the Earth SDK makes use of namespaces within the API
// in this case envitia::maplink::earth
#include <MapLinkEarth.h>

#include "datalayersmanager.h"
#include "tsldatalayer.h"
#include "tslterraindatabase.h"

DatalayersManager::DatalayersManager()
{
}

DatalayersManager::~DatalayersManager()
{
}

bool DatalayersManager::loadMapLayer(std::string filename, std::string layerName, std::string& err_msg) {
	// Create a new map layer
	auto* mapLayer = new TSLMapDataLayer();
	// Load the map into the layer
	TSLThreadedErrorStack::clear();
	if (!mapLayer->loadData(filename.c_str())) {
		TSLSimpleString msg("");
		bool anyErrors = TSLThreadedErrorStack::errorString(msg, "Failed to load map: \n");
		if (anyErrors)
		{
			err_msg = TSLUTF8Decoder(msg);
		}
		mapLayer->destroy();
		return false;
	}

	LayerInfo info;
	info.layer.reset(mapLayer, TSLDestroyPointer<TSLDataLayer>());
	info.layerName = layerName;

	m_dataLayers.push_back(info);

	return true;
}

bool DatalayersManager::loadTerrainDatabase(std::string filename, std::string databaseName, std::string& err_msg) {
	// Create a new database
	auto* database = new TSLTerrainDatabase();
	// Load the map into the layer
	TSLThreadedErrorStack::clear();
	if (database->open(filename.c_str()) != TSLTerrain_OK) {
		TSLSimpleString msg("");
		bool anyErrors = TSLThreadedErrorStack::errorString(msg, "Failed to load terrain database: \n");
		if (anyErrors)
		{
			err_msg = TSLUTF8Decoder(msg);
		}
		delete database;
		return false;
	}

	TerrainInfo info;
	info.database.reset(database);
	info.databaseName = databaseName;

	m_terrainDatabases.emplace_back(info);
	return true;
}

int DatalayersManager::getLayersSize()
{
	return m_dataLayers.size();
}

TSLDataLayer* DatalayersManager::getLayer(unsigned int index) {
	if (index >= m_dataLayers.size()) return nullptr;
	return m_dataLayers[index].layer.get();
}

std::string DatalayersManager::getLayerName(unsigned int index) {
	if (index >= m_dataLayers.size()) return "";
	return m_dataLayers[index].layerName;
}

int DatalayersManager::getTerrainDatabasesSize()
{
	return m_terrainDatabases.size();
}

TSLTerrainDatabase* DatalayersManager::getTerrainDatabase(unsigned int index) {
	if (index >= m_terrainDatabases.size()) return nullptr;
	return m_terrainDatabases[index].database.get();
}

std::string DatalayersManager::getTerrainDatabaseName(unsigned int index) {
	if (index >= m_terrainDatabases.size()) return "";
	return m_terrainDatabases[index].databaseName;
}
