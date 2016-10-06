/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "CreateEnemyMessage.h"
#include "MessageID.h"

CreateEnemyMessage::CreateEnemyMessage(unsigned int type)
	: Message(MessageID::MSG_CREATE_ENEMY) {

	e_Type = type;
}