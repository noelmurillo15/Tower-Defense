/***************************************************************
|	File:		GameplayState.cpp
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	GameplayState class initializes & runs the game logic
***************************************************************/

#include "GameplayState.h"

#include "Game.h"

#include "MainMenuState.h"
#include "CreditsState.h"

#include "MessageID.h"
#include "CreateEnemyMessage.h"
#include "CreatePlasmaMessage.h"
#include "CreateWindStyleShurikenMessage.h"
#include "CreateFireBallMessage.h"
#include "DestroyEntityMessage.h"

#include "Entity.h"
#include "EntityManager.h"
#include "Tower.h"
#include "FireMage.h"
#include "AquaGoddess.h"
#include "Zephyr.h"
#include "Ghast.h"
#include "Spire.h"
#include "UndeadBoxer.h"
#include "Goblin.h"
#include "Plasma.h"
#include "Map.h"
#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Message.h"

#include <cstdlib>
#include <cassert>
#include <stdlib.h>
#include <time.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define NUM_SPAWNS 5


GameplayState* GameplayState::GetInstance( void ) {

	static GameplayState s_Instance;	// stored in global memory once
	return &s_Instance;
}

void GameplayState::Enter(void) {

		//	Seed random
	srand((unsigned int)time(nullptr));
		//	Entity Manager Setup
	m_pEntities = new EntityManager;

		//	Loading Assets
	m_hTower = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/ANM_Tower.png");
	m_hCharacters = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/ANM_Characters.png");
	m_Plasma = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/ANM_Tower_Particle.png");
	m_Fireball = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/ANM_Firemage_Fireball.png");
	m_Windshuri = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/ANM_WindStyleBlast.png");
	m_EnemySleep = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/ANM_EnemySleep.png");
    m_PlasmaSfx = 	SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/ANM_FireBlastGoBoom.wav");
	m_win = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/ANM_WinSound.wav");
	m_lose = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/ANM_LoseSound.wav");


		// Initialize the Event & Message Managers
	SGD::EventManager::GetInstance()->Initialize();
	SGD::MessageManager::GetInstance()->Initialize(&MessageProc);

	Tower* tower = Tower::GetInstance();

		//	Pause menu setup
	m_pause.m_PauseScreen = {
		256, 256,
		Game::GetInstance()->GetScreenWidth() - 256,
		Game::GetInstance()->GetScreenHeight() - 256
	};

		//	Initiate Map
	Map::GetInstance()->Initialize();

		//	Initiate Tower
	tower->Load();
	tower->SetImage(m_hTower);
	tower->SetSize(SGD::Size(128.0f, 128.0f));
	tower->SetPosition(SGD::Point((Game::GetInstance()->GetScreenWidth() / 2) - (tower->GetSize().width / 2),
		(Game::GetInstance()->GetScreenHeight() / 2) - (tower->GetSize().height / 2)));

		//	Interface System
	m_interface.IsSelected = false;
	m_interface.m_SelectedHero = { 848, 96, 1008, 160 };
	m_interface.m_HeroSpawnRect = { 0, 0, 832, 768 };
	m_interface.m_FireMageRect = { 864, 192, 896, 224 };
	m_interface.m_AquaGoddessRect = { 960, 192, 992, 224 };
	m_interface.m_ZephyrRect = { 864, 256, 896, 288 };
	m_interface.m_SelectedType = Entity::ENT_BASE;
}

void GameplayState::Exit( void ) {

	SGD::GraphicsManager* graphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* audio = SGD::AudioManager::GetInstance();

		//	Unload assets
	graphics->UnloadTexture(m_hTower);
	graphics->UnloadTexture(m_hCharacters);
	graphics->UnloadTexture(m_Plasma);
	graphics->UnloadTexture(m_Fireball);
	graphics->UnloadTexture(m_Windshuri);
	graphics->UnloadTexture(m_EnemySleep);
	audio->UnloadAudio(m_PlasmaSfx);
	audio->UnloadAudio(m_win);
	audio->UnloadAudio(m_lose);

	m_pause.isPaused = false;

		//	Unload some more
	Tower::GetInstance()->Unload();
	Map::GetInstance()->Unload();

		// Deallocate the Entity Manager
	if (m_pEntities != nullptr){
		m_pEntities->RemoveAll();
		delete m_pEntities;
		m_pEntities = nullptr;
	}
	
		// Terminate & deallocate the SGD wrappers
	SGD::EventManager::GetInstance()->Terminate();
	SGD::EventManager::DeleteInstance();
	SGD::MessageManager::GetInstance()->Terminate();
	SGD::MessageManager::DeleteInstance();
}

bool GameplayState::Update(float elapsedTime)	{

	SGD::InputManager* input = SGD::InputManager::GetInstance();
	SGD::AudioManager* audio = SGD::AudioManager::GetInstance();
	Tower* tower = Tower::GetInstance();

	SGD::Point cursor = SGD::InputManager::GetInstance()->GetCursorPosition();
	SGD::Rectangle cursorrect = { cursor.x, cursor.y, cursor.x + 1, cursor.y + 1 };
	

		//	If No enemies are alive.. end round
	if (tower->GetNumSpawnsAlive() == 0 && tower->GetWaveIncoming())
		tower->SetWaveIncoming(false);
		//	Winning condition
	else if (tower->GetWaveNum() == 10 && !tower->GetInstance()->GetWaveIncoming() && m_endgame.Losing == false && tower->GetNumSpawnsAlive() == 0 && m_endgame.Winning == false){
		
		if (m_win != SGD::INVALID_HANDLE && audio->IsAudioPlaying(m_win) == false)
			audio->PlayAudio(m_win);
		m_endgame.Winning = true;
	}	//	Losing condition
	else if (tower->GetHealth() <= 0.0f && !m_endgame.Losing){

		tower->SetLives(tower->GetLives() - 1);
		if (m_lose != SGD::INVALID_HANDLE && audio->IsAudioPlaying(m_lose) == false)
			SGD::AudioManager::GetInstance()->PlayAudio(m_lose);
		if (tower->GetLives() == 0)
			m_endgame.GameOver = true;
		m_endgame.Losing = true;
	}

		//	Losing Input
	if (input->IsAnyKeyPressed() == true && m_endgame.Losing && !m_endgame.GameOver){
		tower->Reset();
		m_endgame.Losing = false;
	}	//	Winning Input
	else if (input->IsAnyKeyPressed() == true && m_endgame.Winning){
		m_endgame.Winning = false;
		Game::GetInstance()->ChangeState(CreditsState::GetInstance());
		return true;
	}	//	Game Over Input
	else if (input->IsAnyKeyPressed() == true && m_endgame.GameOver && tower->GetLives() == 0){
		m_endgame.GameOver = false;
		m_endgame.Winning = false;
		m_endgame.Losing = false;
		Game::GetInstance()->ChangeState(CreditsState::GetInstance());
		return true;
	}

		//	Spawn Next Wave
	if (input->IsKeyPressed(SGD::Key::Enter) == true && !tower->GetWaveIncoming() && input->IsKeyDown(SGD::Key::Alt) == false
		&& !m_endgame.Losing) {
		tower->NextWave();
		tower->SetNumSpawns((float)((NUM_SPAWNS) * tower->GetWaveNum()));
	}
		//	If spawn timer is a > 0 .. update
	if (tower->GetSpawnTimer() > 0.0f)
		tower->UpdateSpawnTimer(elapsedTime);

#pragma region Enemy Spawn
	else if (tower->GetSpawnTimer() <= 0.0f && tower->GetNumSpawns() > 0 /*&& tower->GetWaveIncoming()*/){
	
		if (tower->GetWaveNum() > 0){
			CreateEnemyMessage* pMsg = new CreateEnemyMessage{ Entity::E_GHAST };
			pMsg->QueueMessage();
			pMsg = nullptr;

			tower->SetSpawnTimer(0.35f);
			tower->SetNumSpawns(tower->GetNumSpawns() - 1);
			tower->SetNumSpawnsAlive(tower->GetNumSpawnsAlive() + 1);
			tower->SetWaveIncoming(true);
		}
		if(tower->GetWaveNum() > 2){

			CreateEnemyMessage* pMsg2 = new CreateEnemyMessage{ Entity::E_SPIRE };
			pMsg2->QueueMessage();
			pMsg2 = nullptr;
			tower->SetNumSpawns(tower->GetNumSpawns() - 1);
			tower->SetNumSpawnsAlive(tower->GetNumSpawnsAlive() + 1);
		}
		if (tower->GetWaveNum() > 5){

			unsigned int numspawns = (unsigned int)tower->GetNumSpawns();			
			if (numspawns > 0 && numspawns <= 15){
				CreateEnemyMessage* pMsg3 = new CreateEnemyMessage{ Entity::E_UNDEAD };
				pMsg3->QueueMessage();
				pMsg3 = nullptr;

				tower->SetNumSpawns(tower->GetNumSpawns() - 1);
				tower->SetNumSpawnsAlive(tower->GetNumSpawnsAlive() + 1);
			}
		}
		if (tower->GetWaveNum() > 7){

			unsigned int numspawns = (unsigned int)tower->GetNumSpawns();
			if (numspawns != 0 && numspawns <= 12){
				CreateEnemyMessage* pMsg4 = new CreateEnemyMessage{ Entity::E_GOBLIN };
				pMsg4->QueueMessage();
				pMsg4 = nullptr;

				tower->SetNumSpawns(tower->GetNumSpawns() - 1);
				tower->SetNumSpawnsAlive(tower->GetNumSpawnsAlive() + 1);
			}
		}
	}
#pragma endregion


#pragma region Hero Selection Interface
	if (cursorrect.IsIntersecting(m_interface.m_FireMageRect) 
		&& input->IsKeyPressed(SGD::Key::MouseLeft) == true){

		m_interface.IsSelected = true;
		m_interface.m_SelectedType = Entity::P_FIREMAGE;
	}
	else if (cursorrect.IsIntersecting(m_interface.m_AquaGoddessRect)
		&& input->IsKeyPressed(SGD::Key::MouseLeft) == true){

		m_interface.IsSelected = true;
		m_interface.m_SelectedType = Entity::P_AQUAGODDESS;
	}
	else if (cursorrect.IsIntersecting(m_interface.m_ZephyrRect)
		&& input->IsKeyPressed(SGD::Key::MouseLeft) == true){

		m_interface.IsSelected = true;
		m_interface.m_SelectedType = Entity::P_ZEPHYR;
	}

	if (cursorrect.IsIntersecting(m_interface.m_HeroSpawnRect) && m_interface.IsSelected){
		if (input->IsKeyReleased(SGD::Key::MouseLeft)){
			if (!(tower->GetWaveIncoming())){
				if (m_interface.m_SelectedType == Entity::P_FIREMAGE && tower->GetGold() - 125.0f >= 0.0f){

					GameplayState* self = GameplayState::GetInstance();
					Entity* player = self->CreateHero(m_hCharacters, Entity::P_FIREMAGE);

					self->m_pEntities->AddEntity(player, 2);
					tower->SetGold(tower->GetGold()
						- dynamic_cast<FireMage*>(player)->GetCostForPurcahse());

					player->Release();
					player = nullptr;
				}
				else if (m_interface.m_SelectedType == Entity::P_AQUAGODDESS && tower->GetGold() - 250.0f >= 0.0f){

					GameplayState* self = GameplayState::GetInstance();
					Entity* player = self->CreateHero(m_hCharacters, Entity::P_AQUAGODDESS);

					self->m_pEntities->AddEntity(player, 2);
					tower->SetGold(tower->GetGold()
						- dynamic_cast<AquaGoddess*>(player)->GetCostForPurcahse());

					player->Release();
					player = nullptr;
				}
				else if (m_interface.m_SelectedType == Entity::P_ZEPHYR && tower->GetGold() - 400.0f >= 0.0f){

					GameplayState* self = GameplayState::GetInstance();
					Entity* player = self->CreateHero(m_hCharacters, Entity::P_ZEPHYR);

					self->m_pEntities->AddEntity(player, 2);
					tower->SetGold(tower->GetGold()
						- dynamic_cast<Zephyr*>(player)->GetCostForPurcahse());

					player->Release();
					player = nullptr;
				}
			}
		}
	}
#pragma endregion

	
#pragma region Pause Menu
		//	Escape during pause
	if (input->IsKeyPressed(SGD::Key::Escape) == true && !m_pause.isPaused && !m_endgame.Losing) {
		m_pause.isPaused = true;
	}
	else if (input->IsKeyPressed(SGD::Key::Escape) == true && m_pause.isPaused) {
		m_pause.isPaused = false;
	}
		//	Enter during pause
	if (m_pause.isPaused && input->IsKeyPressed(SGD::Key::Enter) == true && m_pause.m_nCursor == 1
		&& !m_endgame.Losing){
		m_pause.isPaused = false;
	}
	else if (m_pause.isPaused && input->IsKeyPressed(SGD::Key::Enter) == true && m_pause.m_nCursor == 2){
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
		return true;
	}
		//	Arrow keys during pause
	if (m_pause.isPaused && input->IsKeyPressed(SGD::Key::UpArrow)){

		if (m_pause.m_nCursor == 2)
			m_pause.m_nCursor = 1;
		else
			m_pause.m_nCursor = 2;
	}
	else if (m_pause.isPaused && input->IsKeyPressed(SGD::Key::DownArrow)){

		if (m_pause.m_nCursor == 2)
			m_pause.m_nCursor = 1;
		else
			m_pause.m_nCursor = 2;
	}
#pragma endregion

	if (!m_pause.isPaused){
			//	Update Everything
		tower->Update(elapsedTime);
		m_pEntities->UpdateAll(elapsedTime);

			//	Check to see if any attacks hit enemies
		m_pEntities->CheckCollisions(0, 1);
			//	check to see if entities have reached the tower
		m_pEntities->CheckCollisions(0, tower->GetRect());
			//	check to see if any entities are in range of players
		m_pEntities->CheckCollisions(0, 2);

		Map::GetInstance()->Update(elapsedTime);

		// Process the events & messages
		SGD::EventManager::GetInstance()->Update();
		SGD::MessageManager::GetInstance()->Update();
	}
	return true;	// keep playing
}

void GameplayState::Render(float elapsedTime)	{

	const BitmapFont* pFont = Game::GetInstance()->GetFont();
	SGD::GraphicsManager* graphics = SGD::GraphicsManager::GetInstance();
	Tower* tower = Tower::GetInstance();

	SGD::OStringStream stats;
	SGD::OStringStream score;

	stats << " Hp: " << tower->GetHealth();
	stats << "\n Lives: " << tower->GetLives();
	stats << "\n Gold: " << tower->GetGold();
	score << " Score: " << tower->GetScore();
	score << "\n Wave: " << tower->GetWaveNum() << "/10";
	score << "\n Alive: " << tower->GetNumSpawnsAlive();

	
	Map::GetInstance()->Render();

	pFont->Draw(stats.str().c_str(), { 1024 - 196, 10 }, 0.5f, SGD::Color(0, 0, 0));
	pFont->Draw(score.str().c_str(), { 1024 - 196, 768 - 64 }, 0.5f, SGD::Color(0, 0, 0));

		// Render the entities
	tower->Render();
	m_pEntities->RenderAll();


	graphics->DrawRectangle(m_interface.m_SelectedHero, SGD::Color(0, 0, 0, 0), SGD::Color(0, 0, 0), 1);
	graphics->DrawRectangle(m_interface.m_FireMageRect, SGD::Color(0, 0, 0, 0), SGD::Color(50,255, 0, 0), 1);
	graphics->DrawRectangle(m_interface.m_AquaGoddessRect, SGD::Color(0, 0, 0, 0), SGD::Color(50, 0, 0, 255), 1);
	graphics->DrawRectangle(m_interface.m_ZephyrRect, SGD::Color(0, 0, 0, 0), SGD::Color(50, 0, 255, 0), 1);

		//	FireMage
	graphics->DrawTextureSection(m_hCharacters,
		SGD::Point(864.0f, 192.0f), { 0.0f, 160.0f, 32.0f, 192.0f });
		//	Aqua
	graphics->DrawTextureSection(m_hCharacters,
		SGD::Point(960, 192.0f), { 0.0f, 128.0f, 32.0f, 160.0f });
		//	Zephyr
	graphics->DrawTextureSection(m_hCharacters,
		SGD::Point(864.0f, 256.0f), { 0.0f, 224.0f, 32.0f, 256.0f });
	

	if (m_interface.IsSelected){
		if (m_interface.m_SelectedType == Entity::P_FIREMAGE){
			SGD::OStringStream SelectedHeroStats;
			SelectedHeroStats << "Fire Mage\n\nCost: 150";
			pFont->Draw(SelectedHeroStats.str().c_str(), { 848, 104 }, 0.5f, SGD::Color(255, 0, 0));
		}
		else if (m_interface.m_SelectedType == Entity::P_AQUAGODDESS){
			SGD::OStringStream SelectedHeroStats;
			SelectedHeroStats << "Aqua\n\nCost: 250";
			pFont->Draw(SelectedHeroStats.str().c_str(), { 848, 104 }, 0.5f, SGD::Color(0, 0, 255));
		}
		else if (m_interface.m_SelectedType == Entity::P_ZEPHYR){
			SGD::OStringStream SelectedHeroStats;
			SelectedHeroStats << "Zephyr\n\nCost: 400";
			pFont->Draw(SelectedHeroStats.str().c_str(), { 848, 104 }, 0.5f, SGD::Color(100, 0, 255, 0));
		}
	}


	if (m_pause.isPaused){
		graphics->DrawRectangle(m_pause.m_PauseScreen,
			SGD::Color(160, 160, 160), SGD::Color(0, 0, 0));

			// Use the game's font
		const BitmapFont* pFont = Game::GetInstance()->GetFont();

			// Display the level options centered at 1x scale
		pFont->Draw("Resume", { m_pause.m_PauseScreen.left + 150, m_pause.m_PauseScreen.top + 50 },
			1.0f, { 255, 0, 0 });
		pFont->Draw("Quit", { m_pause.m_PauseScreen.left + 150, m_pause.m_PauseScreen.top + 100 },
			1.0f, { 255, 0, 0 });


		if (m_pause.m_nCursor == 1)
			pFont->Draw("=       =", { m_pause.m_PauseScreen.left + 100, m_pause.m_PauseScreen.top + (50 * m_pause.m_nCursor) },
			1.0f, { 0, 255, 0 });

		else if (m_pause.m_nCursor == 2)
			pFont->Draw("=      =", { m_pause.m_PauseScreen.left + 100, m_pause.m_PauseScreen.top + (50 * m_pause.m_nCursor) },
			1.0f, { 0, 255, 0 });
	}
	else if (m_endgame.Losing == true && !m_endgame.GameOver){
		SGD::OStringStream losemessage;
		losemessage << "You Lost a Life!";
		SGD::GraphicsManager::GetInstance()->DrawString(losemessage.str().c_str(), { (1024 / 2) - 128, 64 }, SGD::Color(255, 0, 0));
	}
	else if (m_endgame.Winning == true){
		SGD::OStringStream losemessage;
		losemessage << "YOU WIN!";
		SGD::GraphicsManager::GetInstance()->DrawString(losemessage.str().c_str(), { (1024 / 2) - 128, 64 }, SGD::Color(0, 255, 0));
	}
	else if (m_endgame.GameOver == true){
		SGD::OStringStream losemessage;
		losemessage << "GAME OVER!";
		SGD::GraphicsManager::GetInstance()->DrawString(losemessage.str().c_str(), { (1024 / 2) - 128, 64 }, SGD::Color(244, 0, 0));
	}
}

#pragma region Enemy Creation Methods
Entity* GameplayState::CreateEnemy(SGD::HTexture img, int type) const{

	if (type == Entity::E_GHAST){
		Ghast* tmp = new Ghast;

		SGD::Point start = SGD::Point(Map::GetInstance()->GetStartingCell().m_cellposX * 32.0f, Map::GetInstance()->GetStartingCell().m_cellposY * 32.0f);

		tmp->SetImage(img);
		tmp->SetPosition(start);
		tmp->SetSize(SGD::Size(32.0f, 32.0f));

		tmp->SetSleepImg(m_EnemySleep);
		tmp->SetAudio(m_PlasmaSfx);
		tmp->LoadEnemyStats();

		return tmp;
	}
	else if (type == Entity::E_SPIRE){
		Spire* tmp = new Spire;

		SGD::Point start = SGD::Point(Map::GetInstance()->GetStartingCell().m_cellposX * 32.0f, Map::GetInstance()->GetStartingCell().m_cellposY * 32.0f);
	
		tmp->SetImage(img);
		tmp->SetPosition(start);
		tmp->SetSize(SGD::Size(32.0f, 32.0f));

		tmp->SetSleepImg(m_EnemySleep);
		tmp->SetAudio(m_PlasmaSfx);
		tmp->LoadEnemyStats();

		return tmp;
	}
	else if (type == Entity::E_UNDEAD){
		UndeadBoxer* tmp = new UndeadBoxer;

		SGD::Point start = SGD::Point(Map::GetInstance()->GetStartingCell().m_cellposX * 32.0f, Map::GetInstance()->GetStartingCell().m_cellposY * 32.0f);

		tmp->SetImage(img);
		tmp->SetPosition(start);
		tmp->SetSize(SGD::Size(32.0f, 32.0f));

		tmp->SetSleepImg(m_EnemySleep);
		tmp->SetAudio(m_PlasmaSfx);
		tmp->LoadEnemyStats();

		return tmp;
	}
	else{
		Goblin* tmp = new Goblin;

		SGD::Point start = SGD::Point(Map::GetInstance()->GetStartingCell().m_cellposX * 32.0f, Map::GetInstance()->GetStartingCell().m_cellposY * 32.0f);

		tmp->SetImage(img);
		tmp->SetPosition(start);
		tmp->SetSize(SGD::Size(32.0f, 32.0f));

		tmp->SetSleepImg(m_EnemySleep);
		tmp->SetAudio(m_PlasmaSfx);
		tmp->LoadEnemyStats();

		return tmp;
	}
}
#pragma endregion

#pragma region Hero Creation Method
Entity* GameplayState::CreateHero(SGD::HTexture img, unsigned int type) const{

	if (type == Entity::P_FIREMAGE){
		FireMage* tmp = new FireMage;

		//	Grid offset
		SGD::Point cursorpos = SGD::InputManager::GetInstance()->GetCursorPosition();
		int x = (int)cursorpos.x % 32;
		int y = (int)cursorpos.y % 32;

		tmp->SetImage(img);
		tmp->SetPosition(SGD::Point(cursorpos.x - x, cursorpos.y - y));
		tmp->LoadHeroStats();

		return tmp;
	}
	else if (type == Entity::P_AQUAGODDESS){
		AquaGoddess* tmp = new AquaGoddess;

		//	Grid offset
		SGD::Point cursorpos = SGD::InputManager::GetInstance()->GetCursorPosition();
		int x = (int)cursorpos.x % 32;
		int y = (int)cursorpos.y % 32;

		tmp->SetImage(img);
		tmp->SetPosition(SGD::Point(cursorpos.x - x, cursorpos.y - y));
		tmp->LoadHeroStats();

		return tmp;
	}
	else if (type == Entity::P_ZEPHYR){
		Zephyr* tmp = new Zephyr;

		//	Grid offset
		SGD::Point cursorpos = SGD::InputManager::GetInstance()->GetCursorPosition();
		int x = (int)cursorpos.x % 32;
		int y = (int)cursorpos.y % 32;

		tmp->SetImage(img);
		tmp->SetPosition(SGD::Point(cursorpos.x - x, cursorpos.y - y));
		tmp->SetSize(SGD::Size(32.0f, 32.0f));

		tmp->SetCostForPurcahse(400);
		tmp->SetUpgrade(0.0f, 0.0f, 0.0f, 450.0f);

		return tmp;
	}
	else{
		Entity* tmp = new Entity;
		return tmp;
	}
}
#pragma endregion

#pragma region Projectile Creation Methods
Entity*	GameplayState:: CreatePlasma(SGD::Point pos, SGD::Vector _toenemy)	const{

	Plasma* plasma = new Plasma;

	plasma->SetImage(m_Plasma);
	plasma->SetPosition({ pos.x + 30, pos.y - 5 });
	plasma->SetSize(SGD::Size(32.0f, 32.0f));
	plasma->SetType(Entity::ENT_PLASMA);

	plasma->SetPlasmaHitSfx(m_PlasmaSfx);

	_toenemy *= 10;

	plasma->SetVelocity(_toenemy);

	return plasma;
}
Entity*	GameplayState::CreateFireBall(SGD::Point pos, SGD::Vector _toenemy, Entity* caster)	const{

	Plasma* fireball = new Plasma;

	fireball->SetImage(m_Fireball);
	fireball->SetPosition({ pos.x, pos.y });
	fireball->SetSize(SGD::Size(32.0f, 32.0f));

	fireball->SetType(Entity::ENT_FIREBALL);
	fireball->SetCastor(caster);
	fireball->SetPlasmaHitSfx(m_PlasmaSfx);
	fireball->SetFireBallDamage(dynamic_cast<FireMage*>(fireball->GetCastor())->GetFireballDamage());

	_toenemy *= 10;
	fireball->SetVelocity(_toenemy);

	return fireball;
}

Entity*	GameplayState::CreateWindShuriken(SGD::Point pos, SGD::Vector _toenemy, Entity* caster)	const{
	Plasma* WindShuri = new Plasma;

	WindShuri->SetImage(m_Windshuri);
	WindShuri->SetPosition({ pos.x, pos.y });
	WindShuri->SetSize(SGD::Size(32.0f, 32.0f));
	WindShuri->SetType(Entity::ENT_WINDSTYLEBLAST);

	WindShuri->SetPlasmaHitSfx(m_PlasmaSfx);

	_toenemy *= 10;
	WindShuri->SetVelocity(_toenemy);

	return WindShuri;
}
#pragma endregion

#pragma region Message System
void GameplayState::MessageProc(const SGD::Message* pMsg) {

	/* Show warning when a Message ID enumerator is not handled */
#pragma warning( push )
#pragma warning( 1 : 4061 )

	// What type of message?
	switch (pMsg->GetMessageID()) {

	case MessageID::MSG_CREATE_ENEMY: {

		const CreateEnemyMessage* pCreateMsg = dynamic_cast<const CreateEnemyMessage*>(pMsg);

		assert(pCreateMsg != nullptr
			&& "GameplayState::MessageProc - MSG_CREATE_LASER is not actually a CreateEnemyMessage");

		GameplayState* self = GameplayState::GetInstance();

		Entity* enemy = self->CreateEnemy(self->m_hCharacters, pCreateMsg->GetType());

		self->m_pEntities->AddEntity(enemy, 0);

		enemy->Release();
		enemy = nullptr;
	}
		break;

	case MessageID::MSG_CREATE_PLASMA: {

		const CreatePlasmaMessage* pCreateMsg = dynamic_cast<const CreatePlasmaMessage*>(pMsg);

		assert(pCreateMsg != nullptr
			&& "GameplayState::MessageProc - MSG_CREATE_LASER is not actually a CreatePlasmaMessage");

		GameplayState* self = GameplayState::GetInstance();
		
		Entity* plasma = self->CreatePlasma(Tower::GetInstance()->GetPosition(), pCreateMsg->GetToEnemy());

		self->m_pEntities->AddEntity(plasma, 1);

		plasma->Release();
		plasma = nullptr;
	}
		break;

	case MessageID::MSG_CREATE_WIND: {

		const CreateWindStyleShurikenMessage* pCreateMsg = dynamic_cast<const CreateWindStyleShurikenMessage*>(pMsg);

		assert(pCreateMsg != nullptr
			&& "GameplayState::MessageProc - MSG_CREATE_FIREBALL is not actually a CreateFireballMessage");

		GameplayState* self = GameplayState::GetInstance();

		Entity* windshuri = self->CreateWindShuriken(pCreateMsg->GetPlayerPos(), pCreateMsg->GetToEnemy(), pCreateMsg->GetCaster());

		self->m_pEntities->AddEntity(windshuri, 1);

		windshuri->Release();
		windshuri = nullptr;
	}
		break;

	case MessageID::MSG_CREATE_FIREBALL: {

		const CreateFireBallMessage* pCreateMsg = dynamic_cast<const CreateFireBallMessage*>(pMsg);

		assert(pCreateMsg != nullptr
			&& "GameplayState::MessageProc - MSG_CREATE_FIREBALL is not actually a CreateFireballMessage");

		GameplayState* self = GameplayState::GetInstance();

		Entity* fireball = self->CreateFireBall( pCreateMsg->GetPlayerPos() ,pCreateMsg->GetToEnemy(), pCreateMsg->GetCaster());

		self->m_pEntities->AddEntity(fireball, 1);

		fireball->Release();
		fireball = nullptr;
	}
		break;
	case MessageID::MSG_DESTROY_ENTITY: {

		const DestroyEntityMessage* pDestroyMsg =
			dynamic_cast<const DestroyEntityMessage*>(pMsg);

		assert(pDestroyMsg != nullptr
			&& "GameplayState::MessageProc - MSG_DESTROY_ENTITY is not actually a DestroyEntityMessage");

		Entity* ptr = pDestroyMsg->GetEntity();

			// Use the singleton to access members!
		GameplayState::GetInstance()->m_pEntities->RemoveEntity(ptr);

		}
	break;
	}
	/* Restore previous warning levels */
#pragma warning( pop )
}
#pragma endregion