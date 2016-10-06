/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#pragma once

#include "Enemies.h"

class Goblin : public Enemies {

public:		Goblin() = default;
protected:	virtual ~Goblin();

public:

	//	Overrides
	virtual void	Update(float elapsedTime)	override;
	virtual void	Render()					override;
	virtual void	HandleCollision(const IEntity* pOther)	override;

	virtual int		GetType() const	override	{ return E_GOBLIN; }

	virtual SGD::Rectangle GetRect() const		override;
};