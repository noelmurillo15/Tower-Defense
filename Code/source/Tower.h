/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#pragma once

#include "Entity.h"

#include "../SGD Wrappers/SGD_Geometry.h"

class Tower : public Entity {

		//	Tower Data
	float Health;
	float Gold;
	unsigned int Lives;
	float Score;

		//	Upgrade System
	struct Upgrade{

		float IncreasedDamage = 0;
		float IncreasedAttackSpeed = 0;
		float UpgradeCost = 0;
	};
	unsigned int m_Score = 0;
	//	Spawn data
	float spawnTimer = 2.0f;
	float numSpawns = 0;
	float numSpawnsAlive = 0;

	Upgrade m_Upgrade;
	unsigned int m_UpgradeLevel = 0;

		//	Cooldown Bar
	SGD::Rectangle m_cooldownRect;
	SGD::Rectangle m_cooldowntimerRect;

		//	Attack
	float PlasmaDamage = 200.0f;
	float CoolDownTimer = 0.0f;
	float attacktimer = 0.45f;
	bool WeaponExhausted = false;
		//	Wave Count
	bool WaveIncoming = false;
	unsigned int WaveNum = 0;

		//	Quadrilogy of Evil
	Tower() = default;
	~Tower() = default;
	Tower(const Tower&) = delete;
	Tower& operator=(const Tower&) = delete;

public:
		//	Singleton
	static Tower* GetInstance();

		//	Singleton Interface
	void Load();
	void Reset();
	void Unload();

		//	Tower Data Accessors / Mutators
	float			GetHealth(){ return Health; }
	float			GetPlasmaDamage(){ return PlasmaDamage; }
	float			GetGold(){ return Gold; }
	unsigned int	GetLives(){ return Lives; }
	unsigned int	GetWaveNum(){ return WaveNum; }
	bool			GetWaveIncoming(){ return WaveIncoming; }
	unsigned int	GetScore(){ return m_Score; }
	float			GetSpawnTimer(){ return spawnTimer; }
	float			GetNumSpawns(){ return numSpawns; }
	float			GetNumSpawnsAlive(){ return numSpawnsAlive; }
	
	void			SetHealth(float hp){ Health = hp; }
	void			SetPlasmaDamage(float dmg){ PlasmaDamage = dmg; }
	void			SetGold(float gold){ Gold = gold; }
	void			SetLives(unsigned int lives){ Lives = lives; }
	void			NextWave(){ WaveNum++; }
	void			SetWaveIncoming(bool rnd){ WaveIncoming = rnd; }
	void			UpdateSpawnTimer(float time){ spawnTimer -= time; }
	void			SetScore(unsigned int num){ m_Score += num; }
	void			ResetScore(){ m_Score = 0; }
	void			SetSpawnTimer(float time){ spawnTimer = time; }
	void			SetNumSpawns(float num){ numSpawns = num; }
	void			SetNumSpawnsAlive(float num){ numSpawnsAlive = num; }

		//	Overrides
	virtual void	Update(float elapsedTime)	override;
	virtual void	Render()					override;
	virtual void	HandleCollision(const IEntity* pOther)	override;

	virtual int		GetType() const	override	{ return ENT_BASE; }

	virtual SGD::Rectangle GetRect() const		override;
};

