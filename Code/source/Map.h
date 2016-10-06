#pragma once

#include <vector>

#include "../SGD Wrappers/SGD_GraphicsManager.h"

class Map {

	static const int rows = 24;
	static const int cols = 32;

	struct Cell{

		int m_cellposX = 0;
		int m_cellposY = 0;

		bool IsFilled = false;
		bool IsInterface = false;
		bool IsEnemyWalkable = false;
		bool IsPlayerWalkable = false;
		bool IsEnemyStartingPoint = false;

		SGD::Rectangle m_CellRect;
	};

	std::vector<Cell> m_Grid;
	Cell EnemySpawnPoint;

	int Grid[rows][cols];

		//	Quadrilogy of Evil
	Map() = default;
	~Map() = default;
	Map(const Map&) = delete;
	Map& operator=(const Map&) = delete;

public:

		// Singleton Accessor:
	static Map* GetInstance(void);

		//	Singleton Interface
	void Initialize();
	void Unload();

	Cell GetStartingCell(){ return EnemySpawnPoint; }
	int GetGridDir(int row, int col){ return Grid[row][col]; }

	void SetupGridLayout();
	
		//	Interface
	void	Update(float elapsedTime);
	void	Render();	
};

