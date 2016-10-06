/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "Tower.h"

#include "Game.h"
#include "Enemies.h"
#include "CreatePlasmaMessage.h"
#include "BitmapFont.h"
#include "LevelselectionState.h"

#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"


Tower* Tower::GetInstance(){
	static Tower m_Instance;
	return &m_Instance;
}

void Tower::Load() {

		//	Easy
	if (LevelselectionState::GetInstance()->GetLevel() == 1){
		SetHealth(250);
		SetGold(400);
	}	//	Medium
	else if (LevelselectionState::GetInstance()->GetLevel() == 2){
		SetHealth(200);
		SetGold(350);
	}	//	Hard
	else if (LevelselectionState::GetInstance()->GetLevel() == 3){
		SetHealth(1);
		SetGold(300);
	}

	m_Upgrade.IncreasedDamage = 0;
	m_Upgrade.IncreasedAttackSpeed = 0;
	m_Upgrade.UpgradeCost = 300;

	SetLives(3);
	ResetScore();
	SetWaveIncoming(false);
	SetPlasmaDamage(200.0f);

	m_cooldownRect = {150, 5, 350, 20};
	m_cooldowntimerRect = { 150, 5, 350, 20 };
}

void Tower::Reset(){
	if (LevelselectionState::GetInstance()->GetLevel() == 1)
		SetHealth(250);

	else if (LevelselectionState::GetInstance()->GetLevel() == 2)
		SetHealth(200);

	else if (LevelselectionState::GetInstance()->GetLevel() == 3)
		SetHealth(1);

	SetNumSpawns(0);
	SetNumSpawnsAlive(0);
	SetWaveIncoming(false);
	WaveNum -= 1;

	SetScore(0);

}

void Tower::Unload(){
	m_Upgrade.IncreasedDamage = 0;
	m_Upgrade.IncreasedAttackSpeed = 0;
	m_Upgrade.UpgradeCost = 300;

	SetLives(3);
	SetScore(0);
	SetWaveIncoming(false);
	SetPlasmaDamage(200.0f);

	SetScore(0);
	SetNumSpawns(0);
	SetNumSpawnsAlive(0);
	SetWaveIncoming(false);
	WaveNum = 0;
}

void Tower::Update(float elapsedTime){

	SGD::Point cursor = SGD::InputManager::GetInstance()->GetCursorPosition();
	SGD::Rectangle cursorrect = { cursor.x, cursor.y, cursor.x + 1, cursor.y + 1 };
	SGD::Rectangle killarea = { 0, 0, 832, 768 };

		//	Find and set offset to mouse pos
	int xoffset = (int)cursor.x % 32;
	int yoffset = (int)cursor.y % 32;
	cursor.x = cursor.x - xoffset;
	cursor.y = cursor.y - yoffset;
	cursor.x = cursor.x - 32;

	m_cooldowntimerRect = { 150, 5, 155 + (CoolDownTimer * 2.0f), 20 };

	

	if (attacktimer > 0.0f)
		attacktimer -= elapsedTime;
	if (CoolDownTimer > 0.0f && !WeaponExhausted)
		CoolDownTimer -= (elapsedTime * 50);
	else if (CoolDownTimer > 0.0f && WeaponExhausted)
		CoolDownTimer -= (elapsedTime * 10);

	if (CoolDownTimer <= 0.0f && WeaponExhausted){
		WeaponExhausted = false;
	}

	

	if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::MouseRight) == true && !WeaponExhausted){

		if (CoolDownTimer < 100.0f)
			CoolDownTimer += 0.25;
		else{
			CoolDownTimer = 100.0f;
			WeaponExhausted = true;
		}
		if (attacktimer <= 0.0f && cursorrect.IsIntersecting(killarea) == true){
			SGD::Vector toTarget = cursor - this->GetPosition();

			//	Create Plasma Shot
			CreatePlasmaMessage* pMsg = new CreatePlasmaMessage{ 10, toTarget };
			pMsg->QueueMessage();
			pMsg = nullptr;

			//	Shoot every 1 second
			attacktimer = 1.0f - m_Upgrade.IncreasedAttackSpeed;
		}			
	}

	if (cursorrect.IsIntersecting(this->GetRect()) && SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::MouseLeft) == true){
		if (GetGold() - (m_Upgrade.UpgradeCost + (50 * m_UpgradeLevel)) >= 0.0f){
			if (m_UpgradeLevel < 4){

				this->SetGold(GetGold() - m_Upgrade.UpgradeCost);

				m_UpgradeLevel++;
				m_Upgrade.IncreasedDamage += (25 * m_UpgradeLevel);
				m_Upgrade.IncreasedAttackSpeed += 0.20f;
				m_Upgrade.UpgradeCost += (50 * m_UpgradeLevel);

				SetPlasmaDamage(GetPlasmaDamage() + m_Upgrade.IncreasedDamage);
			}
		}
	}
}

void Tower::Render(){
	const BitmapFont* pFont = Game::GetInstance()->GetFont();
	SGD::GraphicsManager* graphics = SGD::GraphicsManager::GetInstance();

	SGD::OStringStream cooldown;

	cooldown << " Cooldown: ";
	pFont->Draw(cooldown.str().c_str(), { 0, 5 }, 0.5f, SGD::Color(0, 0, 0));

	graphics->DrawRectangle(m_cooldownRect, SGD::Color(0, 0, 0, 0), SGD::Color(0, 0, 0), 2);
	if (!WeaponExhausted)
		graphics->DrawRectangle(m_cooldowntimerRect, SGD::Color(150, 0, 0, 255), SGD::Color(0, 0, 0, 0));
	else
		graphics->DrawRectangle(m_cooldowntimerRect, SGD::Color(150, 255, 0, 0), SGD::Color(0, 0, 0, 0));

		//	Draw the Tower
	graphics->DrawTexture(
		m_hImage,
		this->GetPosition(),
		0.0f,
		{},
		SGD::Color(255,255,255),
		SGD::Size(0.75f, 0.75f));
}

SGD::Rectangle Tower::GetRect() const{

	SGD::Rectangle tmp = {
		this->GetPosition().x + 20,
		this->GetPosition().y + 64,
		this->GetPosition().x + (this->GetSize().width * 0.75f) - 20,
		this->GetPosition().y +(this->GetSize().height * 0.75f)
	};

	return tmp;
}

void Tower::HandleCollision(const IEntity* pOther){

}