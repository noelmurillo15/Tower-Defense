/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "CreatePlasmaMessage.h"
#include "MessageID.h"

CreatePlasmaMessage::CreatePlasmaMessage(unsigned int type, SGD::Vector _toenemy)
	: Message(MessageID::MSG_CREATE_PLASMA) {

	toEnemy = _toenemy;
	e_Type = type;
}