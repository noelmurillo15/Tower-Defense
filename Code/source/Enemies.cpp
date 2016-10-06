/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "Enemies.h"

#include "Map.h"
#include "Tower.h"
#include "Plasma.h"
#include "FireMage.h"
#include "AquaGoddess.h"
#include "CellAnimation.h"
#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_AudioManager.h"


Enemies::~Enemies(void){

#pragma region Destroy Animations
		//	Delete Animations
	if (m_PlasmaBlast != nullptr)
	{
		m_PlasmaBlast->Terminate();
		delete m_PlasmaBlast;
		m_PlasmaBlast = nullptr;
	}
	if (m_FireXplosion != nullptr)
	{
		m_FireXplosion->Terminate();
		delete m_FireXplosion;
		m_FireXplosion = nullptr;
	}
	if (m_AquaPrison != nullptr)
	{
		m_AquaPrison->Terminate();
		delete m_AquaPrison;
		m_AquaPrison = nullptr;
	}
	if (m_64PalmRotation != nullptr)
	{
		m_64PalmRotation->Terminate();
		delete m_64PalmRotation;
		m_64PalmRotation = nullptr;
	}
#pragma endregion

		//	Sfx
	if (m_Collisionsfx != SGD::INVALID_HANDLE)
		m_Collisionsfx = SGD::INVALID_HANDLE;
	else if (m_sleepimg != SGD::INVALID_HANDLE)
		m_sleepimg = SGD::INVALID_HANDLE;

		//	Add Score, Update num of enemies Alive
	Tower::GetInstance()->SetNumSpawnsAlive(Tower::GetInstance()->GetNumSpawnsAlive() - 1);
	Tower::GetInstance()->SetScore(2);

	if (this->GetType() == E_UNDEAD)
		Tower::GetInstance()->SetGold(Tower::GetInstance()->GetGold() + this->GetGoldForKill());
	else if (this->GetType() == E_GOBLIN)
		Tower::GetInstance()->SetGold(Tower::GetInstance()->GetGold() + this->GetGoldForKill());
	
}

void Enemies::LoadEnemyStats(){

		//	Starting Direction
	this->SetDirection(2);
	SGD::Vector newVel = SGD::Vector(1, 0);

#pragma region Multiplier

	unsigned int multiplier;
	if (this->GetType() == E_GHAST){	
		if (Tower::GetInstance()->GetWaveNum() < 3){
			multiplier = (Tower::GetInstance()->GetWaveNum());
		}
		else {
			multiplier = 3;
		}
		newVel *= (float)(60 + (10 * multiplier));

		this->SetVelocity(newVel);	//	max 90
		this->SetHealth(5.0f * multiplier); // max 15
		this->SetGoldForKill((unsigned int)(2 * multiplier));	//	max 6

	}
	else if (this->GetType() == E_SPIRE){
		if (Tower::GetInstance()->GetWaveNum() < 5)
			multiplier = (Tower::GetInstance()->GetWaveNum() - 2);
		else 
			multiplier = 3;
		
		newVel *= (float)(100 + (50 * multiplier));

		this->SetVelocity(newVel);	//	max 250
		this->SetHealth(5.0f * multiplier); // max 15
		this->SetGoldForKill((unsigned int)(5 * multiplier));	//	max 15
	}
	else if (this->GetType() == E_UNDEAD){
		if (Tower::GetInstance()->GetWaveNum() < 8)
			multiplier = 1/*(Tower::GetInstance()->GetWaveNum() - 5)*/;		
		else 
			multiplier = 3;
		
		newVel *= (float)(60 + (10 * multiplier));

		this->SetVelocity(newVel);	//	max 90
		this->SetHealth(20.0f * multiplier); // max 60
		this->SetGoldForKill((unsigned int)(10 * multiplier));	//	max 30
	}
	else if (this->GetType() == E_GOBLIN){
		if (Tower::GetInstance()->GetWaveNum() < 10){
			if (Tower::GetInstance()->GetWaveNum() - 7 <= 0)
			multiplier = (Tower::GetInstance()->GetWaveNum() - 7);
			else 
				multiplier = 3;
		}
		else {
			multiplier = 3;
		}
		newVel *= (float)(80 + (20 * multiplier));

		this->SetVelocity(newVel);	//	max 120
		this->SetHealth(50.0f * multiplier); // max 100
		this->SetGoldForKill((unsigned int)(25 * multiplier));	//	max 50
	}
#pragma endregion
}

void Enemies::Update(float elapsedTime){

	Entity::Update(elapsedTime);

#pragma region Animation Updates

	//	Plasma blast anim update
	if (m_PlasmaBlast != nullptr && m_PlasmaBlast->IsPlaying() == true) {

		m_PlasmaBlast->Update(elapsedTime);

		if (m_PlasmaBlast->IsFinished() == true) {
			this->SetHealth(this->GetHealth() - Tower::GetInstance()->GetPlasmaDamage());

			//	if enemy hp reaches 0.. murder them and their babies
			if (this->GetHealth() <= 0.0f){
				this->SetIsDead(true);

				Tower::GetInstance()->SetGold(Tower::GetInstance()->GetGold() + this->GetGoldForKill());

				DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
				pMsg->QueueMessage();
				pMsg = nullptr;
			}
			m_PlasmaBlast->Terminate();
			delete m_PlasmaBlast;
			m_PlasmaBlast = nullptr;
		}
	}	//	Fire blast anim update
	else if (m_FireXplosion != nullptr && m_FireXplosion->IsPlaying() == true) {

		m_FireXplosion->Update(elapsedTime);

		if (m_FireXplosion->IsFinished() == true) {
			//	if enemy hp reaches 0.. murder them and their babies
			if (this->GetHealth() <= 0.0f){
				this->SetIsDead(true);

				Tower::GetInstance()->SetGold(Tower::GetInstance()->GetGold() + this->GetGoldForKill());
				DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
				pMsg->QueueMessage();
				pMsg = nullptr;
			}
			m_FireXplosion->Terminate();
			delete m_FireXplosion;
			m_FireXplosion = nullptr;
		}
	}	//	Aqua prison anim update
	else if (m_AquaPrison != nullptr && m_AquaPrison->IsPlaying() == true) {

		this->SetHealth(this->GetHealth() - elapsedTime);

		m_AquaPrison->Update(elapsedTime);
		if (m_AquaPrison->IsFinished() == true){
			if (this->GetHealth() <= 0.0f){

				this->SetIsDead(true);
				Tower::GetInstance()->SetGold(Tower::GetInstance()->GetGold() + this->GetGoldForKill());
				DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
				pMsg->QueueMessage();
				pMsg = nullptr;
			}
			this->SetThawTimer(4.0f);
			m_AquaPrison->Terminate();
			delete m_AquaPrison;
			m_AquaPrison = nullptr;
		}
	}
	else if (m_64PalmRotation != nullptr && m_64PalmRotation->IsPlaying() == true) {

		this->SetHealth(this->GetHealth() - elapsedTime);
		this->SetVelocity(SGD::Vector(0, 0));

		m_64PalmRotation->Update(elapsedTime);
		if (m_64PalmRotation->IsFinished() == true){
			if (this->GetHealth() <= 0.0f){

				this->SetIsDead(true);
				Tower::GetInstance()->SetGold(Tower::GetInstance()->GetGold() + this->GetGoldForKill());
				DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
				pMsg->QueueMessage();
				pMsg = nullptr;
			}
			m_64PalmRotation->Terminate();
			delete m_64PalmRotation;
			m_64PalmRotation = nullptr;
		}
	}

#pragma endregion

#pragma region Grid Movement

		//	Grid[row][col]
	int col = (int)(this->GetPosition().x) / 32;
	int row = (int)(this->GetPosition().y) / 32;

		//	black magic
	if (this->GetDirection() == 4){
		col++;
	}
	else if (this->GetDirection() == 3){
		row++;
	}

	if (Map::GetInstance()->GetGridDir(row, col) != 5 || Map::GetInstance()->GetGridDir(row, col) != 0){

		unsigned int multiplier;
		if (Tower::GetInstance()->GetWaveNum() <= 6){
			multiplier = (Tower::GetInstance()->GetWaveNum());
		}
		else {
			multiplier = 6;
		}

		// right
		if (Map::GetInstance()->GetGridDir(row, col) == 2 && this->GetDirection() != 2){
			this->SetDirection(2);
			SGD::Vector newVel = SGD::Vector(1, 0);
			if (this->GetType() == E_GHAST)
				newVel *= (float)(60 + (10 * multiplier));
			else
				newVel *= (float)(120 + (10 * multiplier));

			this->SetVelocity(newVel);
		}	//	down
		else if (Map::GetInstance()->GetGridDir(row, col) == 1 && this->GetDirection() != 1){
			this->SetDirection(1);
			SGD::Vector newVel = SGD::Vector(0, 1);
			if (this->GetType() == E_GHAST)
				newVel *= (float)(60 + (10 * multiplier));
			else
				newVel *= (float)(120 + (10 * multiplier));

			this->SetVelocity(newVel);
		}	//	left
		else if (Map::GetInstance()->GetGridDir(row, col) == 4 && this->GetDirection() != 4){
			this->SetDirection(4);
			SGD::Vector newVel = SGD::Vector(-1, 0);
			if (this->GetType() == E_GHAST)
				newVel *= (float)(60 + (10 * multiplier));
			else
				newVel *= (float)(120 + (10 * multiplier));

			this->SetVelocity(newVel);
		}	//	up
		else if (Map::GetInstance()->GetGridDir(row, col) == 3 && this->GetDirection() != 3){
			this->SetDirection(3);
			SGD::Vector newVel = SGD::Vector(0, -1);
			if (this->GetType() == E_GHAST)
				newVel *= (float)(60 + (10 * multiplier));
			else
				newVel *= (float)(120 + (10 * multiplier));

			this->SetVelocity(newVel);
		}
	}
#pragma endregion
}

void Enemies::Render(void){

#pragma region Render Animations
		//	play animations
	if (m_PlasmaBlast != nullptr && m_PlasmaBlast->IsPlaying() == true) {

		SGD::Point blastPos = SGD::Point(m_ptPosition.x - 16, m_ptPosition.y - 32);
		m_PlasmaBlast->Render(blastPos, 0.5f, { 255, 255, 255, 255 });
	}
	else if (m_FireXplosion != nullptr && m_FireXplosion->IsPlaying() == true) {

		SGD::Point blastPos = SGD::Point(m_ptPosition.x - 16, m_ptPosition.y - 32);
		m_FireXplosion->Render(blastPos, 0.5f, { 255, 255, 255, 255 });
	}
	else if (m_AquaPrison != nullptr && m_AquaPrison->IsPlaying() == true) {

		SGD::Point prisonpos = SGD::Point(m_ptPosition.x - 16, m_ptPosition.y - 32);
		m_AquaPrison->Render(prisonpos, 0.5f, { 255, 255, 255, 255 });
	}
	else if (m_64PalmRotation != nullptr && m_64PalmRotation->IsPlaying() == true) {

		SGD::Point prisonpos = SGD::Point(m_ptPosition.x - 16, m_ptPosition.y);
		m_64PalmRotation->Render(prisonpos, 0.5f, { 255, 255, 255, 255 });
	}
#pragma endregion
}

SGD::Rectangle Enemies::GetRect(void) const{
	return Entity::GetRect();
}

void Enemies::HandleCollision(const IEntity* pOther){

#pragma region Start Animation on Collision
		// What Hit me?
	if (pOther->GetType() == ENT_PLASMA) {
		this->SetVelocity(SGD::Vector(0, 0));
		if (m_PlasmaBlast == nullptr) {

			m_PlasmaBlast = new CellAnimation;
			m_PlasmaBlast->Initialize(ENT_PLASMA);
			m_PlasmaBlast->Restart(1.0f, false);
		}
	}
	else if (pOther->GetType() == ENT_FIREBALL){

		IEntity* tmp = const_cast<IEntity*>(pOther);
		Plasma* blast = dynamic_cast<Plasma*>(tmp);

		this->SetHealth(this->GetHealth() - blast->GetFireBallDamage());
		if (m_FireXplosion == nullptr) {

			m_FireXplosion = new CellAnimation;
			m_FireXplosion->Initialize(ENT_FIREBALL);
			m_FireXplosion->Restart(1.0f, false);
		}
	}
	else if (pOther->GetType() == P_AQUAGODDESS){

		IEntity* tmp = const_cast<IEntity*>(pOther);
		AquaGoddess* player = dynamic_cast<AquaGoddess*>(tmp);

		if (player->GetFreezeCooldown() <= 0 && !this->GetIsSlowed()){

			if (m_AquaPrison == nullptr && this->GetIsSlowed() == false) {

				this->SetVelocity(this->GetVelocity() / 2);

				m_AquaPrison = new CellAnimation;
				m_AquaPrison->Initialize(ENT_AQUABOUND);
				m_AquaPrison->Restart(1.0f, false);

				this->SetIsSlowed(true);
				this->SetThawTimer(4.0f);
			}
		}
	}
	else if (pOther->GetType() == ENT_WINDSTYLEBLAST){

		IEntity* tmp = const_cast<IEntity*>(pOther);
		Plasma* blast = dynamic_cast<Plasma*>(tmp);

		//this->SetHealth(this->GetHealth() - blast->GetWindBlastDamage());
		this->SetIsAsleep(true);
		if (m_64PalmRotation == nullptr) {

			m_64PalmRotation = new CellAnimation;
			m_64PalmRotation->Initialize(ENT_WINDSTYLEBLAST);
			m_64PalmRotation->Restart(1.0f, false);
		}
	}
#pragma endregion

	if (pOther->GetType() == ENT_BASE){

		this->SetIsDead(true);
		if (Tower::GetInstance()->GetHealth() > 0.0f)
			Tower::GetInstance()->SetHealth(Tower::GetInstance()->GetHealth() - 1);

		// Play audio sfx
		if (m_Collisionsfx != SGD::INVALID_HANDLE)
			SGD::AudioManager::GetInstance()->PlayAudio(m_Collisionsfx);

		DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;
	}
}