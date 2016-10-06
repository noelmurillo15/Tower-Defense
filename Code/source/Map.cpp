#include "Map.h"

#include "Game.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

Map* Map::GetInstance(){

	static Map m_Instance;
	return &m_Instance;
}

void Map::Initialize(){
	
	SetupGridLayout();

	for (unsigned int w = 0; w < cols; w++){
		for (unsigned int h = 0; h < rows; h++){

			Cell tmp;
			tmp.m_cellposX = w;
			tmp.m_cellposY = h;

			tmp.m_CellRect = {
				tmp.m_cellposX * 32.0f,
				tmp.m_cellposY * 32.0f,
				(tmp.m_cellposX * 32.0f) + 32.0f,
				(tmp.m_cellposY * 32.0f) + 32.0f
			};

			if (Grid[h][w] == 0)
				tmp.IsPlayerWalkable = true;
			else if (Grid[h][w] == 5){
				tmp.IsEnemyStartingPoint = true;
				EnemySpawnPoint = tmp;
				EnemySpawnPoint.IsEnemyStartingPoint = true;
			}
			else if (Grid[h][w] == 8){
				tmp.IsInterface = true;
			}
			else if (Grid[h][w] == 1 || Grid[h][w] == 2 || Grid[h][w] == 3 || Grid[h][w] == 4)
				tmp.IsEnemyWalkable = true;

			m_Grid.push_back(tmp);
		}
	}
}
void Map::Unload(){

}

void Map::Update(float elapsedTime){

}

void Map::Render(){

	SGD::Point cursor = SGD::InputManager::GetInstance()->GetCursorPosition();
	SGD::Rectangle cursorrect = { cursor.x, cursor.y, cursor.x + 1, cursor.y + 1 };

	for (unsigned int x = 0; x < m_Grid.size(); x++){

		if (cursorrect.IsIntersecting(m_Grid[x].m_CellRect)){
			SGD::GraphicsManager::GetInstance()->DrawRectangle(
				SGD::Rectangle(m_Grid[x].m_CellRect),
				SGD::Color(50, 0, 0, 255));
		}
		else if (m_Grid[x].IsEnemyStartingPoint){

			SGD::GraphicsManager::GetInstance()->DrawRectangle(
				SGD::Rectangle(m_Grid[x].m_CellRect),
				SGD::Color(255, 0, 0), SGD::Color(0, 0, 0), 1);
		}
		else if (m_Grid[x].IsPlayerWalkable && !m_Grid[x].IsEnemyStartingPoint){
			SGD::GraphicsManager::GetInstance()->DrawRectangle(
				SGD::Rectangle(m_Grid[x].m_CellRect),
				SGD::Color(0, 80, 0), SGD::Color(0, 0, 0), 1);
		}
		else if (m_Grid[x].IsInterface && !m_Grid[x].IsEnemyStartingPoint){
			SGD::GraphicsManager::GetInstance()->DrawRectangle(
				SGD::Rectangle(m_Grid[x].m_CellRect),
				SGD::Color(200, 200, 200));
		}
		else if (m_Grid[x].IsEnemyWalkable && !m_Grid[x].IsEnemyStartingPoint){
			SGD::GraphicsManager::GetInstance()->DrawRectangle(
				SGD::Rectangle(m_Grid[x].m_CellRect),
				SGD::Color(150, 150, 150), SGD::Color(0, 0, 0), 1);
		}
	}
}

void Map::SetupGridLayout(){
	for (unsigned int w = 0; w < 32; w++){
		for (unsigned int h = 0; h < 24; h++){
			Grid[h][w] = 0;
		}
	}

	Grid[3][0] = 5; 
	Grid[3][1] = 2;		Grid[3][2] = 2;		Grid[3][3] = 2;		Grid[3][4] = 2;		Grid[3][5] = 2;		
	Grid[3][7] = 2;		Grid[3][8] = 2;		Grid[3][9] = 2;		Grid[3][10] = 2;	Grid[3][11] = 2;
	Grid[3][12] = 2;	Grid[3][13] = 2;	Grid[3][14] = 2;	Grid[3][15] = 2;	Grid[3][16] = 2;
	Grid[3][17] = 2;	Grid[3][18] = 2;	Grid[3][19] = 2;	Grid[3][20] = 2;	Grid[3][21] = 2;
	Grid[3][22] = 2;	Grid[3][23] = 2;	Grid[3][24] = 1;	Grid[4][24] = 1;	Grid[5][24] = 1;
	Grid[6][24] = 4;	Grid[6][23] = 4;	Grid[6][22] = 4;	Grid[6][21] = 4;	Grid[3][6] = 2;
	Grid[6][20] = 4;	Grid[6][19] = 4;	Grid[6][18] = 4;	Grid[6][17] = 4;	Grid[6][16] = 4;
	Grid[6][15] = 4;	Grid[6][14] = 4;	Grid[6][13] = 4;	Grid[6][12] = 4;	Grid[6][11] = 4;
	Grid[6][10] = 4;	Grid[6][9] = 4;		Grid[6][8] = 4;		Grid[6][7] = 4;		Grid[6][6] = 4;
	Grid[6][5] = 4;		Grid[6][4] = 4;		Grid[6][3] = 4;		Grid[6][2] = 1;		Grid[7][2] = 1;
	Grid[8][2] = 1;		Grid[9][2] = 1;		Grid[10][2] = 1;	Grid[11][2] = 1;	Grid[12][2] = 1;
	Grid[13][2] = 1;	Grid[14][2] = 1;	Grid[15][2] = 1;	Grid[16][2] = 2;	Grid[17][2] = 2;
	Grid[16][1] = 2;	Grid[16][3] = 2;	Grid[16][4] = 2;	Grid[16][6] = 2;	Grid[16][7] = 2;
	Grid[16][8] = 2;	Grid[16][9] = 2;	Grid[16][10] = 2;	Grid[16][11] = 2;	Grid[16][12] = 2;
	Grid[16][13] = 2;	Grid[16][14] = 2;	Grid[16][15] = 2;	Grid[16][16] = 2;	Grid[16][17] = 2;
	Grid[16][18] = 2;	Grid[16][19] = 2;	Grid[16][20] = 2;	Grid[16][21] = 2;	Grid[16][22] = 2;
	Grid[16][23] = 2;	Grid[16][24] = 1;	Grid[17][24] = 1;	Grid[18][24] = 1;	Grid[19][24] = 1;
	Grid[20][24] = 4;	Grid[20][23] = 4;	Grid[20][22] = 4;	Grid[20][21] = 4;	Grid[20][20] = 4;
	Grid[20][19] = 4;	Grid[20][18] = 4;	Grid[20][17] = 4;	Grid[20][16] = 4;	Grid[20][15] = 4;
	Grid[20][14] = 4;	Grid[20][13] = 4;	Grid[20][12] = 4;	Grid[20][11] = 4;	Grid[20][10] = 4;
	Grid[20][9] = 4;	Grid[20][8] = 4;	Grid[20][7] = 4;	Grid[20][6] = 4;	Grid[20][5] = 3;
	Grid[19][5] = 3;	Grid[18][5] = 3;	Grid[17][5] = 3;	Grid[15][5] = 3;	Grid[14][5] = 3;
	Grid[13][5] = 3;	Grid[12][5] = 2;	Grid[12][6] = 2;	Grid[12][7] = 2;	Grid[12][8] = 2;
	Grid[12][9] = 2;	Grid[12][10] = 2;	Grid[12][11] = 2;	Grid[12][12] = 2;

	for (int c = 26; c < 32; c++){
		for (int r = 0; r < 24; r++){
			Grid[r][c] = 8;
		}
	}
}