/***************************************************************
|	File:		DestroyEntityMessage.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	DestroyEntityMessage class stores the entity
|				to be removed from the Entity Manager
|				when the message is processed
***************************************************************/

#pragma once

#include "../SGD Wrappers/SGD_Message.h"	// uses SGD::Message

class Entity;								// uses Entity*


class DestroyEntityMessage : public SGD::Message
{
public:
	/**********************************************************/
	// Constructor REQUIRES the entity to remove
	DestroyEntityMessage( Entity* ptr );
	virtual ~DestroyEntityMessage();

	/**********************************************************/
	// Accessor
	Entity*		GetEntity( void ) const		{	return m_pEntity;	}

private:
	/**********************************************************/
	// entity to destroy
	Entity*		m_pEntity;
};

