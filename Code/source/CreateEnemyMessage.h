/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#pragma once

#include "../SGD Wrappers/SGD_Message.h"

class CreateEnemyMessage : public SGD::Message {

	unsigned int e_Type = 1;

public:
	CreateEnemyMessage(unsigned int type);
	virtual ~CreateEnemyMessage() = default;

	unsigned int GetType()const{ return e_Type; }
};

