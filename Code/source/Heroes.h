/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#pragma once

#include "Entity.h"

class Heroes : public Entity {

		//	Hero data
	Entity* p_target = nullptr;
	float attacktimer = 2.0f;
	unsigned int p_CostforPurchase = 0;

		//	Upgrade System
	struct Upgrade{
		float IncreasedRange;
		float IncreasedDamage;
		float IncreasedAttackRate;
		float UpgradeCost;
	};

	Upgrade m_Upgrade;
	unsigned int m_UpgradeLevel = 0;



public:		Heroes() = default;		// default constructor
protected:	virtual ~Heroes(void);		// VIRTUAL destructor

public:
		//	Hero data Accessors	/ Mutators
	Entity*			GetTarget(void) const { return p_target; }
	float			GetAttacktimer() { return attacktimer; }
	unsigned int	GetCostForPurcahse(){ return p_CostforPurchase; }
	Upgrade			GetUpgrade(){ return m_Upgrade; }
	unsigned int	GetUpgradeLevel(){ return m_UpgradeLevel; }

	void SetTarget(Entity* ptr);
	void SetAttacktimer(float time){ attacktimer = time; }
	void SetCostForPurcahse(unsigned int cost){ p_CostforPurchase = cost; }
	void SetUpgrade(float range, float dmg, float atck, float cost);
	void NextUpgrade(){ m_UpgradeLevel++; }

		//	Overrides
	virtual void	Update(float elapsedTime) override;
	virtual void	Render(void) override;
	virtual void	HandleCollision(const IEntity* pOther) override;

	virtual int		GetType(void) const override	{ return ENT_BASE; }

	virtual SGD::Rectangle GetRect(void) const	override;
};

