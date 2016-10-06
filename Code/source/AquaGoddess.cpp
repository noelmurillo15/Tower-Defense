/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "AquaGoddess.h"

#include "Enemies.h"
#include "Tower.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"


AquaGoddess::~AquaGoddess(void) {

}

void AquaGoddess::LoadHeroStats(){

	this->SetSize(SGD::Size(32.0f, 32.0f));
	this->SetCostForPurcahse(250);
	this->SetUpgrade(0.0f, 0.0f, 0.0f, 300.0f);
}

void AquaGoddess::Update(float elapsedTime){

		//	if attack timer is > 0 update it
	if (this->GetAttacktimer() > 0.0f)
		this->SetAttacktimer(this->GetAttacktimer() - elapsedTime);
	else
		this->SetFreezeCooldown(this->GetFreezeCooldown() - elapsedTime);

		//	if attack timer is off and not on cool down.. set cooldown
	if (this->GetAttacktimer() <= 0.0f && !IsOnCooldown){
		this->SetFreezeCooldown(5.0f);
		IsOnCooldown = true;
	}
		//	If my target is already dead, set target to nullptr
	if (this->GetTarget() != nullptr && dynamic_cast<Enemies*>(GetTarget())->GetIsDead())
		this->SetTarget(nullptr);

		//	If i have a target
	else if (this->GetTarget() != nullptr){
			//	If target is out of my reach.. find a new taregt
		if (!(this->GetTarget()->GetRect().IsIntersecting(this->GetRect())))
			this->SetTarget(nullptr);
	}


	SGD::Point cursor = SGD::InputManager::GetInstance()->GetCursorPosition();
	SGD::Rectangle cursorrect = { cursor.x, cursor.y, cursor.x + 1, cursor.y + 1 };

	if (cursorrect.IsIntersecting(this->GetUpgradeRect()) && SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::MouseLeft) == true){
		if (Tower::GetInstance()->GetGold() - this->GetUpgrade().UpgradeCost >= 0.0f){
			if (this->GetUpgradeLevel() < 2){

				this->NextUpgrade();
				Tower::GetInstance()->SetGold(Tower::GetInstance()->GetGold() - this->GetUpgrade().UpgradeCost);
				this->SetUpgrade(16.0f * this->GetUpgradeLevel(),
					10.0f * this->GetUpgradeLevel(),
					0.45f,
					125.0f * this->GetUpgradeLevel());
			}
		}
	}

}
void AquaGoddess::Render(){

	SGD::Point cursor = SGD::InputManager::GetInstance()->GetCursorPosition();
	SGD::Rectangle cursorrect = { cursor.x, cursor.y, cursor.x + 1, cursor.y + 1 };

	if (cursorrect.IsIntersecting(this->GetUpgradeRect()))
		SGD::GraphicsManager::GetInstance()->DrawRectangle(this->GetRect(), SGD::Color(30, 0, 80, 155));

	SGD::GraphicsManager::GetInstance()->DrawTextureSection(
		m_hImage,
		this->GetPosition(),
		{ 0.0f, 128.0f, 32.0f, 160.0f },
		0.0f,
		{},
		SGD::Color(255, 255, 255),
		SGD::Size(0.85f, 0.85f));
}

SGD::Rectangle AquaGoddess::GetRect() const{

	float range = const_cast<AquaGoddess*>(this)->GetUpgrade().IncreasedRange;

	return SGD::Rectangle{
		this->GetPosition().x - 48 - range,
		this->GetPosition().y - 48 - range,
		this->GetPosition().x + 48 + range + this->GetSize().width,
		this->GetPosition().y + 48 + range + this->GetSize().height
	};
}
SGD::Rectangle	AquaGoddess::GetUpgradeRect() {

	return SGD::Rectangle{
		this->GetPosition().x,
		this->GetPosition().y,
		this->GetPosition().x + this->GetSize().width,
		this->GetPosition().y + this->GetSize().height
	};
}

void AquaGoddess::HandleCollision(const IEntity* pOther){

	// What is in range
	if (this->GetTarget() == nullptr){
		if (pOther->GetType() == E_GHAST
			|| pOther->GetType() == E_SPIRE
			|| pOther->GetType() == E_UNDEAD
			|| pOther->GetType() == E_GOBLIN) {

		if (this->GetFreezeCooldown() <= 0.0f && IsOnCooldown){
			this->SetAttacktimer(0.85f + this->GetUpgrade().IncreasedAttackRate);
			IsOnCooldown = false;
		}

			IEntity* tmp = const_cast<IEntity*>(pOther);
			if (dynamic_cast<Enemies*>(tmp)->GetIsDead() == false && !dynamic_cast<Enemies*>(tmp)->GetIsSlowed())
				this->SetTarget(dynamic_cast<Entity*>(tmp));
		}
	}
}