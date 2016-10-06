/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "Goblin.h"

#include "Tower.h"
#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"


Goblin::~Goblin(void) {

}

void Goblin::Update(float elapsedTime){

	Enemies::Update(elapsedTime);

	//	if enemy has a thaw timer.. update it
	if (this->GetThawTimer() > 0.0f)
		this->SetThawTimer(this->GetThawTimer() - elapsedTime);
	//	if enemy is already thawed.. normalize their speed
	else if (this->GetThawTimer() <= 0.0f && this->GetIsSlowed()){
		this->SetVelocity(this->GetVelocity() * 2);
		this->SetIsSlowed(false);
	}
}
void Goblin::Render(){

	Enemies::Render();

	if (this->GetIsAsleep()){
		SGD::GraphicsManager::GetInstance()->DrawTexture(
			this->GetAsleepImg(),
			SGD::Point(this->GetPosition().x - 16, this->GetPosition().y - 16),
			0.0f,
			{},
			SGD::Color(255, 255, 255));
	}

	SGD::GraphicsManager::GetInstance()->DrawTextureSection(
		m_hImage,
		this->GetPosition(),
		{ ((float)(this->GetDirection()) * 96.0f) - 32.0f, 64.0f, ((float)(this->GetDirection()) * 96.0f), 96.0f },
		0.0f,
		{},
		SGD::Color(255, 255, 255),
		SGD::Size(0.85f, 0.85f));
}

SGD::Rectangle	Goblin::GetRect() const{

	return Enemies::GetRect();
}

void Goblin::HandleCollision(const IEntity* pOther){

	Enemies::HandleCollision(pOther);
}