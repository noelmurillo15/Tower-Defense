/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#pragma once

#include "Heroes.h"

class Zephyr : public Heroes {

public:		Zephyr() = default;
protected:	virtual ~Zephyr();

public:

	//	Overrides
	virtual void	Update(float elapsedTime) override;
	virtual void	Render() override;
	virtual void	HandleCollision(const IEntity* pOther) override;

	virtual int		GetType() const override	{ return P_ZEPHYR; }

	virtual SGD::Rectangle GetRect()	const			override;
	SGD::Rectangle GetUpgradeRect();
};