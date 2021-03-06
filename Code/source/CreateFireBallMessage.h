/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#pragma once

#include "../SGD Wrappers/SGD_Message.h"
#include "../SGD Wrappers/SGD_Geometry.h"

class Entity;

class CreateFireBallMessage : public SGD::Message {

	SGD::Vector toEnemy;
	SGD::Point playerPos;
	Entity* m_caster;

public:
	CreateFireBallMessage(SGD::Point pos, SGD::Vector _toenemy, Entity* Caster);
	virtual ~CreateFireBallMessage() = default;

	SGD::Vector GetToEnemy()const { return toEnemy; }
	SGD::Point GetPlayerPos()const { return playerPos; }
	Entity* GetCaster() const{ return m_caster; }
};