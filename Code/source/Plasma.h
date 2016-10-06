/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#pragma once

#include "Entity.h"

class Plasma : public Entity {

	float m_PlasmaDamage = 0;
	float m_FireballDamage = 0;
	float m_WindBlastDamage = 0;


	SGD::HAudio		m_PlasmaSfx = SGD::INVALID_HANDLE;
	Entity* m_Castor = nullptr;

	int m_type = 0;

public:		Plasma() = default;
protected:	virtual ~Plasma();

public:

		// Interface:	
	virtual void Update(float elapsedTime)		override;
	virtual void Render() override;
	virtual void HandleCollision(const IEntity* pOther)	override;
	virtual void SetType(int type){ m_type = type; }
	virtual int	 GetType(void)	const	override	{ return m_type; }
	
	virtual SGD::Rectangle GetRect()const override;

		// Accessors / Mutators:
	SGD::HAudio	GetPlasmaHitSfx(void) const		{ return m_PlasmaSfx; }
	void		SetPlasmaHitSfx(SGD::HAudio sfx)	{ m_PlasmaSfx = sfx; }

	float GetPlasmaDamage(){ return m_PlasmaDamage; }
	float GetFireBallDamage(){ return m_FireballDamage; }
	float GetWindBlastDamage(){ return m_WindBlastDamage; }

	void SetFireBallDamage(float dmg){ m_FireballDamage = dmg; }
	void SetPlasmaDamage(float dmg){ m_PlasmaDamage = dmg; }
	void SetWindShurikenDamage(float dmg){ m_WindBlastDamage = dmg; }

	Entity* GetCastor(){ return m_Castor; }
	void SetCastor(Entity* cast);
};

