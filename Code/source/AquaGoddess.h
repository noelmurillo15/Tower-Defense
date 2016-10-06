/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#pragma once

#include "Heroes.h"

class AquaGoddess : public Heroes {

	float m_FreezeCooldown = 0.0f;
	bool IsOnCooldown = false;

public:		AquaGoddess() = default;
protected:	virtual ~AquaGoddess();

public:

	void LoadHeroStats();

		//	Mutators / Accessors
	float GetFreezeCooldown(){ return m_FreezeCooldown; }
	void SetFreezeCooldown(float time){ m_FreezeCooldown = time; }

		//	Overrides
	virtual void	Update(float elapsedTime) override;
	virtual void	Render() override;
	virtual void	HandleCollision(const IEntity* pOther) override;

	virtual int		GetType() const override	{ return P_AQUAGODDESS; }

	virtual SGD::Rectangle GetRect()	const			override;
	SGD::Rectangle GetUpgradeRect();
};