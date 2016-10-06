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

class CreatePlasmaMessage : public SGD::Message {

	unsigned int e_Type = 9;
	SGD::Vector toEnemy;

public:
	CreatePlasmaMessage(unsigned int type, SGD::Vector _toenemy);
	virtual ~CreatePlasmaMessage() = default;

	unsigned int GetType()const{ return e_Type; }
	SGD::Vector GetToEnemy()const { return toEnemy; }
};

