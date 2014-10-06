#include "DataLogger.h"
using namespace Orz;

template<> DataLogger* Singleton<DataLogger>::_singleton = NULL;
DataLogger::DataLogger(void)
{
}

DataLogger::~DataLogger(void)
{
}

void DataLogger::getPlayerData(const unsigned int id, const float* pos, const float* orientation, const float healthPoint)
{
	player.id = id;
	player.position[0] = pos[0];
	player.position[1] = pos[1];
	player.position[2] = pos[2];

	player.orientation[0] = orientation[0];
	player.orientation[1] = orientation[1];
	player.orientation[2] = orientation[2];
	player.orientation[3] = orientation[3];	

	player.healthPoint = healthPoint;
}

void DataLogger::getPlayerProjectileData(const unsigned int id, const float* pos, const float* orientation)
{
	ProjectileData projectile;
	projectile.id = id;
	projectile.position[0] = pos[0];
	projectile.position[1] = pos[1];
	projectile.position[2] = pos[2];

	projectile.orientation[0] = orientation[0];
	projectile.orientation[1] = orientation[1];
	projectile.orientation[2] = orientation[2];
	projectile.orientation[3] = orientation[3];	

	player.activeProjectiles.insert(std::pair<unsigned int, ProjectileData>(id, projectile));
}

void DataLogger::getBotData(const unsigned int id, const float* pos, const float* orientation, const float healthPoint)
{
	FlightData bot;
	bot.id = id;
	bot.position[0] = pos[0];
	bot.position[1] = pos[1];
	bot.position[2] = pos[2];

	bot.orientation[0] = orientation[0];
	bot.orientation[1] = orientation[1];
	bot.orientation[2] = orientation[2];
	bot.orientation[3] = orientation[3];	

	bot.healthPoint = healthPoint;

	bots.insert(std::pair<unsigned int, FlightData>(id, bot));
}

void DataLogger::getBotProjectileData(const unsigned int id, const unsigned int ownerId, const float* pos, const float* orientation)
{
	std::map<unsigned int, FlightData>::iterator it;
	it = bots.find(ownerId);
	
	ProjectileData projectile;
	projectile.id = id;
	projectile.position[0] = pos[0];
	projectile.position[1] = pos[1];
	projectile.position[2] = pos[2];

	projectile.orientation[0] = orientation[0];
	projectile.orientation[1] = orientation[1];
	projectile.orientation[2] = orientation[2];
	projectile.orientation[3] = orientation[3];	

	it->second.activeProjectiles.insert(std::pair<unsigned int, ProjectileData>(id, projectile));
}
		
void DataLogger::getObstacleData(const unsigned int id, const float* pos, const float radius)
{
	ObstacleData obstacle;
	obstacle.id = id;
	obstacle.position[0] = pos[0];
	obstacle.position[1] = pos[1];
	obstacle.position[2] = pos[2];

	obstacle.radius = radius;

	obstacles.insert(std::pair<unsigned int, ObstacleData>(id, obstacle));
}

void DataLogger::writeTest()
{
	char filename[] = "data.dat";
	std::ofstream fout(filename);
	//player data
	fout << "player: " << std::endl;
	fout << "id: " << player.id << std::endl;
	fout << "position: " << player.position[0] << ", " << player.position[1] << ", " << player.position[2] << std::endl;
	fout << "orientation: "<<player.orientation[0] << ", " << player.orientation[1] << ", " << player.orientation[2] << ", " << player.orientation[3] << std::endl;
	fout << "healthPoint: "<< player.healthPoint<<std::endl;

	//BOT data
	fout << "BOTs: " << std::endl;
	std::map<unsigned int, FlightData>::iterator it;
	for(it = bots.begin(); it != bots.end(); it++)
	{
		fout << "id: " << it->second.id << std::endl;
		fout << "position: " << it->second.position[0] << ", " << it->second.position[1] << ", " << it->second.position[2] << std::endl;
		fout << "orientation: "<<it->second.orientation[0] << ", " << it->second.orientation[1] << ", " << it->second.orientation[2] << ", " << it->second.orientation[3] << std::endl;
		fout << "healthPoint: "<< it->second.healthPoint<<std::endl;
	}	

	//obstacle data
	fout << "obstacles: " << std::endl;
	std::map<unsigned int, ObstacleData>::iterator it2;
	for(it2 = obstacles.begin(); it2 != obstacles.end(); it2++)
	{
		fout << "id: " << it2->second.id << std::endl;
		fout << "position: " << it2->second.position[0] << ", " << it2->second.position[1] << ", " << it2->second.position[2] << std::endl;
		fout << "radius: " << it2->second.radius << std::endl;

	}
}