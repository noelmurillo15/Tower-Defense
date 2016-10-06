/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "CreateWindStyleShurikenMessage.h"
#include "MessageID.h"
#include "Entity.h"

CreateWindStyleShurikenMessage::CreateWindStyleShurikenMessage(SGD::Point pos, SGD::Vector _toenemy, Entity* caster)
	: Message(MessageID::MSG_CREATE_WIND) {

	toEnemy = _toenemy;
	playerPos = pos;
	m_caster = caster;
}