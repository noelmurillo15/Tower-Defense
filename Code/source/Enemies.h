/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#pragma once

#include "Entity.h"
#include "../SGD Wrappers/SGD_Listener.h"

class CellAnimation;


class Enemies : public Entity {

		//	On-Hit Animations
	CellAnimation* m_PlasmaBlast = nullptr;
	CellAnimation* m_FireXplosion = nullptr;
	CellAnimation* m_AquaPrison = nullptr;
	CellAnimation* m_64PalmRotation = nullptr;

		//	Enemy data
	float e_Health = 0;
	float m_thawtimer = 0.0f;
	float m_sleeptimer = 0.0f;

	unsigned int e_GoldForKill = 0;
	int direction = 0;
	
	bool IsAsleep = false;
	bool IsSlowed = false;
	bool IsDead = false;

	SGD::HTexture m_sleepimg = SGD::INVALID_HANDLE;
	SGD::HAudio m_Collisionsfx = SGD::INVALID_HANDLE;

public:		Enemies() = default;		// default constructor
protected:	virtual ~Enemies(void);		// VIRTUAL destructor

public:
		//	Accessors	
	float			GetHealth() { return e_Health; }
	float			GetThawTimer() { return m_thawtimer; }
	unsigned int	GetGoldForKill(){ return e_GoldForKill; }
	int				GetDirection(){ return direction; }
	bool			GetIsAsleep() { return IsAsleep; }
	bool			GetIsSlowed() { return IsSlowed; }
	bool			GetIsDead() { return IsDead; }
	SGD::HTexture GetAsleepImg() { return m_sleepimg; }

		//	Muattors
	void SetHealth(float hp){ e_Health = hp; }
	void SetThawTimer(float time){ m_thawtimer = time; }
	void SetGoldForKill(unsigned int pts){ e_GoldForKill = pts; }
	void SetDirection(int dir){ direction = dir; }
	void SetIsAsleep(bool is){ IsAsleep = is; }
	void SetIsSlowed(bool is){ IsSlowed = is; }
	void SetIsDead(bool is){ IsDead = is; }
	void SetSleepImg(SGD::HTexture img){ m_sleepimg = img; }
	void SetAudio(SGD::HAudio a){ m_Collisionsfx = a; }

	void LoadEnemyStats();

		//	Overrides
	virtual void	Update(float elapsedTime) override;
	virtual void	Render(void) override;
	virtual void	HandleCollision(const IEntity* pOther) override;
	virtual int		GetType(void) const override	{ return ENT_BASE; }

	virtual SGD::Rectangle GetRect(void)const override;
};

