/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#pragma once

#include "Heroes.h"

class FireMage : public Heroes {

	float UpgradeRange;
	float m_FirebalDamage;

public:		FireMage() = default;
protected:	virtual ~FireMage();

public:

	void LoadHeroStats();

	float GetFireballDamage(){ return m_FirebalDamage; }
	void SetFireballDamage(float dmg){ m_FirebalDamage = dmg; }

	//	Overrides
	virtual void	Update(float elapsedTime) override;
	virtual void	Render() override;
	virtual void	HandleCollision(const IEntity* pOther) override;

	virtual int		GetType() const override	{ return P_FIREMAGE; }

	virtual SGD::Rectangle GetRect()	const			override;
	SGD::Rectangle GetUpgradeRect();
};