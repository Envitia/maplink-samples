
#pragma once

#include <vector>
#include <memory>
#include <chrono>

class TSLDataLayer;
class TSLTerrainDatabase;

class DatalayersManager
{
public:
	DatalayersManager();
	~DatalayersManager();

public:

	// Add a map layer to the document
	bool loadMapLayer(std::string filename, std::string layerName, std::string& err_msg);

	// Add a terrain database to the document
	bool loadTerrainDatabase(std::string filename, std::string databaseName, std::string& err_msg);

	// Query the layers size
	int getLayersSize();

	// Query a layer
	TSLDataLayer* getLayer(unsigned int index);

	// Query a layer name
	std::string getLayerName(unsigned int index);

	// Query the terrain databases size
	int getTerrainDatabasesSize();

	// Query a terrain database
	TSLTerrainDatabase* getTerrainDatabase(unsigned int index);

	// Query a terrain database name
	std::string getTerrainDatabaseName(unsigned int index);

private:

	struct LayerInfo {
		std::shared_ptr <TSLDataLayer> layer;
		std::string layerName;
	};
	struct TerrainInfo {
		std::shared_ptr<TSLTerrainDatabase> database;
		std::string databaseName;
	};

	// The various data layers available/loaded in the application
	std::vector<LayerInfo> m_dataLayers;

	// The terrain databases available/loaded in the application
	std::vector<TerrainInfo> m_terrainDatabases;
};
