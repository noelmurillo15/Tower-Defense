/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "Plasma.h"

#include "Game.h"
#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"


Plasma::~Plasma(){
	if (this->GetCastor() != nullptr)
		this->SetCastor(nullptr);
}

void Plasma::Update(float elapsedTime) {

		// Call the parent's Update to actually move
	Entity::Update(elapsedTime);

		// Window size
	SGD::Rectangle rSelf = this->GetRect();
	SGD::Rectangle rScreen = {
		0, 0,
		Game::GetInstance()->GetScreenWidth(),
		Game::GetInstance()->GetScreenHeight()
	};
		//	If Plasma goes out of window
	if (rSelf.IsIntersecting(rScreen) == false) {

		DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;
	}
}

void Plasma::Render(){

	Entity::Render();
}

void Plasma::HandleCollision(const IEntity* pOther) {

		// What did I hit?
	if (pOther->GetType() == E_GHAST || pOther->GetType() == E_UNDEAD || pOther->GetType() == E_GOBLIN) {

			// Play audio sfx
		if (GetPlasmaHitSfx() != SGD::INVALID_HANDLE)
			SGD::AudioManager::GetInstance()->PlayAudio(GetPlasmaHitSfx());

			//	Destroy Plasma
		DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;
	}
}

SGD::Rectangle Plasma::GetRect()const{

	if (this->GetType() == ENT_FIREBALL){

		return SGD::Rectangle{
			this->GetPosition().x + this->GetSize().width / 2 - 6,
			this->GetPosition().y + this->GetSize().height / 2 - 6,
			this->GetPosition().x + this->GetSize().width / 2 + 6,
			this->GetPosition().y + this->GetSize().height / 2 + 6
		};
	}
	else if (this->GetType() == ENT_PLASMA){
		return SGD::Rectangle {
			this->GetPosition().x + this->GetSize().width / 2 - 2,
			this->GetPosition().y + this->GetSize().height / 2 - 2,
			this->GetPosition().x + this->GetSize().width / 2 + 2,
			this->GetPosition().y + this->GetSize().height / 2 + 2
		};
	}
	else /*if (this->GetType() == ENT_WINDSTYLEBLAST)*/{
		return SGD::Rectangle{
			this->GetPosition().x + this->GetSize().width / 2 - 5,
			this->GetPosition().y + this->GetSize().height / 2 - 5,
			this->GetPosition().x + this->GetSize().width / 2 + 5,
			this->GetPosition().y + this->GetSize().height / 2 + 5
		};
	}
}

void Plasma::SetCastor(Entity* cast){
	// Release the existing focus
	if (m_Castor != nullptr)
		m_Castor->Release();

	// Store the parameter
	m_Castor = cast;

	// Add a reference to the new focus
	if (m_Castor != nullptr)
		m_Castor->AddRef();
}