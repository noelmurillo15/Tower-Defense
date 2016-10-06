/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "Heroes.h"

Heroes::~Heroes(void){
	if (this->GetTarget() != nullptr)
		SetTarget(nullptr);
}

void Heroes::Update(float elapsedTime){
		
}

void Heroes::Render(void){

}

SGD::Rectangle Heroes::GetRect(void) const{
	SGD::Rectangle tmp = { 0, 0, 0, 0 };

	return tmp;
}

void Heroes::HandleCollision(const IEntity* pOther){

}

void Heroes::SetTarget(Entity* ptr) {

		// Release the existing focus
	if (p_target != nullptr)
		p_target->Release();

		 // Store the parameter
	p_target = ptr;

		// Add a reference to the new focus
	if (p_target != nullptr)
		p_target->AddRef();
}

void Heroes::SetUpgrade(float range, float dmg, float speed, float cost){

	m_Upgrade.IncreasedDamage += dmg;
	m_Upgrade.IncreasedRange += range;
	m_Upgrade.IncreasedAttackRate += speed;
	m_Upgrade.UpgradeCost += cost;
}