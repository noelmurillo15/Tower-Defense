/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "Zephyr.h"

#include "Enemies.h"
#include "CreateWindStyleShurikenMessage.h"
#include "Tower.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"


Zephyr::~Zephyr(void) {

}

void Zephyr::Update(float elapsedTime){

		//	update attack timer
	if (this->GetAttacktimer() > 0.0f)
		this->SetAttacktimer(this->GetAttacktimer() - elapsedTime);

		//	if i have a target
	if (this->GetTarget() != nullptr){
			//	 but hes already dead or out of range.. get a new target
		if (dynamic_cast<Enemies*>(GetTarget())->GetIsDead() || !(this->GetTarget()->GetRect().IsIntersecting(this->GetRect())))
			this->SetTarget(nullptr);
	}
		//	 if i have a target and i can attack
	if (this->GetTarget() != nullptr && this->GetAttacktimer() <= 0.0f && dynamic_cast<Enemies*>(this->GetTarget())->GetIsAsleep() == false){

			//	Target pos
		SGD::Point newpos = SGD::Point(this->GetTarget()->GetPosition().x - this->GetTarget()->GetSize().width / 2,
			this->GetTarget()->GetPosition().y + this->GetTarget()->GetSize().height / 2);

		SGD::Vector toTarget = newpos - this->GetPosition();

			//	Create Fire ball
		CreateWindStyleShurikenMessage* pMsg = new CreateWindStyleShurikenMessage{ this->GetPosition(), toTarget, this };
		pMsg->QueueMessage();
		pMsg = nullptr;

			//	Shoot every 1.75 second
		this->SetAttacktimer(3.0F - this->GetUpgrade().IncreasedAttackRate);
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
void Zephyr::Render(){

	SGD::Point cursor = SGD::InputManager::GetInstance()->GetCursorPosition();
	SGD::Rectangle cursorrect = { cursor.x, cursor.y, cursor.x + 1, cursor.y + 1 };

	if (cursorrect.IsIntersecting(this->GetUpgradeRect()))
		SGD::GraphicsManager::GetInstance()->DrawRectangle(this->GetRect(), SGD::Color(50, 0, 0, 255));

	SGD::GraphicsManager::GetInstance()->DrawTextureSection(
		m_hImage,
		this->GetPosition(),
		{ 0.0f, 224.0f, 32.0f, 256.0f },
		0.0f,
		{},
		SGD::Color(255, 255, 255),
		SGD::Size(0.85f, 0.85f));
}

SGD::Rectangle Zephyr::GetRect() const{

	float range = const_cast<Zephyr*>(this)->GetUpgrade().IncreasedRange;

	return SGD::Rectangle{
		this->GetPosition().x - 48 - range,
		this->GetPosition().y - 48 - range,
		this->GetPosition().x + 48 + range + this->GetSize().width,
		this->GetPosition().y + 48 + range + this->GetSize().height
	};
}

SGD::Rectangle Zephyr::GetUpgradeRect(){
	return SGD::Rectangle{
		this->GetPosition().x,
		this->GetPosition().y,
		this->GetPosition().x + this->GetSize().width,
		this->GetPosition().y + this->GetSize().height
	};
}

void Zephyr::HandleCollision(const IEntity* pOther){

	// What is in range
	if (this->GetTarget() == nullptr){
		if (pOther->GetType() == E_GHAST
			|| pOther->GetType() == E_SPIRE
			|| pOther->GetType() == E_UNDEAD
			|| pOther->GetType() == E_GOBLIN) {

			IEntity* tmp = const_cast<IEntity*>(pOther);
			if (dynamic_cast<Enemies*>(tmp)->GetIsDead() == false )
				this->SetTarget(dynamic_cast<Entity*>(tmp));
		}
	}
}


