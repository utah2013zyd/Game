#pragma once

#include "FCBaseConfig.h"
#include <orz/Toolkit_Base/Singleton.h>
#include <map>
#include <iostream>
#include <fstream>
namespace Orz{
	struct ProjectileData{
		unsigned int id;
		float position[3];
		float orientation[4];
	};

	struct FlightData{
		unsigned int id;
		float position[3];   //position: x, y, z
		float orientation[4];   //orientation: w, x, y, z
		float healthPoint;
		std::map<unsigned int, ProjectileData> activeProjectiles;
	};

	struct ObstacleData{
		unsigned int id;
		float position[3];
		float radius;
	};

	class _FCBaseExport DataLogger: public Singleton<DataLogger>
	{
	public:
		DataLogger(void);
		~DataLogger(void);

		void getPlayerData(const unsigned int id, const float* pos, const float* orientation, const float healthPoint);
		void getPlayerProjectileData(const unsigned int id, const float* pos, const float* orientation);

		void getBotNum(int num){ botNum = num; }
		void getBotData(const unsigned int id, const float* pos, const float* orientation, const float healthPoint);
		void getBotProjectileData(const unsigned int id, const unsigned int ownerId, const float* pos, const float* orientation);
		
		void getObstacleNum(int num){ obstacleNum = num; }
		void getObstacleData(const unsigned int id, const float* pos, const float radius);

		void writeTest();
	private:
		FlightData player;
		int botNum;
		std::map<unsigned int, FlightData> bots;
		int obstacleNum;
		std::map<unsigned int, ObstacleData> obstacles;
		
	};

	typedef  boost::shared_ptr<DataLogger> DataLoggerPtr;
}